#include "speechtotext.hpp"

#include <boost/filesystem.hpp>
#include <nlohmann/json.hpp>

#include <cmath>
#include <fstream>
#include <iostream>
#include <unordered_map>

using json = nlohmann::json;

namespace stt
{
static constexpr const char* configFilePath = "../conf/init.json";
static constexpr const char* audioDirectory = "audio/";
static constexpr const char* recordingName = "recording.flac";
static constexpr const char* convUri =
    "http://www.google.com/speech-api/v2/recognize";
static constexpr const char* resultSignature = "transcript";

static const std::unordered_map<language, std::string> langMap = {
    {language::english, "en"}, {language::polish, "pl"}};

TextFromVoice::TextFromVoice(std::shared_ptr<ShellCommand> commandHandler,
                             language langOfVoice) :
    commandHandler{commandHandler},
    languageId{langMap.at(langOfVoice)}
{
    std::ifstream configFile(configFilePath);
    if (!configFile.is_open())
    {
        throw std::runtime_error("Cannot open config file for STT");
    }

    std::string fileData;
    configFile >> fileData;
    json sttConfig = json::parse(fileData)["stt"];

    if (sttConfig["key"].is_null() ||
        sttConfig["key"].get<std::string>().empty())
    {
        throw std::runtime_error("Cannot retrieve key for STT");
    }
    usageKey = sttConfig["key"];
    init();
}

TextFromVoice::~TextFromVoice()
{
    boost::filesystem::remove_all(stt::audioDirectory);
}

std::pair<std::string, uint32_t> TextFromVoice::listen()
{
    run();

    json data = json::parse(std::move(result));
    json& first = data["result"][0]["alternative"][0];
    std::string sttText = first["transcript"];
    double sttConfid = first["confidence"];
    auto quality = (uint32_t)std::lround(100 * sttConfid);

    result.clear();
    return std::make_pair(std::move(sttText), quality);
}

inline void TextFromVoice::init()
{
    boost::filesystem::create_directory(stt::audioDirectory);
    audioFilePath =
        std::string{stt::audioDirectory} + std::string{stt::recordingName};
    recordVoiceCmd = "sox --no-show-progress --type alsa default --rate 16000 "
                     "--channels 1 " +
                     audioFilePath + " silence -l 1 1 2% 1 1.5 1% pad 0.2 0.5";
    getTextFromVoiceCmd =
        "wget --quiet --user-agent \"Mozilla/5.0\" --post-file " +
        audioFilePath +
        " --header \"Content-Type: audio/x-flac; "
        "rate=16000\" -O - \"" +
        stt::convUri + "?lang=" + languageId + "&key=" + usageKey + "\"";
}

inline void TextFromVoice::run()
{
    while (result.empty())
    {
        if (commandHandler->run(std::move(recordVoiceCmd)))
        {
            continue;
        }

        std::vector<std::string> output;
        if (commandHandler->run(std::move(getTextFromVoiceCmd), output))
        {
            continue;
        }

        for (auto&& resp : output)
        {
            if (resp.find(stt::resultSignature) != std::string::npos)
            {
                result = std::move(resp);
                break;
            }
        }
    }
}

std::shared_ptr<TextFromVoice> TextFromVoiceFactory::create(language lang)
{
    auto shell = std::make_shared<BashCommand>();
    return std::make_shared<TextFromVoice>(shell, lang);
}

} // namespace stt
