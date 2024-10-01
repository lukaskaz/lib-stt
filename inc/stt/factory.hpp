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

class TextFromVoiceIf
{
  public:
    virtual ~TextFromVoiceIf() = default;
    virtual std::pair<std::string, uint32_t> listen() = 0;
    virtual std::pair<std::string, uint32_t> listen(language) = 0;
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
        auto helpers = ssthelpers::HelpersFactory::create();
        return create(shell, helpers, lang);
    }

    static std::shared_ptr<TextFromVoiceIf>
        create(std::shared_ptr<shell::ShellCommand> shell,
               std::shared_ptr<ssthelpers::HelpersIf> helpers, language lang)
    {
        return std::shared_ptr<T>(new T(shell, helpers, lang));
    }
};

} // namespace stt
