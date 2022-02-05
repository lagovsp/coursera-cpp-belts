#include "test_runner.h"

using namespace std;

template <typename T> class Table {
private:
  std::vector<std::vector<T>> table;

public:
  Table(size_t w, size_t h) {
    w = (h == 0) ? 0 : w;
    h = (w == 0) ? 0 : h;
    table.resize(w);
    for (auto itW = table.begin(); itW != table.end(); ++itW) {
      itW->resize(h);
    }
  }

  std::vector<T> &operator[](const size_t ind) { return table[ind]; }
  const std::vector<T> &operator[](const size_t ind) const {
    return table[ind];
  }

  void Resize(size_t w, size_t h) {
    std::vector<std::vector<T>> temp = std::move(table);
    w = (h == 0) ? 0 : w;
    h = (w == 0) ? 0 : h;

    auto tempW = temp.size();
    auto tempH = (!temp.empty()) ? temp[0].size() : 0;

    table.resize(w);
    for (size_t itW = 0; itW < table.size(); ++itW) {
      table[itW].resize(h);
      for (size_t itH = 0; itH < table[itW].size(); ++itH) {
        if (itW < tempW && itH < tempH) {
          table[itW][itH] = std::move(temp[itW][itH]);
        } else {
          table[itW][itH] = T();
        }
      }
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
