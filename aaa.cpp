#include <cassert>
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
  using Predicate_t = std::function<bool(const T &)>;

  virtual Stream<T> *
  filter(const typename Stream<T>::Predicate_t &predicate) = 0;

  virtual vector<T> collectToVector() = 0;

  virtual void browse(ElementProcessor_t elementProcessor) = 0;

  virtual ~Stream() = default;
};

template <typename T> struct StreamFromFilter : public Stream<T> {
public:
  Stream<T> *filter(const typename Stream<T>::Predicate_t &predicate) override {
    return new StreamFromFilter(this, predicate);
  }

  vector<T> collectToVector() override {
    std::vector<T> result{};
    this->browse([&result](const T &element) { result.push_back(element); });
    return result;
  }

  StreamFromFilter(Stream<T> *source,
                   const typename Stream<T>::Predicate_t &predicate)
      : previous(source), predicate(predicate) {}

  void
  browse(typename Stream<T>::ElementProcessor_t elementProcessor) override {
    const auto &_pred = this->predicate;
    const auto &_proc = elementProcessor;

    typename Stream<T>::ElementProcessor_t nestedElementProcessor =
        [&_pred, &_proc](const T &element) {
          if (_pred(element)) {
            _proc(element);
          }
        };
    previous->browse(nestedElementProcessor);
  }

private:
  Stream<T> *const previous;
  typename Stream<T>::Predicate_t predicate;
};

template <typename T, typename Collection_t>
struct StreamFromCollection : public Stream<T> {
public:
  StreamFromCollection(const Collection_t &collection)
      : collection(collection) {}

  Stream<T> *filter(const typename Stream<T>::Predicate_t &predicate) override {
    return new StreamFromFilter<T>(this, predicate);
  }

  vector<T> collectToVector() override { return vector<T>(collection); }

  void
  browse(typename Stream<T>::ElementProcessor_t elementProcessor) override {
    for (const auto &element : collection) {
      elementProcessor(element);
    }
  }

private:
  const Collection_t &collection;
};

namespace Streams {
template <typename T>
Stream<T> *fromCollection(const std::vector<T> &sourceCollection) {

  return new StreamFromCollection<T, vector<T>>(sourceCollection);
}
} // namespace Streams

int main() {
  vector<int> vecint{2, 22, 7, 987, -2, 0};

  const auto lambdaIsPair = [](int input) { return input % 2 == 0; };
  const auto lambdaIsNonNull = [](int input) { return input != 0; };

  Stream<int> *a = Streams::fromCollection(vecint);
  assert(a->collectToVector().size() == 6);

  Stream<int> *b = a->filter(lambdaIsPair);
  assert(b->collectToVector().size() == 4);

  Stream<int> *c = b->filter(lambdaIsNonNull);

  vector<int> result = c->collectToVector();
  cout << "Result size = " << result.size() << endl;

  for (int v : result) {
    cout << v << ", " << endl;
  }

  delete a;
  delete b;
  delete c;
}
