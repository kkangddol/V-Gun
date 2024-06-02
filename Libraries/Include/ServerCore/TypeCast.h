#pragma once
#include "Types.h"

#pragma region TypeList

template <typename... T>
struct TypeList;

template <typename T, typename U>
struct TypeList<T, U>
{
	using Head = T;
	using Tail = U;
};

template <typename T, typename... U>
struct TypeList<T, U...>
{
	using Head = T;
	using Tail = TypeList<U...>;
};

#pragma endregion

#pragma region Length

template <typename T>
struct Length;

template <>
struct Length<TypeList<>>
{
	// enum { value = 0 };
	constexpr static int32 value = 0;
};

template <typename T, typename... U>
struct Length<TypeList<T, U...>>
{
	// enum { value = 1 + Length<TypeList<U...>>::value };
	constexpr static int32 value = 1 + Length<TypeList<U...>>::value;
};

#pragma endregion

#pragma region TypeAt

template <typename TL, int32 Index>
struct TypeAt;

template <typename Head, typename... Tail>
struct TypeAt<TypeList<Head, Tail...>, 0>
{
	using Result = Head;
};

template < typename Head, typename... Tail, int32 Index>
struct TypeAt<TypeList<Head, Tail...>, Index>
{
	using Result = typename TypeAt<TypeList<Tail...>, Index - 1>::Result;
};

#pragma endregion

#pragma region IndexOf

template <typename TL, typename T>
struct IndexOf;

template <typename T>
struct IndexOf<TypeList<>, T>
{
	// enum { value = -1 };
	constexpr static int32 value = -1;
};

template <typename... Tail, typename T>
struct IndexOf<TypeList<T, Tail...>, T>
{
	// enum { value = 0 };
	constexpr static int32 value = 0;
};

template <typename Head, typename... Tail, typename T>
struct IndexOf<TypeList<Head, Tail...>, T>
{
private:
	enum { temp = IndexOf<TypeList<Tail...>, T>::value };
	// constexpr static int32 temp = IndexOf<TypeList<Tail...>, T>::value;
public:
	enum { value = (temp == -1) ? -1 : temp + 1 };
	// constexpr static int32 value = (temp == -1) ? -1 : temp + 1;
};

#pragma endregion

#pragma region Conversion
// From�� �����Ϳ��� To�� �����ͷ� ��ȯ�� ��������
template <typename From, typename To>
class Conversion
{
private:
	using Small = int8;
	using Big = int32;

	// �����Ϸ����� �������� �ɼ��� ���� ��
	// �����Ϸ��� ���� �׷����� �ڵ带 ã�� Ư¡�� �̿���
	static Small Test(const To&) { return 0; }
	static Big Test(...) { return 0; }
	static From MakeFrom() { return 0; }

public:
	enum { exists = sizeof(Test(MakeFrom())) == sizeof(Small) };

};

#pragma endregion

#pragma region TypeCast

// ���ڸ� Ŭ������ ����� ������ Ÿ�Ӷ� �ν��ϰ� �ϱ� ����
template <int32 v>
struct Int2Type
{
	enum { value = v };
};

// ��ȯ ���� ���̺� �����
template <typename TL>
class TypeConversion
{
public:
	enum { length = Length<TL>::value };

	TypeConversion()
	{
		// ���̺� 1x1���� �����
		MakeTable(Int2Type<0>(), Int2Type<0>());
	}

	template<int32 i, int32 j>
	static void MakeTable(Int2Type<i>, Int2Type<j>)
	{
		using FromType = typename TypeAt<TL, i>::Result;
		using ToType = typename TypeAt<TL, j>::Result;

		if (Conversion<const FromType*, const ToType*>::exists)
			s_convert[i][j] = true;
		else
			s_convert[i][j] = false;

		// ��������� ���̺��� J�� ��ĭ�� ��� ä��
		MakeTable(Int2Type<i>(), Int2Type<j + 1>());
	}

	// J�� Ŀ���� length�� ũ����� ���� ���Ⱑ ȣ���
	// i �� ��ĭ �÷��ְ� �ٽ� j ä��� - 2���� �迭�̴ϱ�
	template <int32 i>
	static void MakeTable(Int2Type<i>, Int2Type<length>)
	{
		MakeTable(Int2Type<i + 1>(), Int2Type<0>());
	}

	// i �� length ��ŭ ���� ��
	template <int32 j>
	static void MakeTable(Int2Type<length>, Int2Type<j>)
	{}

	static inline bool CanConvert(const int32& from, const int32& to)
	{
		// �ѹ��� ����� static
		static TypeConversion conversion;
		return s_convert[from][to];
	}

public:
	static bool s_convert[length][length];
};

// static ����
template <typename TL>
bool TypeConversion<TL>::s_convert[length][length];

template <typename To, typename From>
To TypeCast(From* ptr)
{
	if (ptr == nullptr)
		return nullptr;

	using TL = typename From::TL;

	if (TypeConversion<TL>::CanConvert(ptr->_typeId, IndexOf<TL, remove_pointer_t<To>>::value))
		return static_cast<To>(ptr);

	return nullptr;
}

template <typename To, typename From>
shared_ptr<To> TypeCast(shared_ptr<From> ptr)
{
	if (ptr == nullptr)
		return nullptr;

	using TL = typename From::TL;

	if (TypeConversion<TL>::CanConvert(ptr->_typeId, IndexOf<TL, remove_pointer_t<To>>::value))
		return static_pointer_cast<To>(ptr);

	return nullptr;
}

template <typename To, typename From>
bool CanCast(From* ptr)
{
	if (ptr == nullptr)
		return false;

	using TL = typename From::TL;

	return TypeConversion<TL>::CanConvert(ptr->_typeId, IndexOf<TL, remove_pointer_t<To>>::value);
}

template <typename To, typename From>
bool CanCast(shared_ptr<From> ptr)
{
	if (ptr == nullptr)
		return false;

	using TL = typename From::TL;

	return TypeConversion<TL>::CanConvert(ptr->_typeId, IndexOf<TL, remove_pointer_t<To>>::value);
}

#pragma endregion

#define DECLARE_TL		using TL = TL; int32 _typeId;
#define INIT_TL(Type)	_typeId = IndexOf<TL, Type>::value;