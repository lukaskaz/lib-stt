#pragma once

#include "stt/factory.hpp"

#include <cstdint>
#include <memory>
#include <string>

namespace stt::v2::googleapi
{

class TextFromVoice : public TextFromVoiceIf
{
  public:
    virtual ~TextFromVoice();

    transcript_t listen() override;
    transcript_t listen(language) override;

  private:
    friend class stt::TextFromVoiceFactory;
    TextFromVoice(std::shared_ptr<shell::ShellIf>,
                  std::shared_ptr<stthelpers::HelpersIf>, language);

    struct Handler;
    std::unique_ptr<Handler> handler;
};

} // namespace stt::v2::googleapi
