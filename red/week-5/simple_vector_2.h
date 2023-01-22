#include <cstdint>
#include <iterator>
#include <iostream>

using namespace std;

template<typename T>
class SimpleVector {
 public:
  SimpleVector() : data_(nullptr), end_(nullptr), size_(0), capacity_(0) {}

  explicit SimpleVector(const size_t num) : size_(num), capacity_(num) {
	data_ = new T[size_];
	end_ = data_ + size_;
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

  ~SimpleVector() {
	reset();
  }

  T &operator[](size_t num) {
	return data_[num];
  }

  T *begin() { return data_; }
  T *end() { return end_; }

  [[nodiscard]] size_t Size() const {
	return size_;
  }

  [[nodiscard]] size_t Capacity() const {
	return capacity_;
  }

  void PushBack(T val) {
	if (size_ == 0) {
	  data_ = new T[++capacity_];
	} else if (size_ == capacity_) {
	  T *old = data_;
	  capacity_ *= 2;
	  data_ = new T[capacity_];
	  move(old, old + size_, data_);
	  delete[] old;
	}
	data_[size_++] = move(val);
	end_ = data_ + size_;
  }

 private:
  T *data_;
  T *end_;
  size_t size_;
  size_t capacity_;
};
