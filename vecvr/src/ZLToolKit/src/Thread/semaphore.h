/*
 * semaphore.h
 *
 *  Created on: 2015年8月14日
 *      Author: root
 */

#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

/*
 * 目前发现信号量在32位的系统上有问题，
 * 休眠的线程无法被正常唤醒，先禁用之
 * 현재 32 비트 시스템에서 세마포어에 문제가있는 것으로 확인 됨
 * 잠자는 스레드는 정상적으로 깨어날 수 없으므로 먼저 비활성화하십시오.
#ifdef  __linux__
#include <semaphore.h>
#define HAVE_SEM
#endif //HAVE_SEM
*/

#include <mutex>
#include <atomic>
#include <condition_variable>
using namespace std;

namespace ZL {
namespace Thread {

class semaphore {
public:
	explicit semaphore(unsigned int initial = 0) {
#ifdef HAVE_SEM
		sem_init(&sem, 0, initial);
#else
		count_ = 0;
#endif
	}
	~semaphore() {
#ifdef HAVE_SEM
		sem_destroy(&sem);
#endif
	}
	void post(unsigned int n = 1) {
#ifdef HAVE_SEM
		while (n--) {
			sem_post(&sem);
		}
#else
		unique_lock<mutex> lock(mutex_);
		count_ += n;
		while (n--) {
			condition_.notify_one();
		}
#endif

	}
	void wait() {
#ifdef HAVE_SEM
		sem_wait(&sem);
#else
		unique_lock<mutex> lock(mutex_);
		while (count_ == 0) {
			condition_.wait(lock);
		}
		--count_;
#endif
	}
private:
#ifdef HAVE_SEM
	sem_t sem;
#else
	atomic_uint count_;
	mutex mutex_;
	condition_variable_any condition_;
#endif
};
} /* namespace Thread */
} /* namespace ZL */
#endif /* SEMAPHORE_H_ */
