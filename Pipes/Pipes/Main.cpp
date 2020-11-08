#include <windows.h>
#include <iostream>
#include <queue>
#include <map>
#include <algorithm>

using namespace std;

constexpr auto MSGSIZE = 512;;

bool readpipe(HANDLE hRead, char* write_buf, DWORD byt_written) {
	DWORD byt_read = 0;
	bool r_status;
	r_status = ReadFile(hRead, write_buf, byt_written, &byt_read, NULL);
	CloseHandle(hRead);
	if (!r_status) {
		cout << "Read failed || Error: " << GetLastError() << endl;
		return -1;
	}

	cout << "Bytes Read : " << byt_read << endl;
	write_buf[byt_read] = '\0';
	cout << "Read Successful " << endl;
	cout << "Output : " << write_buf << endl;
	
}

DWORD writepipe(HANDLE hWrite, char *read_buf) {
	DWORD byt_written = 0;
	bool w_status;
	w_status = WriteFile(hWrite, read_buf, MSGSIZE, &byt_written, NULL);
	cout << "Bytes Write : " << byt_written << endl;
	CloseHandle(hWrite);
	if (!w_status) {
		cout << "Write failed || Error: " << GetLastError() << endl;
		return -1;
	}
	cout << "Write Successful!" << endl;
	return byt_written;
}

int main() {
	bool c_status;
	HANDLE hWrite = NULL, hRead = NULL;
	char read_buf[512] = "Harmless Monks";
	char write_buf[513] = "";
	DWORD byt_written = 0;

	c_status = CreatePipe(&hRead,&hWrite,NULL, 512);
	if (!c_status) {
		cout << "Pipe creation failed || Error: " << GetLastError() << endl;
		return -1;
	}
	cout << "Pipe Created!\n";
	byt_written = writepipe(hWrite, read_buf);
	if (byt_written < 0) {
		cout << "Nothing Written!" << endl;
		return -1;
	}
	readpipe(hRead, write_buf, byt_written);
	return 0;
}