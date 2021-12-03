#include "DAOLib.hpp"
#include "sqlite3.h"
#include <cstdio>
#include <functional>
#include <stdexcept>

// Thanks to https://zetcode.com/db/sqlitec/ for the helping code!

class ConnectedDatabase {
private:
  sqlite3 *db;
  ConnectedDatabase(sqlite3 *db) : db(db) {}

public:
  static ConnectedDatabase open() {
    sqlite3 *db;
    int rc = sqlite3_open(":memory:", &db);
    if (rc == SQLITE_OK) {
      return ConnectedDatabase(db);
    } else
      throw std::runtime_error("failed to open db!");
  }

  void runSimpleQuery(const std::string &query) {
    char *errMessage = nullptr;
    int rc = sqlite3_exec(db, query.c_str(), 0, 0, &errMessage);
    if (rc != SQLITE_OK) {
      throw std::runtime_error(errMessage);
    }
  }

  static int callbackHelper(void *modernCallbackPointer, int argc, char **argv,
                            char **colName) {
    const auto *callback =
        static_cast<std::function<void(int, char **, char **)> *>(
            modernCallbackPointer);
    try {
      (*callback)(argc, argv, colName);
      return 0;
    } catch (const std::exception &) {
      return 1;
    }
  }

  void
  runWithCallback(const std::string &query,
                  std::function<void(int, char **, char **)> modernCallback) {
    char *errMessage = nullptr;
    int rc = sqlite3_exec(db, query.c_str(), callbackHelper, &modernCallback,
                          &errMessage);
    if (rc != SQLITE_OK) {
      throw std::runtime_error(errMessage);
    }
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
  ConnectedDatabase db = ConnectedDatabase::open();

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
  db.runWithCallback(sql2, [](int argc, char **argv, char **colName) {
    for (int i = 0; i < argc; i++) {
      printf("%s = %s\n", colName[i], argv[i] ? argv[i] : "NULL");
    }
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
