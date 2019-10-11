#include "fiber.h"

#include <cassert>

#include "scheduler.h"

Fiber::Fiber(Scheduler &scheduler, Event event)
    : scheduler_(scheduler), event_(event), status_(Status::kReady)
{
	assert(event != nullptr);

	scheduler_.AddFiber(this);
}

Fiber::~Fiber() { scheduler_.RemoveFiber(this); }

bool Fiber::Status() { return status_ != Status::kDead; }

void Fiber::Resume()
{
	assert(scheduler_.current_fid_ == 0);

	switch (status_) {
	case Status::kReady:
		getcontext(&context_);
		context_.uc_stack.ss_sp = stack_;
		context_.uc_stack.ss_size = STACK_SIZE;
		context_.uc_stack.ss_flags = 0;
		context_.uc_link = main_context_;
		status_ = Status::kRunning;
		makecontext(&context_, (void (*)())main_, 1, this);
		[[fallthrough]];

	case Status::kSuspend:
		scheduler_.current_fid_ = fid_;
		swapcontext(main_context_, &context_);
		break;

	case Status::kRunning:
		[[fallthrough]];

	case Status::kDead:
		[[fallthrough]];

	default:
		assert(0);
	}
}

void Fiber::Run() { event_(); }
