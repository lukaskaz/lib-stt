#include "stt/interfaces/v2/googleapi.hpp"

#include <nlohmann/json.hpp>

#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <unordered_map>

namespace stt::v2::googleapi
{

using json = nlohmann::json;

static const std::string audioDirectory = "audio/";
static const std::string recordingName = "recording.flac";
static const std::string audioFilePath = audioDirectory + recordingName;
static const std::string recordAudioCmd =
    "sox --no-show-progress --type alsa default --rate 16k --channels 1 " +
    audioFilePath + " silence -l 1 1 2.0% 1 1.0t 1.0% pad 0.3 0.2";
static const std::string configFile = "../conf/init.json";
static const std::string convUri =
    "http://www.google.com/speech-api/v2/recognize";
static const std::string resultSignature = "transcript";

static const std::unordered_map<language, std::string> langMap = {
    {language::polish, "pl-PL"},
    {language::english, "en-US"},
    {language::german, "de-DE"}};

struct TextFromVoice::Handler
{
  public:
    Handler(std::shared_ptr<shell::ShellCommand> shell,
            std::shared_ptr<stthelpers::HelpersIf> helpers, language lang) :
        shell{shell},
        filesystem{audioDirectory}, google{configFile, helpers, lang}
    {}

    std::shared_ptr<shell::ShellCommand> shell;
    class Filesystem
    {
      public:
        Filesystem(const std::string& dirname) : basedir{dirname}
        {
            createdirectory();
        }

        ~Filesystem()
        {
            removedirectory();
        }

        void createdirectory()
        {
            direxist = !std::filesystem::create_directories(basedir);
        }
        void removedirectory() const
        {
            direxist ? false : std::filesystem::remove_all(basedir);
        }

      private:
        const std::string basedir;
        bool direxist;
    } filesystem;

    class Google
    {
      public:
        Google(const std::string& configfile,
               std::shared_ptr<stthelpers::HelpersIf> helpers, language lang) :
            helpers{helpers},
            key{[](const std::string& file) {
                std::ifstream ifs(file);
                if (!ifs.is_open())
                {
                    throw std::runtime_error("Cannot open config file for STT");
                }
                auto content = std::string(
                    std::istreambuf_iterator<char>(ifs.rdbuf()), {});
                json sttConfig = json::parse(content)["stt"];
                if (sttConfig["key"].is_null() ||
                    sttConfig["key"].get<std::string>().empty())
                {
                    throw std::runtime_error(
                        "Cannot get STT key from config file");
                }
                return sttConfig["key"].get<std::string>();
            }(configfile)}
        {
            setlang(lang);
        }

        std::optional<transcript_t> gettranscript()
        {
            std::string result;
            helpers->uploadFile(url, audioFilePath, result);
            if (auto startpos = result.find("{\"transcript\"");
                startpos != std::string::npos)
            {
                if (auto endpos = result.substr(startpos).find("}");
                    endpos != std::string::npos)
                {
                    auto jsondata{result.substr(startpos, endpos + 1)};
                    // std::cout << "stt json: " << jsondata << std::endl;
                    auto first = json::parse(std::move(jsondata));
                    auto text = first["transcript"].get<std::string>();
                    auto confid = first["confidence"].get<double>();
                    auto quality = (uint32_t)std::lround(100 * confid);
                    return std::make_optional<transcript_t>(std::move(text),
                                                            quality);
                }
            }
            return std::nullopt;
        }

        std::optional<transcript_t> gettranscript(language tmplang)
        {
            const auto mainlang{lang};
            setlang(tmplang);
            auto transcript = gettranscript();
            setlang(mainlang);
            return transcript;
        }

      private:
        std::shared_ptr<stthelpers::HelpersIf> helpers;
        const std::string key;
        language lang;
        std::string url;

        void setlang(language lang)
        {
            static constexpr auto defaultlang{language::polish};
            this->lang = lang;
            auto langId = langMap.contains(lang) ? langMap.at(lang)
                                                 : langMap.at(defaultlang);
            url = std::string(convUri) + "?lang=" + langId + "&key=" + key;
        }
    } google;
};

TextFromVoice::TextFromVoice(std::shared_ptr<shell::ShellCommand> shell,
                             std::shared_ptr<stthelpers::HelpersIf> helpers,
                             language lang) :
    handler{std::make_unique<Handler>(shell, helpers, lang)}
{}

TextFromVoice::~TextFromVoice() = default;

transcript_t TextFromVoice::listen()
{
    while (true)
    {
        handler->shell->run(recordAudioCmd);
        if (auto transcript = handler->google.gettranscript();
            transcript.has_value())
        {
            return *transcript;
        }
        std::cerr << "Cannot get transcipt, repeating...\n";
    }
    return {};
}

transcript_t TextFromVoice::listen(language lang)
{
    while (true)
    {
        handler->shell->run(recordAudioCmd);
        if (auto transcript = handler->google.gettranscript(lang);
            transcript.has_value())
        {
            return *transcript;
        }
        std::cerr << "Cannot get transcipt, repeating...\n";
    }
    return {};
}

} // namespace stt::v2::googleapi
