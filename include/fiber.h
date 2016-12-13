#ifndef FIBRT_H_
#define FIBRT_H_

#define _XOPEN_SOURCE

#include <functional>
#include <ucontext.h>

class Scheduler;

class Fiber {
public:
	typedef void (*Func)(Fiber *);
	friend class Scheduler;
	enum Status { kDead, kReady, kRunning, kSuspend };
	using Event = std::function<void()>;

	Fiber(Scheduler& scheduler, Event event);
	~Fiber();

	void Resume();
	bool Status();

private:
	uint64_t fid_;
	Scheduler& scheduler_;
	Event event_;
	enum Status status_;
	ucontext_t context_;
	ucontext_t* main_context_;
	Func main_;
	
	const static int STACK_SIZE = 4096;
	char stack_[STACK_SIZE];

	void Run();
	void SetFid(uint64_t fid) { fid_ = fid; }
	// void SetMain(Event main_event) { main_ = main_event; }
};

#endif // FIBRT_H_
