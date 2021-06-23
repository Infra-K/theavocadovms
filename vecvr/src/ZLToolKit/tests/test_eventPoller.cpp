//============================================================================
// Name        : ToolKitTest.cpp
// Author      : xzl
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Util/logger.h"
#include "Poller/EventPoller.h"
#include "Util/TimeTicker.h"

using namespace std;
using namespace ZL::Util;
using namespace ZL::Poller;

int main() {
	Logger::Instance().add(std::make_shared<ConsoleChannel> ("stdout", LTrace));
	Logger::Instance().setWriter(std::make_shared<AsyncLogWriter>());
	EventPoller::Instance(true);
	Ticker timeTicker;
	TraceL << "主线程id:" << this_thread::get_id();//주 스레드 ID :

	DebugL << "开始异步操作"<< endl;//비동기 작업 시작
	timeTicker.resetTime();
	EventPoller::Instance().async([](){
		sleep(1);
		DebugL << "异步操作:" << this_thread::get_id() << endl;//비동기 작업 :
	});
	DebugL << "异步操作消耗时间：" <<  timeTicker.elapsedTime() << "ms" << endl;//비동기 작업은 시간을 소비합니다.

	InfoL << "开始同步操作"<< endl;//동기화 작업 시작
	timeTicker.resetTime();
	EventPoller::Instance().sync([](){
		sleep(1);
		InfoL << "同步操作:" << this_thread::get_id()<< endl;//동기 작업 :
	});
	InfoL << "同步操作消耗时间：" <<  timeTicker.elapsedTime() << "ms" << endl;//동기화 작업은 시간을 소비합니다.

	EventPoller::Destory();
	Logger::Destory();

	return 0;
}
