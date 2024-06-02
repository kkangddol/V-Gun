#pragma once

#define OUT

/*
	Lock
*/

#define USE_MANY_LOCK(count)	Lock _locks[count];
#define USE_LOCK				USE_MANY_LOCK(1)

#define READ_LOCK_INDEX(idx)	ReadLockGuard readLockGuard_##idx(_locks[idx],typeid(this).name());
#define READ_LOCK				READ_LOCK_INDEX(0)

#define WRITE_LOCK_INDEX(idx)	WriteLockGuard writeLockGuard_##idx(_locks[idx],typeid(this).name());
#define WRITE_LOCK				WRITE_LOCK_INDEX(0)


/*
	Memory
*/

//#define _STOMP

/*
#ifdef _DEBUG
#define xxalloc(size)	PoolAllocator::Alloc(size)
#define xxrelease(ptr)	PoolAllocator::Release(ptr)
#else
#define xxalloc(size)	BaseAllocator::Alloc(size)
#define xxrelease(ptr)	BaseAllocator::Release(ptr)
#endif
*/



/*
	Crash
*/

#define CRASH(cause)						\
{											\
	uint32* crash = nullptr;				\
	__analysis_assume(crash != nullptr);	\
	*crash = 0xDEADBEEF;					\
}

#define ASSERT_CRASH(expr)					\
{											\
	if(!(expr))								\
	{										\
		CRASH("ASSERT_CRASH");				\
		__analysis_assume(expr);			\
	}										\
}