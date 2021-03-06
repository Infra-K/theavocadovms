//============================================================================
// Name        : ToolKitTest.cpp
// Author      : xzl
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <signal.h>
#include <unistd.h>
#include <iostream>
#include "Util/logger.h"
#include "Util/util.h"
#include "Util/ResourcePool.h"
#include "Thread/threadgroup.h"
#include <list>

using namespace std;
using namespace ZL::Util;
using namespace ZL::Thread;

bool g_bExitFlag = false;
ResourcePool<string,50> g_pool;

void onRun(int threadNum){
	while(!g_bExitFlag){
		auto obj_ptr = g_pool.obtain();
		if(obj_ptr->empty()){
			InfoL << "thread " << threadNum << ":" << "obtain a emptry object!";
		}else{
			InfoL << "thread " << threadNum << ":" << *obj_ptr;
		}
		obj_ptr->assign(StrPrinter << "keeped by thread:" << threadNum <<endl );
		usleep( 1000 * (random()% 10));
		obj_ptr.reset();//手动释放 //수동 해제
		usleep( 1000 * (random()% 1000));
	}
}
int main() {
	Logger::Instance().add(std::make_shared<ConsoleChannel>("stdout", LTrace));
	Logger::Instance().setWriter(std::make_shared<AsyncLogWriter>());

	//获取一个对象 //개체 가져 오기
	auto reservedObj = g_pool.obtain();
	reservedObj->assign("This is a reserved object , and will never be used!");

	thread_group group;
	for(int i = 0 ;i < 4 ; ++i){
		group.create_thread([i](){
			onRun(i);
		});
	}

	sleep(1);
	//此时reservedObj的值应该尚未被覆盖 // 현재 reservedObj 값을 덮어 쓰지 않아야합니다.
	WarnL << *reservedObj << endl;

	auto &objref = *reservedObj;
	//显式释放对象,让对象重新进入循环列队 // 객체를 명시 적으로 해제하고 객체가 루프 대기열에 다시 들어가도록합니다.
	reservedObj.reset();

	sleep(1);
	//此对象还在循环池内，引用应该还是有效的，但是值应该被覆盖了 ////이 개체는 여전히 순환 풀에 있으며 참조는 여전히 유효해야하지만 값을 덮어 써야합니다.
	WarnL << objref << endl;

	g_bExitFlag = true;
	group.join_all();
	Logger::Destory();

	return 0;
}











