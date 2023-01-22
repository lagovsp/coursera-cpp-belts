#include "xml.h"
#include "json.h"

//#include "json.cpp"
//#include "xml.cpp"

#include "test_runner.h"

#include <vector>
#include <string>
#include <map>
#include <unordered_map>

using namespace std;

Json::Document XmlToJson(const Xml::Document &doc) {
  using std::vector;
  using std::string;

//  using Json::Node;
  vector<Json::Node> result;

  for (const Xml::Node &n: doc.GetRoot().Children()) {
	using namespace Xml;
	result.emplace_back(std::map<string, Json::Node>{
		{"category", Json::Node{n.AttributeValue<string>("category")}},
		{"amount", Json::Node(n.AttributeValue<int>("amount"))}
	});
  }

  return Json::Document{Json::Node(std::move(result))};
}

Xml::Document JsonToXml(const Json::Document &doc, string root_name) {
  using namespace std;
  using Json::Document;

  Xml::Node root(move(root_name), {});
  for (const auto &n: doc.GetRoot().AsArray()) {
	root.AddChild(Xml::Node("spend",
							std::unordered_map<string, string>{
								{"category",
								 n.AsMap().at("category").AsString()},
								{"amount",
//								 n.AsMap().at("amount").AsString()}}));
								 to_string(n.AsMap().at("amount").AsInt())
								}}));
  }
  return Xml::Document{move(root)};
}

void TestXmlToJson() {
//  using namespace Xml;
//  using namespace Json;

  Xml::Node root("july", {});
  root.AddChild(Xml::Node{"spend",
						  {{"category", "travel"}, {"amount", "23400"}}});
  root.AddChild(Xml::Node{"spend", {{"category", "food"}, {"amount", "5000"}}});
  root.AddChild(Xml::Node{"spend",
						  {{"category", "transport"}, {"amount", "1150"}}});
  root.AddChild(Xml::Node{"spend",
						  {{"category", "sport"}, {"amount", "12000"}}});
  // this actually works good

  const Xml::Document xml_doc(std::move(root));

  const auto json_doc = XmlToJson(xml_doc);

  const auto &items = json_doc.GetRoot().AsArray();
  ASSERT_EQUAL(items.size(), 4u);

  const vector<string>
	  expected_category = {"travel", "food", "transport", "sport"};
  const vector<int> expected_amount = {23400, 5000, 1150, 12000};

  for (size_t i = 0; i < items.size(); ++i) {
	const map<string, Json::Node> &item = items[i].AsMap();
	const string feedback_msg = "i = " + std::to_string(i);
	AssertEqual(item.at("category").AsString(),
				expected_category[i],
				feedback_msg); // this assert goes well
//	cout << "item.at(\"amount\").AsInt() = "
//		 << item.at("amount").AsInt() << " expected = " << expected_amount[i]
//		 << endl;
	AssertEqual(item.at("amount").AsInt(), expected_amount[i], feedback_msg);
// this one fails -> now fixed
  }
}

void TestJsonToXml() {
  const Json::Document json_doc{Json::Node(vector<Json::Node>{
	  Json::Node(map<string, Json::Node>{
		  {"category", Json::Node("food")}, {"amount", Json::Node(2500)}
	  }),
	  Json::Node(map<string, Json::Node>{
		  {"category", Json::Node("transport")}, {"amount", Json::Node(1150)}
	  }),
	  Json::Node(map<string, Json::Node>{
		  {"category", Json::Node("restaurants")}, {"amount", Json::Node(5780)}
	  }),
	  Json::Node(map<string, Json::Node>{
		  {"category", Json::Node("clothes")}, {"amount", Json::Node(7500)}
	  }),
	  Json::Node(map<string, Json::Node>{
		  {"category", Json::Node("travel")}, {"amount", Json::Node(23740)}
	  }),
	  Json::Node(map<string, Json::Node>{
		  {"category", Json::Node("sport")}, {"amount", Json::Node(12000)}
	  }),
  })};

  const string root_name = "month";
  const auto xml_doc = JsonToXml(json_doc, root_name);
  const Xml::Node &root = xml_doc.GetRoot();

  ASSERT_EQUAL(root.Name(), root_name);
//  const vector<Json::Node> &children = root.Children();
  const vector<Xml::Node> &children = root.Children();
  ASSERT_EQUAL(children.size(), 6u);

  const vector<string> expected_category = {
	  "food", "transport", "restaurants", "clothes", "travel", "sport"
  };
  const vector<int> expected_amount = {2500, 1150, 5780, 7500, 23740, 12000};

  for (size_t i = 0; i < children.size(); ++i) {
//	using Json::Node;

	const string feedback_msg = "i = " + std::to_string(i);
	const Xml::Node &c = children[i];
	AssertEqual(c.Name(), "spend", feedback_msg); // good
	AssertEqual(c.AttributeValue<string>("category"),
				expected_category[i],
				feedback_msg); // good
	AssertEqual(c.AttributeValue<int>("amount"),
				expected_amount[i],
				feedback_msg);
  }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestXmlToJson);
  RUN_TEST(tr, TestJsonToXml);

  return 0;
}
