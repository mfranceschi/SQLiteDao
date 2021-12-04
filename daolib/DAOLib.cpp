#include "DAOLib.hpp"
#include "sqlite3.h"
#include <cstdio>
#include <functional>
#include <stdexcept>
#include <vector>

// Thanks to https://zetcode.com/db/sqlitec/ for the helping code!

class ConnectedDatabase {
private:
  sqlite3 *db;
  ConnectedDatabase(sqlite3 *db) : db(db) {}
  friend class DatabaseConnectionFactory;

  static int callbackHelper(void *modernCallbackPointer, int nbColumns,
                            char **columnsContent, char **columnsName) {
    const auto &callback =
        *static_cast<ModernCallback_t *>(modernCallbackPointer);

    std::vector<std::string> resultsAsVector(nbColumns);
    for (int i = 0; i < nbColumns; i++) {
      resultsAsVector[i] = columnsContent[i];
    }

    try {
      callback(resultsAsVector);
      return 0;
    } catch (const std::exception &) {
      return 1;
    }
  }

public:
  ~ConnectedDatabase() { sqlite3_close(db); }

  void runSimpleQuery(const std::string &query) {
    char *errMessage = nullptr;
    int rc = sqlite3_exec(db, query.c_str(), 0, 0, &errMessage);
    if (rc != SQLITE_OK) {
      throw std::runtime_error(errMessage);
    }
  }

  using ModernCallback_t =
      std::function<void(const std::vector<std::string> &)>;

  void runWithCallback(const std::string &query,
                       ModernCallback_t modernCallback) {
    char *errMessage = nullptr;
    int rc = sqlite3_exec(db, query.c_str(), callbackHelper, &modernCallback,
                          &errMessage);
    if (rc != SQLITE_OK) {
      throw std::runtime_error(errMessage);
    }
  }
};

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

int callback_display_cars(void *_unused, int argc, char **argv,
                          char **colName) {
  for (int i = 0; i < argc; i++) {

    printf("%s = %s\n", colName[i], argv[i] ? argv[i] : "NULL");
  }
  return 0;
}

void quickstart() {
  ConnectedDatabase db = DatabaseConnectionFactory::open();

  std::string sql = "DROP TABLE IF EXISTS Cars;"
                    "CREATE TABLE Cars(Id INT, Name TEXT, Price INT);"
                    "INSERT INTO Cars VALUES(1, 'Audi', 52642);"
                    "INSERT INTO Cars VALUES(2, 'Mercedes', 57127);"
                    "INSERT INTO Cars VALUES(3, 'Skoda', 9000);"
                    "INSERT INTO Cars VALUES(4, 'Volvo', 29000);"
                    "INSERT INTO Cars VALUES(5, 'Bentley', 350000);"
                    "INSERT INTO Cars VALUES(6, 'Citroen', 21000);"
                    "INSERT INTO Cars VALUES(7, 'Hummer', 41400);"
                    "INSERT INTO Cars VALUES(8, 'Volkswagen', 21600);";
  db.runSimpleQuery(sql);

  std::string sql2 = "SELECT * FROM Cars";
  db.runWithCallback(sql2, [](const std::vector<std::string> &row) {
    for (const std::string &string : row) {
      printf("- %s ", string.c_str());
    }
    printf("\n");
  });
}

void quickstart2() {
  sqlite3 *db;
  char *err_msg = 0;

  int rc = sqlite3_open(":memory:", &db);
  const char *sql = "DROP TABLE IF EXISTS Cars;"
                    "CREATE TABLE Cars(Id INT, Name TEXT, Price INT);"
                    "INSERT INTO Cars VALUES(1, 'Audi', 52642);"
                    "INSERT INTO Cars VALUES(2, 'Mercedes', 57127);"
                    "INSERT INTO Cars VALUES(3, 'Skoda', 9000);"
                    "INSERT INTO Cars VALUES(4, 'Volvo', 29000);"
                    "INSERT INTO Cars VALUES(5, 'Bentley', 350000);"
                    "INSERT INTO Cars VALUES(6, 'Citroen', 21000);"
                    "INSERT INTO Cars VALUES(7, 'Hummer', 41400);"
                    "INSERT INTO Cars VALUES(8, 'Volkswagen', 21600);";
  rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

  const char *sql2 = "SELECT * FROM Cars";

  rc = sqlite3_exec(db, sql2, callback_display_cars, 0, &err_msg);

  rc = sqlite3_close(db);
}
