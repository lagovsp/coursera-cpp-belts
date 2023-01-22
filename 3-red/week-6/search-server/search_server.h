#pragma once

#include <istream>
#include <ostream>
#include <set>
#include <list>
#include <vector>
#include <map>
#include <future>
#include <unordered_map>
#include <string>
#include <queue>
#include <shared_mutex>

using namespace std;

class InvertedIndex {
 public:
  InvertedIndex() = default;
  void Add(const string &);

  [[nodiscard]]
  const vector<pair<uint16_t, uint16_t>> &Lookup(const string &) const;

  [[nodiscard]]
  uint16_t DocsNumber() const { return DocCounter; }

 private:
  uint16_t DocCounter = 0;
  map<string, vector<pair<uint16_t, uint16_t>>> DocsMatchWord;
  vector<pair<uint16_t, uint16_t>> vec_empty;
};

class SearchServer {
 public:
  SearchServer() = default;
  explicit SearchServer(istream &);
  void UpdateDocumentBase(istream &);
  void AddQueriesStream(istream &, ostream &);

 private:
  InvertedIndex index;
  mutable shared_mutex index_mut;
  list<future<void>> threads;
  atomic_bool isBaseReady = false;
};
