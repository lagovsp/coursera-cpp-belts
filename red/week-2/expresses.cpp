#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <set>

using namespace std;

class RouteManager {
 public:
  void AddRoute(int start, int finish) {
	reach_lists[start].insert(finish);
	reach_lists[finish].insert(start);
  }

  [[nodiscard]] int FindNearestFinish(int start, int fin) const {
	int res = abs(start - fin);
	if (reach_lists.count(start) == 0) {
	  return res;
	}
	const auto &stations = reach_lists.at(start);
	int found = std::numeric_limits<int>::max();
	if (!stations.empty()) {
	  auto lb = stations.lower_bound(fin);
	  if (lb == stations.cbegin()) {
		found = abs(fin - *lb);
	  } else if (lb != stations.cend()) {
		found = min(abs(fin - *lb), abs(fin - *prev(lb)));
	  } else {
		found = abs(fin - *(--lb));
	  }
	}
	return (res < found) ? res : found;
  }

 private:
  map<int, set<int>> reach_lists;
};

int main() {
  RouteManager routes;
  int query_count;
  cin >> query_count;

  for (int query_id = 0; query_id < query_count; ++query_id) {
	string query_type;
	cin >> query_type;
	int start, finish;
	cin >> start >> finish;
	if (query_type == "ADD") {
	  routes.AddRoute(start, finish);
	} else if (query_type == "GO") {
	  cout << routes.FindNearestFinish(start, finish) << "\n";
	}
  }

  return 0;
}
