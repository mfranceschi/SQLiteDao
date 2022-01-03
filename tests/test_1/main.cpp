#include "Test_1_Db.hpp"

int main() {
  MfDao dao(MfDao::open(":memory:"));

  dao.getTable<custom_ns::CarsTable>().createTable(true);
  dao.getTable<custom_ns::CarsTable>().countAll();
  return 0;
}
