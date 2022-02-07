#ifndef INCLUDE_PROFILER_H_
#define INCLUDE_PROFILER_H_

#include <chrono>
#include <iostream>
#include <string>

using namespace std;
using namespace std::chrono;

class LogDuration {
 public:
  explicit LogDuration(const string &msg = "")
	  : message{msg + ": "}, start{steady_clock::now()} {}

  ~LogDuration() {
	auto finish = steady_clock::now();
	auto dur = finish - start;
	cerr << message
		 << duration_cast<milliseconds>(dur).count()
		 << " ms" << endl;
  }

 private:
  string message;
  steady_clock::time_point start;
};

#define UNIQ_ID_HELPER2(line) log_duration_var##line
#define UNIQ_ID_HELPER(line) UNIQ_ID_HELPER2(line)
#define UNIQ_ID UNIQ_ID_HELPER(__LINE__)

#define LOG_DURATION(message) LogDuration UNIQ_ID{message}

#endif //INCLUDE_PROFILER_H_
