#pragma once

#include <cstdlib>
#include <algorithm>

template<typename T>
class SimpleVector {
 public:
  SimpleVector() : data_(nullptr), end_(nullptr), size_(0), capacity_(0) {}
  explicit SimpleVector(const size_t num) {
	data_ = new T[num];
	size_ = num;
	capacity_ = num;
	end_ = data_ + num;
  }

  void reset() {
	if (capacity_ != 0) {
	  delete[] data_;
	}
	data_ = nullptr;
	end_ = nullptr;
	size_ = 0;
	capacity_ = 0;
  }

  SimpleVector &operator=(const SimpleVector &other) {
	if (this != &other) {
	  reset();
	  size_ = other.size_;
	  capacity_ = other.capacity_;
	  data_ = new T[capacity_];
	  std::copy(other.data_, other.data_ + size_, data_);
	  end_ = data_ + size_;
	}
	return *this;
  }

  ~SimpleVector() {
	reset();
  }

  T &operator[](const size_t index) {
	return data_[index];
  }

  T *begin() { return data_; }
  T *end() { return end_; }
  const T *begin() const { return data_; }
  const T *end() const { return end_; }

  [[nodiscard]] size_t Size() const {
	return size_;
  }

  [[nodiscard]] size_t Capacity() const {
	return capacity_;
  }

  void PushBack(const T &value) {
	if (size_ == 0) {
	  data_ = new T[++capacity_];
	} else if (size_ == capacity_) {
	  T *old = data_;
	  capacity_ *= 2;
	  data_ = new T[capacity_];
	  for (size_t i = 0; i < Size(); ++i) {
		data_[i] = old[i];
	  }
	  delete[] old;
	}
	data_[size_++] = value;
	end_ = data_ + size_;
  }

 private:
  T *data_;
  T *end_;
  size_t size_;
  size_t capacity_;
};
