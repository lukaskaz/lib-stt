#include "shellcommand.hpp"
#include "speechtotext.hpp"

#include <iomanip>
#include <iostream>
#include <memory>

int main()
{
    try
    {
        auto stt = stt::TextFromVoiceFactory::create(stt::language::polish);
        std::cout << "Speak now...\n";
        auto [text, quality] = stt->listen();
        std::cout << "Text: " << std::quoted(text) << ", quality: " << quality
                  << "%\n";
    }
    catch (std::exception& err)
    {
        std::cerr << "[ERROR] " << err.what() << '\n';
    }

    return 0;
}
