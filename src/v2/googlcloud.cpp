#include "google/cloud/common_options.h"
#include "google/cloud/credentials.h"
#include "google/cloud/project.h"
#include "google/cloud/speech/v2/speech_client.h"

#include "shell/interfaces/linux/bash/shell.hpp"
#include "stt/helpers.hpp"
#include "stt/interfaces/v2/googlecloud.hpp"

#include <cmath>
#include <filesystem>
#include <fstream>
#include <optional>
#include <source_location>
#include <unordered_map>

namespace stt::v2::googlecloud
{

using namespace helpers;
using namespace std::string_literals;
namespace speech = google::cloud::speech::v2;
namespace speech_type = google::cloud::speech_v2;

using recognizer_t =
    std::tuple<std::string, std::string, std::string, std::string>;

static const std::filesystem::path keyFile = "../conf/key.json";
static const std::filesystem::path audioDirectory = "audio";
static const std::filesystem::path recordingName = "recording.wav";
static const auto audioFilePath = audioDirectory / recordingName;
static const auto recordAudioCmd = getrecordingcmd(audioFilePath.native());
// static const recognizer_t recognizerInfo = {"lukaszsttproject",
// "europe-west4", "stt-region", "chirp_2"};
static const recognizer_t recognizerInfo = {"lukaszsttproject", "eu",
                                            "stt-global", "short"};

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
        filesystem{this, audioDirectory}, google{this, keyFile, recognizerInfo,
                                                 std::get<language>(config)}
    {}

    Handler(const configall_t& config) :
        logif{std::get<std::shared_ptr<logs::LogIf>>(config)},
        shell{std::get<std::shared_ptr<shell::ShellIf>>(config)},
        filesystem{this, audioDirectory}, google{this, keyFile, recognizerInfo,
                                                 std::get<language>(config)}
    {}

    transcript_t listen()
    {
        while (true)
        {
            log(logs::level::debug, "Waiting for speech to process");
            shell->run(recordAudioCmd);
            google.uploadaudio(audioFilePath);
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
            google.uploadaudio(audioFilePath);
            if (auto transcript = google.gettranscript(lang))
                return *transcript;
        }
        return {};
    }

  private:
    const std::shared_ptr<logs::LogIf> logif;
    const std::shared_ptr<shell::ShellIf> shell;
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
        Google(const Handler* handler, const std::filesystem::path& keyfile,
               recognizer_t recognizer, language lang) :
            handler{handler},
            client{speech_type::MakeSpeechConnection(
                std::get<1>(recognizer),
                google::cloud::Options{}
                    .set<google::cloud::UnifiedCredentialsOption>(
                        google::cloud::MakeServiceAccountCredentials(
                            [](const std::filesystem::path& file) {
                                std::ifstream ifs(file);
                                if (!ifs.is_open())
                                    throw std::runtime_error(
                                        "Cannot open key file for STT");
                                return std::string(
                                    std::istreambuf_iterator<char>(ifs.rdbuf()),
                                    {});
                            }(keyfile))))},
            lang{lang}
        {
            const auto& config = request.mutable_config();
            *config->mutable_auto_decoding_config() = {};
            config->set_model(std::get<3>(recognizer));
            request.set_recognizer("projects/" + std::get<0>(recognizer) +
                                   "/locations/" + std::get<1>(recognizer) +
                                   "/recognizers/" + std::get<2>(recognizer));
            setlang(lang);

            handler->log(logs::level::info,
                         "Created v2::gcloud stt [langcode/langid]: " +
                             getparams());
        }

        ~Google()
        {
            handler->log(logs::level::info,
                         "Released v2::gcloud stt [langcode/langid]: " +
                             getparams());
        }

        void uploadaudio(const std::filesystem::path& filepath)
        {
            request.set_content([](const std::filesystem::path& path) {
                std::ifstream ifs(path, std::ios::in | std::ios::binary);
                if (!ifs.is_open())
                    throw std::runtime_error("Cannot open audio file for STT");
                return std::string(std::istreambuf_iterator<char>(ifs.rdbuf()),
                                   {});
            }(filepath));
            handler->log(logs::level::debug,
                         "Uploaded audio to stt engine: " + filepath.native());
        }

        std::optional<transcript_t> gettranscript()
        {
            if (auto response = client.Recognize(request))
            {
                handler->log(logs::level::debug,
                             "Received results: " +
                                 str(response->results_size()));
                for (const auto& result : response->results())
                {
                    handler->log(logs::level::debug,
                                 "Received alternatives: " +
                                     str(result.alternatives_size()));
                    for (const auto& alternative : result.alternatives())
                    {
                        auto text = alternative.transcript();
                        if (text.empty())
                            continue;
                        auto confid = alternative.confidence();
                        auto quality = (uint32_t)std::lround(100 * confid);
                        handler->log(logs::level::debug,
                                     "Returning transcript [text/confid]: '" +
                                         text + "'/" + str(confid));
                        return std::make_optional<transcript_t>(std::move(text),
                                                                quality);
                    }
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
        speech_type::SpeechClient client;
        speech::RecognizeRequest request;
        language lang;

        void setlang(language lang)
        {
            auto langId = [this](language newlang) {
                this->lang = newlang;
                static constexpr auto deflang{language::polish};
                return langMap.contains(newlang) ? langMap.at(newlang)
                                                 : langMap.at(deflang);
            }(lang);

            const auto& config = request.mutable_config();
            config->language_codes_size()
                ? config->set_language_codes(0, langId)
                : config->add_language_codes(langId);
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

} // namespace stt::v2::googlecloud
