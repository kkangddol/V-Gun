#pragma once

class Session;

enum class EventType : uint8
{
	Connect,
	Disconnect,
	Accept,
	// PreRecv,
	Recv,
	Send,
};

/*
	IocpEvent
*/

class IocpEvent : public OVERLAPPED
{
public:
	IocpEvent(EventType type);

	void Init();

public:
	EventType eventType;
	IocpObjectRef owner;
};

/// IocpEvent�� ��ӹ޴� Ŭ�������� Virtual �Լ��� ����ϸ� �ȵ�
/// OVERLAPPED�� ��ӹ޾� ù��° Offset�� OVERLAPPED ����ü�� ����Ű����
/// ��ӹ����� ���� �Լ� ���̺��� ����Ű�� ������

/*
	ConnectEvent
*/

class ConnectEvent : public IocpEvent
{
public:
	ConnectEvent() : IocpEvent(EventType::Connect) {}
};

/*
	DisConnectEvent
*/

class DisconnectEvent : public IocpEvent
{
public:
	DisconnectEvent() : IocpEvent(EventType::Disconnect) {}
};

/*
	AcceptEvent
*/

class AcceptEvent : public IocpEvent
{
public:
	AcceptEvent() : IocpEvent(EventType::Accept) {}

public:
	SessionRef session = nullptr;
};

/*
	RecvEvent
*/

class RecvEvent : public IocpEvent
{
public:
	RecvEvent() : IocpEvent(EventType::Recv) {}
};

/*
	SendEvent
*/

class SendEvent : public IocpEvent
{
public:
	SendEvent() : IocpEvent(EventType::Send) {}

	// Temp
	Vector<SendBufferRef> sendBuffers;
};



