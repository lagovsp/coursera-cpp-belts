#pragma once

#include <stdexcept>
#include <array>

using namespace std;

template<typename T, size_t N>
class StackVector {
 public:
  explicit StackVector(const size_t size = 0) {
	if (size > N) {
	  throw invalid_argument("StackVector::StackVector: size > N");
	}
	size_ = size;
  }

  T &operator[](const size_t index) {
	return array_[index];
  }

  const T &operator[](const size_t index) const {
	return array_[index];
  }

  auto begin() { return array_.begin(); }
  auto end() { return array_.begin() + size_; }

  auto begin() const { return array_.begin(); }
  auto end() const { return array_.begin() + size_; }

  [[nodiscard]] size_t Size() const {
	return size_;
  }
  [[nodiscard]] size_t Capacity() const {
	return N;
  }

  void PushBack(const T &value) {
	if (size_ == N) {
	  throw overflow_error("StackVector::PushBack: no more space");
	}
	array_[size_] = value;
	++size_;
  }

  T PopBack() {
	if (size_ == 0) {
	  throw underflow_error("StackVector::PopBack: it is empty");
	}
	return array_[--size_];
  }

 private:
  array<T, N> array_ = {};
  size_t size_ = 0;
};

