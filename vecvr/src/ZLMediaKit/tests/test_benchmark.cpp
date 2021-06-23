
#include <signal.h>
#include <unistd.h>
#include <atomic>
#include <iostream>
#include <list>
#include "Util/logger.h"
#include "Util/onceToken.h"
#include "Rtsp/UDPServer.h"
#include "Network/sockutil.h"
#include "Poller/EventPoller.h"
#include "Device/PlayerProxy.h"
#include "Thread/WorkThreadPool.h"

using namespace std;
using namespace ZL::DEV;
using namespace ZL::Util;
using namespace ZL::Rtsp;
using namespace ZL::Thread;
using namespace ZL::Network;

void programExit(int arg) {
	EventPoller::Instance().shutdown();
}

int main(int argc, char *argv[]){
	Logger::Instance().add(std::make_shared<ConsoleChannel>("stdout", LTrace));
	//Logger::Instance().setWriter(std::make_shared<AsyncLogWriter>());
	signal(SIGINT, programExit);

	if(argc != 5){
		FatalL << "\r\n测试方法：./test_benchmark player_count play_interval rtxp_url rtp_type\r\n"
			   << "例如你想每隔50毫秒启动共计100个播放器（tcp方式播放rtsp://127.0.0.1/live/0 ）可以输入以下命令：\r\n"
		       << "./test_benchmark 100 50 rtsp://127.0.0.1/live/0 0\r\n"
		       <<endl;
		//테스트 방법 : ./test_benchmark player_count play_interval rtxp_url rtp_type 예를 들어 50 밀리 초마다 총 100 명의 플레이어를 시작하려는 경우 (tcp 모드에서 rtsp : //127.0.0.1/live/0 재생) 다음 명령을 입력 할 수 있습니다. : ./test_benchmark 100 50 rtsp : //127.0.0.1/live/00
		Logger::Destory();
		return 0;
	}

	list<MediaPlayer::Ptr> playerList;
	auto playerCnt = atoi(argv[1]);//启动的播放器个数 //시작된 플레이어 수
	atomic_int alivePlayerCnt(0);
	//每隔若干毫秒启动一个播放器（如果一次性全部启动，服务器和客户端可能都承受不了）//몇 밀리 초마다 플레이어를 시작합니다 (한 번에 모두 시작하면 서버와 클라이언트 모두 견딜 수 없습니다.
	AsyncTaskThread::Instance().DoTaskDelay(0, atoi(argv[2]), [&](){
		MediaPlayer::Ptr player(new MediaPlayer());
		player->setOnPlayResult([&](const SockException &ex){
			if(!ex){
				++alivePlayerCnt;
			}
		});
		player->setOnShutdown([&](const SockException &ex){
			--alivePlayerCnt;
		});
		player->play(argv[3], "", "", (PlayerBase::eRtpType)atoi(argv[4]));
		playerList.push_back(player);
		return playerCnt--;
	});

	AsyncTaskThread::Instance().DoTaskDelay(0, 1000, [&](){
		InfoL << "存活播放器个数：" << alivePlayerCnt.load(); //살아남은 플레이어 수 :
		return true;
	});
	EventPoller::Instance().runLoop();
	playerList.clear();

	static onceToken token(nullptr, []() {
		WorkThreadPool::Instance();
		UDPServer::Destory();
        AsyncTaskThread::Destory();
		EventPoller::Destory();
		Logger::Destory();
	});
	return 0;
}
