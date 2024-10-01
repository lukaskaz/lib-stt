#pragma once

#include "stt/factory.hpp"

#include <cstdint>
#include <memory>
#include <string>

namespace stt
{

namespace googleapi
{

class TextFromVoice : public TextFromVoiceIf
{
  public:
    ~TextFromVoice();

    std::pair<std::string, uint32_t> listen() override;
    std::pair<std::string, uint32_t> listen(language) override;

  private:
    friend class TextFromVoiceFactory<TextFromVoice>;
    TextFromVoice(std::shared_ptr<shell::ShellCommand>,
                  std::shared_ptr<ssthelpers::HelpersIf>, language);

    std::shared_ptr<shell::ShellCommand> commandHandler;
    std::shared_ptr<ssthelpers::HelpersIf> helpers;
    std::string languageId;
    std::string audioFilePath;
    std::string recordVoiceCmd;
    std::string textFromVoiceUrl;
    std::string usageKey;

    void init();
    std::string run();
    void setlang(language);
};

} // namespace googleapi

} // namespace stt
