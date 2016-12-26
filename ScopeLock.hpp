#include <mutex>
namespace fox {
class B {
public:
  virtual  void lock() { }
  virtual  void unlock() { }
  virtual ~B()
  {
  }
};
template <class T>
class D :public B {
  T& f_;
public:
  D(T&f) :B(), f_(f) { f_.lock(); }
  void lock() { f_.lock(); }
  void unlock() { f_.unlock(); }
  ~D() { unlock(); }
};
class ScopeLock {
private:
  std::shared_ptr<B> uB;
public:
  template<class Mutex>
  ScopeLock(Mutex& m) :uB(std::make_shared<D<Mutex> >(m)) {}
  ScopeLock(const ScopeLock&) = delete;
  ScopeLock(ScopeLock&&) = delete;
  ScopeLock& operator=(const ScopeLock&) = delete;
  ScopeLock& operator=(ScopeLock&&) = delete;
};

}
