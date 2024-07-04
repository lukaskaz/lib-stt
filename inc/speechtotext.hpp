#pragma once

#include "shellcommand.hpp"

#include <memory>
#include <string>

namespace stt
{
enum class language
{
    deflang = 0,
    english,
    polish
};

class TextFromVoice
{
  public:
    TextFromVoice(std::shared_ptr<ShellCommand>, language);
    ~TextFromVoice();

    std::pair<std::string, uint32_t> listen();

  private:
    std::shared_ptr<ShellCommand> commandHandler;
    const std::string languageId;
    std::string audioFilePath;
    std::string recordVoiceCmd;
    std::string getTextFromVoiceCmd;
    std::string usageKey;
    std::string result;

    void init();
    void run();
};

class TextFromVoiceFactory
{
  public:
    TextFromVoiceFactory() = delete;
    TextFromVoiceFactory(const TextFromVoiceFactory&) = delete;
    TextFromVoiceFactory(TextFromVoiceFactory&&) = delete;
    TextFromVoiceFactory& operator=(const TextFromVoiceFactory&) = delete;
    TextFromVoiceFactory& operator=(TextFromVoiceFactory&&) = delete;

    static std::shared_ptr<TextFromVoice> create(language lang)
    {
        auto shell = std::make_shared<BashCommand>();
        return std::make_shared<TextFromVoice>(shell, lang);
    }
};

} // namespace stt
