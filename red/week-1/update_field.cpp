#include "airline_ticket.h"
#include "test_runner.h"

using namespace std;

bool operator<(const Date &d1, const Date &d2) {
  return ((d1.year * 100 + d1.month) * 100 + d1.day) < ((d2.year * 100 + d2.month) * 100 + d2.day);
}

bool operator<(const Time &t1, const Time &t2) {
  return (t1.hours * 100 + t1.minutes) < (t2.hours * 100 + t2.minutes);
}

bool operator==(const Date &d1, const Date &d2) {
  return !(d1 < d2) && !(d2 < d1);
}

bool operator==(const Time &t1, const Time &t2) {
  return !(t1 < t2) && !(t2 < t1);
}

ostream &operator<<(ostream &os, const Date &d) {
  os << d.year << "-" << d.month << "-" << d.day;
  return os;
}

ostream &operator<<(ostream &os, const Time &t) {
  os << t.hours << "-" << t.minutes;
  return os;
}

istream &operator>>(istream &is, Date &d) {
  is >> d.year;
  is.ignore(1);
  is >> d.month;
  is.ignore(1);
  is >> d.day;
  return is;
}

istream &operator>>(istream &is, Time &t) {
  is >> t.hours;
  is.ignore(1);
  is >> t.minutes;
  return is;
}

#define UPDATE_FIELD(ticket, field, values) {    \
    auto it = (values).find(#field);            \
    if (it != (values).end()) {                    \
      istringstream is(it->second);            \
      is >> (ticket).field;                    \
    }                                        \
  }

void TestUpdate() {
  AirlineTicket t;
  t.price = 0;

  const map<string, string> updates1 = {
	  {"departure_date", "2018-2-28"},
	  {"departure_time", "17:40"},
  };
  UPDATE_FIELD(t, departure_date, updates1);
  UPDATE_FIELD(t, departure_time, updates1);
  UPDATE_FIELD(t, price, updates1);

  ASSERT_EQUAL(t.departure_date, (Date{2018, 2, 28}));
  ASSERT_EQUAL(t.departure_time, (Time{17, 40}));
  ASSERT_EQUAL(t.price, 0);

  const map<string, string> updates2 = {
	  {"price", "12550"},
	  {"arrival_time", "20:33"},
  };
  UPDATE_FIELD(t, departure_date, updates2);
  UPDATE_FIELD(t, departure_time, updates2);
  UPDATE_FIELD(t, arrival_time, updates2);
  UPDATE_FIELD(t, price, updates2);

  ASSERT_EQUAL(t.departure_date, (Date{2018, 2, 28}));
  ASSERT_EQUAL(t.departure_time, (Time{17, 40}));
  ASSERT_EQUAL(t.price, 12550);
  ASSERT_EQUAL(t.arrival_time, (Time{20, 33}));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestUpdate);

  return 0;
}
