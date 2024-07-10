#include "helpers.hpp"

#include <curl/curl.h>
#include <curl/easy.h>

#include <filesystem>
#include <fstream>

namespace helpers
{

static size_t WriteFunction(char* data, size_t size, size_t nmemb,
                            std::string* output)
{
    size_t datasize{size * nmemb};
    output->append(data, datasize);
    return datasize;
}

bool Helpers::uploadFile(const std::string& url, const std::string& filepath,
                         std::string& output)
{
    CURLcode res{CURLE_FAILED_INIT};
    if (auto curl = curl_easy_init(); curl != nullptr)
    {
        static constexpr auto header{"Content-Type: audio/x-flac; rate=16000;"};
        if (curl_slist * hlist{}; (hlist = curl_slist_append(hlist, header)))
        {
            auto size = std::filesystem::file_size(filepath);
            std::vector<char> data(size);
            std::ifstream ifs(filepath, std::ios::in | std::ifstream::binary);
            ifs.read(&data[0], data.size());

            curl_easy_setopt(curl, CURLOPT_POST, 1);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, &data[0]);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, size);

            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hlist);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteFunction);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &output);
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
            res = curl_easy_perform(curl); // synchronous file upload
            curl_slist_free_all(hlist);
        }
        curl_easy_cleanup(curl);
    }
    return res == CURLE_OK;
}

std::shared_ptr<HelpersIf> HelpersFactory::create()
{
    return std::make_shared<Helpers>();
}

} // namespace helpers
