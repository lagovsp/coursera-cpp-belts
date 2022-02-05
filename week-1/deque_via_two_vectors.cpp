#include <iostream>
#include <vector>

#define DEQUE_EXCEPTION                                                        \
  do {                                                                         \
    if (index >= Size()) {                                                     \
      throw std::out_of_range("Deque out of range");                           \
    }                                                                          \
  } while (false)

template<typename T>
class Deque {
 public:
  Deque() = default;

  bool Empty() const { return f.empty() && b.empty(); }
  size_t Size() const { return f.size() + b.size(); }

  void PushFront(const T &item) { f.push_back(item); }
  void PushBack(const T &item) { b.push_back(item); }

  T &operator[](size_t index) {
	return index < f.size() ? f[f.size() - index - 1] : b[index - f.size()];
  }

  const T &operator[](size_t index) const {
	return index < f.size() ? f[f.size() - index - 1] : b[index - f.size()];
  }

  T &At(size_t index) {
	DEQUE_EXCEPTION;
	return (*this)[index];
  }

  const T &At(size_t index) const {
	DEQUE_EXCEPTION;
	return this->operator[](index);
  }

  T &Front() { return f.empty() ? b.front() : f.back(); }
  const T &Front() const { return f.empty() ? b.front() : f.back(); }

  T &Back() { return b.empty() ? f.front() : b.back(); }
  const T &Back() const { return b.empty() ? f.front() : b.back(); }

 private:
  std::vector<T> f, b;
};

int main() {
  return 0;
}