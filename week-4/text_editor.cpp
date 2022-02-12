#include <string>
#include <list>
#include <algorithm>

#include "test_runner.h"

using namespace std;

class Editor {
 public:
  Editor() = default;

  void Left() {
	if (pos != text.begin()) {
	  --pos;
	}
  }

  void Right() {
	if (pos != text.end()) {
	  ++pos;
	}
  }

  void Insert(char token) {
	text.insert(pos, token);
  }

  void Cut(size_t tokens = 1) {
	auto moved = AdvanceForwardFromPos(tokens);
	buffer.clear();
	buffer.splice(buffer.begin(), text, pos, moved);
	pos = moved;
  }

  void Copy(size_t tokens = 1) {
	buffer.assign(pos, AdvanceForwardFromPos(tokens));
  }

  void Paste() {
	text.insert(pos, buffer.begin(), buffer.end());
  }

  [[nodiscard]] string GetText() const {
	string s;
	for (const auto &c: text) {
	  s.push_back(c);
	}
	return s;
  }

  [[nodiscard]] string showBuf() const {
	string s;
	for (const auto &c: buffer) {
	  s.push_back(c);
	}
	return s;
  }

 private:
  list<char> text, buffer;
  list<char>::iterator pos = text.begin();

  list<char>::iterator AdvanceForwardFromPos(size_t tokens) {
	auto moved = pos;
	for (size_t i = 0; i < tokens; ++i) {
	  if (moved == text.end()) {
		break;
	  }
	  ++moved;
	}
	return moved;
  }
};

void TypeText(Editor &editor, const string &text) {
  for (char c: text) {
	editor.Insert(c);
  }
}

void TestEditing() {
  {
	Editor editor;

	const size_t text_len = 12;
	const size_t first_part_len = 7;
	TypeText(editor, "hello, world");
	for (size_t i = 0; i < text_len; ++i) {
	  editor.Left();
	}
	editor.Cut(first_part_len);
	for (size_t i = 0; i < text_len - first_part_len; ++i) {
	  editor.Right();
	}

	TypeText(editor, ", ");
	editor.Paste();
	editor.Left();
	editor.Left();
	editor.Cut(3);

	ASSERT_EQUAL(editor.GetText(), "world, hello");
  }
  {
	Editor editor;

	TypeText(editor, "misprnit");
	editor.Left();
	editor.Left();
	editor.Left();
	editor.Cut(1);
	editor.Right();
	editor.Paste();

	ASSERT_EQUAL(editor.GetText(), "misprint");
  }
}

void TestReverse() {
  Editor editor;

  const string text = "esreveR";
  for (char c: text) {
	editor.Insert(c);
	editor.Left();
  }

  ASSERT_EQUAL(editor.GetText(), "Reverse");
}

void TestNoText() {
  Editor editor;

  ASSERT_EQUAL(editor.GetText(), "");

  editor.Left();
  editor.Left();
  editor.Right();
  editor.Right();
  editor.Copy(0);
  editor.Cut(0);
  editor.Paste();

  ASSERT_EQUAL(editor.GetText(), "");
}

void TestEmptyBuffer() {
  Editor editor;

  editor.Paste();
  TypeText(editor, "example");
  editor.Left();
  editor.Left();
  editor.Paste();
  editor.Right();
  editor.Paste();
  editor.Copy(0);
  editor.Paste();
  editor.Left();
  editor.Cut(0);
  editor.Paste();

  ASSERT_EQUAL(editor.GetText(), "example");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestEditing);
  RUN_TEST(tr, TestReverse);
  RUN_TEST(tr, TestNoText);
  RUN_TEST(tr, TestEmptyBuffer);

  return 0;
}