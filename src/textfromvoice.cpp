#include "stt/interfaces/textfromvoice.hpp"

#include "shell/interfaces/linux/bash/shell.hpp"

namespace stt
{

void stt::TextFromVoiceIf::kill()
{
    shell::Factory::create<shell::lnx::bash::Shell>()->run(
        "killall -s KILL sox");
}

} // namespace stt
