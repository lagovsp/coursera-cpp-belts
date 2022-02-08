#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <unordered_set>
#include <stdexcept>

using namespace std;

template<class T>
class ObjectPool {
 public:
  T *Allocate() {
	if (!free.empty()) {
	  auto buf = free.front();
	  free.pop();
	  active.insert(buf);
	  return buf;
	}
	auto created = new T;
	active.insert(created);
	return created;
  }

  T *TryAllocate() {
	if (free.empty()) {
	  return nullptr;
	}
	auto buf = free.front();
	free.pop();
	active.insert(buf);
	return buf;
  }

  void Deallocate(T *obj) {
	if (obj == nullptr) {
	  throw invalid_argument("cannot deallocate nullptr");
	}
	auto f = active.find(obj);
	if (f != active.end()) {
	  active.erase(f);
	  free.push(obj);
	} else {
	  throw invalid_argument("object is not allocated");
	}
  }

  ~ObjectPool() {
	while (!free.empty()) {
	  auto buf = free.front();
	  free.pop();
	  delete buf;
	}
	for (auto it = active.begin(); it != active.end(); ++it) {
	  delete *it;
	}
  }

 private:
  queue<T *> free;
  unordered_set<T *> active;
};

void TestObjectPool() {
  ObjectPool<string> pool;

  auto p1 = pool.Allocate();
  auto p2 = pool.Allocate();
  auto p3 = pool.Allocate();

  *p1 = "first";
  *p2 = "second";
  *p3 = "third";

  pool.Deallocate(p2);
  ASSERT_EQUAL(*pool.Allocate(), "second");

  pool.Deallocate(p3);
  pool.Deallocate(p1);
  ASSERT_EQUAL(*pool.Allocate(), "third");
  ASSERT_EQUAL(*pool.Allocate(), "first");

  pool.Deallocate(p1);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestObjectPool);

  return 0;
}
