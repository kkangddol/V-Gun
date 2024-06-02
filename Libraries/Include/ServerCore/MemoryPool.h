#pragma once

enum
{
	SLIST_ALIGNMENT = 16,
};

/*
	MemoryHeader
*/

// ������
DECLSPEC_ALIGN(SLIST_ALIGNMENT)
struct MemoryHeader : public SLIST_ENTRY
{
	MemoryHeader(int32 size)
		: allocSize(size)
	{}

	// [MemoryHeader][Data]
	// memoryheader�� placement new�� ���� �� data�� ���� �ּҸ� ��ȯ
	static void* AttachHeader(MemoryHeader* header, int32 size)
	{
		new(header)MemoryHeader(size);

		return reinterpret_cast<void*>(++header);
	}

	// [MemoryHeader][Data]
	// MemoryHeader�� �����ϱ�
	static MemoryHeader* DetachHeader(void* ptr)
	{
		MemoryHeader* header = reinterpret_cast<MemoryHeader*>(ptr) - 1;

		return header;
	}

	int32 allocSize;
	// TODO : �ʿ��� �߰� ����
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

	// ����ϴ� �޸� ������
	int32 _allocSize = 0;

	// Ǯ���� ���� �޸� ����
	Atomic<int32> _useCount = 0;
	Atomic<int32> _reserveCount = 0;
};

