#include "test_runner.h"

#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <optional>
#include <unordered_set>

using namespace std;

static const string GET = "GET";
static const string POST = "POST";

static const string PROTOCOL_TITLE = "HTTP/1.1";
static const string CONTENT_LENGTH_HEADER = "Content-Length";

static const string USER_COMMENTS_PATH = "/user_comments";
static const string CAPTCHA_PATH = "/captcha";

static const string ADD_USER_PATH = "/add_user";
static const string ADD_COMMENT_PATH = "/add_comment";
static const string CHECK_CAPTCHA_PATH = "/checkcaptcha";

static const string LOCATION_HEADER = "Location";

static const string CAPTCHA_QUESTION =
	"What's the answer for The Ultimate Question of Life, the Universe, and Everything?";

static const set<string_view> POST_PATHS = {
	ADD_USER_PATH,
	ADD_COMMENT_PATH,
	CHECK_CAPTCHA_PATH
};

static const set<string_view> GET_PATHS = {
	USER_COMMENTS_PATH,
	CAPTCHA_PATH
};

struct HttpHeader { string name, value; };

static const string OK = "OK";
static const string NOT_FOUND = "Not found";
static const string FOUND = "Found";

enum class HttpCode {
  Ok = 200,
  NotFound = 404,
  Found = 302
};

ostream &operator<<(ostream &output, const HttpHeader &h) {
  return output << h.name << ": " << h.value;
}

bool operator==(const HttpHeader &lhs, const HttpHeader &rhs) {
  return lhs.name == rhs.name && lhs.value == rhs.value;
}

struct HttpRequest {
  string method, path, body;
  map<string, string> get_params;
};

class HttpResponse {
 public:
  explicit HttpResponse(HttpCode code) : http_code(code) {}

  HttpResponse &AddHeader(string name, string value) {
	headers.push_back({move(name), move(value)});
	return *this;
  }

  HttpResponse &SetContent(string a_content) {
	body = move(a_content);
	return *this;
  }

  HttpResponse &SetCode(HttpCode a_code) {
	http_code = a_code;
	return *this;
  }

  [[nodiscard]] const HttpCode &Code() const {
	return http_code;
  }

  [[nodiscard]] const vector<HttpHeader> &Headers() const {
	return headers;
  }

  [[nodiscard]] const string &Content() const {
	return body;
  }

  friend ostream &operator<<(ostream &output, const HttpResponse &resp);

 private:
  HttpCode http_code;
  vector<HttpHeader> headers;
  string body;
};

ostream &operator<<(ostream &output, const HttpCode &code) {
  switch (code) {
	case HttpCode::Ok: {
	  output << OK;
	  break;
	}
	case HttpCode::NotFound: {
	  output << NOT_FOUND;
	  break;
	}
	case HttpCode::Found: {
	  output << FOUND;
	  break;
	}
  }
  return output;
}

ostream &operator<<(ostream &output, const HttpResponse &resp) {
  output << PROTOCOL_TITLE << " " << static_cast<size_t>(resp.http_code) << " "
		 << resp.http_code;
  for (const auto &header: resp.headers) {
	output << "\n" << header;
  }
  if (!resp.body.empty()) {
	output << "\n" << CONTENT_LENGTH_HEADER << ": " << resp.body.size();
  }
  output << "\n\n" << resp.body;
  return output;
}

pair<string, string> SplitBy(const string &what, const string &by) {
  size_t pos = what.find(by);
  if (by.size() < what.size() && pos < what.size() - by.size()) {
	return {what.substr(0, pos), what.substr(pos + by.size())};
  } else {
	return {what, {}};
  }
}

template<typename T>
T FromString(const string &s) {
  T x;
  istringstream is(s);
  is >> x;
  return x;
}

pair<size_t, string> ParseIdAndContent(const string &body) {
  auto[id_string, content] = SplitBy(body, " ");
  return {FromString<size_t>(id_string), content};
}

struct LastCommentInfo {
  size_t user_id, consecutive_count;
};

class CommentServer {
 private:
  vector<vector<string>> comments_;
  optional<LastCommentInfo> last_comment;
  unordered_set<size_t> banned_users;

  HttpResponse HandlePost(const HttpRequest &req,
						  HttpResponse &resp) {
	if (POST_PATHS.count(req.path) == 0) {
	  return resp;
	}
	if (req.path == ADD_USER_PATH) {
	  return HandlePostAddUser(req, resp);
	}
	if (req.path == ADD_COMMENT_PATH) {
	  return HandlePostAddComment(req, resp);
	}
	if (req.path == CHECK_CAPTCHA_PATH) {
	  return HandlePostCheckCaptcha(req, resp);
	}
	return resp;
  }

  HttpResponse HandlePostAddUser(const HttpRequest &req,
								 HttpResponse &resp) {
	comments_.emplace_back();
	return resp.SetCode(HttpCode::Ok).SetContent(to_string(
		comments_.size() - 1));
  }

  HttpResponse HandlePostAddComment(const HttpRequest &req,
									HttpResponse &resp) {
	auto[user_id, comment] = ParseIdAndContent(req.body);

	if (!last_comment || last_comment->user_id != user_id) {
	  last_comment = LastCommentInfo{user_id, 1};
	} else if (++last_comment->consecutive_count > 3) {
	  banned_users.insert(user_id);
	}

	if (banned_users.count(user_id) == 0) {
	  comments_[user_id].push_back(string(comment));
	  return resp.SetCode(HttpCode::Ok);
	} else {
	  return resp.SetCode(HttpCode::Found).AddHeader(LOCATION_HEADER,
													 CAPTCHA_PATH);
	}
  }

