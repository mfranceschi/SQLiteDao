#include "SQLiteCpp/SQLiteCpp.h"
#include <string>

class AbstractTable {
public:
  virtual void createTable(bool dropIfExists) {
    std::string statement;
    if (dropIfExists) {
      statement += this->getDropTableIfExistsStatement();
    }
    statement += this->getCreateTableStatement();

    std::printf("Sql statement: %s\n", statement.c_str());

    db.exec(statement);
  }

  virtual void deleteTable() {
    std::string statement = this->getDeleteTableStatement();

    db.exec(statement);
  }

protected:
  AbstractTable(SQLite::Database &db) : db(db) {}

  virtual const char *getCreateTableStatement() const = 0;
  virtual const char *getDropTableIfExistsStatement() const = 0;
  virtual const char *getDeleteTableStatement() const = 0;

  SQLite::Database &db;
};

class MfDao {
public:
  static MfDao open(const std::string &file) {
    SQLite::Database::getHeaderInfo(file);
    return MfDao(
        SQLite::Database(file, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE));
  }

  explicit MfDao(SQLite::Database &&db) : db(std::move(db)) {}

  template <typename Table_t> Table_t getTable() {
    static_assert(std::is_base_of_v<AbstractTable, Table_t>,
                  "Table type must inherit from AbstractTable.");
    return Table_t(db);
  }

private:
  SQLite::Database db;
};
