#include <iostream>
#include <unordered_map>
#include <list>

using namespace std;

class Coach {
 public:
  Coach() = default;

  void call(size_t n, size_t b) {
	auto f = um.find(b);
	if (f == um.end()) {
	  toBack(n);
	  return;
	}
	beforeThis(n, f->second);
  }

  void revealList() const {
	for (const auto &pos: l) {
	  cout << pos << endl;
	}
  }

 private:
  unordered_map<size_t, list<size_t>::iterator> um;
  list<size_t> l;

  void beforeThis(size_t s, list<size_t>::iterator it) {
	um[s] = l.insert(it, s);
  }

  void toBack(size_t s) {
	um[s] = l.insert(l.end(), s);
  }
};

int main() {
  Coach c;
  int query_count;
  cin >> query_count;

  for (size_t query_id = 0; query_id < query_count; ++query_id) {
	int n, b;
	cin >> n >> b;
	c.call(n, b);
  }
  c.revealList();

  return 0;
}