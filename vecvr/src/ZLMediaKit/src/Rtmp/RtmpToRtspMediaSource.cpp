/*
 * RtmpToRtspMediaSource.cpp
 *
 *  Created on: 2016年10月20日
 *      Author: xzl
 */

#include "Common/config.h"
#include "RtmpToRtspMediaSource.h"
#include "Util/util.h"
#include "Device/base64.h"
#include "Network/sockutil.h"

using namespace ZL::Util;
using namespace ZL::Network;

namespace ZL {
namespace Rtmp {

#ifdef ENABLE_RTMP2RTSP
RtmpToRtspMediaSource::RtmpToRtspMediaSource(const string &_app, const string &_id) :
		RtmpMediaSource(_app,_id) {
}
RtmpToRtspMediaSource::~RtmpToRtspMediaSource() {
}

void RtmpToRtspMediaSource::regist() {
	RtmpMediaSource::regist();
	if (m_pRtspSrc) {
		m_pRtspSrc->regist();
	}
}

void RtmpToRtspMediaSource::unregist() {
	RtmpMediaSource::unregist();
	if(m_pRtspSrc){
		m_pRtspSrc->unregist();
	}
}

void RtmpToRtspMediaSource::onGetH264(const H264Frame &frame) {
	m_pRecorder->inputH264((char *) frame.data.data(), frame.data.size(), frame.timeStamp, frame.type);

	if(m_pRtpMaker_h264) {
		m_pRtpMaker_h264->makeRtp(frame.data.data() + 4, frame.data.size() - 4, frame.timeStamp);
	}
}
inline void RtmpToRtspMediaSource::onGetAdts(const AdtsFrame &frame) {
	m_pRecorder->inputAAC((char *) frame.data, frame.aac_frame_length, frame.timeStamp);

	if (m_pRtpMaker_aac) {
		m_pRtpMaker_aac->makeRtp((char *) frame.data + 7, frame.aac_frame_length - 7, frame.timeStamp);
	}
}

void RtmpToRtspMediaSource::makeSDP() {
	string strSDP;
	strSDP = "v=0\r\n";
	strSDP += "o=- 1383190487994921 1 IN IP4 0.0.0.0\r\n";
	strSDP += "s=RTSP Session, streamed by the ZL\r\n";
	strSDP += "i=ZL Live Stream\r\n";
	strSDP += "c=IN IP4 0.0.0.0\r\n";
	strSDP += "t=0 0\r\n";
	if(m_pParser->getDuration() <= 0) {
		strSDP += "a=range:npt=0-\r\n";
	}
	else {
		strSDP += StrPrinter << "0-"<<  m_pParser->getDuration()<< "\r\n" << endl;
	}
	strSDP += "a=control:*\r\n";
	if (m_pParser->containVideo()) {
		uint32_t ssrc0;
		memcpy(&ssrc0, makeRandStr(4, false).data(), 4);
		auto lam = [this](const RtpPacket::Ptr &pkt, bool bKeyPos) {
			m_pRtspSrc->onGetRTP(pkt,bKeyPos);
		};
		static uint32_t videoMtu = mINI::Instance()[Config::Rtp::kVideoMtuSize].as<uint32_t>();
		m_pRtpMaker_h264.reset(new RtpMaker_H264(lam, ssrc0,videoMtu));

		char strTemp[100];
		int profile_level_id = 0;
		string strSPS =m_pParser->getSps().substr(4);
		string strPPS =m_pParser->getPps().substr(4);
		if (strSPS.length() >= 4) { // sanity check
			profile_level_id = (strSPS[1] << 16) | (strSPS[2] << 8) | strSPS[3]; // profile_idc|constraint_setN_flag|level_idc
		}

		//视频通道 //비디오 채널
		strSDP += StrPrinter << "m=video 0 RTP/AVP " << m_pRtpMaker_h264->getPlayloadType()
				<< "\r\n" << endl;
		strSDP += "b=AS:5100\r\n";
		strSDP += StrPrinter << "a=rtpmap:" << m_pRtpMaker_h264->getPlayloadType()
				<< " H264/" << m_pRtpMaker_h264->getSampleRate() << "\r\n" << endl;
		strSDP += StrPrinter << "a=fmtp:" << m_pRtpMaker_h264->getPlayloadType()
				<< " packetization-mode=1;profile-level-id=" << endl;

		memset(strTemp, 0, 100);
		sprintf(strTemp, "%06X", profile_level_id);
		strSDP += strTemp;
		strSDP += ";sprop-parameter-sets=";
		memset(strTemp, 0, 100);
		av_base64_encode(strTemp, 100, (uint8_t *) strSPS.data(), strSPS.size());
		strSDP += strTemp;
		strSDP += ",";
		memset(strTemp, 0, 100);
		av_base64_encode(strTemp, 100, (uint8_t *) strPPS.data(), strPPS.size());
		strSDP += strTemp;
		strSDP += "\r\n";
		strSDP += StrPrinter << "a=control:trackID=" << m_pRtpMaker_h264->getInterleaved() / 2
				<< "\r\n" << endl;
    }
    
	if (m_pParser->containAudio()) {
		uint32_t ssrc1;
		memcpy(&ssrc1, makeRandStr(8, false).data() + 4, 4);
		auto lam = [this](const RtpPacket::Ptr &pkt, bool bKeyPos) {
			m_pRtspSrc->onGetRTP(pkt,bKeyPos);
		};
		static uint32_t audioMtu = mINI::Instance()[Config::Rtp::kAudioMtuSize].as<uint32_t>();
		m_pRtpMaker_aac.reset(new RtpMaker_AAC(lam, ssrc1, audioMtu,m_pParser->getAudioSampleRate()));

		char configStr[32];
		const string & strAacCfg = m_pParser->getAudioCfg();
		snprintf(configStr, sizeof(configStr), "%02X%02x", strAacCfg[0], strAacCfg[1]);
		strSDP += StrPrinter << "m=audio 0 RTP/AVP " << m_pRtpMaker_aac->getPlayloadType()
				<< "\r\n" << endl;
		strSDP += "b=AS:96\r\n";
		strSDP += StrPrinter << "a=rtpmap:" << m_pRtpMaker_aac->getPlayloadType()
				<< " MPEG4-GENERIC/" << m_pRtpMaker_aac->getSampleRate() << "\r\n"
				<< endl;
		strSDP += StrPrinter << "a=fmtp:" << m_pRtpMaker_aac->getPlayloadType()
				 << " streamtype=5;profile-level-id=1;mode=AAC-hbr;"
				 << "sizelength=13;indexlength=3;indexdeltalength=3;config="
				 << endl;
		strSDP.append(configStr, 4);
		strSDP += "\r\n";
		strSDP += StrPrinter << "a=control:trackID=" << m_pRtpMaker_aac->getInterleaved() / 2
				<< "\r\n" << endl;
	}

	m_pRtspSrc.reset(new RtspMediaSource(getApp(),getId()));
	m_pRtspSrc->setOnSeek(m_onSeek);
	m_pRtspSrc->setOnStamp(m_onStamp);
	m_pRtspSrc->onGetSDP(strSDP);
	m_pRtspSrc->regist();
}

#endif // ENABLE_RTMP2RTSP

} /* namespace Rtmp */
} /* namespace ZL */
