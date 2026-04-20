#include "ProcessMessage.h"
#include <string>
#include <sstream>

bool ClientProcessMessage::connect_to_pip(){

	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\Pipe"),
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,   // FILE_FLAG_FIRST_PIPE_INSTANCE is not needed but forces CreateNamedPipe(..) to fail if the pipe already exists...
		1,
		1024 * 16,
		1024 * 16,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);
	while (hPipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(hPipe, NULL) != FALSE)
			return true;
	}
		
	return false;
}

void ClientProcessMessage::disconnect() {
	if (hPipe != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hPipe);
	}
}

std::string ClientProcessMessage::wait_to_data()
{
	if (hPipe == INVALID_HANDLE_VALUE)
		return "end";

	memset(buffer, '\0', sizeof(buffer));
	std::ostringstream  res;
	while (ReadFile(hPipe, buffer, sizeof(buffer), &dwRead, NULL) != FALSE)
	{
		res << buffer;
		if (dwRead == sizeof(buffer))
		{
			memset(buffer, '\0', sizeof(buffer));
			continue;
		}
	}

	return res.str();
}

bool ServerProcessMessage::connect_to_pip()
{
	hPipe = CreateFile(TEXT("\\\\.\\pipe\\Pipe"),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if (hPipe != INVALID_HANDLE_VALUE)
		return true;

	return false;
}

void ServerProcessMessage::disconnect()
{
	if (hPipe != INVALID_HANDLE_VALUE)
	{
		DisconnectNamedPipe(hPipe);
	}
}

void ServerProcessMessage::send(char const* send_data, uint32_t length)
{
	if (hPipe == INVALID_HANDLE_VALUE)
		return;

	WriteFile(hPipe,
		send_data,
		length,   // = length of string + terminating '\0' !!!
		&dwWritten,
		NULL);
}