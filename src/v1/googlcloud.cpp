#include "google/cloud/common_options.h"
#include "google/cloud/credentials.h"
#include "google/cloud/project.h"
#include "google/cloud/speech/v1/speech_client.h"

#include "stt/interfaces/v1/googlecloud.hpp"

#include <cmath>
#include <filesystem>
#include <fstream>
#include <optional>
#include <unordered_map>

namespace stt::v1::googlecloud
{

namespace speech = google::cloud::speech::v1;
namespace speech_type = google::cloud::speech_v1;

static const std::string audioDirectory = "audio/";
static const std::string recordingName = "recording.wav";
static const std::string audioFilePath = audioDirectory + recordingName;
static const std::string recordAudioCmd =
    "sox --no-show-progress --type alsa default --rate 16k --channels 1 " +
    audioFilePath + " silence -l 1 1 2.0% 1 1.0t 1.0% pad 0.3 0.2";
static const std::string keyFile = "../conf/key.json";

static const std::unordered_map<language, std::string> langMap = {
    {language::polish, "pl-PL"},
    {language::english, "en-US"},
    {language::german, "de-DE"}};

struct TextFromVoice::Handler
{
  public:
    Handler(std::shared_ptr<shell::ShellCommand> shell, language lang) :
        shell{shell}, filesystem{audioDirectory}, google{keyFile, lang}
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
        Google(const std::string& keyfile, language lang) :
            client{speech_type::MakeSpeechConnection(
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
                            }(keyfile))))}
        {
            const auto& config = request.mutable_config();
            config->set_profanity_filter(false);
            config->set_use_enhanced(false);
            config->set_model("latest_short");
            // config->set_encoding(speech::RecognitionConfig::FLAC);
            config->set_encoding(speech::RecognitionConfig::LINEAR16);
            config->set_sample_rate_hertz(16000);
            config->set_audio_channel_count(1);
            config->set_max_alternatives(1);
            setlang(lang);
        }

        void uploadaudio(const std::string& filepath)
        {
            std::ifstream ifs(filepath);
            if (!ifs.is_open())
            {
                throw std::runtime_error("Cannot open audio file for STT");
            }
            request.mutable_audio()->mutable_content()->assign(
                std::istreambuf_iterator<char>(ifs.rdbuf()), {});
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
                            break;
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
            static constexpr auto defaultlang{language::polish};
            this->lang = lang;
            auto langId = langMap.contains(lang) ? langMap.at(lang)
                                                 : langMap.at(defaultlang);
            request.mutable_config()->set_language_code(langId);
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
    }
    return {};
}

} // namespace stt::v1::googlecloud
