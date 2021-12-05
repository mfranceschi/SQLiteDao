#include "DAOLib.hpp"
#include "ConnectedDatabaseFactory.hpp"
#include "IncludeSQLite.hpp"
#include <cstdio>
#include <functional>
#include <stdexcept>
#include <vector>

// Thanks to https://zetcode.com/db/sqlitec/ for the helping code!

void quickstart() {
  ConnectedDatabase db = DatabaseConnectionFactory::openFile("xyx");

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
