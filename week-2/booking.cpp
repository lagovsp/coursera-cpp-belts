#include <queue>
#include <string>
#include <map>
#include <iostream>

//#include "test_runner.h"
//#include "profiler.h"

using namespace std;

class Booker {
 public:
  void Book(int64_t time,
			const string &hotel,
			const int client,
			const int rooms) {
	cur_time = time;
	hotels[hotel].Book({time, client, rooms});
  }

  [[nodiscard]] int Clients(const string &hotel_name) {
	return hotels[hotel_name].GetClients(cur_time);
  }

  [[nodiscard]] int Rooms(const string &hotel_name) {
	return hotels[hotel_name].GetRooms(cur_time);
  }

 private:
  struct Booking {
	int64_t time;
	int client;
	int rooms;
  };

  class HotelInfo {
   public:
	void Book(const Booking &booking) {
	  last_bookings.push(booking);
	  rooms += booking.rooms;
	  ++client_bookings[booking.client];
	}

	int GetClients(const int64_t time) {
	  RemoveOldBookings(time);
	  return static_cast<int>(client_bookings.size());
	}

	int GetRooms(const int64_t time) {
	  RemoveOldBookings(time);
	  return rooms;
	}

   private:
	static const int SECONDS_DAY = 86'400;
	int rooms = 0;
	queue<Booking> last_bookings;
	map<int, int> client_bookings;

	void PopBooking() {
	  const auto &b = last_bookings.front();
	  rooms -= b.rooms;
	  const auto client_stat_it = client_bookings.find(b.client);
	  if (--client_stat_it->second == 0) {
		client_bookings.erase(client_stat_it);
	  }
	  last_bookings.pop();
	}

	void RemoveOldBookings(const int64_t time) {
	  while (!last_bookings.empty()
		  && last_bookings.front().time <= time - SECONDS_DAY) {
		PopBooking();
	  }
	}
  };

  int64_t cur_time = 0;
  map<string, HotelInfo> hotels;
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