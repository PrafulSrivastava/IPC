#include <windows.h>
#include <iostream>
#include <queue>
#include <mutex>
#include <thread>

using namespace std;

constexpr auto MSGSIZE = 5;
constexpr auto MSGREADSIZE = MSGSIZE/2;
mutex mut;

bool readpipe(HANDLE hRead, char* write_buf, DWORD byt_written) {
	cout << "Read Thread ID :" << this_thread::get_id() << endl;
	lock_guard<mutex> lg(mut);
	DWORD byt_read = 0;
	bool r_status;
	DWORD read = 0;
	while (byt_read < byt_written) {
		r_status = ReadFile(hRead, write_buf+byt_read, MSGREADSIZE, &read, NULL);
		if (!r_status) {
			cout << "Read failed || Error: " << GetLastError() << endl;
			return -1;
		}
		byt_read += read;
		cout << "Bytes Read : " << byt_read << endl;
		cout << "Cycle Output : " << write_buf << endl;
	}	

	cout << "Output : " << write_buf << endl;
	write_buf[byt_read] = '\0';
	cout << "Read Successful " << endl;
	CloseHandle(hRead);
}

void writepipe(HANDLE hWrite, char *read_buf, DWORD &byt_written) {
	cout << "Write Thread ID :" << this_thread::get_id() << endl;
	lock_guard<mutex> lg(mut);
	bool w_status;
	w_status = WriteFile(hWrite, read_buf, MSGSIZE, &byt_written, NULL);
	cout << "Bytes Write : " << byt_written << endl;
	CloseHandle(hWrite);
	if (!w_status) {
		cout << "Write failed || Error: " << GetLastError() << endl;
		return ;
	}
	cout << "Write Successful!" << endl;
}

int main() {
	bool c_status;
	HANDLE hWrite = NULL, hRead = NULL;
	char read_buf[512] = "Harmless Monks";
	char write_buf[513] = "";
	DWORD byt_written = 0;

	c_status = CreatePipe(&hRead,&hWrite,NULL, MSGSIZE);
	if (!c_status) {
		cout << "Pipe creation failed || Error: " << GetLastError() << endl;
		return -1;
	}
	cout << "Pipe Created!\n";
	thread writeThread(&writepipe, hWrite,read_buf , std::ref(byt_written));
	writeThread.join();
	if (byt_written < 0) {
		cout << "Nothing Written!" << endl;
		return -1;
	}
	thread readThread(&readpipe, hRead, write_buf, byt_written);
	readThread.join();
	cout << "Final o/p: " << write_buf << endl;
	return 0;
}