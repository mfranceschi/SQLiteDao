#include "DAOLib.hpp"
#include "CommonQueries.hpp"
#include "SQLiteCpp/SQLiteCpp.h"
#include "Types.hpp"
#include <iostream>

// Thanks to https://zetcode.com/db/sqlitec/ for the helping code!

struct CarsTable {
  static const std::string TABLE_NAME;
  static const std::vector<ColumnData> COLUMNS;
  struct Row {
    int Id;
    std::string Name;
    int Price;
  };
  Row readFromQuerySelectStar(SQLite::Statement &statement) {
    return Row{statement.getColumn(0), statement.getColumn(1),
               statement.getColumn(2)};
  }
  static std::string makeCreateTableQuery(bool dropIfExists = true) {
    const std::string createTableQuery =
        CommonQueries::createTable(TABLE_NAME, COLUMNS);
    return dropIfExists
               ? CommonQueries::dropTableIfExists(TABLE_NAME) + createTableQuery
               : createTableQuery;
  }

  static std::string makeInsertQuery() {
    return "INSERT INTO Cars VALUES(?1, ?2, ?3);";
  }

  static void bindRowToInsertStatement(const Row &row,
                                       SQLite::Statement &statement) {
    statement.bind(1, row.Id);
    statement.bind(2, row.Name);
    statement.bind(3, row.Price);
  }

  static std::vector<Row> selectAll(SQLite::Database &db) {
    std::vector<Row> result;

    std::string sql = CommonQueries::selectAll(TABLE_NAME);
    SQLite::Statement query(db, sql);
    while (query.executeStep()) {
      result.push_back(
          Row{query.getColumn(0), query.getColumn(1), query.getColumn(2)});
    }
    return result;
  }
};
const std::string CarsTable::TABLE_NAME = "Cars";
const std::vector<ColumnData> CarsTable::COLUMNS = std::vector<ColumnData>(
    {ColumnData{"Id", ColumnType::INT}, ColumnData{"Name", ColumnType::TEXT},
     ColumnData{"Price", ColumnType::INT}});

void quickstart() {
  SQLite::Database db("aaa.sqlite",
                      SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE);
  db.exec(CarsTable::makeCreateTableQuery());

  SQLite::Statement insertStatement(db, CarsTable::makeInsertQuery());
  for (const CarsTable::Row &row :
       {CarsTable::Row{1, "Audi", 52642}, CarsTable::Row{2, "Mercedes", 57127},
        CarsTable::Row{3, "Skoda", 9000}, CarsTable::Row{4, "Volvo", 29000},
        CarsTable::Row{5, "Bentley", 350000},
        CarsTable::Row{6, "Citroen", 21000}, CarsTable::Row{7, "Hummer", 41400},
        CarsTable::Row{8, "Volkswagen", 21600}}) {
    CarsTable::bindRowToInsertStatement(row, insertStatement);
    insertStatement.exec();
    insertStatement.reset();
  }

  for (const CarsTable::Row &row : CarsTable::selectAll(db)) {
    std::cout << "- Car #" << row.Id << " \"" << row.Name
              << "\" (price: " << row.Price << ")." << std::endl;
  }
}
