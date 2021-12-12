#pragma once

#include <string>
#include <vector>

enum class ColumnType { TEXT, INT };

struct ColumnData {
  std::string name;
  ColumnType type;
};

using ColumnList_t = std::vector<ColumnData>;

template <class Row_t> struct Table {};
