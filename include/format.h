#pragma once
#include <string>

std::string formatResult(int index, const std::string &title,
                         const std::string &snippet, const std::string &link);

std::string formatError(const std::string &message);