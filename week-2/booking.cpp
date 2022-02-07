#include <set>
#include <vector>
#include <queue>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <map>

using namespace std;

class Booker {
 public:
  Booker() = default;

 private:
  map<string, size_t> booked_rooms_by_hotel;
  map<string, queue<size_t>> booked_rooms_by_times;
};