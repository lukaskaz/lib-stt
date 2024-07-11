#pragma once

#include "helpers.hpp"
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
    static void kill();
};

class TextFromVoice : public TextFromVoiceIf
{
  public:
    TextFromVoice(std::shared_ptr<shell::ShellCommand>,
                  std::shared_ptr<ssthelpers::HelpersIf>, language);
    ~TextFromVoice();

    std::pair<std::string, uint32_t> listen() override;

  private:
    std::shared_ptr<shell::ShellCommand> commandHandler;
    std::shared_ptr<ssthelpers::HelpersIf> helpers;
    const std::string languageId;
    std::string audioFilePath;
    std::string recordVoiceCmd;
    std::string textFromVoiceUrl;
    std::string usageKey;

    void init();
    std::string run();
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
        create(std::shared_ptr<shell::ShellCommand>, language);
};

} // namespace stt
