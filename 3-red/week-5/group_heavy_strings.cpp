#include "test_runner.h"

#include <algorithm>
#include <string>
#include <vector>

using namespace std;

template<typename String>
using Group = vector<String>;

template<typename String>
using Char = typename String::value_type;

template<typename String>
using Char = typename String::value_type;

template<typename String>
String ComputeKey(const String &s) {
  String sKey(s);
  sort(sKey.begin(), sKey.end());
  sKey.erase(unique(sKey.begin(), sKey.end()), sKey.end());
  return sKey;
}

template<typename String>
vector<Group<String>> GroupHeavyStrings(vector<String> strings) {
  map<String, Group<String>> mapKeyToGroup;
  for (auto &s: strings) {
	mapKeyToGroup[ComputeKey(s)].push_back(move(s));
  }
  vector<Group<String>> res;
  res.reserve(mapKeyToGroup.size());
  for (auto &g: mapKeyToGroup) {
	res.push_back(move(g.second));
  }
  return res;
}

void TestGroupingABC() {
  vector<string> strings = {"caab", "abc", "cccc", "bacc", "c"};
  auto groups = GroupHeavyStrings(strings);
  ASSERT_EQUAL(groups.size(), 2);
  sort(begin(groups), end(groups));
  ASSERT_EQUAL(groups[0], vector<string>({"caab", "abc", "bacc"}));
  ASSERT_EQUAL(groups[1], vector<string>({"cccc", "c"}));
}

void TestGroupingReal() {
  vector<string> strings =
	  {"law", "port", "top", "laptop", "pot", "paloalto", "wall", "awl"};
  auto groups = GroupHeavyStrings(strings);
  ASSERT_EQUAL(groups.size(), 4);
  sort(begin(groups), end(groups));
  ASSERT_EQUAL(groups[0], vector<string>({"laptop", "paloalto"}));
  ASSERT_EQUAL(groups[1], vector<string>({"law", "wall", "awl"}));
  ASSERT_EQUAL(groups[2], vector<string>({"port"}));
  ASSERT_EQUAL(groups[3], vector<string>({"top", "pot"}));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestGroupingABC);
  RUN_TEST(tr, TestGroupingReal);

  return 0;
}
