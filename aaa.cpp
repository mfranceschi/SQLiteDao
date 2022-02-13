#include <cassert>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

using namespace std;

namespace Streams {

template <typename T>
using ElementProcessor_t = const std::function<void(const T &)> &;

template <typename T> using Predicate_t = std::function<bool(const T &)>;

template <typename Source, typename Target>
using Mapper_t = std::function<Target(const Source &)>;
} // namespace Streams

template <typename T> struct Stream {
public:
  using ElementProcessor_t = const std::function<void(const T &)> &;
  using Predicate_t = std::function<bool(const T &)>;
  template <typename R> using Mapper_t = std::function<R(const T &)>;

  virtual Stream<T> *
  filter(const typename Stream<T>::Predicate_t &predicate) = 0;

  template <typename Target>
  inline Stream<Target> *
  map(const typename Stream<T>::Mapper_t<Target> &mapper);

  virtual vector<T> collectToVector() = 0;

  virtual std::size_t size() = 0;

  virtual void browse(ElementProcessor_t elementProcessor) = 0;

  virtual ~Stream() = default;
};

/**
 * @brief Parent class that implements common behaviours of streams.
 */
template <typename T> struct StreamImpl : public Stream<T> {
  std::size_t size() override {
    std::size_t result = 0;
    this->browse([&result](const T &) { result++; });
    return result;
  }

  Stream<T> *filter(const typename Stream<T>::Predicate_t &predicate) override {
    return new StreamFromFilter(this, predicate);
  }

  vector<T> collectToVector() override {
    std::vector<T> result{};
    this->browse([&result](const T &element) { result.push_back(element); });
    return result;
  }
};

template <typename T> struct StreamFromFilter : public StreamImpl<T> {
protected:
  using _Predicate_t = typename Stream<T>::Predicate_t;

public:
  StreamFromFilter(Stream<T> *source, const _Predicate_t &predicate)
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
  const _Predicate_t predicate;
};

template <typename Target, typename Source>
struct StreamFromMap : public StreamImpl<Target> {
protected:
  using _Mapper_t = Streams::Mapper_t<Source, Target>;

public:
  StreamFromMap(Stream<Source> *source, const _Mapper_t &mapper)
      : previous(source), mapper(mapper) {}

  void browse(
      typename Stream<Target>::ElementProcessor_t elementProcessor) override {
    const auto &_mapper = this->mapper;
    const auto &_proc = elementProcessor;

    typename Stream<Target>::ElementProcessor_t nestedElementProcessor =
        [&_mapper, &_proc](const Target &element) { _proc(_mapper(element)); };
    previous->browse(nestedElementProcessor);
  }

private:
  Stream<Source> *const previous;
  const _Mapper_t mapper;
};

template <typename Source>
template <typename Target>
Stream<Target> *
Stream<Source>::map(const typename Stream<Source>::Mapper_t<Target> &mapper) {
  return new StreamFromMap<Target, Source>(this, mapper);
}

template <typename T, typename Collection_t>
struct StreamFromCollection : public StreamImpl<T> {
public:
  StreamFromCollection(const Collection_t &collection)
      : collection(collection) {}

  vector<T> collectToVector() override { return vector<T>(collection); }

  std::size_t size() override { return collection.size(); }

  void
  browse(typename Stream<T>::ElementProcessor_t elementProcessor) override {
    for (const auto &element : collection) {
      elementProcessor(element);
    }
  }

protected:
  const Collection_t &collection;
};

template <typename T>
struct StreamFromEmbeddedCollection : public StreamImpl<T> {
  StreamFromEmbeddedCollection(const std::vector<T> &collection)
      : actualCollection(collection),
        underlyingStream(StreamFromCollection(collection)) {}

  vector<T> collectToVector() override { return actualCollection; }

  std::size_t size() override { return actualCollection.size(); }

  void
  browse(typename Stream<T>::ElementProcessor_t elementProcessor) override {
    return underlyingStream.browse(elementProcessor);
  }

protected:
  const std::vector<T> actualCollection;
  const StreamFromCollection<T, std::vector<T>> underlyingStream;
};

namespace Streams {
template <typename T> Stream<T> *fromCollection(const std::vector<T> &source) {

  return new StreamFromCollection<T, vector<T>>(source);
}
} // namespace Streams

namespace MyLambdas {
bool isIntPair(int input) { return input % 2 == 0; }
bool isIntNonZero(int input) { return input != 0; }
double convertToDoubleAndInvert(int input) { return 1. / (double)input; }

} // namespace MyLambdas

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &v) {
  os << "Vector,size=" << v.size() << "[";
  for (auto value : v) {
    os << value << ", ";
  }
  os << "]";
  return os;
}

int main() {
  vector<int> vecint{2, 22, 7, 987, -2, 0};

  Stream<int> *a = Streams::fromCollection(vecint);
  assert(a->collectToVector().size() == 6);

  Stream<int> *b = a->filter(MyLambdas::isIntPair);
  assert(b->collectToVector().size() == 4);

  Stream<int> *c = b->filter(MyLambdas::isIntNonZero);
  cout << c->collectToVector();

  Stream<double> *d = c->map<double>(MyLambdas::convertToDoubleAndInvert);
  cout << d->collectToVector();

  delete a;
  delete b;
  delete c;
  delete d;
}
