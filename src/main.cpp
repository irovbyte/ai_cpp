#include "ai.h"
#include "memory.h"
#include "math.h"
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

    // 🔢 Проверка на математику прямо в main
    if (isMathExpression(line)) {
      try {
        double result = evalExpression(line);
        std::string answer = std::to_string(result);
        std::cout << "ИИ: Результат вычисления: " << answer << std::endl;
        db.save(line, answer); // сохраняем в память
        continue;              // не идём в processInput
      } catch (const std::exception &e) {
        std::cout << "ИИ: Ошибка вычисления: " << e.what() << std::endl;
        continue;
      }
    }

    // 🧠 Остальная логика — память, веб, обучение
    processInput(line, db);
  }

  return 0;
}