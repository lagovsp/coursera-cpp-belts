#include "test_runner.h"
#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std;

typedef unordered_set<string> us_string;

class Translator {
 public:
  void Add(string_view source, string_view target) {
	auto sf = words_source.insert(string(source)).first;
	auto tf = words_target.insert(string(target)).first;
	source_to_target[*sf] = tf;
	target_to_source[*tf] = sf;
  }

  string_view TranslateForward(string_view source) const {
	auto s = source_to_target.find(source);
	return (s == source_to_target.end()) ? "" : string_view(*s->second);
  }

  string_view TranslateBackward(string_view target) const {
	auto s = target_to_source.find(target);
	return (s == target_to_source.end()) ? "" : string_view(*s->second);
  }

 private:
  unordered_set<string> words_source;
  unordered_set<string> words_target;
  unordered_map<string_view, us_string::iterator> source_to_target;
  unordered_map<string_view, us_string::iterator> target_to_source;
};

void TestSimple() {
  Translator translator;
  translator.Add(string("okno"), string("window"));
  translator.Add(string("stol"), string("table"));

  ASSERT_EQUAL(translator.TranslateForward("okno"), "window");
  ASSERT_EQUAL(translator.TranslateBackward("table"), "stol");
  ASSERT_EQUAL(translator.TranslateBackward("stol"), "");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSimple);

  return 0;
}
