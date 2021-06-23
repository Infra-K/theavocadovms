//============================================================================
// Name        : ToolKitTest.cpp
// Author      : xzl
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <signal.h>
#include <iostream>
#include "Util/logger.h"
#include "Poller/EventPoller.h"
#include "Poller/Pipe.h"
#include "Util/util.h"
using namespace std;
using namespace ZL::Util;
using namespace ZL::Poller;

void programExit(int arg) {
	EventPoller::Instance().shutdown();
}
int main() {
	Logger::Instance().add(std::make_shared<ConsoleChannel>("stdout", LTrace));
	Logger::Instance().setWriter(std::make_shared<AsyncLogWriter>());
	auto parentPid = getpid();
	InfoL << "parent pid:" << parentPid << endl;
	Pipe pipe([](int size,const char *buf) {
		InfoL << getpid() << " recv:" << buf;
	});
	auto pid = fork();
	if (pid == 0) {
		//子进程 // 하위 프로세스
		int i = 10;
		while (i--) {
			sleep(1);
			auto msg = StrPrinter << "message " << i << " form subprocess:" << getpid() << endl;
			DebugL << "子进程发送：" << msg << endl;//하위 프로세스 전송 :
			pipe.send(msg.data(), msg.size());
		}
		DebugL << "子进程退出" << endl;//하위 프로세스가 종료됩니다.
	} else {
		//父进程 // 부모 프로세스
		signal(SIGINT, programExit);
		EventPoller::Instance().runLoop();
		EventPoller::Destory();
		InfoL << "父进程退出" << endl;//부모 프로세스가 종료됩니다.
	}

	Logger::Destory();

	return 0;
}
