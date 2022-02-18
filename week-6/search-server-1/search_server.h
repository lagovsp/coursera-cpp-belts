#pragma once

#include <istream>
#include <ostream>
#include <set>
#include <list>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>

using namespace std;

class InvertedIndex {
 public:
  InvertedIndex() = default;
  void Add(const string &);
  [[nodiscard]] vector<pair<uint16_t, uint16_t>> Lookup(const string &) const;

  [[nodiscard]] uint16_t DocsNumber() const {
	return DocCounter;
  }

 private:
  uint16_t DocCounter = 0;
  map<string, vector<pair<uint16_t, uint16_t>>> DocsMatchWord;
};

class SearchServer {
 public:
  SearchServer() = default;
  explicit SearchServer(istream &);
  void UpdateDocumentBase(istream &);
  void AddQueriesStream(istream &, ostream &);

 private:
  InvertedIndex index;
};
