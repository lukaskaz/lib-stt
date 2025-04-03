#pragma once

#include "shell/interfaces/linux/bash/shell.hpp"
#include "stt/helpers.hpp"
#include "stt/interfaces/textfromvoice.hpp"

#include <memory>

namespace stt
{

class TextFromVoiceFactory
{
  public:
    template <typename T>
    static std::shared_ptr<TextFromVoiceIf> create(language lang)
    {
        auto shell = shell::Factory::create<shell::lnx::bash::Shell>();
        return create<T>(shell, lang);
    }

    template <typename T>
    static std::shared_ptr<TextFromVoiceIf>
        create(std::shared_ptr<shell::ShellIf> shell, language lang)
    {
        auto helpers = stthelpers::HelpersFactory::create();
        return create<T>(shell, helpers, lang);
    }

    template <typename T>
    static std::shared_ptr<TextFromVoiceIf>
        create(std::shared_ptr<shell::ShellIf> shell,
               std::shared_ptr<stthelpers::HelpersIf> helpers, language lang)
    {
        return std::shared_ptr<T>(new T(shell, helpers, lang));
    }
};

} // namespace stt
