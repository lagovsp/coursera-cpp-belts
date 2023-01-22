#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <set>
#include <utility>
#include <vector>

using namespace std;

template<typename T>
class PriorityCollection {
 public:
  using Node = pair<int, size_t>;
  using Id = size_t;

  Id Add(T object) {
	return add(object);
  }

  template<typename ObjInputIt, typename IdOutputIt>
  void Add(ObjInputIt range_begin, ObjInputIt range_end,
		   IdOutputIt ids_begin) {
	for (auto it = range_begin; it != range_end; ++it) {
	  *(ids_begin++) = add(*it); // ?
	}
  }

  [[nodiscard]] bool IsValid(Id id) const {
	if (id > vec.size() - 1) {
	  return false;
	}
	return (vec[id].second != priors.end());
  }

  [[nodiscard]] const T &Get(Id id) const {
	return vec[id].first;
  }

  void Promote(Id id) {
	auto i = vec[id].second;
	auto node = *i;
	priors.erase(i);
	++node.first;
	itByid(id, priors.insert(node).first);
  }

  [[nodiscard]] pair<const T &, int> GetMax() const {
	auto id = (--priors.end())->second;
	return {vec[id].first, vec[id].second->first};
  }

  pair<T, int> PopMax() {
	auto id = (--priors.end())->second;
	pair<T, int> element = {move(vec[id].first), vec[id].second->first};
	priors.erase(vec[id].second);
	itByid(id, priors.end());
	return element;
  }

 private:
  set<Node> priors;
  vector<pair<T, set<Node>::iterator>> vec;

  Id add(T &element) {
	vec.push_back({move(element), priors.end()});
	vec[vec.size() - 1].second = priors.insert({0, vec.size() - 1}).first;
	return vec.size() - 1;
  }

  void itByid(Id id, set<Node>::iterator it) { vec[id].second = it; }
};

class StringNonCopyable : public string {
 public:
  using string::string;
  StringNonCopyable(const StringNonCopyable &) = delete;
  StringNonCopyable(StringNonCopyable &&) = default;
  StringNonCopyable &operator=(const StringNonCopyable &) = delete;
  StringNonCopyable &operator=(StringNonCopyable &&) = default;
};

void TestNoCopy() {
  PriorityCollection<StringNonCopyable> strings;
  const auto white_id = strings.Add("white");
  const auto yellow_id = strings.Add("yellow");
  const auto red_id = strings.Add("red");

  strings.Promote(yellow_id);
  for (int i = 0; i < 2; ++i) {
	strings.Promote(red_id);
  }
  strings.Promote(yellow_id);
  {
	const auto item = strings.PopMax();
	ASSERT_EQUAL(item.first, "red");
	ASSERT_EQUAL(item.second, 2);
  }
  {
	const auto item = strings.PopMax();
	ASSERT_EQUAL(item.first, "yellow");
	ASSERT_EQUAL(item.second, 2);
  }
  {
	const auto item = strings.PopMax();
	ASSERT_EQUAL(item.first, "white");
	ASSERT_EQUAL(item.second, 0);
  }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestNoCopy);

  return 0;
}
