#pragma once

enum
{
	SLIST_ALIGNMENT = 16,
};

/*
	MemoryHeader
*/

// 디버깅용
DECLSPEC_ALIGN(SLIST_ALIGNMENT)
struct MemoryHeader : public SLIST_ENTRY
{
	MemoryHeader(int32 size)
		: allocSize(size)
	{}

	// [MemoryHeader][Data]
	// memoryheader를 placement new로 만든 후 data의 시작 주소를 반환
	static void* AttachHeader(MemoryHeader* header, int32 size)
	{
		new(header)MemoryHeader(size);

		return reinterpret_cast<void*>(++header);
	}

	// [MemoryHeader][Data]
	// MemoryHeader를 추출하기
	static MemoryHeader* DetachHeader(void* ptr)
	{
		MemoryHeader* header = reinterpret_cast<MemoryHeader*>(ptr) - 1;

		return header;
	}

	int32 allocSize;
	// TODO : 필요한 추가 정보
};

/*
	MemoryPool
*/

DECLSPEC_ALIGN(SLIST_ALIGNMENT)
class MemoryPool
{
public:
	MemoryPool(int32 allocSize);
	~MemoryPool();

	void Push(MemoryHeader* ptr);
	MemoryHeader* Pop();

private:
	SLIST_HEADER _header;

	// 담당하는 메모리 사이즈
	int32 _allocSize = 0;

	// 풀에서 나온 메모리 개수
	Atomic<int32> _useCount = 0;
	Atomic<int32> _reserveCount = 0;
};

