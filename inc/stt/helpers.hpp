#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

namespace helpers
{

class HelpersIf
{
  public:
    virtual ~HelpersIf()
    {}
    virtual bool uploadFile(const std::string&, const std::string&,
                            std::string&) = 0;
};

class Helpers : public HelpersIf
{
  public:
    bool uploadFile(const std::string&, const std::string&,
                    std::string&) override;
};

class HelpersFactory
{
  public:
  public:
    HelpersFactory() = delete;
    HelpersFactory(const HelpersFactory&) = delete;
    HelpersFactory(HelpersFactory&&) = delete;
    HelpersFactory& operator=(const HelpersFactory&) = delete;
    HelpersFactory& operator=(HelpersFactory&&) = delete;

    static std::shared_ptr<HelpersIf> create();
};

std::string str(const auto& value)
{
    if constexpr (std::is_same<const std::string&, decltype(value)>())
        return value;
    else
        return std::to_string(value);
}

std::string getrecordingcmd(const std::string&, const std::string&);

} // namespace helpers
