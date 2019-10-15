#ifndef FIBRT_H_
#define FIBRT_H_

#define _XOPEN_SOURCE

#include <functional>
#include <ucontext.h>

class Scheduler;

class Fiber
{
public:
	typedef void (*Func)(Fiber *);
	friend class Scheduler;
	enum class Status { kDead, kReady, kRunning, kSuspend };
	using Event = std::function<void()>;

	Fiber(Scheduler &scheduler, Event event);
	~Fiber();

	void Resume();
	bool Status();

private:
	void run();
	void setFid(uint64_t fid) { fid_ = fid; }
	// void setMain(Event main_event) { main_ = main_event; }

private:
	enum Status status_;
	uint64_t fid_;
	Scheduler &scheduler_;
	Event event_;
	ucontext_t context_;
	ucontext_t *main_context_;
	Func main_;

	constexpr static int kStackSize = 4096;
	char stack_[kStackSize];
};

#endif // FIBRT_H_
