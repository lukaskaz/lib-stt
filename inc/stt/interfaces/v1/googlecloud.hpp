#pragma once

#include "stt/factory.hpp"

#include <cstdint>
#include <memory>
#include <string>

namespace stt::v1::googlecloud
{

class TextFromVoice : public TextFromVoiceIf
{
  public:
    virtual ~TextFromVoice();

    transcript_t listen() override;
    transcript_t listen(language) override;

  private:
    friend class stt::TextFromVoiceFactory;
    TextFromVoice(std::shared_ptr<shell::ShellCommand>,
                  std::shared_ptr<stthelpers::HelpersIf>, language);

    struct Handler;
    std::unique_ptr<Handler> handler;
};

} // namespace stt::v1::googlecloud
