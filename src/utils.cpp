#include "utils.h"
#include <algorithm>

std::string normalize(const std::string &input) {
  std::string result = input;
  std::transform(result.begin(), result.end(), result.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  return result;
}