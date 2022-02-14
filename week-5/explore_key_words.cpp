#include "test_runner.h"
//#include "profiler.h"

#include <map>
#include <string>
#include <cmath>
#include <iterator>
#include <future>

using namespace std;

template<typename It>
class IteratorRange {
 public:
  IteratorRange(const It &f, const It &l, size_t s) : first{f},
													  last{l},
													  page_size{s} {}
  [[nodiscard]] size_t size() const { return page_size; }
  [[nodiscard]] It begin() const { return first; }
  [[nodiscard]] It end() const { return last; }

 private:
  It first, last;
  size_t page_size = 0;
};

struct Stats {
  map<string, int> word_frequences;

  Stats &operator+=(const Stats &rhs) {
	for (const auto&[word, frequency]: rhs.word_frequences) {
	  word_frequences[word] += frequency;
	}
	return *this;
  }
};

// this could be beneficial
vector<string> Split(const string &line) {
  // http://en.cppreference.com/w/cpp/iterator/istream_iterator
  istringstream line_splitter(line);
  return {istream_iterator<string>(line_splitter), istream_iterator<string>()};
}

Stats ProcessLine(const set<string> &key_words, const string &line) {
  string tmp;
  Stats stats;
  for (const char &c: line) {
	if (c == ' ') {
	  if (!tmp.empty() && key_words.find(tmp) != key_words.end()) {
		++stats.word_frequences[move(tmp)];
	  }
	  tmp = string();
	  continue;
	}
	tmp.push_back(c);
  }
  if (!tmp.empty() && key_words.find(tmp) != key_words.end()) {
	++stats.word_frequences[move(tmp)];
  }
  return stats;
}

Stats ExploreSingleThread(const set<string> &key_words, istream &input) {
  Stats result;
  for (string line; getline(input, line);) {
	result += ProcessLine(key_words, line);
  }
  return result;
}

Stats ExploreKeyWords(const set<string> &key_words, istream &input) {
  const size_t threads = 4;
  Stats ans;
  vector<string> strings;
  string s;
  while (getline(input, s)) {
	strings.push_back(move(s));
  }
  vector<stringstream> streams;
  streams.reserve(threads);
  for (size_t si = 0; si < 4; ++si) {
	stringstream ss;
	for (auto it = strings.begin() + strings.size() * si / threads;
		 it != strings.end()
			 && it != strings.begin() + strings.size() * (si + 1) / threads;
		 ++it) {
	  ss << *it << '\n';
	}
	streams.emplace_back(move(ss));
  }
  vector<future<Stats>> futures;
  futures.reserve(threads);
  for (auto &e: streams) {
	futures.emplace_back(async(ExploreSingleThread,
							   ref(key_words),
							   ref(e)));
  }
  for (auto &fut: futures) {
	ans += fut.get();
  }
  return ans;
}

void TestBasic() {
  const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

  stringstream ss;
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

  const auto stats = ExploreKeyWords(key_words, ss);
  const map<string, int> expected = {
	  {"yangle", 6},
	  {"rocks", 2},
	  {"sucks", 1}
  };
  ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestBasic);

  return 0;
}
