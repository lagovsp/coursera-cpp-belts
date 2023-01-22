#include "test_runner.h"
#include <vector>
#include <cmath>
#include <future>

using namespace std;

int64_t CalculateMatrixSum(const vector<vector<int>> &matrix) {
  int64_t k = floor(matrix.size() / 4);
  int64_t ans = 0;
  vector<future<int64_t>> futures;
  size_t start = 0, end = 0;
  for (size_t i = 0; i < 4; ++i) {
	if (i == 3) {
	  end = matrix.size();
	} else {
	  end += k;
	}
	futures.push_back(async([&matrix, start, end]() {
	  int64_t sum = 0, s = start;
	  for (; s < end; ++s) {
		for (const auto &el: matrix[s]) {
		  sum += el;
		}
	  }
	  return sum;
	}));
	start = end;
  }
  for (auto &f: futures) {
	ans += f.get();
  }
  return ans;
}

void TestCalculateMatrixSum() {
  const vector<vector<int>> matrix = {
	  {1, 2, 3, 4},
	  {5, 6, 7, 8},
	  {9, 10, 11, 12},
	  {13, 14, 15, 16}
  };
  ASSERT_EQUAL(CalculateMatrixSum(matrix), 136);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestCalculateMatrixSum);

  return 0;
}
