/*
 * MediaReader.cpp
 *
 *  Created on: 2016年12月14日
 *      Author: xzl
 */

#include "MediaReader.h"
#include "Common/config.h"
#include "Util/mini.h"
#include "Http/HttpSession.h"

using namespace ZL::Util;

namespace ZL {
namespace MediaFile {

#ifdef ENABLE_MP4V2
MediaReader::MediaReader(const string &strApp, const string &strId) {
	static string recordPath = mINI::Instance()[Config::Record::kFilePath];
	auto strFileName = recordPath + "/" + strApp + "/" + strId;

	m_hMP4File = MP4Read(strFileName.data());
	if(m_hMP4File == MP4_INVALID_FILE_HANDLE) {
		throw runtime_error(StrPrinter << "打开MP4文件失败：" << strFileName << endl); //MP4 파일을 열지 못했습니다.
	}
	m_video_trId = MP4FindTrackId(m_hMP4File, 0, MP4_VIDEO_TRACK_TYPE, 0);
	if(m_video_trId != MP4_INVALID_TRACK_ID) {
		 if(strcmp(MP4GetTrackMediaDataName(m_hMP4File, m_video_trId),"avc1") ==0) {
			auto m_video_timescale 		= MP4GetTrackTimeScale(m_hMP4File, m_video_trId);
			auto m_video_duration 		= MP4GetTrackDuration(m_hMP4File, m_video_trId);
			m_video_num_samples     = MP4GetTrackNumberOfSamples(m_hMP4File, m_video_trId);
			m_video_sample_max_size = MP4GetTrackMaxSampleSize(m_hMP4File, m_video_trId);
			m_video_width 			= MP4GetTrackVideoWidth(m_hMP4File, m_video_trId);
			m_video_height 			= MP4GetTrackVideoHeight(m_hMP4File, m_video_trId);
			m_video_framerate       = MP4GetTrackVideoFrameRate(m_hMP4File, m_video_trId);
			m_pcVideoSample = std::shared_ptr<uint8_t> (new uint8_t[m_video_sample_max_size],[](uint8_t *ptr){
				delete [] ptr;
			});
			uint8_t **seqheader;
			uint8_t **pictheader;
			uint32_t *pictheadersize;
			uint32_t *seqheadersize;
			uint32_t ix;
			if(MP4GetTrackH264SeqPictHeaders(m_hMP4File, m_video_trId, &seqheader, &seqheadersize, &pictheader, &pictheadersize)){
				for (ix = 0; seqheadersize[ix] != 0; ix++) {
					m_strSps.assign((char *)(seqheader[ix]), seqheadersize[ix]);
					float framerate;
					getAVCInfo(m_strSps, (int &)m_video_width, (int &)m_video_height, framerate);
					m_video_framerate = framerate;
					m_strSps = string("\x0\x0\x0\x1",4) + m_strSps;
					free(seqheader[ix]);
				}
				free(seqheader);
				free(seqheadersize);
				for (ix = 0; pictheadersize[ix] != 0; ix++) {
					m_strPps.assign("\x0\x0\x0\x1",4);
					m_strPps.append((char *)(pictheader[ix]), pictheadersize[ix]);
					free(pictheader[ix]);
				}
				free(pictheader);
				free(pictheadersize);
			}
			m_video_ms = 1000.0 * m_video_duration / m_video_timescale;
			/*InfoL 	<< "\r\n"
					<< m_video_ms << "\r\n"
					<< m_video_num_samples << "\r\n"
					<< m_video_framerate << "\r\n"
					<< m_video_width << "\r\n"
					<< m_video_height << "\r\n";*/
		} 
		else {
			//如果不是h264，则忽略 //h264가 아니면 무시하십시오.
			m_video_trId = MP4_INVALID_TRACK_ID;
		}
	}

	m_audio_trId = MP4FindTrackId(m_hMP4File, 0, MP4_AUDIO_TRACK_TYPE, 0);
	if (m_audio_trId != MP4_INVALID_TRACK_ID) {
		if (strcmp(MP4GetTrackMediaDataName(m_hMP4File, m_audio_trId), "mp4a") == 0) {
			m_audio_sample_rate = MP4GetTrackTimeScale(m_hMP4File, m_audio_trId);
			auto m_audio_duration = MP4GetTrackDuration(m_hMP4File, m_audio_trId);
			m_audio_num_samples = MP4GetTrackNumberOfSamples(m_hMP4File,m_audio_trId);
			m_audio_num_channels = MP4GetTrackAudioChannels(m_hMP4File, m_audio_trId);
			m_audio_sample_max_size = MP4GetTrackMaxSampleSize(m_hMP4File,m_audio_trId);
			uint8_t *ppConfig;
			uint32_t pConfigSize;
			if(MP4GetTrackESConfiguration(m_hMP4File,m_audio_trId,&ppConfig,&pConfigSize)) {
				m_strAacCfg.assign((char *)ppConfig, pConfigSize);
				makeAdtsHeader(m_strAacCfg, m_adts);
				writeAdtsHeader(m_adts,m_adts.data);
				getAACInfo(m_adts, (int &)m_audio_sample_rate, (int &)m_audio_num_channels);
				free(ppConfig);
			}
			m_audio_ms = 1000.0 * m_audio_duration / m_audio_sample_rate;
			/*InfoL 	<< "\r\n"
					<< m_audio_ms << "\r\n"
					<< m_audio_num_samples << "\r\n"
					<< m_audio_num_channels << "\r\n"
					<< m_audio_sample_rate << "\r\n";*/
		}
		else {
			m_audio_trId = MP4_INVALID_TRACK_ID;
		}
	}
	if(m_audio_trId == MP4_INVALID_TRACK_ID && m_video_trId == MP4_INVALID_TRACK_ID) {
		MP4Close(m_hMP4File);
		m_hMP4File = MP4_INVALID_FILE_HANDLE;
		throw runtime_error(StrPrinter << "该MP4文件音视频格式不支持：" << strFileName << endl); //이 MP4 파일의 오디오 및 비디오 형식은 다음을 지원하지 않습니다.
	}

	m_iDuration	= MAX(m_video_ms,m_audio_ms);
	m_pChn.reset(new DevChannel(strApp.data(),strId.data(),m_iDuration/1000.0,false));
	if (m_audio_trId != MP4_INVALID_TRACK_ID) {
		AudioInfo info;
		info.iChannel = m_audio_num_channels;
		info.iSampleBit = 16;
		info.iSampleRate = m_audio_sample_rate;
		m_pChn->initAudio(info);
	}

	if (m_video_trId != MP4_INVALID_TRACK_ID) {
		VideoInfo info;
		info.iFrameRate = m_video_framerate;
		info.iWidth = m_video_width;
		info.iHeight = m_video_height;
		m_pChn->initVideo(info);
	}

	if (m_audio_trId != MP4_INVALID_TRACK_ID) {
		m_pChn->inputAAC((char *)m_adts.data, 7, 0);
	}

	if (m_video_trId != MP4_INVALID_TRACK_ID) {
		//m_pChn->initVideo(info);
		m_pChn->inputH264((char *) m_strSps.data(), m_strSps.size(), 0);
		m_pChn->inputH264((char *) m_strPps.data(), m_strPps.size(), 0);
	}
}

MediaReader::~MediaReader() {
	if (m_hMP4File != MP4_INVALID_FILE_HANDLE) {
		MP4Close(m_hMP4File);
		m_hMP4File = MP4_INVALID_FILE_HANDLE;
	}
}

void MediaReader::startReadMP4() {
	auto strongSelf = shared_from_this();
	static uint32_t sampleMS = mINI::Instance()[Config::Record::kSampleMS].as<uint32_t>();

	AsyncTaskThread::Instance().DoTaskDelay(reinterpret_cast<uint64_t>(this), sampleMS, [strongSelf]() {
		return strongSelf->readSample();
	});
	weak_ptr<MediaReader> weakSelf = strongSelf;
	m_pChn->setOnSeek([weakSelf](uint32_t ui32Stamp) {
		auto strongSelf = weakSelf.lock();
		if(!strongSelf) {
			return false;
		}
		strongSelf->seek(ui32Stamp);
		return true;
	});
	m_pChn->setOnStamp([weakSelf]() -> uint32_t {
		auto strongSelf = weakSelf.lock();
		if(!strongSelf) {
			return 0;
		}
		return strongSelf-> m_iSeekTime + strongSelf->m_ticker.elapsedTime();
	});
}

bool MediaReader::readSample(int iTimeInc) {
	TimeTicker();
	lock_guard<recursive_mutex> lck(m_mtx);
	auto bFlag0 = readVideoSample(iTimeInc);//数据没读完 //데이터를 읽지 않습니다.
	auto bFlag1 = readAudioSample(iTimeInc);//数据没读完 //데이터를 읽지 않습니다.
	auto bFlag2 = m_pChn->readerCount() > 0;//读取者大于0 //리더가 0보다 큽니다.
	if((bFlag0 || bFlag1) && bFlag2) {
		m_alive.resetTime();
	}
	//DebugL << "alive ...";
	//3秒延时关闭 //3 초 지연 종료
	return  m_alive.elapsedTime() <  3 * 1000;
}
inline bool MediaReader::readVideoSample(int iTimeInc) {
	if (m_video_trId != MP4_INVALID_TRACK_ID) {
		auto iNextSample = getVideoSampleId(iTimeInc);
		MP4SampleId iIdx = m_video_current;
		for (iIdx = m_video_current; iIdx < iNextSample; iIdx++) {
			uint8_t *pBytes = m_pcVideoSample.get();
			uint32_t numBytes = m_video_sample_max_size;
			if(MP4ReadSample(m_hMP4File, m_video_trId, iIdx + 1, &pBytes, &numBytes,NULL,NULL,NULL,&m_bSyncSample)){
				if (!iTimeInc) {
					uint32_t iOffset = 0;
					while (iOffset < numBytes) {
						uint32_t iFrameLen;
						memcpy(&iFrameLen,pBytes + iOffset,4);
						iFrameLen = ntohl(iFrameLen);
                        if(iFrameLen + iOffset + 4> numBytes) {
                            break;
                        }
						memcpy(pBytes + iOffset, "\x0\x0\x0\x1", 4);
						writeH264(pBytes + iOffset, iFrameLen + 4, (double) m_video_ms * iIdx / m_video_num_samples);
						iOffset += (iFrameLen + 4);
					}
				}
				else if(m_bSyncSample) {
					break;
				}
			}
			else {
				FatalL << "读取视频失败:" << iIdx + 1;//비디오 읽기 실패 :
			}
		}
		m_video_current = iIdx;

		return m_video_current < m_video_num_samples;
	}

	return false;
}

inline bool MediaReader::readAudioSample(int iTimeInc) {
	if (m_audio_trId != MP4_INVALID_TRACK_ID) {
		auto iNextSample = getAudioSampleId(iTimeInc);
		for (auto i = m_audio_current; i < iNextSample; i++) {
			uint32_t numBytes = m_audio_sample_max_size;
			uint8_t *pBytes = m_adts.data + 7;
			if(MP4ReadSample(m_hMP4File, m_audio_trId, i + 1, &pBytes, &numBytes)) {
				if (!iTimeInc) {
					m_adts.aac_frame_length = 7 + numBytes;
					writeAdtsHeader(m_adts, m_adts.data);
					writeAAC(m_adts.data, m_adts.aac_frame_length, (double) m_audio_ms * i / m_audio_num_samples);
				}
			}
			else {
				FatalL << "读取音频失败:" << i+ 1;//오디오를 읽지 못했습니다.
			}
		}
		m_audio_current = iNextSample;

		return m_audio_current < m_audio_num_samples;
	}

	return false;
}

inline void MediaReader::writeH264(uint8_t *pucData,int iLen,uint32_t uiStamp) {
	m_pChn->inputH264((char *)pucData, iLen, uiStamp);
}

inline void MediaReader::writeAAC(uint8_t *pucData,int iLen,uint32_t uiStamp) {
	m_pChn->inputAAC((char *)pucData, iLen, uiStamp);
}

inline MP4SampleId MediaReader::getVideoSampleId(int iTimeInc ) {
	MP4SampleId video_current = (double)m_video_num_samples * (m_iSeekTime + m_ticker.elapsedTime() + iTimeInc) / m_video_ms;
	video_current = MAX(0,MIN(m_video_num_samples, video_current));
	return video_current;
}

inline MP4SampleId MediaReader::getAudioSampleId(int iTimeInc) {
	MP4SampleId audio_current = (double)m_audio_num_samples * (m_iSeekTime + m_ticker.elapsedTime() + iTimeInc)  / m_audio_ms ;
	audio_current = MAX(0,MIN(m_audio_num_samples,audio_current));
	return audio_current;
}

inline void MediaReader::setSeekTime(int iSeekTime) {
	m_iSeekTime = MAX(0, MIN(iSeekTime,m_iDuration));
	m_ticker.resetTime();
	if (m_audio_trId != MP4_INVALID_TRACK_ID) {
		m_audio_current = getAudioSampleId();
	}
	if (m_video_trId != MP4_INVALID_TRACK_ID) {
		m_video_current = getVideoSampleId();
	}
}

inline uint32_t MediaReader::getVideoCurrentTime() {
	return (double)m_video_current * m_video_ms /m_video_num_samples;
}

void MediaReader::seek(int iSeekTime,bool bReStart) {
	lock_guard<recursive_mutex> lck(m_mtx);
	if(iSeekTime == 0 || m_video_trId == MP4_INVALID_TRACK_ID) {
		setSeekTime(iSeekTime);
	}
	else {
		setSeekTime(iSeekTime - 5000);
		//在之后的10秒查找关键帧 //다음 10 초 동안 키 프레임 찾기
		readVideoSample(10000);
		if (m_bSyncSample) {
			//找到关键帧 //키 프레임 찾기
			auto iIdr =  m_video_current;
			setSeekTime(getVideoCurrentTime());
			m_video_current = iIdr;
		}else{
			//未找到关键帧 //키 프레임을 찾을 수 없습니다.
			setSeekTime(iSeekTime);
		}
	}
	m_pChn->updateTimeStamp(m_iSeekTime);

	if(bReStart) {
		AsyncTaskThread::Instance().CancelTask(reinterpret_cast<uint64_t>(this));
		startReadMP4();
	}
}

#endif //ENABLE_MP4V2

RtspMediaSource::Ptr MediaReader::onMakeRtsp(const string &strApp, const string &strId) {
#ifdef ENABLE_MP4V2
	static string appName = mINI::Instance()[Config::Record::kAppName];
	if (strApp != appName) {
		return nullptr;
	}
	try {
		MediaReader::Ptr pReader(new MediaReader(strApp,strId));
		pReader->startReadMP4();
		return RtspMediaSource::find(strApp, strId, false);
	} catch (std::exception &ex) {
		WarnL << ex.what();
		return nullptr;
	}
#else
	return nullptr;
#endif //ENABLE_MP4V2
}

RtmpMediaSource::Ptr MediaReader::onMakeRtmp(const string &strApp, const string &strId) {
#ifdef ENABLE_MP4V2
	static string appName = mINI::Instance()[Config::Record::kAppName];
	if (strApp != appName) {
		return nullptr;
	}
	try {
		MediaReader::Ptr pReader(new MediaReader(strApp, strId));
		pReader->startReadMP4();
		return RtmpMediaSource::find(strApp, strId, false);
	} catch (std::exception &ex) {
		WarnL << ex.what();
		return nullptr;
	}
#else
	return nullptr;
#endif //ENABLE_MP4V2
}

} /* namespace MediaFile */
} /* namespace ZL */
