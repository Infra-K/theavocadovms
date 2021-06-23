//============================================================================
// Name        : main.cpp
// Author      : 熊子良
// Version     :
//============================================================================

#include <signal.h>
#include <unistd.h>
#include <iostream>
#include "Rtsp/UDPServer.h"
#include "Rtsp/RtspSession.h"
#include "Rtmp/RtmpSession.h"
#include "Http/HttpSession.h"

#ifdef ENABLE_OPENSSL
#include "Util/SSLBox.h"
#include "Http/HttpsSession.h"
#endif//ENABLE_OPENSSL

#include "Util/logger.h"
#include "Util/onceToken.h"
#include "Util/File.h"
#include "Network/TcpServer.h"
#include "Poller/EventPoller.h"
#include "Thread/WorkThreadPool.h"
#include "Device/PlayerProxy.h"
#include "Shell/ShellSession.h"
#include "Common/config.h"
#include <map>
using namespace std;
using namespace ZL::Util;
using namespace ZL::Http;
using namespace ZL::Rtsp;
using namespace ZL::Rtmp;
using namespace ZL::Shell;
using namespace ZL::Thread;
using namespace ZL::Network;
using namespace ZL::DEV;

void programExit(int arg) {
	EventPoller::Instance().shutdown();
}

int main(int argc,char *argv[]) {
    setExePath(argv[0]);
	signal(SIGINT, programExit);
	Logger::Instance().add(std::make_shared<ConsoleChannel>("stdout", LTrace));
	Config::loaIniConfig();
	//support rtmp and rtsp url
	//just support H264+AAC
	auto urlList = {"rtmp://live.hkstv.hk.lxdns.com/live/hks",
					"rtsp://184.72.239.149/vod/mp4://BigBuckBunny_175k.mov"};
	map<string , PlayerProxy::Ptr> proxyMap;
	int i=0;
	for(auto url : urlList) {
		 //PlayerProxy构造函数前两个参数分别为应用名（app）,流id（streamId）//PlayerProxy 생성자의 처음 두 매개 변수는 애플리케이션 이름 (앱)과 스트림 ID (streamId)입니다.
		 //比如说应用为live，流id为0，那么直播地址为：//예를 들어 애플리케이션이 라이브이고 스트림 ID가 0이면 라이브 브로드 캐스트 주소는 다음과 같습니다.
		 //http://127.0.0.1/live/0/hls.m3u8
		 //rtsp://127.0.0.1/live/0
		 //rtmp://127.0.0.1/live/0
		 //录像地址为：
		 //http://127.0.0.1/record/live/0/2017-04-11/11-09-38.mp4
		 //rtsp://127.0.0.1/record/live/0/2017-04-11/11-09-38.mp4
		 //rtmp://127.0.0.1/record/live/0/2017-04-11/11-09-38.mp4
		 PlayerProxy::Ptr player(new PlayerProxy("live",to_string(i++).data()));
		 player->play(url);
		 proxyMap.emplace(string(url),player);
	 }

#ifdef ENABLE_OPENSSL
	//请把证书"test_server.pem"放置在本程序可执行程序同目录下 //이 프로그램의 실행 프로그램과 동일한 디렉토리에 인증서 "test_server.pem"을 배치하십시오.
	try {
		SSL_Initor::Instance().loadServerPem((exePath() + ".pem").data());
	} catch(...) {
		FatalL << "请把证书:" << (exeName() + ".pem") << "放置在本程序可执行程序同目录下:" << exeDir() << endl; //인증서를 보내주세요 : //이 프로그램의 실행 프로그램과 동일한 디렉토리에 있습니다.
		return 0;
	}
#endif //ENABLE_OPENSSL

	TcpServer<RtspSession>::Ptr rtspSrv(new TcpServer<RtspSession>());
	TcpServer<RtmpSession>::Ptr rtmpSrv(new TcpServer<RtmpSession>());
	TcpServer<HttpSession>::Ptr httpSrv(new TcpServer<HttpSession>());
	TcpServer<ShellSession>::Ptr shellSrv(new TcpServer<ShellSession>());

	rtspSrv->start(mINI::Instance()[Config::Rtsp::kPort]);
	rtmpSrv->start(mINI::Instance()[Config::Rtmp::kPort]);
	httpSrv->start(mINI::Instance()[Config::Http::kPort]);

	//简单的telnet服务器，可用于服务器调试，但是不能使用23端口 //서버 디버깅에 사용할 수 있지만 포트 23은 사용할 수없는 간단한 텔넷 서버
	//测试方法:telnet 127.0.0.1 8023 //테스트 방법 : 텔넷 127.0.0.1 8023
	//输入用户名和密码登录(user:test,pwd:123456)，输入help命令查看帮助 //로그인 할 사용자 이름과 암호 (user : test, pwd : 123456)를 입력하고 help 명령을 입력하여 도움말을 봅니다.
	ShellSession::addUser("test","123456");
	shellSrv->start(8023);

#ifdef ENABLE_OPENSSL
	TcpServer<HttpsSession>::Ptr httpsSrv(new TcpServer<HttpsSession>());
	httpsSrv->start(mINI::Instance()[Config::Http::kSSLPort]);
#endif //ENABLE_OPENSSL

	EventPoller::Instance().runLoop();
	proxyMap.clear();
	rtspSrv.reset();
	rtmpSrv.reset();
	httpSrv.reset();
	shellSrv.reset();

#ifdef ENABLE_OPENSSL
	httpsSrv.reset();
#endif //ENABLE_OPENSSL

	UDPServer::Destory();
	WorkThreadPool::Destory();
	EventPoller::Destory();
	Logger::Destory();

	return 0;
}
