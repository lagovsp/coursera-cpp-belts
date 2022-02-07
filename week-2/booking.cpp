#include <queue>
#include <string>
#include <map>
#include <iostream>

//#include "test_runner.h"
//#include "profiler.h"

using namespace std;

using HotelEvent = tuple<int64_t, int, int>;

class HotelManager {
 public:
  HotelManager() = default;

  void DoBook(int64_t time, string &hotel, int user_id, int rooms) {
	hotel_events[hotel].push_back({time, user_id, rooms});
	++hotel_unique_clients[hotel][user_id];
	hotel_rooms_count[hotel] += rooms;
	cur_time = time;
  }

  int GetClientsLast24H(const string &hotel) {
	if (!hotel_events.count(hotel)) return 0;
	UpdateHotelEvents(hotel);
	return static_cast<int>(hotel_unique_clients[hotel].size());
  }

  int GetRoomsLast24H(const string &hotel) {
	if (!hotel_events.count(hotel)) return 0;
	UpdateHotelEvents(hotel);
	return hotel_rooms_count[hotel];
  }

 private:
  static const int ONE_DAY_SEC = 86400;
  map<string, deque<HotelEvent> > hotel_events;
  map<string, int> hotel_rooms_count;
  map<string, map<int, int> > hotel_unique_clients;
  int64_t cur_time = static_cast<int64_t>(-1e18);

  void UpdateHotelEvents(const string &hotel) {
	auto &events = hotel_events.at(hotel);
	while (!events.empty()) {
	  HotelEvent event = events.front();
	  auto &[time_, user_id, rooms] = event;
	  if (time_ + ONE_DAY_SEC <= cur_time) {
		hotel_rooms_count[hotel] -= rooms;
		hotel_unique_clients[hotel][user_id] -= 1;

		if (!hotel_unique_clients[hotel][user_id])
		  hotel_unique_clients[hotel].erase(user_id);

		events.pop_front();
	  } else break;
	}
  }
};

int main() {
  {
	HotelManager manager;
	size_t query_count;
	cin >> query_count;
	for (size_t query_id = 0; query_id < query_count; ++query_id) {
	  string query_type;
	  cin >> query_type;

	  if (query_type == "BOOK") {
		size_t t, client, rooms;
		string hotel;
		cin >> t;
		cin >> hotel;
		cin >> client;
		cin >> rooms;
		manager.DoBook(t, hotel, client, rooms);
	  } else {
		string hotel;
		cin >> hotel;
		if (query_type == "CLIENTS") {
		  cout << manager.GetClientsLast24H(hotel) << endl;
		} else {
		  cout << manager.GetRoomsLast24H(hotel) << endl;
		}
	  }
	}
  }

  return 0;
}