  HttpResponse HandlePostCheckCaptcha(const HttpRequest &req,
									  HttpResponse &resp) {
	if (auto[id, response] = ParseIdAndContent(req.body); response == "42") {
	  banned_users.erase(id);
	  if (last_comment && last_comment->user_id == id) {
		last_comment.reset();
	  }
	  resp.SetCode(HttpCode::Ok);
	} else {
	  resp.SetCode(HttpCode::Found).AddHeader(LOCATION_HEADER, CAPTCHA_PATH);
	}
	return resp;
  }

  HttpResponse HandleGet(const HttpRequest &req, HttpResponse &resp) {
	if (GET_PATHS.count(req.path) == 0) {
	  return resp;
	}
	if (req.path == USER_COMMENTS_PATH) {
	  return HandleGetUserComments(req, resp);
	}
	if (req.path == CAPTCHA_PATH) {
	  return resp.SetCode(HttpCode::Ok).SetContent(CAPTCHA_QUESTION);
	}
	return resp;
  }

  HttpResponse HandleGetUserComments(const HttpRequest &req,
									 HttpResponse &resp) {
	auto user_id = FromString<size_t>(req.get_params.at("user_id"));
	string response;
	for (const string &c: comments_[user_id]) { response += c + '\n'; }
	return resp.SetCode(HttpCode::Ok).SetContent(move(response));
  }

 public:
  HttpResponse ServeRequest(const HttpRequest &req) {
	HttpResponse resp(HttpCode::NotFound);
	if (req.method == POST) {
	  resp = HandlePost(req, resp);
	} else if (req.method == GET) {
	  resp = HandleGet(req, resp);
	}
	cout << resp << endl;
	return resp;
  }
};

struct ParsedResponse {
  int code;
  vector<HttpHeader> headers;
  string content;
};

istream &operator>>(istream &input, ParsedResponse &r) {
  string line;
  getline(input, line);

  {
	istringstream code_input(line);
	string dummy;
	code_input >> dummy >> r.code;
  }

  size_t content_length = 0;

  r.headers.clear();
  while (getline(input, line) && !line.empty()) {
	if (auto[name, value] = SplitBy(line, ": "); name == "Content-Length") {
	  istringstream length_input(value);
	  length_input >> content_length;
	} else {
	  r.headers.push_back({move(name), move(value)});
	}
  }

  r.content.resize(content_length);
  input.read(r.content.data(), r.content.size());
  return input;
}

void Test(CommentServer &srv,
		  const HttpRequest &request,
		  const ParsedResponse &expected) {
  HttpResponse resp = srv.ServeRequest(request);

  ASSERT_EQUAL(static_cast<size_t>(resp.Code()), expected.code);
  ASSERT_EQUAL(resp.Headers(), expected.headers);
  ASSERT_EQUAL(resp.Content(), expected.content);
}

template<typename CommentServer>
void TestServer() {
  CommentServer cs;

  const ParsedResponse ok{200};
  const ParsedResponse
	  redirect_to_captcha{302, {{"Location", "/captcha"}}, {}};
  const ParsedResponse not_found{404};

  Test(cs, {"POST", "/add_user"}, {200, {}, "0"});
  Test(cs, {"POST", "/add_user"}, {200, {}, "1"});
  Test(cs, {"POST", "/add_comment", "0 Hello"}, ok);
  Test(cs, {"POST", "/add_comment", "1 Hi"}, ok);
  Test(cs, {"POST", "/add_comment", "1 Buy my goods"}, ok);
  Test(cs, {"POST", "/add_comment", "1 Enlarge"}, ok);
  Test(cs, {"POST", "/add_comment", "1 Buy my goods"}, redirect_to_captcha);
  Test(cs, {"POST", "/add_comment", "0 What are you selling?"}, ok);
  Test(cs, {"POST", "/add_comment", "1 Buy my goods"}, redirect_to_captcha);
  Test(
	  cs,
	  {"GET", "/user_comments", "", {{"user_id", "0"}}},
	  {200, {}, "Hello\nWhat are you selling?\n"}
  );
  Test(
	  cs,
	  {"GET", "/user_comments", "", {{"user_id", "1"}}},
	  {200, {}, "Hi\nBuy my goods\nEnlarge\n"}
  );
  Test(
	  cs,
	  {"GET", "/captcha"},
	  {200, {},
	   {"What's the answer for The Ultimate Question of Life, the Universe, and Everything?"}}
  );
  Test(cs, {"POST", "/checkcaptcha", "1 24"}, redirect_to_captcha);
  Test(cs, {"POST", "/checkcaptcha", "1 42"}, ok);
  Test(cs, {"POST", "/add_comment", "1 Sorry! No spam any more"}, ok);
  Test(
	  cs,
	  {"GET", "/user_comments", "", {{"user_id", "1"}}},
	  {200, {}, "Hi\nBuy my goods\nEnlarge\nSorry! No spam any more\n"}
  );

  Test(cs, {"GET", "/user_commntes"}, not_found);
  Test(cs, {"POST", "/add_uesr"}, not_found);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestServer<CommentServer>);

  return 0;
}
