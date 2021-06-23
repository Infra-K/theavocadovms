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
#include "Thread/AsyncTaskThread.h"

using namespace std;
using namespace ZL::Util;
using namespace ZL::Poller;

void programExit(int arg) {
	EventPoller::Instance().shutdown();
}
int main() {
	signal(SIGINT, programExit);
	Logger::Instance().add(std::make_shared<ConsoleChannel>("stdout", LTrace));
	Logger::Instance().setWriter(std::make_shared<AsyncLogWriter>());

	std::shared_ptr<int> pCount1(new int(0));
	AsyncTaskThread::Instance().DoTaskDelay(1,1000,[pCount1](){
		DebugL << "定时器1:" << ++(*pCount1);//타이머 1 :
		return true;
	});

	AsyncTaskThread::Instance().DoTaskDelay(1,1000,[](){
		DebugL << "类型1定时器可以同名";//유형 1 타이머는 동일한 이름을 가질 수 있습니다.
		return true;
	});

	AsyncTaskThread::Instance().DoTaskDelay(2,5000,[](){
		AsyncTaskThread::Instance().CancelTask(1);
		DebugL << "五秒后取消类型1的所有定时器";//5 초 후에 유형 1의 모든 타이머를 취소합니다.
		return false;
	});

	EventPoller::Instance().runLoop();
	EventPoller::Destory();
	Logger::Destory();

	return 0;
}

