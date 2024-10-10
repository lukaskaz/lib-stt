#include "google/cloud/common_options.h"
#include "google/cloud/credentials.h"
#include "google/cloud/project.h"
#include "google/cloud/speech/v2/speech_client.h"

#include "stt/interfaces/v2/googlecloud.hpp"

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <optional>
#include <unordered_map>

namespace stt::googlecloud::v2
{

namespace speech = google::cloud::speech::v2;
namespace speech_type = google::cloud::speech_v2;
using locale_t = std::tuple<std::string, std::string, std::string, std::string>;

static const std::string audioDirectory = "audio/";
static const std::string recordingName = "recording.wav";
static const std::string audioFilePath = audioDirectory + recordingName;
static const std::string recordAudioCmd =
    "sox --no-show-progress --type alsa default --rate 16k --channels 1 " +
    audioFilePath + " silence -l 1 1 2.0% 1 1.0t 1.0% pad 0.3 0.2";
static const std::string keyFile = "../conf/key.json";
static const locale_t localeInfo = {"lukaszsttproject", "europe-west4",
                                    "stt-region", "chirp_2"};
// static const locale_t localeInfo = {"lukaszsttproject", "global",
// "stt-global",
//                                     "short"};

static const std::unordered_map<language, std::string> langMap = {
    {language::polish, "pl-PL"}};
// static const std::unordered_map<language, std::string> langMap = {
//     {language::polish, "pl-PL"},
//     {language::english, "en-US"},
//     {language::german, "de-DE"}};

struct TextFromVoice::Handler
{
  public:
    Handler(std::shared_ptr<shell::ShellCommand> shell) :
        shell{shell}, filesystem{audioDirectory}, google{keyFile, localeInfo}
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
        Google(const std::string& keyfile, locale_t locale) :
            client{speech_type::MakeSpeechConnection(
                std::get<1>(locale),
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
            // const auto& features = config->mutable_features();
            *config->mutable_auto_decoding_config() = {};
            config->set_model(std::get<3>(locale));
            request.set_recognizer("projects/" + std::get<0>(locale) +
                                   "/locations/" + std::get<1>(locale) +
                                   "/recognizers/" + std::get<2>(locale));
            std::ranges::for_each(langMap, [this](const auto& id) {
                request.mutable_config()->add_language_codes(id.second);
            });
        }

        void uploadaudio(const std::string& filepath)
        {
            std::ifstream ifs(filepath);
            if (!ifs.is_open())
            {
                throw std::runtime_error("Cannot open audio file for STT");
            }
            request.set_content(
                std::string(std::istreambuf_iterator<char>(ifs.rdbuf()), {}));
        }

        std::optional<transcript_t> gettranscript()
        {
            const auto& response = client.Recognize(request);
            if (const auto& results = response->results(); !results.empty())
            {
                if (const auto& alternatives = results.at(0).alternatives();
                    !alternatives.empty())
                {
                    const auto& first = alternatives.at(0);
                    auto quality =
                        (uint32_t)std::lround(100 * first.confidence());
                    return std::make_optional<transcript_t>(
                        std::move(first.transcript()), quality);
                }
            }
            return std::nullopt;
        }

      private:
        speech_type::SpeechClient client;
        speech::RecognizeRequest request;
    } google;
};

TextFromVoice::TextFromVoice(std::shared_ptr<shell::ShellCommand> shell,
                             std::shared_ptr<stthelpers::HelpersIf>, language) :
    handler{std::make_unique<Handler>(shell)}
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
    }
    return {};
}

transcript_t TextFromVoice::listen(language)
{
    return listen();
}

} // namespace stt::googlecloud::v2
