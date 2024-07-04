#include "shellcommand.hpp"
#include "speechtotext.hpp"

#include <iomanip>
#include <iostream>
#include <memory>

int main()
{
    try
    {
        auto shellCommand = std::make_shared<BashCommand>();
        auto stt = std::make_unique<stt::TextFromVoice>(shellCommand,
                                                        stt::language::polish);
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
    {"result" : []} {
        "result" : [ {
            "alternative" :
                [ {"transcript" : "imię", "confidence" : 0.91898549} ],
            "final" : true
        } ],
        "result_index" : 0
    };
}
