/*
 * RtspSession.h
 *
 *  Created on: 2016年8月12日
 *      Author: xzl
 */

#ifndef SESSION_RTSPSESSION_H_
#define SESSION_RTSPSESSION_H_

#include <netinet/in.h>
#include <set>
#include <vector>
#include <unordered_map>
#include "Common/config.h"
#include "Rtsp.h"
#include "RtpBroadCaster.h"
#include "RtspMediaSource.h"
#include "Player/PlayerBase.h"
#include "Util/util.h"
#include "Util/logger.h"
#include "Network/TcpLimitedSession.h"

using namespace std;
using namespace ZL::Util;
using namespace ZL::Rtsp;
using namespace ZL::Player;
using namespace ZL::Network;

namespace ZL {
namespace Rtsp {

class RtspSession;
class RtspSession: public TcpLimitedSession<MAX_TCP_SESSION> {
public:
	typedef std::shared_ptr<RtspSession> Ptr;
	RtspSession(const std::shared_ptr<ThreadPool> &pTh, const Socket::Ptr &pSock);
	virtual ~RtspSession();
	void onRecv(const Socket::Buffer::Ptr &pBuf) override;
	void onError(const SockException &err) override;
	void onManager() override;
private:
	typedef bool (RtspSession::*rtspCMDHandle)();
	int send(const string &strBuf) override {
		return m_pSender->send(strBuf);
	}
	int send(const char *pcBuf, int iSize) override {
		return m_pSender->send(pcBuf, iSize);
	}
	void shutdown() override;
	bool handleReq_Options(); //处理options方法 // 옵션 메서드 처리
	bool handleReq_Describe(); //处理describe方法 // describe 메소드 처리
	bool handleReq_Setup(); //处理setup方法 // 설정 방법 처리
	bool handleReq_Play(); //处理play方法 // 재생 메서드 처리
	bool handleReq_Pause(); //处理pause方法 // pause 메소드 처리
	bool handleReq_Teardown(); //处理teardown方法 // 해체 방법 처리
	bool handleReq_Get(); //处理Get方法 // Get 메서드 처리
	bool handleReq_Post(); //处理Post方法 // Post 메서드 처리
	bool handleReq_SET_PARAMETER(); //处理SET_PARAMETER方法 // SET_PARAMETER 메서드 처리

	void inline send_StreamNotFound(); //rtsp资源未找到 // rtsp 리소스를 찾을 수 없음
	void inline send_UnsupportedTransport(); //不支持的传输模式 // 지원되지 않는 전송 모드
	void inline send_SessionNotFound(); //会话id错误 // 세션 ID 오류
	void inline send_NotAcceptable(); //rtsp同时播放数限制 // RTSP 동시 재생 제한
	inline bool findStream(); //根据rtsp url查找 MediaSource实例 // rtsp URL에 따라 MediaSource 인스턴스 찾기

	inline void initSender(const std::shared_ptr<RtspSession> &pSession); //处理rtsp over http，quicktime使用的 // 퀵타임에서 사용되는 http를 통한 rtsp 처리
	inline void sendRtpPacket(const RtpPacket &pkt);
	inline string printSSRC(uint32_t ui32Ssrc) {
		char tmp[9] = { 0 };
		ui32Ssrc = htonl(ui32Ssrc);
		uint8_t *pSsrc = (uint8_t *) &ui32Ssrc;
		for (int i = 0; i < 4; i++) {
			sprintf(tmp + 2 * i, "%02X", pSsrc[i]);
		}
		return tmp;
	}
	inline int getTrackIndexByTrackId(int iTrackId) {
		for (unsigned int i = 0; i < m_uiTrackCnt; i++) {
			if (iTrackId == m_aTrackInfo[i].trackId) {
				return i;
			}
		}
		return -1;
	}
	inline void onRcvPeerUdpData(int iTrackIdx, const Socket::Buffer::Ptr &pBuf, const struct sockaddr &addr);
	inline void tryGetPeerUdpPort();

	char *m_pcBuf = nullptr;
	Ticker m_ticker;
	Parser m_parser; //rtsp解析类 //rtsp 구문 분석 클래스
	string m_strUrl;
	string m_strSdp;
	string m_strSession;
	bool m_bFirstPlay = true;
	string m_strApp;
	string m_strStream;
	std::weak_ptr<RtspMediaSource> m_pMediaSrc;
	static unordered_map<string, rtspCMDHandle> g_mapCmd;

	//RTP缓冲 //RTP 버퍼
	weak_ptr<RingBuffer<RtpPacket::Ptr> > m_pWeakRing;
	RingBuffer<RtpPacket::Ptr>::RingReader::Ptr m_pRtpReader;

	PlayerBase::eRtpType m_rtpType = PlayerBase::RTP_UDP;
	bool m_bSetUped = false;
	int m_iCseq = 0;
	unsigned int m_uiTrackCnt = 0; //媒体track个数 // 미디어 트랙 수
	RtspTrack m_aTrackInfo[2]; //媒体track信息,trackid idx 为数组下标 // 미디어 트랙 정보, trackid idx는 배열 첨자입니다.

#ifdef RTSP_SEND_RTCP
	RtcpCounter m_aRtcpCnt[2]; //rtcp统计,trackid idx 为数组下标 // rtcp 통계, trackid idx는 배열 첨자입니다.
	Ticker m_aRtcpTicker[2]; //rtcp发送时间,trackid idx 为数组下标 // rtcp 전송 시간, trackid idx는 배열 첨자입니다.
	inline void sendRTCP();
#endif

	//RTP over UDP
	bool m_abGotPeerUdp[2] = { false, false }; //获取客户端udp端口计数 // 클라이언트 udp 포트 수 가져 오기
	weak_ptr<Socket> m_apUdpSock[2]; //发送RTP的UDP端口,trackid idx 为数组下标 // RTP를 보내기위한 UDP 포트, trackid idx는 배열 첨자입니다.
	std::shared_ptr<struct sockaddr> m_apPeerUdpAddr[2]; //播放器接收RTP的地址,trackid idx 为数组下标 // RTP를받는 플레이어의 주소, trackid idx는 배열 첨자입니다.
	bool m_bListenPeerUdpPort = false;
	RtpBroadCaster::Ptr m_pBrdcaster;

	//RTSP over HTTP
	function<void(void)> m_onDestory;
	bool m_bBase64need = false; //是否需要base64解码 // base64 디코딩이 필요합니까?
	Socket::Ptr m_pSender; //回复rtsp时走的tcp通道，供quicktime用 // quicktime을 위해 rtsp에 응답 할 때 사용되는 tcp 채널
	//quicktime 请求rtsp会产生两次tcp连接，// Quicktime 요청 rtsp는 두 개의 tcp 연결을 생성합니다.
	//一次发送 get 一次发送post，需要通过sessioncookie关联起来 // 한 번 보내기 게시물을 한 번 보내기, 세션 쿠키와 연결되어야 함
	string m_strSessionCookie;
	static recursive_mutex g_mtxGetter; //对quicktime上锁保护 // 퀵타임을위한 잠금 보호
	static recursive_mutex g_mtxPostter; //对quicktime上锁保护 // 퀵타임을위한 잠금 보호
	static unordered_map<string, weak_ptr<RtspSession> > g_mapGetter;
	static unordered_map<void *, std::shared_ptr<RtspSession> > g_mapPostter;
	static string g_serverName;
};

} /* namespace Session */
} /* namespace ZL */

#endif /* SESSION_RTSPSESSION_H_ */
