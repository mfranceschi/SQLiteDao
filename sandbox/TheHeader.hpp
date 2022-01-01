#include <string>

class AbstractTable {
public:
  virtual void createTable(bool dropIfExists) {
    std::string statement;
    if (dropIfExists) {
      statement += this->getDropTableIfExistsStatement();
    }
    statement += this->getCreateTableStatement();
    // TODO execute statement
  }

  virtual void deleteTable() {
    std::string statement = this->getDeleteTableStatement();
  }

protected:
  virtual const char *getCreateTableStatement() const = 0;
  virtual const char *getDropTableIfExistsStatement() const = 0;
  virtual const char *getDeleteTableStatement() const = 0;
};
