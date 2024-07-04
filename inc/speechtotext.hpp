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

} // namespace stt
