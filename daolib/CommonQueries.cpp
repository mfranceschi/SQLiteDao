
#include "CommonQueries.hpp"

#include <algorithm>

#include "magic_enum.hpp"
#include <fmt/core.h>
#include <fmt/format.h>

std::string enquote(const std::string &text) {
  return fmt::format("`{}`", text);
};

namespace CommonQueries {
std::string dropTableIfExists(const std::string &tableName) {
  return fmt::format("DROP TABLE IF EXISTS {}; ", enquote(tableName));
}

std::string createTable(const std::string &tableName,
                        const ColumnList_t &columnList) {
  std::vector<std::string> columnDeclarations(columnList.size());
  std::transform(columnList.cbegin(), columnList.cend(),
                 columnDeclarations.begin(), [](const ColumnData &columnData) {
                   return fmt::format("{} {}", columnData.name,
                                      magic_enum::enum_name(columnData.type));
                 });

  return fmt::format("CREATE TABLE {} ({}); ", tableName,
                     fmt::join(columnDeclarations, ", "));
}

std::string selectAll(const std::string &tableName) {
  return fmt::format("SELECT * FROM {}; ", enquote(tableName));
}

std::string listTables() {
  return "SELECT name FROM sqlite_schema WHERE type = 'table' ORDER BY NAME; ";
}
} // namespace CommonQueries
