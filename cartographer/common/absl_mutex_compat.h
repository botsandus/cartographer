#ifndef CARTOGRAPHER_COMMON_ABSL_MUTEX_COMPAT_H_
#define CARTOGRAPHER_COMMON_ABSL_MUTEX_COMPAT_H_

#include "absl/base/thread_annotations.h"
#include "absl/synchronization/mutex.h"

// Compatibility shim for absl::MutexLock across Abseil versions.
//
// Newer Abseil deprecated the pointer-taking scoped-lock constructors
// (`MutexLock(Mutex*)`) in favour of reference-taking ones
// (`MutexLock(Mutex&)`), emitting -Wdeprecated-declarations for the former.
// Older Abseil only provides the pointer-taking constructor and has no
// reference overload at all. That leaves no single call form that compiles
// warning-free against both.
//
// This drop-in scoped lock locks/unlocks the mutex directly (Lock()/Unlock()
// are not deprecated and exist in every Abseil version) and accepts both a
// reference and a pointer, so it builds cleanly everywhere with no version
// detection. Use it in place of absl::MutexLock.
namespace cartographer {
namespace common {

class ABSL_SCOPED_LOCKABLE MutexLock {
 public:
  explicit MutexLock(absl::Mutex& mu) ABSL_EXCLUSIVE_LOCK_FUNCTION(&mu)
      : mu_(&mu) {
    mu_->Lock();
  }
  explicit MutexLock(absl::Mutex* mu) ABSL_EXCLUSIVE_LOCK_FUNCTION(mu)
      : mu_(mu) {
    mu_->Lock();
  }
  ~MutexLock() ABSL_UNLOCK_FUNCTION() { mu_->Unlock(); }

  MutexLock(const MutexLock&) = delete;
  MutexLock(MutexLock&&) = delete;
  MutexLock& operator=(const MutexLock&) = delete;
  MutexLock& operator=(MutexLock&&) = delete;

 private:
  absl::Mutex* const mu_;
};

}  // namespace common
}  // namespace cartographer

#endif  // CARTOGRAPHER_COMMON_ABSL_MUTEX_COMPAT_H_
