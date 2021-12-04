#include "Database.hpp"

class DatabaseConnectionFactory {
private:
  static ConnectedDatabase open_internal(const char *filename, int flags) {
    sqlite3 *db;
    int rc = sqlite3_open_v2(filename, &db, flags, nullptr);
    if (rc == SQLITE_OK) {
      return ConnectedDatabase(db);
    } else {
      throw std::runtime_error("failed to open db!");
    }
  }

public:
  static ConnectedDatabase open() {
    return open_internal(":memory:",
                         SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
  }

  static ConnectedDatabase open(const std::string &filename) {
    return open_internal(filename.c_str(), SQLITE_OPEN_READWRITE);
  }

  static ConnectedDatabase openOrCreate(const std::string &filename) {
    return open_internal(filename.c_str(),
                         SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
  }
};
