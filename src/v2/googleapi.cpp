#include "stt/interfaces/v2/googleapi.hpp"

#include "shell/interfaces/linux/bash/shell.hpp"
#include "stt/helpers.hpp"

#include <nlohmann/json.hpp>

#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <source_location>
#include <unordered_map>

namespace stt::v2::googleapi
{

using json = nlohmann::json;
using namespace helpers;
using namespace std::string_literals;

static const std::filesystem::path configFile = "../conf/init.json";
static const std::filesystem::path audioDirectory = "audio";
static const std::filesystem::path recordingName = "recording.flac";
static const auto audioFilePath = audioDirectory / recordingName;
static const auto recordAudioCmd = getrecordingcmd(audioFilePath.native());
static const auto convUri = "http://www.google.com/speech-api/v2/recognize"s;
static const auto resultSignature = "transcript"s;

static const std::unordered_map<language, std::string> langMap = {
    {language::polish, "pl-PL"},
    {language::english, "en-US"},
    {language::german, "de-DE"}};

struct TextFromVoice::Handler
{
  public:
    Handler(const configmin_t& config) :
        logif{std::get<std::shared_ptr<logs::LogIf>>(config)},
        shell{shell::Factory::create<shell::lnx::bash::Shell>()},
        helpers{helpers::HelpersFactory::create()},
        filesystem{this, audioDirectory}, google{this, configFile,
                                                 std::get<language>(config)}
    {}

    Handler(const configall_t& config) :
        logif{std::get<std::shared_ptr<logs::LogIf>>(config)},
        shell{std::get<std::shared_ptr<shell::ShellIf>>(config)},
        helpers{std::get<std::shared_ptr<helpers::HelpersIf>>(config)},
        filesystem{this, audioDirectory}, google{this, configFile,
                                                 std::get<language>(config)}
    {}

    transcript_t listen()
    {
        while (true)
        {
            log(logs::level::debug, "Waiting for speech to process");
            shell->run(recordAudioCmd);
            if (auto transcript = google.gettranscript())
                return *transcript;
        }
        return {};
    }

    transcript_t listen(language lang)
    {
        while (true)
        {
            log(logs::level::debug, "Waiting for speech to process");
            shell->run(recordAudioCmd);
            if (auto transcript = google.gettranscript(lang))
                return *transcript;
        }
        return {};
    }

  private:
    const std::shared_ptr<logs::LogIf> logif;
    const std::shared_ptr<shell::ShellIf> shell;
    const std::shared_ptr<helpers::HelpersIf> helpers;
    class Filesystem
    {
      public:
        Filesystem(const Handler* handler,
                   const std::filesystem::path& dirname) :
            handler{handler},
            basedir{dirname}
        {
            createdirectory();
        }

        ~Filesystem()
        {
            removedirectory();
        }

        void createdirectory()
        {
            if ((direxist = !std::filesystem::create_directories(basedir)))
                handler->log(logs::level::warning,
                             "Cannot create already existing directory: '" +
                                 basedir.native() + "'");
            else
                handler->log(logs::level::debug,
                             "Created directory: '" + basedir.native() + "'");
        }
        void removedirectory() const
        {
            direxist ? false : std::filesystem::remove_all(basedir);
            if (direxist)
                handler->log(logs::level::warning,
                             "Not removing previously existed directory: '" +
                                 basedir.native() + "'");
            else
                handler->log(logs::level::debug,
                             "Removed directory: '" + basedir.native() + "'");
        }

      private:
        const Handler* handler;
        const std::filesystem::path basedir;
        bool direxist;
    } filesystem;

    class Google
    {
      public:
        Google(const Handler* handler, const std::filesystem::path& configfile,
               language lang) :
            handler{handler},
            key{[](const std::filesystem::path& file) {
                std::ifstream ifs(file);
                if (!ifs.is_open())
                    throw std::runtime_error("Cannot open config file for STT");
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

            handler->log(logs::level::info,
                         "Created v2::gapi stt [langcode/langid]: " +
                             getparams());
        }

        ~Google()
        {
            handler->log(logs::level::info,
                         "Released v2::gapi stt [langcode/langid]: " +
                             getparams());
        }

        std::optional<transcript_t> gettranscript()
        {
            std::string result;
            handler->helpers->uploadFile(url, audioFilePath, result);
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
                    handler->log(logs::level::debug,
                                 "Returning transcript [text/confid]: '" +
                                     text + "'/" + str(confid));
                    return std::make_optional<transcript_t>(std::move(text),
                                                            quality);
                }
            }
            handler->log(logs::level::debug, "Cannot recognize transcript");
            return std::nullopt;
        }

        std::optional<transcript_t> gettranscript(language tmplang)
        {
            const auto mainlang{lang};
            setlang(tmplang);
            auto transcript = gettranscript();
            handler->log(logs::level::debug,
                         "Speech detected for " + getparams());
            setlang(mainlang);
            return transcript;
        }

      private:
        const Handler* handler;
        const std::string key;
        language lang;
        std::string url;

        void setlang(language lang)
        {
            auto langId = [this](language newlang) {
                this->lang = newlang;
                static constexpr auto deflang{language::polish};
                return langMap.contains(newlang) ? langMap.at(newlang)
                                                 : langMap.at(deflang);
            }(lang);
            url = std::string(convUri) + "?lang=" + langId + "&key=" + key;
        }

        std::string getparams() const
        {
            auto langid = (std::underlying_type_t<decltype(lang)>)lang;
            auto langcode = langMap.contains(lang) ? langMap.at(lang) : "UNDEF";
            return langcode + "/" + str(langid);
        }
    } google;

    void log(
        logs::level level, const std::string& msg,
        const std::source_location loc = std::source_location::current()) const
    {
        if (logif)
            logif->log(level, std::string{loc.function_name()}, msg);
    }
};

TextFromVoice::TextFromVoice(const config_t& config)
{
    handler = std::visit(
        [](const auto& config) -> decltype(TextFromVoice::handler) {
            if constexpr (!std::is_same<const std::monostate&,
                                        decltype(config)>())
            {
                return std::make_unique<TextFromVoice::Handler>(config);
            }
            throw std::runtime_error(
                std::source_location::current().function_name() +
                "-> config not supported"s);
        },
        config);
}

TextFromVoice::~TextFromVoice() = default;

transcript_t TextFromVoice::listen()
{
    return handler->listen();
}

transcript_t TextFromVoice::listen(language lang)
{
    return handler->listen(lang);
}

} // namespace stt::v2::googleapi
