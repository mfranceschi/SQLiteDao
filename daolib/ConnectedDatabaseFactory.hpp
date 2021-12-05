#include "Database.hpp"
#include "SQLiteError.hpp"

class DatabaseConnectionFactory {
private:
  static ConnectedDatabase open_internal(const char *filename,
                                         int flags = SQLITE_OPEN_READWRITE |
                                                     SQLITE_OPEN_CREATE) {
    sqlite3 *db;
    int rc = sqlite3_open_v2(filename, &db, flags, nullptr);
    if (rc == SQLITE_OK) {
      return ConnectedDatabase(db);
    } else {
      throw SQLiteError("failed to open db!", sqlite3_errstr(rc), rc);
    }
  }

public:
  static ConnectedDatabase openInMemoy() { return open_internal(":memory:"); }

  static ConnectedDatabase openTempFile() { return open_internal(""); }

  static ConnectedDatabase openFile(const std::string &filename) {
    return open_internal(filename.c_str(), SQLITE_OPEN_READWRITE);
  }

  static ConnectedDatabase openOrCreate(const std::string &filename) {
    return open_internal(filename.c_str());
  }
};
