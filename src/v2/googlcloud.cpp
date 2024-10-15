#include "google/cloud/common_options.h"
#include "google/cloud/credentials.h"
#include "google/cloud/project.h"
#include "google/cloud/speech/v2/speech_client.h"

#include "stt/interfaces/v2/googlecloud.hpp"

#include <cmath>
#include <filesystem>
#include <fstream>
#include <optional>
#include <unordered_map>

namespace stt::v2::googlecloud
{

namespace speech = google::cloud::speech::v2;
namespace speech_type = google::cloud::speech_v2;

using recognizer_t =
    std::tuple<std::string, std::string, std::string, std::string>;

static const std::string audioDirectory = "audio/";
static const std::string recordingName = "recording.wav";
static const std::string audioFilePath = audioDirectory + recordingName;
static const std::string recordAudioCmd =
    "sox --no-show-progress --type alsa default --rate 8k --channels 1 " +
    audioFilePath + " silence -l 1 1 2.0% 1 1.0t 1.0% pad 0.3 0.2";
static const std::string keyFile = "../conf/key.json";
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
    Handler(std::shared_ptr<shell::ShellCommand> shell, language lang) :
        shell{shell}, filesystem{audioDirectory},
        google{keyFile, recognizerInfo, lang}
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
        Google(const std::string& keyfile, recognizer_t recognizer,
               language lang) :
            client{speech_type::MakeSpeechConnection(
                std::get<1>(recognizer),
                google::cloud::Options{}
                    .set<google::cloud::UnifiedCredentialsOption>(
                        google::cloud::MakeServiceAccountCredentials(
                            [](const std::string& file) {
                                std::ifstream ifs(file);
                                if (!ifs.is_open())
                                {
                                    throw std::runtime_error(
                                        "Cannot open key file for STT");
                                }
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
        }

        void uploadaudio(const std::string& filepath)
        {
            request.set_content([](const std::string file) {
                std::ifstream ifs(file, std::ios::in | std::ios::binary);
                if (!ifs.is_open())
                {
                    throw std::runtime_error("Cannot open audio file for STT");
                }
                return std::string(std::istreambuf_iterator<char>(ifs.rdbuf()),
                                   {});
            }(filepath));
        }

        std::optional<transcript_t> gettranscript()
        {
            if (auto response = client.Recognize(request))
            {
                for (const auto& result : response->results())
                {
                    for (const auto& alternative : result.alternatives())
                    {
                        auto text = alternative.transcript();
                        if (text.empty())
                            continue;
                        auto confid = alternative.confidence();
                        auto quality = (uint32_t)std::lround(100 * confid);
                        return std::make_optional<transcript_t>(std::move(text),
                                                                quality);
                    }
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
    } google;
};

TextFromVoice::TextFromVoice(std::shared_ptr<shell::ShellCommand> shell,
                             std::shared_ptr<stthelpers::HelpersIf>,
                             language lang) :
    handler{std::make_unique<Handler>(shell, lang)}
{}

TextFromVoice::~TextFromVoice() = default;

transcript_t TextFromVoice::listen()
{
    while (true)
    {
        handler->shell->run(recordAudioCmd);
        handler->google.uploadaudio(audioFilePath);
        if (auto transcript = handler->google.gettranscript();
            transcript.has_value())
        {
            return *transcript;
        }
        std::cerr << "Cannot get transcipt, repeating...\n";
        break;
    }
    return {};
}

transcript_t TextFromVoice::listen(language lang)
{
    while (true)
    {
        handler->shell->run(recordAudioCmd);
        handler->google.uploadaudio(audioFilePath);
        if (auto transcript = handler->google.gettranscript(lang);
            transcript.has_value())
        {
            return *transcript;
        }
        std::cerr << "Cannot get transcipt, repeating...\n";
        break;
    }
    return {};
}

} // namespace stt::v2::googlecloud
