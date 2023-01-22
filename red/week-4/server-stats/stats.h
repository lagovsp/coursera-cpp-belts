#pragma once

#include "http_request.h"

#include <string_view>
#include <map>

using namespace std;

class Stats {
 public:
  Stats() = default;
  void AddMethod(string_view);
  void AddUri(string_view);
  [[nodiscard]] const map<string_view, int> &GetMethodStats() const;
  [[nodiscard]] const map<string_view, int> &GetUriStats() const;

 private:
  map<string_view, int> UriStatsMap = {
	  {"/", 0},
	  {"/order", 0},
	  {"/product", 0},
	  {"/basket", 0},
	  {"/help", 0},
	  {"unknown", 0}
  };
  map<string_view, int> MethodStatsMap = {
	  {"GET", 0},
	  {"PUT", 0},
	  {"POST", 0},
	  {"DELETE", 0},
	  {"UNKNOWN", 0}
  };
};

HttpRequest ParseRequest(string_view line);