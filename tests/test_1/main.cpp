#include "a.hpp"

int main() {
  MfDao::open(":memory:").getTable<custom_ns::CarsTable>().createTable(true);
  return 0;
}
