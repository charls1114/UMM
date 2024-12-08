#include "ProcessManager.hpp"
namespace pipe_cpptopy {
	void _CPPTOPY_PIPE_INFO::clear() {
		(this->outText).clear();

		(this->isReportResultString) = false;
		(this->isUseInput) = false;
		(this->isUseWriteOutHandle) = false;

		(this->inputBuf) = nullptr;
		(this->WriteOutHandle) = nullptr;
	}

	int ProcessManager::CreateChildPipe() {
		BOOL fSuccess = FALSE;
		const wchar_t *lpszPipename = TEXT("\\\\.\\pipe\\module_piping");

		// Try to open a named pipe; wait for it, if necessary.
		while (true)
		{
			this->hPipe = CreateFile(
				lpszPipename,  // pipe name
				GENERIC_READ | // read and write access
				GENERIC_WRITE,
				0,             // no sharing
				NULL,          // default security attributes
				OPEN_EXISTING, // opens existing pipe
				0,             // default attributes
				NULL);         // no template file

			// Break if the pipe handle is valid.
			if (hPipe == INVALID_HANDLE_VALUE)
			{
				continue;
			}
			else
			{
				break;
			}
		}
		// The pipe connected; change to message-read mode.
		DWORD dwMode = PIPE_READMODE_MESSAGE;
		fSuccess = SetNamedPipeHandleState(
			this->hPipe,   // pipe handle
			&dwMode, // new pipe mode
			NULL,    // don't set maximum bytes
			NULL);   // don't set maximum time
		if (!fSuccess)
		{
			_tprintf(TEXT("SetNamedPipeHandleState failed. GLE=%d\n"), GetLastError());
			return -1;
		}
	}

	void ProcessManager::CreateChildProcess(LPCWSTR childProcess) {
		STARTUPINFO strInfo;

		RtlZeroMemory(&processInfo, sizeof(PROCESS_INFORMATION));
		RtlZeroMemory(&strInfo, sizeof(STARTUPINFO));
		//strInfo.hStdError = (this->Out_Write);
		//strInfo.hStdOutput = (this->Out_Write);
		//strInfo.hStdInput = (this->In_Read);
		//strInfo.dwFlags |= STARTF_USESTDHANDLES;
		strInfo.cb = sizeof(STARTUPINFO);

		wchar_t buffer[0x200];
		RtlZeroMemory(&buffer, 0x200);

		wsprintfW(buffer, L"../project/.venv/python.exe \"%s\"", childProcess);

		int bSuccess = CreateProcessW(0, buffer, 0, 0, TRUE, 0, 0, 0, &strInfo, &processInfo);
		if (bSuccess == 0) {
			throw std::runtime_error("CreateProcessW : Failed");
		}
	}

	void ProcessManager::WritePipe(LPPER_PIPEINFO pipeInfo) {
		char cbBuf[BUFSIZE];
		RtlZeroMemory(cbBuf, BUFSIZE);

		DWORD dwRead, bSuccess;

		if (pipeInfo->isUseInput == true) {
			char cbBuf[BUFSIZE];
			RtlZeroMemory(&cbBuf, BUFSIZE);
			sprintf(cbBuf, "%s_temp_file.txt", std::filesystem::temp_directory_path().string().c_str());

			std::ofstream ofs(cbBuf);
			ofs << pipeInfo->inputBuf << '\n';
		}

		for (;;) {
			bSuccess = ReadFile((this->hPipe), cbBuf, BUFSIZE, &dwRead, 0);
			if (bSuccess == FALSE || dwRead == 0) {
				break;
			}

			bSuccess = WriteFile((this->In_Write), cbBuf, dwRead, 0, 0);
			if (bSuccess == FALSE) {
				break;
			}
		}
	}

	void ProcessManager::ReadPipe(LPPER_PIPEINFO pipeInfo) {
		char cbBuf[BUFSIZE];

		DWORD dwRead, bSuccess;

		for (;;) {
			RtlZeroMemory(cbBuf, BUFSIZE);
			bSuccess = ReadFile((this->Out_Read), cbBuf, BUFSIZE, &dwRead, 0);
			if (bSuccess == FALSE || dwRead == 0) {
				break;
			}

			if (pipeInfo->isReportResultString == true) pipeInfo->outText.push_back(std::move(cbBuf));

			if (pipeInfo->isUseWriteOutHandle == true) {
				bSuccess = WriteFile(pipeInfo->WriteOutHandle, cbBuf, dwRead, 0, 0);
				if (bSuccess == FALSE) {
					break;
				}
			}
			else {
				bSuccess = WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), cbBuf, dwRead, 0, 0);
				if (bSuccess == FALSE) {
					break;
				}
			}
		}
	}

	void ProcessManager::Dispose() {
		TerminateProcess(processInfo.hProcess, 0);
		CloseHandle(processInfo.hProcess);
		CloseHandle(processInfo.hThread);
	}
	ProcessManager::ProcessManager() {

	}

	ProcessManager::~ProcessManager() {

	}
}