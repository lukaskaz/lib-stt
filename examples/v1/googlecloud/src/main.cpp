#include "stt/interfaces/v1/googlecloud.hpp"

#include <signal.h>

#include <iomanip>
#include <iostream>
#include <memory>

void signalhandler(int signum)
{
    stt::TextFromVoiceIf::kill();
    exit(signum);
}

int main()
{
    try
    {
        signal(SIGINT, signalhandler);

        using namespace stt::v1::googlecloud;
        auto stt = stt::TextFromVoiceFactory::create<TextFromVoice>(
            stt::language::polish);
        std::cout << "Speak now in polish ...\n";
        auto result = stt->listen();
        std::cout << "Text: " << std::quoted(result.first)
                  << ", quality: " << result.second << "%\n";
        exit(1);
        std::cout << "Speak now in english ...\n";
        result = stt->listen(stt::language::english);
        std::cout << "Text: " << std::quoted(result.first)
                  << ", quality: " << result.second << "%\n";
        std::cout << "Speak now in polish again ...\n";
        result = stt->listen();
        std::cout << "Text: " << std::quoted(result.first)
                  << ", quality: " << result.second << "%\n";
    }
    catch (std::exception& err)
    {
        std::cerr << "[ERROR] " << err.what() << '\n';
    }

    return 0;
}
