#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>

vector<string> SplitIntoWords(const string &line) {
  istringstream words_input(line);
  return {istream_iterator<string>(words_input), istream_iterator<string>()};
}

SearchServer::SearchServer(istream &document_input) {
  UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream &document_input) {
  InvertedIndex new_index;

  for (string current_document; getline(document_input, current_document);) {
	new_index.Add(current_document);
  }

  index = move(new_index);
}

void SearchServer::AddQueriesStream(istream &query_input,
									ostream &search_results_output) {
  vector<pair<uint16_t, uint16_t>> stats(index.DocsNumber());

  for (string current_query; getline(query_input, current_query);) {
	const auto words = SplitIntoWords(current_query);

	for (size_t i = 0; i < stats.size(); ++i) {
	  stats[i] = {i, 0};
	}

	for (const auto &w: words) {
	  for (const auto &[docid, hitcount]: index.Lookup(w)) {
		stats[docid].second += hitcount;
	  }
	}

	partial_sort(
		stats.begin(),
		stats.size() > 5 ? stats.begin() + 5 : stats.end(),
		stats.end(),
		[](pair<uint16_t, uint16_t> lhs, pair<uint16_t, uint16_t> rhs) {
		  uint16_t lhs_docid = lhs.first;
		  auto lhs_hit_count = lhs.second;
		  uint16_t rhs_docid = rhs.first;
		  auto rhs_hit_count = rhs.second;
		  return make_pair(lhs_hit_count, -lhs_docid) > make_pair(rhs_hit_count, -rhs_docid);
		}
	);

	search_results_output << current_query << ':';
	for (const auto&[docid, hitcount]: Head(stats, 5)) {
	  if (hitcount == 0) {
		break;
	  }
	  search_results_output << " {docid: " << docid << ", hitcount: " << hitcount << '}';
	}
	search_results_output << endl;
  }
}

void InvertedIndex::Add(const string &doc) {
  for (auto &w: SplitIntoWords(doc)) {
	auto &it = DocsMatchWord[move(w)];
	if (it.empty() || it.back().first != DocCounter) {
	  it.emplace_back(DocCounter, 1);
	  continue;
	}
	++it.back().second;
  }
  ++DocCounter;
}

vector<pair<uint16_t,
			uint16_t>> InvertedIndex::Lookup(const string &word) const {
  if (auto it = DocsMatchWord.find(word); it != DocsMatchWord.cend()) {
	return it->second;
  }
  return {};
}