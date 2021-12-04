#include "Database.hpp"

#include "IncludeSQLite.hpp"
#include <functional>
#include <stdexcept>
#include <string>
#include <vector>

ConnectedDatabase::ConnectedDatabase(sqlite3 *db) : db(db) {}

int ConnectedDatabase::callbackHelper(void *modernCallbackPointer,
                                      int nbColumns, char **columnsContent,
                                      char **columnsName) {
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

ConnectedDatabase::~ConnectedDatabase() { sqlite3_close(db); }

void ConnectedDatabase::runSimpleQuery(const std::string &query) {
  char *errMessage = nullptr;
  int rc = sqlite3_exec(db, query.c_str(), 0, 0, &errMessage);
  if (rc != SQLITE_OK) {
    throw std::runtime_error(errMessage);
  }
}

void ConnectedDatabase::runWithCallback(const std::string &query,
                                        ModernCallback_t modernCallback) {
  char *errMessage = nullptr;
  int rc = sqlite3_exec(db, query.c_str(), callbackHelper, &modernCallback,
                        &errMessage);
  if (rc != SQLITE_OK) {
    throw std::runtime_error(errMessage);
  }
}
