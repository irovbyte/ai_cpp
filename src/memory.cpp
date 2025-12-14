#include "memory.h"
#include <iostream>
#include <sqlite3.h>

MemoryDB::MemoryDB(const std::string &dbPath) : db(nullptr) {
  if (sqlite3_open(dbPath.c_str(), (sqlite3 **)&db) != SQLITE_OK) {
    std::cerr << "Ошибка открытия базы: " << sqlite3_errmsg((sqlite3 *)db)
              << std::endl;
    db = nullptr;
  } else {
    init();
  }
}

MemoryDB::~MemoryDB() { close(); }

void MemoryDB::init() {
  const char *sql = "CREATE TABLE IF NOT EXISTS memory ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "key TEXT,"
                    "value TEXT);";
  char *errMsg = nullptr;
  if (sqlite3_exec((sqlite3 *)db, sql, nullptr, nullptr, &errMsg) !=
      SQLITE_OK) {
    std::cerr << "Ошибка создания таблицы: " << errMsg << std::endl;
    sqlite3_free(errMsg);
  }
}

void MemoryDB::close() {
  if (db)
    sqlite3_close((sqlite3 *)db);
}

bool MemoryDB::save(const std::string &key, const std::string &value) {
  const char *sql = "INSERT INTO memory (key, value) VALUES (?, ?);";
  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2((sqlite3 *)db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    return false;

  sqlite3_bind_text(stmt, 1, key.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 2, value.c_str(), -1, SQLITE_TRANSIENT);

  bool success = (sqlite3_step(stmt) == SQLITE_DONE);
  sqlite3_finalize(stmt);
  return success;
}

std::vector<MemoryEntry> MemoryDB::search(const std::string &query) {
  std::vector<MemoryEntry> results;
  const char *sql =
      "SELECT key, value FROM memory WHERE key LIKE ? OR value LIKE ?;";
  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2((sqlite3 *)db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    return results;

  std::string pattern = "%" + query + "%";
  sqlite3_bind_text(stmt, 1, pattern.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 2, pattern.c_str(), -1, SQLITE_TRANSIENT);

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    MemoryEntry entry;
    entry.key = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
    entry.value = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
    results.push_back(entry);
  }
  sqlite3_finalize(stmt);
  return results;
}