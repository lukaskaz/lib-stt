#include "stt/interfaces/googleapi.hpp"

#include <boost/filesystem.hpp>
#include <nlohmann/json.hpp>

#include <cmath>
#include <fstream>
#include <unordered_map>

namespace stt
{

namespace googleapi
{

using json = nlohmann::json;

static constexpr const char* configFilePath = "../conf/init.json";
static constexpr const char* audioDirectory = "audio/";
static constexpr const char* recordingName = "recording.flac";
static constexpr const char* convUri =
    "http://www.google.com/speech-api/v2/recognize";
static constexpr const char* resultSignature = "transcript";

static const std::unordered_map<language, std::string> langMap = {
    {language::polish, "pl-PL"},
    {language::english, "en-US"},
    {language::german, "de-DE"}};

TextFromVoice::TextFromVoice(
    std::shared_ptr<shell::ShellCommand> commandHandler,
    std::shared_ptr<ssthelpers::HelpersIf> helpers, language langOfVoice) :
    commandHandler{commandHandler},
    helpers{helpers}
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

    setlang(langOfVoice);
    init();
}

TextFromVoice::~TextFromVoice()
{
    boost::filesystem::remove_all(audioDirectory);
}

std::pair<std::string, uint32_t> TextFromVoice::listen()
{
    auto result = run();
    if (auto startpos = result.find("{\"transcript\"");
        startpos != std::string::npos)
    {
        if (auto endpos = result.substr(startpos).find("}");
            endpos != std::string::npos)
        {
            auto jsondata{result.substr(startpos, endpos + 1)};
            // std::cout << "stt json: " << jsondata << std::endl;
            json first = json::parse(std::move(jsondata));
            std::string text = first["transcript"];
            double confid = first["confidence"];
            auto quality = (uint32_t)std::lround(100 * confid);
            return {std::move(text), quality};
        }
    }
    return {};
}

std::pair<std::string, uint32_t> TextFromVoice::listen(language lang)
{
    auto prevLanguageId{languageId};
    setlang(lang);
    init();
    auto result = listen();
    languageId = prevLanguageId;
    init();
    return result;
}

inline void TextFromVoice::init()
{
    boost::filesystem::create_directory(audioDirectory);

    audioFilePath = std::string{audioDirectory} + std::string{recordingName};
    recordVoiceCmd =
        "sox --no-show-progress --type alsa default --rate 16k --channels 1 " +
        audioFilePath + " silence -l 1 1 2.0% 1 1.0t 1.0% pad 0.3 0.2";
    textFromVoiceUrl =
        std::string(convUri) + "?lang=" + languageId + "&key=" + usageKey;
}

inline std::string TextFromVoice::run()
{
    while (true)
    {
        if (!commandHandler->run(std::move(recordVoiceCmd)))
        {
            std::string result;
            if (helpers->uploadFile(textFromVoiceUrl, audioFilePath, result) &&
                result.contains(resultSignature))
            {
                return result;
            }
        }
    }
    return {};
}

inline void TextFromVoice::setlang(language lang)
{
    static constexpr language defaultlang = {language::polish};
    languageId =
        langMap.contains(lang) ? langMap.at(lang) : langMap.at(defaultlang);
}

} // namespace googleapi

} // namespace stt
