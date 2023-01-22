#include "test_runner.h"

#include <iostream>
#include <map>
#include <string>
#include <unordered_map>

using namespace std;

struct Record {
  string id;
  string title;
  string user;
  int timestamp;
  int karma;

  struct Its {
	multimap<int, Record *>::iterator ItByIimestamp;
	multimap<int, Record *>::iterator ItByKarma;
	multimap<string, Record *>::iterator ItByUser;
  };

  Its dbIterators;

  bool operator==(const Record &rhs) const {
	return id == rhs.id
		&& title == rhs.title
		&& user == rhs.user
		&& timestamp == rhs.timestamp
		&& karma == rhs.karma;
  }
};

class Database {
 public:
  bool Put(const Record &record) {
	auto[it, inserted] = data.try_emplace(record.id, record);
	if (inserted) {
	  auto address = &it->second;
	  it->second.dbIterators = {ByTimestamp.emplace(record.timestamp, address),
								ByKarma.emplace(record.karma, address),
								ByUser.emplace(record.user, address),};
	}
	return inserted;
  }

  const Record *GetById(const string &id) const {
	auto it = data.find(id);
	return it == data.cend() ? nullptr : &it->second;
  }

  bool Erase(const string &id) {
	auto it = data.find(id);
	if (it == data.cend()) {
	  return false;
	}
	ByTimestamp.erase(it->second.dbIterators.ItByIimestamp);
	ByKarma.erase(it->second.dbIterators.ItByKarma);
	ByUser.erase(it->second.dbIterators.ItByUser);
	data.erase(it);
	return true;
  }

  template<typename Callback>
  void RangeByTimestamp(int low, int high, Callback callback) const {
	RangeBy(ByTimestamp, low, high, callback);
  }

  template<typename Callback>
  void RangeByKarma(int low, int high, Callback callback) const {
	RangeBy(ByKarma, low, high, callback);
  }

  template<typename Callback>
  void AllByUser(const string &user, Callback callback) const {
	RangeBy(ByUser, user, user, callback);
  }

 private:
  unordered_map<string, Record> data;
  multimap<int, Record *> ByTimestamp;
  multimap<int, Record *> ByKarma;
  multimap<string, Record *> ByUser;

  template<typename Key, typename Callback>
  void RangeBy(const multimap<Key, Record *> &ranged_by_key,
			   const Key &low, const Key &high,
			   Callback callback) const {
	if (low > high) {
	  return;
	}
	auto f = ranged_by_key.lower_bound(low);
	auto l = ranged_by_key.upper_bound(high);
	for (auto it = f; it != l && callback(*it->second); ++it) {}
  }
};

void TestRangeBoundaries() {
  const int good_karma = 1000;
  const int bad_karma = -10;

  Database db;
  db.Put({"id1", "Hello there", "master", 1536107260, good_karma});
  db.Put({"id2", "O>>-<", "general2", 1536107260, bad_karma});

  int count = 0;
  db.RangeByKarma(bad_karma, good_karma, [&count](const Record &) {
	++count;
	return true;
  });

  ASSERT_EQUAL(2, count);
}

void TestSameUser() {
  Database db;
  db.Put({"id1", "Don't sell", "master", 1536107260, 1000});
  db.Put({"id2", "Rethink life", "master", 1536107260, 2000});

  int count = 0;
  db.AllByUser("master", [&count](const Record &) {
	++count;
	return true;
  });

  ASSERT_EQUAL(2, count);
}

void TestReplacement() {
  const string final_body = "Feeling sad";

  Database db;
  db.Put({"id", "Have a hand", "not-master", 1536107260, 10});
  db.Erase("id");
  db.Put({"id", final_body, "not-master", 1536107260, -10});

  auto record = db.GetById("id");
  ASSERT(record != nullptr);
  ASSERT_EQUAL(final_body, record->title);
}

void TestAllUser() {
  Database db;
  db.Put({"id324", "Have a hand", "aaa", 1536107260, 10});
  db.Put({"id523", "Have a hand", "aba", 1536107260, 10});
  db.Put({"id5w23", "Have a hand", "aba", 1536107260, 10});
  db.Put({"id23", "", "aab", 1536107260, -10});
  db.Put({"id5", "", "x", 1536107260, -10});

  int count = 0;
  db.AllByUser("aba", [&count](const Record &) {
	++count;
	return true;
  });
  ASSERT_EQUAL(count, 2);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestRangeBoundaries);
  RUN_TEST(tr, TestSameUser);
  RUN_TEST(tr, TestReplacement);

  RUN_TEST(tr, TestAllUser);

  return 0;
}
