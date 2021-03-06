/** <!--
 *
 *  Copyright (C) 2017 veyesys support@veyesys.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  If you would like this software to be made available to you under an 
 *  alternate commercial license please email support@veyesys.com 
 *  for more information.
 *
 * -->
 */
/** <!--
 *
 *  Copyright (C) 2021 infrak infrak@naver.com
 *  <http://infra-k.net>
 *
 * -->
 */
#include "client/storstream.hpp"

StorStream::StorStream(VidStor &stor, astring strId, unsigned int nStream, bool bPlayback, u32 nPlaytime, bool bHWAccel)
						:m_strId(strId), m_nStream(nStream), m_pCallback(NULL), m_pParam(NULL), m_stor(stor), m_nLastTime(nPlaytime), 
						m_bPlayback(bPlayback), m_nPlaytime(nPlaytime), m_bPbPause(false), 
						StorWebSocketClient(stor.strip(), stor.strport(), LINK_PROTO_WS_STREAM_PATH), 
						m_nRecvSize(0), m_nNoFrameCnt(0), m_nRecvSizeLast(0)
{
	m_play.Init(FALSE, "fake", "fake", "fake", bHWAccel, VSC_CONNECT_TCP);
	// Login(stor.struser(), stor.strpasswd(), "Nonce");
	StartKeepThread();
}

StorStream::~StorStream()
{
	Disconnect();
}

bool StorStream::IsKeep()
{
	return false;
}

