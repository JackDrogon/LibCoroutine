#pragma once

#define COROUTINE_DISALLOW_COPY_AND_ASSIGN(Type)                               \
	Type(const Type &) = delete;                                           \
	void operator=(const Type &) = delete
