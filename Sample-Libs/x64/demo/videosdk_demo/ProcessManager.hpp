#pragma once
#pragma warning(disable:4996)

#include <vector>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

#define BUFSIZE (0x1000)

namespace pipe_cpptopy {
	typedef struct _CPPTOPY_PIPE_INFO {
		std::vector<std::string> outText;
		HANDLE WriteOutHandle;
		bool isUseInput, isUseWriteOutHandle, isReportResultString;
		char* inputBuf;

		void clear();

	} PER_PIPEINFO, *LPPER_PIPEINFO;

	class ProcessManager {
	public:
		/**
		 * @brief CreateChildPipe
		 * @exception std::runtime_error
		*/
		int CreateChildPipe();

		/**
		 * @brief CreateChildProcess
		 * @exception std::runtime_error
		*/
		void CreateChildProcess(LPCWSTR);

		/**
		 * @brief WritePipe
		 * @exception std::runtime_error
		*/
		void WritePipe(LPPER_PIPEINFO);

		/**
		 * @brief ReadPipe
		 * @exception std::runtime_error
		*/
		void ReadPipe(LPPER_PIPEINFO);

		/**
		 * @brief Dispose
		 * @exception std::runtime_error
		*/
		void Dispose();

		const HANDLE GetPipe() { return hPipe; }

		ProcessManager();
		~ProcessManager();

	private:
		PROCESS_INFORMATION processInfo;
		HANDLE hPipe;
		HANDLE Out_Write, Out_Read, In_Write, In_Read;

	};
}