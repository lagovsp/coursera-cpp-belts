#include <algorithm>
#include <string>
#include <iostream>
#include <vector>
#include <set>
#include <sstream>

using namespace std;

class Learner {
 private:
  set<string> dict;

 public:
  int Learn(const vector<string> &words) {
	size_t learned = 0;
	for (const auto &word: words) {
	  if (dict.find(word) == dict.cend()) {
		++learned;
		dict.insert(word);
	  }
	}
	return static_cast<int>(learned);
  }

  vector<string> KnownWords() {
	return {dict.cbegin(), dict.cend()};
  }
};

int main() {
  Learner learner;
  string line;
  while (getline(cin, line)) {
	vector<string> words;
	stringstream ss(line);
	string word;
	while (ss >> word) {
	  words.push_back(word);
	}
	cout << learner.Learn(words) << "\n";
  }

  cout << "=== known words ===\n";
  for (auto word: learner.KnownWords()) {
	cout << word << "\n";
  }
}
