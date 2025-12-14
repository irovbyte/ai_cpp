#include "ai.h"
#include "utils.h"
#include "web.h"
#include <iostream>
#include <sstream>
#include <vector>

void processInput(const std::string &userInput, MemoryDB &db) {
  std::string normInput = normalize(userInput);

  auto results = db.search(normInput);
  if (!results.empty()) {
    std::cout << "ИИ: У меня уже есть ответ: \"" << results[0].value << "\""
              << std::endl;
    return;
  }

  if (normInput.size() > 2) {
    std::string sub = normInput.substr(0, normInput.size() / 2);
    auto similar = db.search(sub);
    if (!similar.empty()) {
      std::cout << "ИИ: Я нашёл похожие ответы:" << std::endl;
      for (size_t i = 0; i < similar.size() && i < 5; i++) {
        std::cout << i + 1 << ". " << similar[i].key << " >> "
                  << similar[i].value << std::endl;
      }
      return;
    }
  }

  std::string webResult = fetchFromWeb(normInput);
  if (!webResult.empty() && webResult.find("Ошибка") == std::string::npos) {
    if (webResult != "Нет результатов.") {
      std::cout << "ИИ: Я нашёл ответ в интернете:\n" << webResult << std::endl;
      db.save(normInput, webResult);
      return;
    }
  }

  std::cout << "ИИ: Я не знаю ответа. Обучи меня сам!" << std::endl;
  std::cout << "Напиши правильный ответ:" << std::endl;
  std::string correctAnswer;
  std::getline(std::cin, correctAnswer);

  db.save(normInput, correctAnswer);

  std::istringstream iss(normInput);
  std::string word;
  while (iss >> word) {
    db.save(word, correctAnswer);
  }

  std::cout << "ИИ: Запомнил новый ответ!" << std::endl;
}