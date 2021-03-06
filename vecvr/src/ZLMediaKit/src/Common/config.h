//
//  appConfig.h
//  ZLMedia
//
//  Created by lyl on 16/10/22.
//  Copyright © 2016年 jizan. All rights reserved.
//

#ifndef appConfig_h
#define appConfig_h

#include "Util/mini.h"
using namespace ZL::Util;

namespace Config {

void loaIniConfig();
////////////TCP最大连接数/////////// 최대 TCP 연결 수
#ifdef __x86_64__
#define MAX_TCP_SESSION 100000
#else
#define MAX_TCP_SESSION 128
#endif
 
////////////其他宏定义/////////// 기타 매크로 정의
#ifndef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b) )
#endif //MAX

#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b) )
#endif //MIN

#ifndef CLEAR_ARR
#define CLEAR_ARR(arr) for(auto &item : arr){ item = 0;}
#endif //CLEAR_ARR


////////////广播名称/////////// 방송 명
namespace Broadcast {
extern const char kBroadcastRtspSessionPlay[];
#define BroadcastRtspSessionPlayArgs const char *app,const char *stream

extern const char kBroadcastRtspSrcRegisted[];
#define BroadcastRtspSrcRegistedArgs const char *app,const char *stream

extern const char kBroadcastRtmpSrcRegisted[];
#define BroadcastRtmpSrcRegistedArgs const char *app,const char *stream

extern const char kBroadcastRecordMP4[];
#define BroadcastRecordMP4Args const Mp4Info &info

extern const char kBroadcastHttpRequest[];
#define BroadcastHttpRequestArgs const Parser &parser,HttpSession::HttpResponseInvoker &invoker

} //namespace Broadcast

//代理失败最大重试次数 //프록시 실패에 대한 최대 재시도 횟수
namespace Proxy {
extern const char kReplayCount[];
}//namespace Proxy


////////////HTTP配置///////////
namespace Http {
extern const char kPort[];
extern const char kSSLPort[];
//http 文件发送缓存大小 //캐시 크기를 보내는 HTTP 파일
extern const char kSendBufSize[];
//http 最大请求字节数 //http 최대 요청 바이트
extern const char kMaxReqSize[];
//http keep-alive秒数 //http 연결 유지 초
extern const char kKeepAliveSecond[];
//http keep-alive最大请求数 //최대 http 연결 유지 요청 수
extern const char kMaxReqCount[];
//http 字符编码 //http 문자 인코딩
extern const char kCharSet[];
//http 服务器名称 //http 서버 이름
extern const char kServerName[];
//http 服务器根目录 //http 서버 루트 디렉토리
extern const char kRootPath[];
//http 404错误提示内容 //http 404 오류 메시지 내용
extern const char kNotFound[];
//HTTP访问url前缀 //HTTP 액세스 URL 접두사
extern const char kHttpPrefix[];
}//namespace Http

////////////SHELL配置/////////// 셸 구성
namespace Shell {
extern const char kServerName[];
extern const char kMaxReqSize[];
extern const char kPort[];
} //namespace Shell

////////////RTSP服务器配置/////////// RTSP 서버 구성
namespace Rtsp {
#define RTSP_VERSION 1.30
#define RTSP_BUILDTIME __DATE__" CST"

extern const char kServerName[];
extern const char kPort[];
} //namespace Rtsp

////////////RTMP服务器配置/////////// RTMP 서버 구성
namespace Rtmp {
extern const char kPort[];
} //namespace RTMP


////////////RTP配置/////////// RTP 구성
namespace Rtp {
//RTP打包最大MTU,公网情况下更小 //RTP 패키지 최대 MTU, 공용 네트워크에서 더 작음
extern const char kVideoMtuSize[];
//RTP打包最大MTU,公网情况下更小 //공용 네트워크에서 더 작은 RTP 패키지의 최대 MTU
extern const char kAudioMtuSize[];
//udp方式接受RTP包的最大缓存 //udp 모드는 RTP 패킷의 가장 큰 버퍼를 허용합니다.
extern const char kUdpBufSize[];
//RTP排序缓存最大个数 //최대 RTP 정렬 버퍼 수
extern const char kMaxRtpCount[];
//如果RTP序列正确次数累计达到该数字就启动清空排序缓存 //올바른 RTP 시퀀스 수가이 수에 도달하면 정렬 캐시가 지워집니다.
extern const char kClearCount[];
//最大RTP时间为13个小时，每13小时回环一次 //최대 RTP 시간은 13 시간이며 13 시간마다 한 번씩 반복됩니다.
extern const char kCycleMS[];
} //namespace Rtsp

////////////组播配置/////////// 멀티 캐스트 구성
namespace MultiCast {
//组播分配起始地址 // 멀티 캐스트 배포 시작 주소
extern const char kAddrMin[];
//组播分配截止地址 // 멀티 캐스트 배포 끝 주소
extern const char kAddrMax[];
//组播TTL // 멀티 캐스트 TTL
extern const char kUdpTTL[];
} //namespace MultiCast

////////////录像配置/////////// 기록 구성
namespace Record {
//查看录像的应用名称 // 비디오의 애플리케이션 이름보기
extern const char kAppName[];
//每次流化MP4文件的时长,单位毫秒 // MP4 파일의 각 스트림 시간 (밀리 초)
extern const char kSampleMS[];
//MP4文件录制大小,不能太大,否则MP4Close函数执行事件太长 // MP4 파일 기록 크기, 너무 클 수 없습니다. 그렇지 않으면 MP4Close 함수 실행 이벤트가 너무 깁니다.
extern const char kFileSecond[];
//Rtsp访问url前缀 // Rtsp 액세스 URL 접두사
extern const char kRtspPrefix[];
//录制文件路径 // 기록 파일 경로
extern const char kFilePath[];
} //namespace Record

////////////HLS相关配置/////////// HLS 관련 구성
namespace Hls {
//HLS切片时长,单位秒 // HLS 슬라이스 기간 (초)
extern const char kSegmentDuration[];
//HLS切片个数 // HLS 슬라이스 수
extern const char kSegmentNum[];
//HLS文件写缓存大小 // HLS 파일 쓰기 캐시 크기
extern const char kFileBufSize[];
//录制文件路径 // 기록 파일 경로
extern const char kFilePath[];
} //namespace Hls

}  // namespace Config

#endif /* appConfig_h */
