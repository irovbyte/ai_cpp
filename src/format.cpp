#include "format.h"

std::string formatResult(int index, const std::string &title,
                         const std::string &snippet, const std::string &link) {
  return std::to_string(index) + ". " + title + "\n" + snippet + "\n" + link +
         "\n\n";
}

std::string formatError(const std::string &message) {
  return "Ошибка: " + message;
}