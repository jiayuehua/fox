#include <mutex>
#include <memory>
namespace fox {
class B {
public:
  virtual  void lock() = 0 ;
  virtual  void  unlock() = 0;
  virtual ~B()=0;
};
B::~B(){}

template <class T>
class D final :public B {
  T& f_;
public:
  D(T&f) :B(), f_(f) { f_.lock(); }
  void lock() { f_.lock(); }
  void unlock() { f_.unlock(); }
  ~D() { unlock(); }
};
class ScopedLock {
private:
  std::shared_ptr<B> uB;
public:
  template<class Mutex>
  ScopedLock(Mutex& m) :uB(std::make_shared<D<Mutex> >(m)) {}
  ScopedLock(const ScopedLock&) = delete;
  ScopedLock(ScopedLock&&) = delete;
  ScopedLock& operator=(const ScopedLock&) = delete;
  ScopedLock& operator=(ScopedLock&&) = delete;
};

}