bool StorStream::ProcessLogined()
{
	Link::LinkCmd cmd;
	if (m_bPlayback == false)
	{
		cmd.set_type(Link::LINK_CMD_START_LIVE_CMD); // ???? ????
		LinkStartLiveCmd *req = new LinkStartLiveCmd;
		req->set_strid(m_strId);
		req->set_nstream(m_nStream);

		cmd.set_allocated_startlivecmd(req);
	}
	else
	{
		cmd.set_type(Link::LINK_CMD_PLAY_BACK_CMD);
		LinkPlayBackCmd *req = new LinkPlayBackCmd;
		req->set_strid(m_strId);
		req->set_nplaytime(m_nLastTime);
		
		cmd.set_allocated_playbackcmd(req);
	}

	std::string strMsg;
	::google::protobuf::util::Status status = ::google::protobuf::util::MessageToJsonString(cmd, &strMsg);
	if (!status.ok())
	{
#ifdef AVOCADO_DEBUG
		VDC_DEBUG("ERROR: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
		return false;
	}
	if (SendMsg(strMsg) == false)
	{
#ifdef AVOCADO_DEBUG
		VDC_DEBUG("ERROR: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
		return false;
	}

	return true;
}

bool StorStream::NeedReconnect()
{
	// ????ȭ?? ?????? ???? ȣ????
	// R_LOG(logRINFO, "%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);

	/* Playback don't need reconnect */
	if (m_bPlayback == true)
	{
#ifdef AVOCADO_DEBUG
		VDC_DEBUG("ERROR: %s %s %d Playback don't need reconnect\n", __FUNCTION__, __FILE__, __LINE__);
#endif
		return false;
	}

	/* live check the stream  */
	m_nNoFrameCnt++;

	if (m_nNoFrameCnt >= 5)
	{
		if (m_nRecvSizeLast != m_nRecvSize)
		{
			m_nNoFrameCnt = 0;
			m_nRecvSizeLast = m_nRecvSize;
			// 10?ʸ??? ????
//#ifdef AVOCADO_DEBUG
			//VDC_DEBUG("ERROR: %s %s %d m_nRecvSizeLast:%d m_nRecvSize:%d\n", __FUNCTION__, __FILE__, __LINE__, m_nRecvSizeLast, m_nRecvSize);
//#endif
			return false;
		}
		/* reconnect to vidstor */
		m_nNoFrameCnt = 0;
		m_nRecvSizeLast = m_nRecvSize;
//#ifdef AVOCADO_DEBUG
		//R_LOG(logRINFO, "%s %s %d, m_nRecvSizeLast=%lld\n", __FUNCTION__, __FILE__, __LINE__, m_nRecvSizeLast);//???? infrak
//#endif
		return true;
	}
	// 2?ʸ??? ????
	// R_LOG(logRINFO, "ERROR: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);

	return false;
}

bool StorStream::ProcessRecvMsg(char *data, size_t data_len)
{
	// ????ȭ?? ?ޱ?
	// R_LOG(logRINFO, "%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);

	/* after logined, all the data is stream */
	if (m_bLogined)
	{
		VideoFrameHeader * frameHeader = (VideoFrameHeader *)data;
		VideoFrame pFrame;

		pFrame.streamType = (VideoStreamType)ntohl(frameHeader->streamType);
		pFrame.frameType = (VideoFrameType)ntohl(frameHeader->frameType);
		pFrame.secs = ntohl(frameHeader->secs);
		pFrame.msecs = ntohl(frameHeader->msecs);
		pFrame.dataBuf = (u8 *)(data + sizeof(VideoFrameHeader));
		pFrame.dataLen = data_len - sizeof(VideoFrameHeader);
		pFrame.bufLen = pFrame.dataLen;
		m_nRecvSize += pFrame.dataLen;
		if (m_pCallback != NULL)
		{
			m_pCallback(pFrame, m_pParam);
		}
		else
		{
			if (m_nLastTime == 0 || pFrame.secs != m_nLastTime)
			{
				emit SignalPlayTime(m_nLastTime);
				m_nLastTime = pFrame.secs;
			}
			m_play.PutRawData(pFrame);
		}

		return true;
	}
	
	//R_LOG(logRINFO, "%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);

	/* lock guard */
	std::lock_guard<std::mutex> guard(m_lock);

	std::string strMsg(data, data_len);
	Link::LinkCmd cmd;

	::google::protobuf::util::Status status = ::google::protobuf::util::JsonStringToMessage(strMsg, &cmd);
	if (!status.ok())
	{
#ifdef AVOCADO_DEBUG
		VDC_DEBUG("ERROR: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
		return false;
	}

	switch (cmd.type())
	{
		case Link::LINK_CMD_LOGIN_RESP:
		{
			//R_LOG(logRINFO, "%s %s %d LINK_CMD_LOGIN_RESP: user=%s passwd=%s\n", __FUNCTION__, __FILE__, __LINE__, m_stor.struser().c_str(), m_stor.strpasswd().c_str());

			return ProcessLoginResp(cmd, m_stor.struser(), m_stor.strpasswd());
			break;
		}
		default:
		{
#ifdef AVOCADO_DEBUG
			R_LOG(logRINFO, "ERROR: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
			break;
		}
	};

	//R_LOG(logRINFO, "%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
	return true;
}

bool StorStream::ProcessOnline()
{
	//R_LOG(logRINFO, "%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
	Login(m_stor.struser(), m_stor.strpasswd(), "Nonce");
	
	return true;
}

bool StorStream::PausePlayback()
{
	XGuard guard(m_cMutex);

	if (Connect() == false)
	{
#ifdef AVOCADO_DEBUG
		VDC_DEBUG("ERROR: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
		return false;
	}
	//R_LOG(logRINFO, "%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);

	Link::LinkCmd cmd;

	cmd.set_type(Link::LINK_CMD_PLAY_PAUSE_CMD);
	LinkPlayPauseCmd *req = new LinkPlayPauseCmd;
	req->set_strid(m_strId);

	cmd.set_allocated_playpausecmd(req);
	
	std::string strMsg;
	::google::protobuf::util::Status status = ::google::protobuf::util::MessageToJsonString(cmd, &strMsg);
	if (!status.ok())
	{
#ifdef AVOCADO_DEBUG
		VDC_DEBUG("ERROR: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
		return false;
	}

	if (SendMsg(strMsg) == false)
	{
#ifdef AVOCADO_DEBUG
		VDC_DEBUG("ERROR: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
		return false;
	}
	
	return true;
}

bool StorStream::ResumePlayback()
{
	XGuard guard(m_cMutex);
	
	if (Connect() == false)
	{
#ifdef AVOCADO_DEBUG
		VDC_DEBUG("ERROR: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
		return false;
	}
	//R_LOG(logRINFO, "%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);

	Link::LinkCmd cmd;

	cmd.set_type(Link::LINK_CMD_PLAY_RESUME_CMD);
	LinkPlayResumeCmd * req = new LinkPlayResumeCmd;
	req->set_strid(m_strId);

	cmd.set_allocated_playresumecmd(req);
	
	std::string strMsg;
	::google::protobuf::util::Status status = ::google::protobuf::util::MessageToJsonString(cmd, &strMsg);
	if (!status.ok())
	{
#ifdef AVOCADO_DEBUG
		VDC_DEBUG("ERROR: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
		return false;
	}

	if (SendMsg(strMsg) == false)
	{
#ifdef AVOCADO_DEBUG
		VDC_DEBUG("ERROR: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
		return false;
	}

	return true;
}

bool StorStream::SeekPlayback(u32 nPlaytime)
{	
	XGuard guard(m_cMutex);
	
	if (Connect() == false)
	{
#ifdef AVOCADO_DEBUG
		VDC_DEBUG("ERROR: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
		return false;
	}
	//R_LOG(logRINFO, "%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);

	Link::LinkCmd cmd;

	cmd.set_type(Link::LINK_CMD_PLAY_SEEK_CMD);
	LinkPlaySeekCmd * req = new LinkPlaySeekCmd;
	req->set_strid(m_strId);
	req->set_nplaytime(nPlaytime);

	cmd.set_allocated_playseekcmd(req);
	
	std::string strMsg;
	::google::protobuf::util::Status status = ::google::protobuf::util::MessageToJsonString(cmd, &strMsg);
	if (!status.ok())
	{
#ifdef AVOCADO_DEBUG
		VDC_DEBUG("ERROR: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
		return false;
	}

	if (SendMsg(strMsg) == false)
	{
#ifdef AVOCADO_DEBUG
		VDC_DEBUG("ERROR: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
		return false;
	}

	return true;
}

bool StorStream::StartStorStream()
{
	XGuard guard(m_cMutex);
	
	if (Connect() == false)
	{
#ifdef AVOCADO_DEBUG
		VDC_DEBUG("ERROR: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
		return false;
	}
	//R_LOG(logRINFO, "%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);

	Link::LinkCmd cmd;
	if (m_bPlayback == false)
	{
		cmd.set_type(Link::LINK_CMD_START_LIVE_CMD);//???? ??????
		LinkStartLiveCmd * req = new LinkStartLiveCmd;
		req->set_strid(m_strId);
		req->set_nstream(m_nStream);

		cmd.set_allocated_startlivecmd(req);
	}
	else
	{
		cmd.set_type(Link::LINK_CMD_PLAY_BACK_CMD);
		LinkPlayBackCmd * req = new LinkPlayBackCmd;
		req->set_strid(m_strId);
		req->set_nplaytime(m_nPlaytime);

		cmd.set_allocated_playbackcmd(req);
	}

	std::string strMsg;
	::google::protobuf::util::Status status = ::google::protobuf::util::MessageToJsonString(cmd, &strMsg);
	if (!status.ok())
	{
#ifdef AVOCADO_DEBUG
		VDC_DEBUG("ERROR: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
		return false;
	}

	if (SendMsg(strMsg) == false)
	{
#ifdef AVOCADO_DEBUG
		VDC_DEBUG("ERROR: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
		return false;
	}
	
	return true;
}

bool StorStream::StopStorStream()
{	
	if (Connect() == false)
	{
#ifdef AVOCADO_DEBUG
		VDC_DEBUG("ERROR: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
		return false;
	}
	
	Link::LinkCmd cmd;

	XGuard guard(m_cMutex); // infrak

	if (m_bPlayback == false)
	{
		cmd.set_type(Link::LINK_CMD_STOP_LIVE_CMD);
		LinkStopLiveCmd * req = new LinkStopLiveCmd;
		req->set_strid(m_strId);
		req->set_nstream(m_nStream);

		cmd.set_allocated_stoplivecmd(req);
		//R_LOG(logRINFO, "REPORT: %s %s %d m_strId=%s\n", __FUNCTION__, __FILE__, __LINE__, m_strId.c_str());
	}
	else
	{
		cmd.set_type(Link::LINK_CMD_PLAY_STOP_CMD);
		LinkPlayStopCmd * req = new LinkPlayStopCmd;
		req->set_strid(m_strId);

		cmd.set_allocated_playstopcmd(req);
		//R_LOG(logRINFO, "REPORT: %s %s %d m_strId=%s\n", __FUNCTION__, __FILE__, __LINE__, m_strId.c_str());
	}

	std::string strMsg;
	::google::protobuf::util::Status status = ::google::protobuf::util::MessageToJsonString(cmd, &strMsg);
	if (!status.ok())
	{
#ifdef AVOCADO_DEBUG
		VDC_DEBUG("ERROR: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
		return false;
	}
	if (SendMsg(strMsg) == false)
	{
#ifdef AVOCADO_DEBUG
		VDC_DEBUG("ERROR: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
		return false;
	}
	//R_LOG(logRINFO, "REPORT: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);

	return true;
}

bool StorStream::RegDataCallback(CameraDataCallbackFunctionPtr pCallback, void * pParam)
{
	XGuard guard(m_cMutex);
	m_pCallback = pCallback;
	m_pParam = pParam;

#ifdef AVOCADO_DEBUG
	R_LOG(logRINFO, "%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
	return true;
}

bool StorStream::UnRegDataCallback()
{
	XGuard guard(m_cMutex);
	m_pCallback = NULL;
	m_pParam = NULL;

#ifdef AVOCADO_DEBUG
	R_LOG(logRINFO, "%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
	return true;	
}

bool StorStream::EnableMot(HWND hWnd, bool bEnable, astring strConf)
{
	m_play.EnableMot(hWnd, bEnable, strConf);

	return true;
}

bool StorStream::AttachWidget(HWND hWnd, int w, int h, RenderType render)
{
	m_play.AttachWidget(hWnd, w, h, render);

	return true;
}

bool StorStream::UpdateWidget(HWND hWnd, int w, int h)
{
	m_play.UpdateWidget(hWnd, w, h);

	return true;
}

bool StorStream::DetachWidget(HWND hWnd)
{
	m_play.DetachWidget(hWnd);

	return true;
}

bool StorStream::GetStreamInfo(VideoStreamInfo &pInfo)
{
	return m_play.GetStreamInfo(pInfo);
}
