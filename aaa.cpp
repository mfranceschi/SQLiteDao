#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

using namespace std;

template <typename T> struct Stream {
public:
  using ElementProcessor_t = const std::function<void(const T &)> &;
  using Predicate_t = const std::function<bool(const T &)> &;

  virtual Stream<T> *filter(Predicate_t predicate) = 0;

  virtual vector<T> collectToVector() = 0;

  virtual void browse(ElementProcessor_t elementProcessor) = 0;
};

template <typename T> struct StreamFromFilter : public Stream<T> {
public:
  Stream<T> *filter(typename Stream<T>::Predicate_t predicate) override {
    return new StreamFromFilter(this, predicate);
  }

  vector<T> collectToVector() override {
    std::vector<T> result{};
    this->browse([&result](const T &element) { result.push_back(element); });
    return result;
  }

  StreamFromFilter(Stream<T> *source, typename Stream<T>::Predicate_t predicate)
      : previous(source), predicate(predicate) {}

  void
  browse(typename Stream<T>::ElementProcessor_t elementProcessor) override {
    const typename Stream<T>::ElementProcessor_t nestedElementProcessor =
        [&](const T &element) {
          if (predicate(element)) {
            elementProcessor(element);
          }
        };
    previous->browse(nestedElementProcessor);
  }

private:
  Stream<T> *const previous;
  typename Stream<T>::Predicate_t predicate;
};

template <typename T, typename T_iterator_t>
struct StreamFromCollection : public Stream<T> {
public:
  StreamFromCollection(const T_iterator_t &start, const T_iterator_t &end)
      : sourceIterator(start), endSourceIterator(end) {}

  Stream<T> *filter(typename Stream<T>::Predicate_t predicate) override {
    return new StreamFromFilter<T>(this, predicate);
  }

  vector<T> collectToVector() override {
    return vector<T>(sourceIterator, endSourceIterator);
  }

  void
  browse(typename Stream<T>::ElementProcessor_t elementProcessor) override {
    std::remove_reference_t<std::remove_const_t<T_iterator_t>> browseIterator(
        sourceIterator);
    for (; browseIterator != endSourceIterator; browseIterator++) {
      elementProcessor(*browseIterator);
    }
  }

private:
  const T_iterator_t &sourceIterator;
  const T_iterator_t &endSourceIterator;
};

namespace Streams {
template <typename T>
Stream<T> *fromCollection(const std::vector<T> &sourceCollection) {

  auto cbegin = sourceCollection.cbegin();
  auto cend = sourceCollection.cend();

  return new StreamFromCollection<T, decltype(cbegin)>(cbegin, cend);
}
} // namespace Streams

int main() {
  vector<int> vecint{2, 22, 7, 987, -2, 0};

  const auto lambdaIsPair = [](int input) { return input % 2 == 0; };
  const auto lambdaIsNonNull = [](int input) { return input != 0; };

  vector<int> result = Streams::fromCollection(vecint)
                           ->filter(lambdaIsPair)
                           ->filter(lambdaIsNonNull)
                           ->collectToVector();
  cout << "Result size = " << result.size() << endl;

  for (int v : result) {
    cout << v << ", " << endl;
  }
}
