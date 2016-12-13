#include "fiber.h"
#include "scheduler.h"

#include <cassert>

Fiber::Fiber(Scheduler& scheduler, Event event)
	: scheduler_(scheduler), event_(event), status_(kReady)
{
	scheduler_.AddFiber(this);
}

Fiber::~Fiber()
{
	scheduler_.RemoveFiber(this);
}

bool Fiber::Status()
{
	return status_;
}

void Fiber::Resume()
{
	assert(scheduler_.current_fid_ == 0);
	switch(status_) {
		case kReady:
			getcontext(&context_);
			context_.uc_stack.ss_sp = stack_;
			context_.uc_stack.ss_size = STACK_SIZE;
			context_.uc_stack.ss_flags = 0;
			context_.uc_link = main_context_;
			status_ = kRunning;
			makecontext(&context_,  (void (*)())main_, 1, this);
		case kSuspend:
			scheduler_.current_fid_ = fid_;
			swapcontext(main_context_, &context_);
			break;
		default:
			assert(0);
	}
}

void Fiber::Run()
{
	event_();
}
