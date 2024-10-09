#pragma once

#include "shellcommand.hpp"
#include "stt/helpers.hpp"

#include <memory>
#include <string>

namespace stt
{

enum class language
{
    polish,
    english,
    german
};

using transcript_t = std::pair<std::string, uint32_t>;

class TextFromVoiceIf
{
  public:
    virtual ~TextFromVoiceIf() = default;
    virtual transcript_t listen() = 0;
    virtual transcript_t listen(language) = 0;
    static void kill();
};

template <typename T>
class TextFromVoiceFactory
{
  public:
    static std::shared_ptr<TextFromVoiceIf> create(language lang)
    {
        auto shell = std::make_shared<shell::BashCommand>();
        return create(shell, lang);
    }

    static std::shared_ptr<TextFromVoiceIf>
        create(std::shared_ptr<shell::ShellCommand> shell, language lang)
    {
        auto helpers = stthelpers::HelpersFactory::create();
        return create(shell, helpers, lang);
    }

    static std::shared_ptr<TextFromVoiceIf>
        create(std::shared_ptr<shell::ShellCommand> shell,
               std::shared_ptr<stthelpers::HelpersIf> helpers, language lang)
    {
        return std::shared_ptr<T>(new T(shell, helpers, lang));
    }
};

} // namespace stt
