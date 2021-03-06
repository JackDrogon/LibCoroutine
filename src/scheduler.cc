#include "scheduler.hh"

#include <functional>
#include <cassert>

#include "fiber.hh"

Scheduler::Scheduler() : fid_(0), current_fid_(0) {}

void Scheduler::addFiber(Fiber *fiber)
{
	fiber->setFid(++fid_);
	fibers_.insert(std::make_pair(fid_, fiber));
	fiber->main_ = &Scheduler::main;
	fiber->main_context_ = &main_context_;
}

void Scheduler::main(Fiber *fiber)
{
	fiber->run();
	fiber->status_ = Fiber::Status::kDead;
	fiber->scheduler_->current_fid_ = 0;
	fiber->scheduler_->RemoveFiber(fiber);
}

void Scheduler::Yield()
{
	Fiber *current_fiber = fibers_[current_fid_];
	current_fid_ = 0;
	current_fiber->status_ = Fiber::Status::kSuspend;
	swapcontext(&current_fiber->context_, &main_context_);
}


void Scheduler::RemoveFiber(Fiber *fiber)
{
	assert(current_fid_ != fiber->fid_);
	fibers_.erase(fiber->fid_);
}

const Fiber *Scheduler::Current() { return fibers_[current_fid_]; }
