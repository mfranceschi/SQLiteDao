#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

using namespace std;

template <typename T> struct Stream {
public:
  virtual std::unique_ptr<Stream<T>>
  filter(const std::function<bool(const T &)> &predicate) const = 0;

  virtual vector<T> collectToVector() const = 0;
};

template <typename T> struct StreamFromFilter : public Stream<T> {
public:
  std::unique_ptr<Stream<T>>
  filter(const std::function<bool(const T &)> &predicate) const override {
    return std::make_unique<Stream<T>>(StreamFromFilter(this));
  }

  vector<T> collectToVector() const override {
    return vector<int>{}; // vector(sourceIterator, endSourceIterator);
  }
  StreamFromFilter(const Stream<T> *source) : previous(source) {}

private:
  const Stream<T> *previous;
};

template <typename T, typename T_iterator_t = T *>
struct StreamFromCollection : public Stream<T> {
public:
  StreamFromCollection(const T_iterator_t &start, const T_iterator_t &end)
      : sourceIterator(start), endSourceIterator(end) {}

  std::unique_ptr<Stream<T>>
  filter(const std::function<bool(const T &)> &predicate) const override {
    return make_unique<Stream<T>>(StreamFromFilter(this));
  }

  vector<T> collectToVector() const override {
    return vector(sourceIterator, endSourceIterator);
  }

private:
  T_iterator_t sourceIterator;
  const T_iterator_t endSourceIterator;
};

namespace Streams {
template <typename T>
std::unique_ptr<Stream<T>>
fromCollection(const std::vector<T> &sourceCollection) {
  const auto cbegin = sourceCollection.cbegin();
  const auto cend = sourceCollection.cend();

  return make_unique<Stream<T>>(
      StreamFromCollection<T, decltype(cbegin)>(cbegin, cend));
}
} // namespace Streams

int main() {
  vector<int> vecint;

  const auto lambdaIsPair = [](int input) { return input % 2 == 0; };
  vector<int> result =
      Streams::fromCollection(vecint)->filter(lambdaIsPair)->collectToVector();

  for (int v : result) {
    cout << v << ", " << endl;
  }
}
