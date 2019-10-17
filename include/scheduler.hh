#pragma once

#define _XOPEN_SOURCE

#include <unordered_map>
#include <memory>

#include <ucontext.h>

#include "common.hh"

// ThreadLocal

class Fiber;

class Scheduler
{
public:
	friend class Fiber;
	Scheduler();
	~Scheduler() = default;

	COROUTINE_DISALLOW_COPY_AND_ASSIGN(Scheduler);

	const Fiber *Current();
	void Yield();
	void RemoveFiber(Fiber *fiber);

private:
	void addFiber(Fiber *fiber);
	static void main(Fiber *fiber);

private:
	std::unordered_map<uint64_t, Fiber *> fibers_;
	uint64_t fid_;
	uint64_t current_fid_;
	ucontext_t main_context_;
};
