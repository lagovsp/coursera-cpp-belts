#include <queue>
#include <string>
#include <map>
#include <iostream>

//#include "test_runner.h"
//#include "profiler.h"

using namespace std;

class Booker {
 private:
  struct Booking {
	uint64_t time = 0;
	string hotel_name;
	uint64_t client_id = 0;
	uint64_t room_count = 0;
  };

  uint64_t cur_time = 0;
  queue<Booking> events;
  map<string, map<uint64_t, uint64_t>> map_cls;
  map<string, uint64_t> map_rms;

  static const uint64_t SECONDS_DAY = 86'400;

  void Update() {
	while (!events.empty() && events.front().time + SECONDS_DAY <= cur_time) {
	  auto f = events.front();
	  map_cls[f.hotel_name][f.client_id] -= f.room_count;
	  if (map_cls[f.hotel_name][f.client_id] == 0) {
		map_cls[f.hotel_name].erase(f.client_id);
	  }
	  map_rms[f.hotel_name] -= f.room_count;
	  events.pop();
	}
  }

 public:
  Booker() = default;

  void Book(const uint64_t time,
			const string &hotel,
			const uint64_t client,
			const uint64_t rooms) {
	events.push({time, hotel, client, rooms});
	map_cls[hotel][client] += rooms;
	map_rms[hotel] += rooms;
	cur_time = time;
	Update();
  }

  [[nodiscard]] uint64_t Clients(const string &hotel) {
	return map_cls[hotel].size();
  }

  [[nodiscard]] uint64_t Rooms(const string &hotel) {
	return map_rms[hotel];
  }
};

int main() {
  {
	Booker manager;
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
		manager.Book(t, hotel, client, rooms);
	  } else {
		string hotel;
		cin >> hotel;
		if (query_type == "CLIENTS") {
		  cout << manager.Clients(hotel) << endl;
		} else {
		  cout << manager.Rooms(hotel) << endl;
		}
	  }
	}
  }

  return 0;
}