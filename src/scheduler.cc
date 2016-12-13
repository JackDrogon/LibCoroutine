#include "scheduler.h"
#include "fiber.h"

#include <functional>
#include <cassert>

Scheduler::Scheduler() : fid_(0), current_fid_(0)
{
}

void Scheduler::AddFiber(Fiber* fiber)
{
	fiber->SetFid(++fid_);
	fibers_.insert(std::make_pair(fid_, fiber));
	fiber->main_ = &Scheduler::Main;
	fiber->main_context_ = &main_context_;
}

void Scheduler::Yield()
{
	Fiber* current_fiber = fibers_[current_fid_];
	current_fid_ = 0;
	current_fiber->status_ = Fiber::kSuspend;
	swapcontext(&current_fiber->context_, &main_context_);
}

void Scheduler::Main(Fiber* fiber)
{
	fiber->Run();
	fiber->status_ = Fiber::kDead;
	fiber->scheduler_.current_fid_ = 0;
	fiber->scheduler_.RemoveFiber(fiber);
}

void Scheduler::RemoveFiber(Fiber *fiber)
{
	assert(current_fid_ != fiber->fid_);
	fibers_.erase(fiber->fid_);
}

const Fiber* Scheduler::Current()
{
	return fibers_[current_fid_];
}
