#pragma once

#include "shellcommand.hpp"

#include <gmock/gmock.h>

class ShellMock : public shell::ShellCommand
{
  public:
    MOCK_METHOD(int, run, (std::string &&), (override));
    MOCK_METHOD(int, run, (std::string&&, std::vector<std::string>&),
                (override));
};
