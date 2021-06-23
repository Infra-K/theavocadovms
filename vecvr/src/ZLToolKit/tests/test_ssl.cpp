//============================================================================
// Name        : ToolKitTest.cpp
// Author      : xzl
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Util/logger.h"
#include "Util/util.h"
#ifdef ENABLE_OPENSSL
#include "Util/SSLBox.h"
#endif
using namespace std;
using namespace ZL::Util;

int main(int argc,char *argv[]) {
    setExePath(argv[0]);
	Logger::Instance().add(std::make_shared<ConsoleChannel> ("stdout", LTrace));
	Logger::Instance().setWriter(std::make_shared<AsyncLogWriter>());

#ifdef ENABLE_OPENSSL
	//请把证书"test_ssl.pem"放置在本程序同目录下 //이 프로그램의 동일한 디렉토리에 "test_ssl.pem"인증서를 넣으십시오.
	SSL_Initor::Instance().loadServerPem((exePath() + ".pem").data());
	SSL_Box client(false),server(true);

	client.setOnDecData([&](const char *data, uint32_t len){
		//解密后的明文;由client.onRecv触发 //복호화 된 일반 텍스트, client.onRecv에 의해 트리거 됨
		string str(data,len);
		InfoL << "client recv:" << str;
	});

	client.setOnEncData([&](const char *data, uint32_t len){
		//加密后的密文,发送给服务器;由client.onSend触发 //암호화 된 암호문이 서버로 전송되고 클라이언트에 의해 트리거됩니다.
		server.onRecv(data,len);
	});

	server.setOnDecData([&](const char *data, uint32_t len){
		//服务器收到数据并解密成明文，回显给客户端;由server.onRecv触发 //서버는 데이터를 수신하여 일반 텍스트로 복호화하고이를 클라이언트에 다시 에코합니다. server.onRecv에 의해 트리거됩니다.
		string str(data,len);
		InfoL << "server recv:" << str;
		server.onSend(data,len);
	});

	server.setOnEncData([&](const char *data, uint32_t len){
		//把加密的回显后信息回复给客户端;由server.onSend触发 //암호화 된 에코 정보를 사용하여 클라이언트에 응답하고 server.onSend에 의해 트리거됩니다.
		client.onRecv(data,len);
	});

	FatalL << "请输入字符开始测试,输入quit停止测试：" << endl; //테스트를 시작하려면 문자를 입력하고 테스트를 중지하려면 quit를 입력하십시오.
	string inbug;
	while(true){
		std::cin >> inbug;
		if(inbug == "quit"){
			break;
		}
		client.onSend(inbug.data(),inbug.size());
	}

#else
	FatalL << "ENABLE_OPENSSL 宏未打开";
#endif //ENABLE_OPENSSL

	Logger::Destory();

	return 0;
}
