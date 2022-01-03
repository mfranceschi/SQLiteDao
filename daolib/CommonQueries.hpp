#pragma once

#include "Types.hpp"

std::string enquote(const std::string &text);

namespace CommonQueries {
std::string dropTableIfExists(const std::string &tableName);

std::string createTable(const std::string &tableName,
                        const ColumnList_t &columnList);

std::string selectAll(const std::string &tableName);

std::string listTables();
} // namespace CommonQueries
