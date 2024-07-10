#include "speechtotext.hpp"

#include <boost/filesystem.hpp>
#include <nlohmann/json.hpp>

#include <cmath>
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

void TextFromVoiceIf::kill()
{
    shell::BashCommand().run("killall -s KILL sox");
}

TextFromVoice::TextFromVoice(
    std::shared_ptr<shell::ShellCommand> commandHandler,
    std::shared_ptr<helpers::HelpersIf> helpers, language langOfVoice) :
    commandHandler{commandHandler},
    helpers{helpers}, languageId{langMap.at(langOfVoice)}
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

inline void TextFromVoice::init()
{
    boost::filesystem::create_directory(stt::audioDirectory);
    audioFilePath =
        std::string{stt::audioDirectory} + std::string{stt::recordingName};
    recordVoiceCmd = "sox --no-show-progress --type alsa default --rate 16000 "
                     "--channels 1 " +
                     audioFilePath + " silence -l 1 1 2% 1 1.5 1% pad 0.2 0.5";
    textFromVoiceUrl =
        std::string(stt::convUri) + "?lang=" + languageId + "&key=" + usageKey;
}

inline std::string TextFromVoice::run()
{
    while (true)
    {
        if (!commandHandler->run(std::move(recordVoiceCmd)))
        {
            std::string result;
            if (helpers->uploadFile(textFromVoiceUrl, audioFilePath, result) &&
                result.contains(stt::resultSignature))
            {
                return result;
            }
        }
    }
    return {};
}

std::shared_ptr<TextFromVoiceIf> TextFromVoiceFactory::create(language lang)
{
    auto shell = std::make_shared<shell::BashCommand>();
    return create(shell, lang);
}

std::shared_ptr<TextFromVoiceIf>
    TextFromVoiceFactory::create(std::shared_ptr<shell::ShellCommand> shell,
                                 language lang)
{
    auto helpers = helpers::HelpersFactory::create();
    return std::make_shared<TextFromVoice>(shell, helpers, lang);
}

} // namespace stt
