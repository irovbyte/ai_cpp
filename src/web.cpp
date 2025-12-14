#include "web.h"
#include "format.h"
#include <algorithm>
#include <cstdlib>
#include <curl/curl.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                            void *userp) {
  auto *output = static_cast<std::string *>(userp);
  size_t totalSize = size * nmemb;
  output->append(static_cast<const char *>(contents), totalSize);
  return totalSize;
}

std::string fetchFromWeb(const std::string &query) {
  CURL *curl = nullptr;
  CURLcode res;
  std::string response;

  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();

  if (!curl) {
    curl_global_cleanup();
    return formatError("curl init failed");
  }

  const char *keyEnv = getenv("GOOGLE_API_KEY");
  const char *cxEnv = getenv("GOOGLE_CX");
  std::string apiKey = keyEnv ? keyEnv : "";
  std::string cx = cxEnv ? cxEnv : "";

  char *encoded = curl_easy_escape(curl, query.c_str(), query.size());
  std::string url =
      "https://www.googleapis.com/customsearch/v1?q=" + std::string(encoded) +
      "&key=" + apiKey + "&cx=" + cx;
  curl_free(encoded);

  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
  curl_easy_setopt(curl, CURLOPT_USERAGENT, "cpp-bot/1.0");
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

  res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    response = std::string("curl error: ") + curl_easy_strerror(res);
  }

  curl_easy_cleanup(curl);
  curl_global_cleanup();

  try {
    // std::cout << "RAW RESPONSE:\n" << response << std::endl;
    json j = json::parse(response);
    if (j.contains("items") && j["items"].is_array() && !j["items"].empty()) {
      std::string result = "Вот несколько подробных ответов:\n\n";
      int count = std::min<int>(j["items"].size(), 3);
      for (int i = 0; i < count; i++) {
        std::string title = j["items"][i].value("title", "");
        std::string snippet = j["items"][i].value("snippet", "");
        std::string link = j["items"][i].value("link", "");
        result += formatResult(i + 1, title, snippet, link);
      }
      return result;
    } else {
      return "Нет результатов.";
    }
  } catch (...) {
    return formatError("Ответ не в формате JSON");
  }
}