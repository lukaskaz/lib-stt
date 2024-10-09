#pragma once

#include "stt/helpers.hpp"

#include <gmock/gmock.h>

class HelpersMock : public stthelpers::HelpersIf
{
  public:
    MOCK_METHOD(bool, uploadFile,
                (const std::string&, const std::string&, std::string&),
                (override));
};
