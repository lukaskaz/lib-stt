#include "stt/interfaces/googleapi.hpp"

#include <nlohmann/json.hpp>

#include <cmath>
#include <unordered_map>

namespace stt
{

void stt::TextFromVoiceIf::kill()
{
    shell::BashCommand().run("killall -s KILL sox");
}

} // namespace stt
