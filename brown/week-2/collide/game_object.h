#pragma once

//#include "geo2d.h"

class Unit;
class Building;
class Tower;
class Fence;

//#define COLLIDE return geo2d::Collide(this->Get(), that.Get())

struct GameObject {
  virtual ~GameObject() = default;

  virtual bool Collide(const GameObject &that) const = 0;
  virtual bool CollideWith(const Unit &that) const = 0;
//  bool CollideWith(const Unit &that) const { COLLIDE; }
  virtual bool CollideWith(const Building &that) const = 0;
  virtual bool CollideWith(const Tower &that) const = 0;
  virtual bool CollideWith(const Fence &that) const = 0;
};

bool Collide(const GameObject &first, const GameObject &second);
