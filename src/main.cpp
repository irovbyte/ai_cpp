#include "ai.h"
#include "memory.h"
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

int main() {

  const char *envPath = getenv("AI_CPP_DB");
  std::string dbPath = envPath ? envPath : "data/memory.db";

  std::filesystem::create_directories("data");

  MemoryDB db(dbPath);

  std::cout << "ИИ (C++): Введите запрос (или 'exit' для выхода):\n";
  std::string line;

  while (true) {
    std::cout << "> ";
    if (!std::getline(std::cin, line))
      break;
    if (line == "exit")
      break;
    if (line.empty())
      continue;

    processInput(line, db);
  }

  return 0;
}