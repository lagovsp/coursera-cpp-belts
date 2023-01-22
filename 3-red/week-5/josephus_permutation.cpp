#include "test_runner.h"

#include <cstdint>
#include <iterator>
#include <numeric>
#include <vector>
#include <list>
#include <iostream>

using namespace std;

template<typename RandomIt>
void MakeJosephusPermutation(RandomIt first,
							 RandomIt last,
							 uint32_t step_size) {
  list<typename RandomIt::value_type> pool;
  auto it = first;
  for (size_t i = 0; i < last - first; ++i) {
	pool.push_back(move(*(it++)));
  }
  auto pit = pool.begin();
  while (!pool.empty()) {
	*(first++) = move(*pit);
	pit = pool.erase(pit);
	if (pit == pool.end()) {
	  pit = pool.begin();
	}
	if (pool.empty()) {
	  break;
	}
	for (size_t i = 0; i < step_size - 1; ++i) {
	  if (++pit == pool.end()) {
		pit = pool.begin();
	  }
	}
  }
}

vector<int> MakeTestVector() {
  vector<int> numbers(10);
  iota(begin(numbers), end(numbers), 0);
  return numbers;
}

void TestIntVector() {
  const vector<int> numbers = MakeTestVector();
  {
	vector<int> numbers_copy = numbers;
	MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 1);
	ASSERT_EQUAL(numbers_copy, numbers);
  }
  {
	vector<int> numbers_copy = numbers;
	MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 3);
	ASSERT_EQUAL(numbers_copy, vector<int>({0, 3, 6, 9, 4, 8, 5, 2, 7, 1}));
  }
}

struct NoncopyableInt {
  int value;

  NoncopyableInt(int value) : value(value) {}

  NoncopyableInt(const NoncopyableInt &) = delete;
  NoncopyableInt &operator=(const NoncopyableInt &) = delete;

  NoncopyableInt(NoncopyableInt &&) = default;
  NoncopyableInt &operator=(NoncopyableInt &&) = default;
};

bool operator==(const NoncopyableInt &lhs, const NoncopyableInt &rhs) {
  return lhs.value == rhs.value;
}

ostream &operator<<(ostream &os, const NoncopyableInt &v) {
  return os << v.value;
}

void TestAvoidsCopying() {
  vector<NoncopyableInt> numbers;
  numbers.push_back({1});
  numbers.push_back({2});
  numbers.push_back({3});
  numbers.push_back({4});
  numbers.push_back({5});

  MakeJosephusPermutation(begin(numbers), end(numbers), 2);

  vector<NoncopyableInt> expected;
  expected.push_back({1});
  expected.push_back({3});
  expected.push_back({5});
  expected.push_back({4});
  expected.push_back({2});

  ASSERT_EQUAL(numbers, expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestIntVector);
  RUN_TEST(tr, TestAvoidsCopying);

  return 0;
}
