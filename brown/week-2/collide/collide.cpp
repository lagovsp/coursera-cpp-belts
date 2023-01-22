#include "geo2d.h"
#include "game_object.h"

#include "test_runner.h"
#include "geo2d.cpp"

#include <vector>
#include <memory>

using namespace std;

#define COLLIDE return geo2d::Collide(this->Get(), that.Get())

#define COLLIDE_WITH_DECLARATION \
    [[nodiscard]] bool CollideWith(const Unit &that) const override; \
    [[nodiscard]] bool CollideWith(const Building &that) const override; \
    [[nodiscard]] bool CollideWith(const Tower &that) const override; \
    [[nodiscard]] bool CollideWith(const Fence &that) const override

#define COLLIDE_WITH_IMPLEMENTATION_FOR(TYPE) \
    bool TYPE::CollideWith(const Unit &that) const { COLLIDE; } \
    bool TYPE::CollideWith(const Building &that) const { COLLIDE; } \
    bool TYPE::CollideWith(const Tower &that) const { COLLIDE; } \
    bool TYPE::CollideWith(const Fence &that) const { COLLIDE; }

template<typename T>
struct Collider : public GameObject {
  [[nodiscard]] bool Collide(const GameObject &that) const final {
	return that.CollideWith(static_cast<const T &>(*this));
  }
};

class Unit : public Collider<Unit> {
 private:
  geo2d::Point p;

 public:
  explicit Unit(geo2d::Point geo) : p(geo) {}
  [[nodiscard]] const geo2d::Point &Get() const { return p; }

  COLLIDE_WITH_DECLARATION;
};

class Building : public Collider<Building> {
 private:
  geo2d::Rectangle r;

 public:
  explicit Building(geo2d::Rectangle geo) : r(geo) {}
  [[nodiscard]] const geo2d::Rectangle &Get() const { return r; }

  COLLIDE_WITH_DECLARATION;
};

class Tower : public Collider<Tower> {
 private:
  geo2d::Circle c;

 public:
  explicit Tower(geo2d::Circle geo) : c(geo) {}
  [[nodiscard]] const geo2d::Circle &Get() const { return c; }

  COLLIDE_WITH_DECLARATION;
};

class Fence : public Collider<Fence> {
 private:
  geo2d::Segment s;

 public:
  explicit Fence(geo2d::Segment geo) : s(geo) {}
  [[nodiscard]] const geo2d::Segment &Get() const { return s; }

  COLLIDE_WITH_DECLARATION;
};

COLLIDE_WITH_IMPLEMENTATION_FOR(Unit);
COLLIDE_WITH_IMPLEMENTATION_FOR(Building);
COLLIDE_WITH_IMPLEMENTATION_FOR(Tower);
COLLIDE_WITH_IMPLEMENTATION_FOR(Fence);

bool Collide(const GameObject &first, const GameObject &second) {
  return first.Collide(second);
}

void TestAddingNewObjectOnMap() {
  using namespace geo2d;

  const vector<shared_ptr<GameObject>> game_map = {
	  make_shared<Unit>(Point{3, 3}),
	  make_shared<Unit>(Point{5, 5}),
	  make_shared<Unit>(Point{3, 7}),
	  make_shared<Fence>(Segment{{7, 3}, {9, 8}}),
	  make_shared<Tower>(Circle{Point{9, 4}, 1}),
	  make_shared<Tower>(Circle{Point{10, 7}, 1}),
	  make_shared<Building>(Rectangle{{11, 4}, {14, 6}})
  };

  for (size_t i = 0; i < game_map.size(); ++i) {
	Assert(
		Collide(*game_map[i], *game_map[i]),
		"An object doesn't collide with itself: " + to_string(i)
	);

	for (size_t j = 0; j < i; ++j) {
	  Assert(
		  !Collide(*game_map[i], *game_map[j]),
		  "Unexpected collision found " + to_string(i) + ' ' + to_string(j)
	  );
	}
  }

  auto new_warehouse = make_shared<Building>(Rectangle{{4, 3}, {9, 6}});
  ASSERT(!Collide(*new_warehouse, *game_map[0]));
  ASSERT(Collide(*new_warehouse, *game_map[1]));
  ASSERT(!Collide(*new_warehouse, *game_map[2]));
  ASSERT(Collide(*new_warehouse, *game_map[3]));
  ASSERT(Collide(*new_warehouse, *game_map[4]));
  ASSERT(!Collide(*new_warehouse, *game_map[5]));
  ASSERT(!Collide(*new_warehouse, *game_map[6]));

  auto new_defense_tower = make_shared<Tower>(Circle{{8, 2}, 2});
  ASSERT(!Collide(*new_defense_tower, *game_map[0]));
  ASSERT(!Collide(*new_defense_tower, *game_map[1]));
  ASSERT(!Collide(*new_defense_tower, *game_map[2]));
  ASSERT(Collide(*new_defense_tower, *game_map[3]));
  ASSERT(Collide(*new_defense_tower, *game_map[4]));
  ASSERT(!Collide(*new_defense_tower, *game_map[5]));
  ASSERT(!Collide(*new_defense_tower, *game_map[6]));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestAddingNewObjectOnMap);

  return 0;
}
