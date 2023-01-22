#include "test_runner.h"
#include <algorithm>
#include <memory>
#include <vector>

using namespace std;

template<typename RandomIt>
void MergeSort(RandomIt rBeg, RandomIt rEnd) {
  using V = typename RandomIt::value_type;
  using vec = typename vector<V>::iterator;
  using pair = pair<vec, vec>;
  if (distance(rBeg, rEnd) < 2) {
	return;
  }
  vector<V> v(make_move_iterator(rBeg), make_move_iterator(rEnd));
  auto first = v.begin();
  size_t len = v.size() / 3;
  vector<pair> iters;
  for (size_t i = 0; i < 3; ++i) {
	iters.push_back({first, first + len});
	MergeSort(first, first + len);
	first += len;
  }
  vector<V> temp;
  merge(make_move_iterator(iters[0].first),
		make_move_iterator(iters[0].second),
		make_move_iterator(iters[1].first),
		make_move_iterator(iters[1].second),
		back_inserter(temp));
  merge(make_move_iterator(temp.begin()),
		make_move_iterator(temp.end()),
		make_move_iterator(iters[2].first),
		make_move_iterator(iters[2].second),
		rBeg);
}

void TestIntVector() {
  vector<int> numbers = {6, 1, 3, 9, 1, 9, 8, 12, 1};
  MergeSort(begin(numbers), end(numbers));
  ASSERT(is_sorted(begin(numbers), end(numbers)));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestIntVector);

  return 0;
}
