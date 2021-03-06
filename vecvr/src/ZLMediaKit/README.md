# C ++ 11 기반의 간단하고 사용하기 쉬운 경량 스트리밍 미디어 라이브러리
플랫폼 | 컴파일 상태
# 一个基于C++11简单易用的轻量级流媒体库
平台|编译状态
----|-------
Linux | [![Build Status](https://travis-ci.org/xiongziliang/ZLMediaKit.svg?branch=master)](https://travis-ci.org/xiongziliang/ZLMediaKit)
macOS | [![Build Status](https://travis-ci.org/xiongziliang/ZLMediaKit_build_for_mac.svg?branch=master)](https://travis-ci.org/xiongziliang/ZLMediaKit_build_for_mac)
iOS | [![Build Status](https://travis-ci.org/xiongziliang/ZLMediaKit-build_for_ios.svg?branch=master)](https://travis-ci.org/xiongziliang/ZLMediaKit-build_for_ios)
Android | [![Build Status](https://travis-ci.org/xiongziliang/ZLMediaKit_build_for_android.svg?branch=master)](https://travis-ci.org/xiongziliang/ZLMediaKit_build_for_android)

## 项目特点
- 基于C++11开发，避免使用裸指针，代码稳定可靠；同时跨平台移植简单方便，代码清晰简洁。
- 打包多种流媒体协议(RTSP/RTMP/HLS），支持协议间的互相转换，提供一站式的服务。
- 使用epoll+线程池+异步网络IO模式开发，并发性能优越。
- 只实现主流的的H264+AAC流媒体方案，代码精简,脉络清晰，适合学习。
## 프로젝트 특징
-C ++ 11 개발을 기반으로 베어 포인터 사용을 피하고 코드가 안정적이고 신뢰할 수 있으며 동시에 크로스 플랫폼 이식이 간단하고 편리하며 코드가 명확하고 간결합니다.
-다중 스트리밍 미디어 프로토콜 (RTSP / RTMP / HLS)을 패키징하고 프로토콜 간의 상호 변환을 지원하며 원 스톱 서비스를 제공합니다.
-epoll + thread pool + asynchronous 네트워크 IO 모드 개발, 뛰어난 동시성 성능을 사용합니다.
-주류 H264 + AAC 스트리밍 미디어 솔루션 만 구현되고 코드가 단순화되고 컨텍스트가 명확하며 학습에 적합합니다.

## 功能清单
- RTSP
  - RTSP 服务器，支持RTMP/MP4转RTSP。
  - RTSP 播放器，支持RTSP代理。
  - 支持 `rtp over udp` `rtp over tcp` `rtp over http` `rtp组播`  四种RTP传输方式 。
## 기능 목록
-RTSP
   -RTSP 서버, RTMP / MP4 ~ RTSP 지원.
   -RTSP 플레이어, RTSP 프록시를 지원합니다.
   -`rtp over udp``rtp over tcp``rtp over http``rtp multicast` 네 가지 RTP 전송 방법을 지원합니다.

- RTMP
  - RTMP 播放服务器，支持RTSP/MP4转RTMP。
  - RTMP 发布服务器，支持录制发布流。
  - RTMP 播放器，支持RTMP代理。
  - RTMP 推流客户端。
-RTMP
   -RTMP 플레이어 서버, RTSP / MP4 to RTMP 지원.
   -RTMP 게시 서버, 녹화 및 게시 스트림을 지원합니다.
   -RTMP 플레이어, RTMP 프록시를 지원합니다.
   -RTMP 스트리밍 클라이언트.

- HLS
  - 支持HLS文件生成，自带HTTP文件服务器。
-HLS
   -HTTP 파일 서버로 HLS 파일 생성을 지원합니다.
   
- HTTP[S]
  - 服务器支持`目录索引生成`,`文件下载`,`表单提交请求`。
  - 客户端提供`文件下载器(支持断点续传)`,`接口请求器`。
-HTTP [S]
   -서버는 '카탈로그 인덱스 생성', '파일 다운로드', '양식 제출 요청'을 지원합니다.
   -클라이언트는`파일 다운로더 (재개 가능한 업로드 지원)`,`인터페이스 요청자`를 제공합니다.

- 其他
  - 支持输入YUV+PCM自动生成RTSP/RTMP/HLS/MP4.
  - 支持简单的telnet调试。
  - 支持H264的解析，支持B帧的POC计算排序。
-다른
   -RTSP / RTMP / HLS / MP4를 자동으로 생성하는 지원 입력 YUV + PCM.
   -간단한 텔넷 디버깅을 지원합니다.
   -H264 분석 및 POC 계산 및 B- 프레임 정렬을 지원합니다.
   
## 后续任务
- 提供更多的示例代码
## 후속 작업
-더 많은 샘플 코드 제공

## 编译(Linux)
- 我的编译环境
  - Ubuntu16.04 64 bit + gcc5.4(最低gcc4.7)
  - cmake 3.5.1
- 编译
## 编译(Linux)
- 我的编译环境
  - Ubuntu16.04 64 bit + gcc5.4(最低gcc4.7)
  - cmake 3.5.1
- 编译  
  ```
  cd ZLMediaKit
  ./build_for_linux.sh
  ```  
    
## 编译(macOS)
- 我的编译环境
  - macOS Sierra(10.12.1) + xcode8.3.1
  - Homebrew 1.1.3
  - cmake 3.8.0
- 编译
## 컴파일 (macOS)
-내 컴파일 환경
   -macOS Sierra (10.12.1) + xcode8.3.1
   -Homebrew 1.1.3
   -cmake 3.8.0
-엮다  
  ```
  cd ZLMediaKit
  ./build_for_mac.sh
  ```
	 
## 编译(iOS)
- 编译环境:`请参考macOS的编译指导。`
- 编译
## 컴파일 (iOS)
-컴파일 환경 :`macOS의 컴파일 가이드를 참조하세요. `
-엮다  
  ```
  cd ZLMediaKit
  ./build_for_ios.sh
  ```
- 你也可以生成Xcode工程再编译：-Xcode 프로젝트를 생성하고 컴파일 할 수도 있습니다.

  ```
  cd ZLMediaKit
  mkdir -p build
  cd build
  # 生成Xcode工程，工程文件在build目录下 # Xcode 프로젝트 생성, 프로젝트 파일은 빌드 디렉토리에 있습니다.
  cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/iOS.cmake -DIOS_PLATFORM=SIMULATOR64 -G "Xcode"
  ```
  
## 编译(Android) ## 컴파일 (Android)
- 我的编译环境 -내 컴파일 환경
  - macOS Sierra(10.12.1) + xcode8.3.1
  - Homebrew 1.1.3
  - cmake 3.8.0
  - [android-ndk-r14b](https://dl.google.com/android/repository/android-ndk-r14b-darwin-x86_64.zip)
- 编译

  ```
  cd ZLMediaKit
  export ANDROID_NDK_ROOT=/path/to/ndk
  ./build_for_android.sh
  ```

## 使用方法## 지침
-서버로서 :
- 作为服务器：
	```
	TcpServer<RtspSession>::Ptr rtspSrv(new TcpServer<RtspSession>());
	TcpServer<RtmpSession>::Ptr rtmpSrv(new TcpServer<RtmpSession>());
	TcpServer<HttpSession>::Ptr httpSrv(new TcpServer<HttpSession>());
	TcpServer<HttpsSession>::Ptr httpsSrv(new TcpServer<HttpsSession>());
	
	rtspSrv->start(mINI::Instance()[Config::Rtsp::kPort]);
	rtmpSrv->start(mINI::Instance()[Config::Rtmp::kPort]);
	httpSrv->start(mINI::Instance()[Config::Http::kPort]);
	httpsSrv->start(mINI::Instance()[Config::Http::kSSLPort]);
	EventPoller::Instance().runLoop();
	```
-플레이어로서 :
- 作为播放器： 
	```
	MediaPlayer::Ptr player(new MediaPlayer());
	player->setOnPlayResult([](const SockException &ex) {
		InfoL << "OnPlayResult:" << ex.what();
	});
	player->setOnShutdown([](const SockException &ex) {
		ErrorL << "OnShutdown:" << ex.what();
	});
	player->setOnVideoCB([&](const H264Frame &frame){
		//在这里解码H264并显示 여기에서 H264를 디코딩하고 표시
	});
	player->setOnAudioCB([&](const AdtsFrame &frame){
		//在这里解码AAC并播放 여기에서 AAC를 디코딩하고 재생
	});
	//支持rtmp、rtsp rtmp, rtsp 지원
	player->play("rtsp://192.168.0.122/","admin","123456",PlayerBase::RTP_TCP);
	EventPoller::Instance().runLoop();
	```
-프록시 서버로 :
- 作为代理服务器：
	```
	//support rtmp and rtsp url
	//just support H264+AAC
	auto urlList = {"rtmp://live.hkstv.hk.lxdns.com/live/hks",
			"rtsp://184.72.239.149/vod/mp4://BigBuckBunny_175k.mov"};
	map<string , PlayerProxy::Ptr> proxyMap;
	int i=0;
	for(auto url : urlList){
		//PlayerProxy构造函数前两个参数分别为应用名（app）,流id（streamId）// PlayerProxy 생성자의 처음 두 매개 변수는 애플리케이션 이름 (app)과 스트림 ID (streamId)입니다.
		//比如说应用为live，流id为0，那么直播地址为：예를 들어 애플리케이션이 라이브이고 스트림 ID가 0이면 라이브 브로드 캐스트 주소는 다음과 같습니다.
		//http://127.0.0.1/live/0/hls.m3u8
		//rtsp://127.0.0.1/live/0
		//rtmp://127.0.0.1/live/0
		//录像地址为：비디오 주소는 다음과 같습니다.
		//http://127.0.0.1/record/live/0/2017-04-11/11-09-38.mp4
		//rtsp://127.0.0.1/record/live/0/2017-04-11/11-09-38.mp4
		//rtmp://127.0.0.1/record/live/0/2017-04-11/11-09-38.mp4
		PlayerProxy::Ptr player(new PlayerProxy("live",to_string(i++).data()));
		player->play(url);
		proxyMap.emplace(string(url),player);
	}
	```
-푸시 스트리밍 클라이언트 :	
- 作为推流客户端器：
	```
	PlayerProxy::Ptr player(new PlayerProxy("app","stream"));
	//拉一个流，生成一个RtmpMediaSource，源的名称是"app/stream" 스트림을 가져오고 RtmpMediaSource를 생성합니다. 소스 이름은 "app / stream"입니다.
	//你也可以以其他方式生成RtmpMediaSource，比如说MP4文件（请研读MediaReader代码）MP4 파일과 같은 다른 방법으로 RtmpMediaSource를 생성 할 수도 있습니다 (MediaReader 코드를 살펴보십시오).
	player->play("rtmp://live.hkstv.hk.lxdns.com/live/hks");

	RtmpPusher::Ptr pusher;
	//监听RtmpMediaSource注册事件,在PlayerProxy播放成功后触发。PlayerProxy가 성공적으로 재생 된 후 트리거되는 RtmpMediaSource 등록 이벤트를 모니터링합니다.
	NoticeCenter::Instance().addListener(nullptr,Config::Broadcast::kBroadcastRtmpSrcRegisted,
			[&pusher](BroadcastRtmpSrcRegistedArgs){
		미디어 소스 "app / stream"이 등록되었습니다. 그러면 새 RtmpPusher 객체를 만들고 미디어 소스를 바인딩 할 수 있습니다.
		//媒体源"app/stream"已经注册，这时方可新建一个RtmpPusher对象并绑定该媒体源
		const_cast<RtmpPusher::Ptr &>(pusher).reset(new RtmpPusher(app,stream));

		//推流地址，请改成你自己的服务器。스트리밍 주소는 자신의 서버로 변경하십시오.
		//这个范例地址（也是基于mediakit）是可用的，但是带宽只有1mb，访问可能很卡顿。이 예제 주소 (또한 mediakit 기반)를 사용할 수 있지만 대역폭은 1MB에 불과하며 액세스 속도가 매우 느릴 수 있습니다.
		pusher->publish("rtmp://jizan.iok.la/live/test");
	});

	EventPoller::Instance().runLoop();
	```
## QA
- 为什么VLC播放一段时间就停止了？-VLC가 잠시 동안 재생을 중지하는 이유는 무엇입니까?
    ZLMediaKit은 RTSP 프로토콜을 구현할 때 하트 비트 패킷으로 OPTIONS 명령을 사용하므로 (UDP를 통한 RTP의 경우 유효) 플레이어가 OPTIONS 명령을 계속 전송하지 않으면 ZLMediaKit의 연결이 끊어집니다. 타사 플레이어를 사용하여 테스트하려면 TCP를 통한 RTP 방법을 변경하거나 ZLMediaKit의 소스 코드를 수정하고 위치를 src / Rtsp / RtspSession.cpp RtspSession :: onManager 함수로 수정하고 다음과 같이 수정할 수 있습니다. 다음과 같습니다.
    由于ZLMediaKit在实现RTSP协议时，采用OPTIONS命令作为心跳包（在RTP over UDP时有效），如果播放器不持续发送OPTIONS指令，那么ZLMediaKit会断开连接。如果你要用第三方播放器测试，你可以改RTP over TCP方式或者修改ZLMediaKit的源码，修改位置位置为src/Rtsp/RtspSession.cpp RtspSession::onManager函数,修改成如下所示：
	```
  	void RtspSession::onManager() {
		if (m_ticker.createdTime() > 10 * 1000) {
			if (m_strSession.size() == 0) {
				WarnL << "非法链接:" << getPeerIp(); 잘못된 링크 :
				shutdown();
				return;
			}
			if (m_bListenPeerUdpPort) {
				UDPServer::Instance().stopListenPeer(getPeerIp().data(), this);
				m_bListenPeerUdpPort = false;
			}
		}
		/*if (m_rtpType != PlayerBase::RTP_TCP && m_ticker.elapsedTime() > 15 * 1000) {
			WarnL << "RTSP会话超时:" << getPeerIp(); RTSP 세션 시간 초과 :
			shutdown();
			return;
 		/*}
  	}
	```
- 怎么测试服务器性能？ -서버 성능을 테스트하는 방법?
    ZLMediaKit은 성능 테스트를위한 예제를 제공하며 코드는 tests / test_benchmark.cpp에 있습니다. ZLToolKit은 기본적으로 tcp 클라이언트 멀티 스레딩 지원을 비활성화하므로 테스트 동시성을 늘려야하는 경우 ZLToolKit를 컴파일 할 때 ENABLE_ASNC_TCP_CLIENT 매크로를 활성화해야합니다. 구체적인 작업은 다음과 같습니다.
    ZLMediaKit提供了测试性能的示例，代码在tests/test_benchmark.cpp。由于ZLToolKit默认关闭了tcp客户端多线程的支持，如果需要提高测试并发量，需要在编译ZLToolKit时启用ENABLE_ASNC_TCP_CLIENT宏，具体操作如下：
	```
	#编译ZLToolKit #ZLToolKit 컴파일 
	cd ZLToolKit
	mkdir -p build
	cd build -DENABLE_ASNC_TCP_CLIENT
	cmake ..
	make -j4
	sudo make install
	```
-github 다운로드가 너무 느립니다. 다운로드 할 다른 방법이 있습니까?
- github下载太慢了，有其他下载方式吗？
    
    你可以在通过开源中国获取最新的代码，地址为：오픈 소스 중국을 통해 최신 코드를 얻을 수 있습니다. 주소는 다음과 같습니다.
 
    [ZLToolKit](http://git.oschina.net/xiahcu/ZLToolKit)
  
    [ZLMediaKit](http://git.oschina.net/xiahcu/ZLMediaKit)

## 联系方式
- 邮箱：<771730766@qq.com>
- QQ群：542509000
## 연락처 정보
-이메일 : <771730766@qq.com>
-QQ 그룹 : 542509000
