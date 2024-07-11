#pragma once

#include "helpers.hpp"

#include <gmock/gmock.h>

class HelpersMock : public ssthelpers::HelpersIf
{
  public:
    MOCK_METHOD(bool, uploadFile,
                (const std::string&, const std::string&, std::string&),
                (override));
};
