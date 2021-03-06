//============================================================================
// Name        : main.cpp
// Author      : 熊子良
// Version     :
//============================================================================

#include <signal.h>
#include <unistd.h>
#include <iostream>
#include "Util/logger.h"
#include "Util/onceToken.h"
#include "Util/NoticeCenter.h"
#include "Poller/EventPoller.h"
#include "Device/PlayerProxy.h"
#include "Rtmp/RtmpPusher.h"
#include "Common/config.h"

using namespace std;
using namespace ZL::Util;
using namespace ZL::Rtmp;
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

	PlayerProxy::Ptr player(new PlayerProxy("app","stream"));
	//拉一个流，生成一个RtmpMediaSource，源的名称是"app/stream"
	//你也可以以其他方式生成RtmpMediaSource，比如说MP4文件（请研读MediaReader代码）
	//스트림을 가져 와서 RtmpMediaSource를 생성합니다. 소스 이름은 "app / stream"입니다. MP4 파일과 같은 다른 방법으로 RtmpMediaSource를 생성 할 수도 있습니다 (MediaReader 코드를 살펴보십시오).
	player->play("rtmp://live.hkstv.hk.lxdns.com/live/hks");

	RtmpPusher::Ptr pusher;
	//监听RtmpMediaSource注册事件,在PlayerProxy播放成功后触发。
	//PlayerProxy가 성공적으로 재생 된 후 트리거되는 RtmpMediaSource 등록 이벤트를 모니터링합니다.
	NoticeCenter::Instance().addListener(nullptr,Config::Broadcast::kBroadcastRtmpSrcRegisted,
			[&pusher](BroadcastRtmpSrcRegistedArgs) {
		//媒体源"app/stream"已经注册，这时方可新建一个RtmpPusher对象并绑定该媒体源
		//미디어 소스 "app / stream"이 등록되었습니다. 그러면 새 RtmpPusher 객체를 만들고 미디어 소스를 바인딩 할 수 있습니다.
		const_cast<RtmpPusher::Ptr &>(pusher).reset(new RtmpPusher(app,stream));

		pusher->setOnShutdown([](const SockException &ex) {
			WarnL << "已断开与服务器连接：" << ex.getErrCode() << " " << ex.what();//서버에서 연결 해제 :
		});

		pusher->setOnPublished([](const SockException &ex) {
			if(ex) {
				WarnL << "发布失败：" << ex.getErrCode() << " "<< ex.what();//게시 실패 :
			}
			else {
				InfoL << "发布成功，请用播放器打开：rtmp://jizan.iok.la/live/test";//릴리스가 성공적입니다. 플레이어를 사용하여여십시오 : rtmp : //jizan.iok.la/live/test
			}
		});

		//推流地址，请改成你自己的服务器。//스트리밍 주소는 자신의 서버로 변경하십시오.
		//这个范例地址（也是基于mediakit）是可用的，但是带宽只有1mb，访问可能很卡顿。//이 예제 주소 (또한 mediakit 기반)를 사용할 수 있지만 대역폭은 1MB에 불과하며 액세스 속도가 매우 느릴 수 있습니다.
		pusher->publish("rtmp://jizan.iok.la/live/test");
	});

	EventPoller::Instance().runLoop();
	NoticeCenter::Instance().delListener(nullptr);
	player.reset();
	pusher.reset();

	EventPoller::Destory();
	Logger::Destory();

	return 0;
}
