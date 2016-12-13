#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#define _XOPEN_SOURCE

#include <unordered_map>
#include <memory>
#include <ucontext.h>

// ThreadLocal

class Fiber;

class Scheduler {
public:
	friend class Fiber;
	Scheduler();
	const Fiber* Current();
	void Yield();
	void RemoveFiber(Fiber *fiber);

private:
	std::unordered_map<uint64_t, Fiber*> fibers_;
	uint64_t fid_;
	uint64_t current_fid_;
	ucontext_t main_context_;

	void AddFiber(Fiber* fiber);

	static void Main(Fiber* fiber);
};

#endif // SCHEDULER_H_
