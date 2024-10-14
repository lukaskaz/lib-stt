#include "stt/factory.hpp"

namespace stt
{

void stt::TextFromVoiceIf::kill()
{
    shell::BashCommand().run("killall -s KILL sox");
}

} // namespace stt
