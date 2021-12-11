#include "DAOLib.hpp"
#include <cstdio>
#include <functional>
#include <stdexcept>
#include <vector>

// Thanks to https://zetcode.com/db/sqlitec/ for the helping code!

#include "SQLiteCpp/SQLiteCpp.h"

void quickstart() {
  SQLite::Database db("aaa.sqlite", SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE);

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
  db.exec(sql);

  std::string sql2 = "SELECT * FROM Cars";
  SQLite::Statement query2(db, sql2);
  while (query2.executeStep()) {
    std::string a = query2.getColumn(0);
    std::string b = query2.getColumn(1);
    std::string c = query2.getColumn(2);
    for (const std::string &string :
         {query2.getColumn(0), query2.getColumn(1), query2.getColumn(2)}) {
      printf("- %s ", string.c_str());
    }
    printf("\n");
  }
}
