#include "test_runner.h"

using namespace std;

template <typename T> class Table {
private:
  std::vector<std::vector<T>> table;

public:
  Table(size_t w, size_t h) { Resize(w, h); }

  std::vector<T> &operator[](const size_t ind) { return table[ind]; }
  const std::vector<T> &operator[](const size_t ind) const {
    return table[ind];
  }

  void Resize(size_t w, size_t h) {
    table.resize(w);
    for (auto &item : table) {
      item.resize(h);
    }
  }

  [[nodiscard]] std::pair<size_t, size_t> Size() const {
    return {table.size(), (!table.empty()) ? table[0].size() : 0};
  }
};

void TestTable() {
  Table<int> t(1, 1);
  ASSERT_EQUAL(t.Size().first, 1u);
  ASSERT_EQUAL(t.Size().second, 1u);
  t[0][0] = 42;
  ASSERT_EQUAL(t[0][0], 42);
  t.Resize(3, 4);
  ASSERT_EQUAL(t.Size().first, 3u);
  ASSERT_EQUAL(t.Size().second, 4u);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestTable);

  return 0;
}
