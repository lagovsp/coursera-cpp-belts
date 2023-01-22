#include "ini.h"

#include <string>

using namespace std;

namespace Ini {
Section &Document::AddSection(string name) {
  auto it = sections.insert({move(name), {}});
  return it.first->second;
}

const Section &Document::GetSection(const string &name) const {
  return sections.at(name);
}

size_t Document::SectionCount() const {
  return sections.size();
}

Document Load(istream &input) {
  Document doc;
  Section *sec;
  for (string s; getline(input, s);) {
	if (s.empty()) {
	  continue;
	} else if (s.front() == '[') {
	  sec = &doc.AddSection(string{s.substr(1, s.size() - 2)});
	} else {
	  sec->insert({s.substr(0, s.find('=')), s.substr(s.find('=') + 1)});
	}
  }
  return doc;
}
}
