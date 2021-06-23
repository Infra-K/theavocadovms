//============================================================================
// Name        : ToolKitTest.cpp
// Author      : xzl
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <unistd.h>
#include <iostream>
#include "Util/logger.h"
#ifdef ENABLE_MYSQL
#include "Util/SqlPool.h"
#endif
using namespace std;
using namespace ZL::Util;

int main() {
	Logger::Instance().add(std::make_shared<ConsoleChannel> ("stdout", LTrace));
	//Logger::Instance().setWriter(std::make_shared<AsyncLogWriter>());

#ifdef ENABLE_MYSQL

	/*
	 * 测试方法：테스트 방법 :
	 * 请按照实际数据库情况修改源码然后编译执行测试 실제 데이터베이스 상황에 따라 소스 코드를 수정 한 후 컴파일하여 테스트를 실행하십시오.
	 */
	string sql_ip = "127.0.0.1";
	unsigned short sql_port = 3306;
	string dbname = "db_test";
	string user = "user";
	string password = "password";
	string character = "utf8mb4";
#if (!defined(__GNUC__)) || (__GNUC__ >= 5)
	SqlPool::Instance().Init(sql_ip,sql_port,dbname,user,password/*,character*/);
#endif //(!defined(__GNUC__)) || (__GNUC__ >= 5)
	SqlPool::Instance().reSize(thread::hardware_concurrency());

	SqlPool::SqlRetType sqlRet;

	//同步插入 //동기식 삽입
	SqlWriter insertSql("insert into table_test(row0,row1) values('?','?');");
	insertSql<< "value0" << "value1" << sqlRet;
	DebugL << insertSql.getAffectedRows() << "," << insertSql.getRowID();

	//同步查询 //동기 쿼리
	SqlWriter sqlSelect("select row0,row1 from table_test where row_id=? order by row_id asc;") ;
	sqlSelect << "10010" << sqlRet;
	for(auto &line : sqlRet){
		DebugL << "row0:" << line[0] << ",row1:"<<  line[1];
	}

	//异步删除 //비동기 삭제
	SqlWriter insertDel("delete from table_test where row_id = '?';");
	insertDel << "10010" << endl;

	//如果SqlWriter 的 "<<" 操作符后面紧跟SqlPool::SqlRetType类型，则说明是同步操作并等待结果 // SqlWriter의 "<<"연산자 바로 뒤에 SqlPool :: SqlRetType 유형이 오면 동기 연산이며 결과를 기다리고 있음을 의미합니다.
	//如果紧跟std::endl ,则是异步操作，在后台线程完成sql操作。// std :: endl이 뒤에 오면 비동기 작업이며 SQL 작업은 백그라운드 스레드에서 완료됩니다.

#else
	FatalL << "ENABLE_MYSQL not defined!" << endl;
#endif//ENABLE_MYSQL

#ifdef ENABLE_MYSQL
	sleep(3);
	SqlPool::Destory();
#endif//ENABLE_MYSQL
	Logger::Destory();
	return 0;
}
