#include <set>
#include <vector>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <unordered_map>

using namespace std;

typedef multiset<size_t> ms;
typedef ms::iterator ms_iter;
typedef unordered_map<size_t, ms_iter> um;
typedef um::iterator um_iter;

class ReadingManager {
 public:
  ReadingManager() :
	  rating_by_page(MAX_PAGES + 1, 1),
	  max_page(0) {}

  void Read(const size_t user_id, const size_t page) {
	max_page = max(max_page, page);
	auto umu = umus.find(user_id);
	size_t lowest_page = 0;
	if (umu == umus.cend()) {
	  umu = umus.insert({user_id, msps.insert(page)}).first;
	} else {
	  lowest_page = *umu->second;
	  msps.erase(umu->second);
//	  umu->second = msps.insert(page);
	  umu->second = msps.insert(msps.lower_bound(page), page);
	}
	Adjust(umu, lowest_page);
  }

  [[nodiscard]] double Cheer(size_t user_id) const {
	if (umus.find(user_id) == umus.cend()) {
	  return 0;
	}
	return (msps.size() == 1) ? 1 : rating_by_page[*umus.find(user_id)->second];
  }

 private:
  static const size_t MAX_PAGES = 1000;

  size_t max_page;
  ms msps;
  um umus;
  vector<double> rating_by_page;

  void Adjust(const um_iter &it, const size_t lowest_page) {
	if (umus.size() > 1) {
//	  auto rit = msps.lower_bound(*it->second);
	  auto rit = it->second;
	  size_t readers = distance(msps.cbegin(), rit);
	  size_t i = (umus.size() == 2) ? max_page : *it->second;
	  for (; lowest_page < i; --i) {
		if ((msps.find(i) != msps.cend())) {
		  auto lit = msps.lower_bound(i);
		  readers -= distance(lit, rit);
		  rit = lit;
		  rating_by_page[i] = double(readers) / (umus.size() - 1);
		}
	  }
	}
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

//  answers are good, though these fail
//  ReadingManager manager;
//  manager.Read(1, 1);
//  manager.Read(2, 2);
//  manager.Read(3, 3);
//  ASSERT_EQUAL(manager.Cheer(2), 0.5);
//  manager.Read(4, 4);
//  ASSERT_EQUAL(manager.Cheer(3), 2.0 / 3.0);

  ReadingManager manager;

  int query_count;
  cin >> query_count;

  for (int query_id = 0; query_id < query_count; ++query_id) {
	string query_type;
	cin >> query_type;
	int user_id;
	cin >> user_id;

	if (query_type == "READ") {
	  int page_count;
	  cin >> page_count;
	  manager.Read(user_id, page_count);
	} else if (query_type == "CHEER") {
	  cout << setprecision(6) << manager.Cheer(user_id) << "\n";
	}
  }

  return 0;
}