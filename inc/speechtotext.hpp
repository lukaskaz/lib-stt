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

class TextFromVoiceIf
{
  public:
    virtual ~TextFromVoiceIf(){};
    virtual std::pair<std::string, uint32_t> listen() = 0;
};

class TextFromVoice : public TextFromVoiceIf
{
  public:
    TextFromVoice(std::shared_ptr<ShellCommand>, language);
    ~TextFromVoice();

    std::pair<std::string, uint32_t> listen() override;

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

    static std::shared_ptr<TextFromVoiceIf> create(language);
    static std::shared_ptr<TextFromVoiceIf>
        create(std::shared_ptr<ShellCommand>, language);
};

} // namespace stt
