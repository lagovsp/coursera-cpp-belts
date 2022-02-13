#include "test_runner.h"

#include <vector>

using namespace std;

template<typename Token>
using Sentence = vector<Token>;

template<typename Token>
vector<Sentence<Token>> SplitIntoSentences(vector<Token> tokens) {
  vector<Sentence<Token>> res;
  vector<Token> sentence;
  bool wait_for_end = true;
  for (auto &t: tokens) {
	auto isEnd = t.IsEndSentencePunctuation();
	if (isEnd) {
	  sentence.push_back(move(t));
	  wait_for_end = false;
	  continue;
	}
//	if (!isEnd && !wait_for_end) {
//	  res.push_back(move(sentence));
//	  sentence.push_back(move(t));
//	  wait_for_end = true;
//	  continue;
//	}
//	if (!isEnd && wait_for_end) {
//	  sentence.push_back(move(t));
//	  continue;
//	}
	if (!isEnd) {
	  if (!wait_for_end) {
		res.push_back(move(sentence));
		wait_for_end = true;
	  }
	  sentence.push_back(move(t));
	}
  }
  if (!sentence.empty()) {
	res.push_back(move(sentence));
  }
  return res;
}

struct TestToken {
  string data;
  bool is_end_sentence_punctuation = false;

  [[nodiscard]] bool IsEndSentencePunctuation() const {
	return is_end_sentence_punctuation;
  }
  bool operator==(const TestToken &other) const {
	return data == other.data
		&& is_end_sentence_punctuation == other.is_end_sentence_punctuation;
  }
  bool operator!=(const TestToken &other) const {
	return !(*this == other);
  }
};

ostream &operator<<(ostream &stream, const TestToken &token) {
  return stream << token.data;
}

void TestSplitting() {
  ASSERT_EQUAL(
	  SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"},
											{"!"}})),
	  vector<Sentence<TestToken>>({
									  {{"Split"}, {"into"}, {"sentences"},
									   {"!"}}
								  })
  );

  ASSERT_EQUAL(
	  SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"},
											{"!", true}})),
	  vector<Sentence<TestToken>>({
									  {{"Split"}, {"into"}, {"sentences"},
									   {"!", true}}
								  })
  );

  ASSERT_EQUAL(
	  SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"},
											{"!", true}, {"!", true},
											{"Without"}, {"copies"},
											{".", true}})),
	  vector<Sentence<TestToken>>({
									  {{"Split"}, {"into"}, {"sentences"},
									   {"!", true}, {"!", true}},
									  {{"Without"}, {"copies"}, {".", true}},
								  })
  );
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSplitting);

  return 0;
}
