#include "logs/interfaces/console/logs.hpp"
#include "logs/interfaces/group/logs.hpp"
#include "logs/interfaces/storage/logs.hpp"
#include "stt/interfaces/v2/googleapi.hpp"

#include <signal.h>

#include <iomanip>
#include <iostream>

void signalhandler(int signum)
{
    stt::TextFromVoiceIf::kill();
    exit(signum);
}

int main(int argc, char** argv)
{
    try
    {
        signal(SIGINT, signalhandler);
        if (argc > 1)
        {
            auto loglvl =
                (bool)atoi(argv[1]) ? logs::level::debug : logs::level::info;

            auto logconsole = logs::Factory::create<logs::console::Log,
                                                    logs::console::config_t>(
                {loglvl, logs::time::hide, logs::tags::hide});
            auto logstorage = logs::Factory::create<logs::storage::Log,
                                                    logs::storage::config_t>(
                {loglvl, logs::time::show, logs::tags::show, {}});
            auto logif =
                logs::Factory::create<logs::group::Log, logs::group::config_t>(
                    {logconsole, logstorage});

            using namespace stt::v2::googleapi;
            auto stt =
                stt::TextFromVoiceFactory::create<TextFromVoice, configmin_t>(
                    {stt::language::polish, logif});

            std::cout << "Speak now in polish ...\n";
            auto result = stt->listen();
            std::cout << "Text: " << std::quoted(result.first)
                      << ", quality: " << result.second << "%\n";

            std::cout << "Speak now in english ...\n";
            result = stt->listen(stt::language::english);
            std::cout << "Text: " << std::quoted(result.first)
                      << ", quality: " << result.second << "%\n";

            std::cout << "Speak now in polish again ...\n";
            result = stt->listen();
            std::cout << "Text: " << std::quoted(result.first)
                      << ", quality: " << result.second << "%\n";
        }
    }
    catch (std::exception& err)
    {
        std::cerr << "[ERROR] " << err.what() << '\n';
    }

    return 0;
}
