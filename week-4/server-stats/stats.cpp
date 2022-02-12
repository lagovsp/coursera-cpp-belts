#include "stats.h"

void Stats::AddMethod(string_view method) {
  if (MethodStatsMap.find(method) != MethodStatsMap.end()) {
	++MethodStatsMap[method];
	return;
  }
  ++MethodStatsMap["UNKNOWN"];
}

void Stats::AddUri(string_view uri) {
  if (UriStatsMap.find(uri) != UriStatsMap.end()) {
	++UriStatsMap[uri];
	return;
  }
  ++UriStatsMap["unknown"];
}

const map<string_view, int> &Stats::GetMethodStats() const {
  return MethodStatsMap;
}

const map<string_view, int> &Stats::GetUriStats() const {
  return UriStatsMap;
}

HttpRequest ParseRequest(string_view line) {
  HttpRequest req;
  size_t pos = 0;

  while (line[pos] == ' ' && pos != line.npos) {
	++pos;
  }

  line.remove_prefix(pos);
  size_t sep = line.find(' ');
  req.method = line.substr(0, sep);
  line.remove_prefix(++sep);
  sep = line.find(' ');
  req.uri = line.substr(0, sep);
  line.remove_prefix(++sep);
  req.protocol = line;

  return req;
}
