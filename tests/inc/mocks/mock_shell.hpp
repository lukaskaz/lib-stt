#pragma once

#include "shell/interfaces/shell.hpp"

#include <gmock/gmock.h>

class ShellMock : public shell::ShellIf
{
  public:
    MOCK_METHOD(int, run, (const std::string&), (override));
    MOCK_METHOD(int, run, (const std::string&, std::vector<std::string>&),
                (override));
};
