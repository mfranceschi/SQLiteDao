#include "TheHeader.hpp"



class CarsTable : public AbstractTable {
protected: 
static constexpr const char* CREATE_TABLE_STATEMENT = "CREATE TABLE Cars (id INTEGER, name TEXT, price INTEGER, PRIMARY KEY id); "; 
const char* getCreateTableStatement() const override { return CREATE_TABLE_STATEMENT; }

static constexpr const char* DROP_TABLE_IF_EXISTS_STATEMENT = "DROP TABLE IF EXISTS Cars; "; 
const char* getDropTableIfExistsStatement() const override { return DROP_TABLE_IF_EXISTS_STATEMENT; }

static constexpr const char* DELETE_TABLE_STATEMENT = "DROP TABLE IF EXISTS Cars; "; 
const char* getDeleteTableStatement() const override { return DELETE_TABLE_STATEMENT; }

};

