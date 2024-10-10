#include "stt/interfaces/v2/googlecloud.hpp"

#include <iomanip>
#include <iostream>
#include <memory>

int main()
{
    try
    {
        using namespace stt::googlecloud::v2;
        auto stt = stt::TextFromVoiceFactory<TextFromVoice>::create(
            stt::language::polish);
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
    catch (std::exception& err)
    {
        std::cerr << "[ERROR] " << err.what() << '\n';
    }

    return 0;
}
