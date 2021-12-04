#include "IncludeSQLite.hpp"
#include <functional>
#include <stdexcept>
#include <string>
#include <vector>

class ConnectedDatabase {
  // Only a factory can instantiate me.
  friend class DatabaseConnectionFactory;

public:
  // Releases resources.
  ~ConnectedDatabase();

  // Just runs the query and returns when finished.
  void runSimpleQuery(const std::string &query);

  using ModernCallback_t =
      std::function<void(const std::vector<std::string> &)>;

  // Runs the query and, for each resulting row, calls the callback function.
  // If the callback throws anything then it is intercepted and no more SQL code
  // is run.
  void runWithCallback(const std::string &query,
                       ModernCallback_t modernCallback);

private:
  sqlite3 *db;
  ConnectedDatabase(sqlite3 *db);

  friend class DatabaseConnectionFactory;

  static int callbackHelper(void *modernCallbackPointer, int nbColumns,
                            char **columnsContent, char **columnsName);
};
