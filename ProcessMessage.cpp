#include "ProcessMessage.h"
#include <string>
#include <sstream>

bool ClientProcessMessage::connect_to_pip(){

	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\Pipe"),
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
		1,
		1024 * 16,
		1024 * 16,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);

	if (hPipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(hPipe, NULL) != FALSE || GetLastError() == ERROR_PIPE_CONNECTED)
			return true;
		CloseHandle(hPipe);
		hPipe = INVALID_HANDLE_VALUE;
	}

	return false;
}

void ClientProcessMessage::disconnect() {
	if (hPipe != INVALID_HANDLE_VALUE)
	{
		DisconnectNamedPipe(hPipe);
		CloseHandle(hPipe);
		hPipe = INVALID_HANDLE_VALUE;
	}
}

std::string ClientProcessMessage::wait_to_data()
{
	if (hPipe == INVALID_HANDLE_VALUE)
		return "end";

	if (ReadFile(hPipe, buffer, sizeof(buffer), &dwRead, NULL) != FALSE)
	{
		if (dwRead > 0)
			return std::string(buffer, dwRead);
	}

	return "end";
}

bool ServerProcessMessage::connect_to_pip()
{
	while (true)
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

		if (GetLastError() != ERROR_FILE_NOT_FOUND)
			return false;
	}
}

void ServerProcessMessage::disconnect()
{
	if (hPipe != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hPipe);
		hPipe = INVALID_HANDLE_VALUE;
	}
}

void ServerProcessMessage::send(char const* send_data, uint32_t length)
{
	if (hPipe == INVALID_HANDLE_VALUE)
		return;

	DWORD dwWrittenTotal = 0;
	while (dwWrittenTotal < length)
	{
		if (!WriteFile(hPipe,
			send_data + dwWrittenTotal,
			length - dwWrittenTotal,
			&dwWritten,
			NULL))
			break;
		dwWrittenTotal += dwWritten;
	}

	FlushFileBuffers(hPipe);
}