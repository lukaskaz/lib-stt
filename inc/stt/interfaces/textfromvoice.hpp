#pragma once

#include <cstdint>
#include <string>
#include <utility>

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

} // namespace stt
