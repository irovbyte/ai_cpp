#pragma once
#include <string>
#include <vector>

struct MemoryEntry {
  std::string key;
  std::string value;
};

class MemoryDB {
public:
  MemoryDB(const std::string &dbPath);
  ~MemoryDB();

  bool save(const std::string &key, const std::string &value);
  std::vector<MemoryEntry> search(const std::string &query);

private:
  void init();
  void close();
  void *db;
};