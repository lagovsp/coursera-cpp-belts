#include <queue>
#include <string>
#include <map>
#include <iostream>

using namespace std;

class Booker {
 private:
  struct Event {
	size_t time;
	string hotel;
	size_t client;
	size_t rooms;
  };

  queue<Event> events;
  mutable map<string, map<size_t, size_t>> hotel_clients;
  mutable map<string, size_t> hotel_rooms;
  static const size_t SECONDS_DAY = 86'400;

  void Update(const size_t time) {
	while (!events.empty() && events.front().time + SECONDS_DAY <= time) {
	  auto it = events.front();
	  hotel_clients[it.hotel][it.client] -= it.rooms;
	  if (hotel_clients[it.hotel][it.client] == 0) {
		hotel_clients[it.hotel].erase(it.client);
	  }
	  hotel_rooms[it.hotel] -= it.rooms;
	  events.pop();
	}
  }

 public:
  Booker() = default;

  void Book(const size_t time,
			const string &hotel,
			const size_t client,
			const size_t rooms) {
	events.push({time, hotel, client, rooms});
	hotel_rooms[hotel] += rooms;
	hotel_clients[hotel][client] += rooms;
	Update(time);
  }

  [[nodiscard]] size_t Clients(const string &hotel_name) const {
	return hotel_clients[hotel_name].size();
  }

  [[nodiscard]] size_t Rooms(const string &hotel_name) const {
	return hotel_rooms[hotel_name];
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