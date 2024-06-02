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

/// IocpEvent를 상속받는 클래스들은 Virtual 함수를 사용하면 안됨
/// OVERLAPPED를 상속받아 첫번째 Offset이 OVERLAPPED 구조체를 가르키지만
/// 상속받으면 가상 함수 테이블을 가리키기 때문에

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



