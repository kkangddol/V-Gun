#pragma once


/*
	NetAddress
*/

class NetAddress
{
public:
	NetAddress() = default;
	NetAddress(SOCKADDR_IN sockAddr);
	NetAddress(std::wstring ip, uint16 port);

	SOCKADDR_IN& GetSockAddr() { return _sockAddr; }
	std::wstring GetIpAddr();
	uint16 GetPortAddr() { return ::ntohs(_sockAddr.sin_port); }

public:
	static IN_ADDR Ip2Address(const WCHAR* ip);
	

private:
	SOCKADDR_IN _sockAddr = {};
};

