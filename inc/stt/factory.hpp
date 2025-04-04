#pragma once

#include "stt/interfaces/textfromvoice.hpp"

#include <memory>

namespace stt
{

class TextFromVoiceFactory
{
  public:
    template <typename T, typename C>
    static std::shared_ptr<TextFromVoiceIf> create(const C& config)
    {
        return std::shared_ptr<T>(new T(config));
    }
};

} // namespace stt
