#pragma once
#include <string>
#include <Windows.h>
class ProcessMessage
{
public:
	ProcessMessage(): hPipe(INVALID_HANDLE_VALUE) {}
	~ProcessMessage() {}
protected:
	HANDLE hPipe;
};


class ClientProcessMessage : public ProcessMessage
{
public:
	ClientProcessMessage() {}
	~ClientProcessMessage() { disconnect(); }
	bool connect_to_pip();
	void disconnect();
	std::string wait_to_data();
private:
	char buffer[2048];
	DWORD dwRead;
};

class ServerProcessMessage : public ProcessMessage
{
public:
	ServerProcessMessage() {}
	~ServerProcessMessage() { disconnect(); }
	bool connect_to_pip();
	void disconnect();
	void send(char const* send_data, uint32_t length);
private:
	DWORD dwWritten;
};
