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
 *  Copyright (C) 2021 infrak infrak@naver.com
 *  <http://infra-k.net>
 * -->
 */
#include "server/camera.hpp"

CameraParam::CameraParam()
{
	Poco::UUIDGenerator uuidCreator;
	
	astring strId = uuidCreator.createRandom().toString();
	m_Conf.set_ntype(VID_ONVIF_S);
	m_Conf.set_strid(strId);
	m_Conf.set_strname("Camera");
	m_Conf.set_strip("192.168.0.1");
	m_Conf.set_strport("80");
	m_Conf.set_struser("admin");
	m_Conf.set_strpasswd("admin");
	m_Conf.set_strrtspurl("rtsp://192.168.0.1:554/Streaming");

	astring filePath = "camera.mp4";

	m_Conf.set_strfile(filePath.c_str());
	m_Conf.set_stronvifaddress("/onvif/device_service");
	m_Conf.set_bprofiletoken(false);
	m_Conf.set_bhwaccel(false);
	m_Conf.set_strprofiletoken1("quality_h264");
	m_Conf.set_strprofiletoken1("second_h264");
	m_Conf.set_bhdfsrecord(false);

	astring *pSched = m_Conf.add_crecsched();
	*pSched = REC_SCHED_ALL_DAY;
	
	m_bOnvifUrlGetted = false;
	m_bHasSubStream = false;

	m_Conf.set_nconnecttype(VID_CONNECT_TCP);

	m_Online = false;
	m_OnlineUrl = false;

	astring IP = m_Conf.strip();
	m_strUrl = "rtsp://" + IP + ":" + "554" + "/Streaming";
	m_strUrlSubStream = "rtsp://" + IP + ":" + "554" + "/Streaming";
}

CameraParam::CameraParam(VidCamera &pData)
{
	m_Conf = pData;
	
	m_bOnvifUrlGetted = FALSE;
	m_bHasSubStream = FALSE;
	m_Online = FALSE;
	m_OnlineUrl = FALSE;
}

BOOL CameraParam::UpdateDefaultUrl()
{
	astring IP = m_Conf.strip();
	m_strUrl = "rtsp://" + IP + ":" + "554" + "/Streaming";
	m_strUrlSubStream = "rtsp://" + IP + ":" + "554" + "/Streaming";

	return TRUE;
}

std::string Replace(std::string &str, const char *string_to_replace, const char *new_string)
{
	// Find the first string to replace
	int index = str.find(string_to_replace);

	// while there is one
	while(index != std::string::npos)
	{
		// Replace it
		str.replace(index, strlen(string_to_replace), new_string);
		// Find the next one
		index = str.find(string_to_replace, index + strlen(new_string));
	}

	return str;
}

BOOL CameraParam::CheckOnline()
{
    if (m_Conf.ntype() == VID_FILE || m_Conf.ntype() == VID_RTSP || m_Conf.ntype() == VID_MJPEG )
    {
#ifdef AVOCADO_DEBUG
		VDC_DEBUG("%s %s %d TryCheckDevice False IP %s\n", __FUNCTION__, __FILE__, __LINE__, m_Conf.strip().c_str());
#endif
    	return TRUE;
    }

    astring IP = m_Conf.strip();
    astring Port = m_Conf.strport();
    astring User = m_Conf.struser();
    astring Password = m_Conf.strpasswd();
    astring OnvifAddress = m_Conf.stronvifaddress();
	astring OnvifCameraService = "http://" + IP + ":" + Port + OnvifAddress;
	astring url = "rtsp://" + IP + ":" + "554" + "/";

	if (TryCheckDevice(IP, Port) == false)
	{
#ifdef AVOCADO_DEBUG
		VDC_DEBUG("ERROR: %s %s %d TryCheckDevice False %s\n", __FUNCTION__, __FILE__, __LINE__, OnvifCameraService.c_str());
#endif
		return false;
	}

	if (VVidOnvifC::CheckOnline(User, Password, OnvifCameraService) == true)
	{
		// 주로 이곳
#ifdef AVOCADO_DEBUG
		VDC_DEBUG("%s %s %d TryCheckDevice Begin %s\n", __FUNCTION__, __FILE__, __LINE__, OnvifCameraService.c_str());
#endif
		return true;
	}

	VDC_DEBUG("ERROR: %s %s %d TryCheckDevice False %s\n", __FUNCTION__, __FILE__, __LINE__, OnvifCameraService.c_str());
    return false;
}

BOOL CameraParam::UpdateUrlOnvif()
{
	BOOL bGotUrl = FALSE;
	astring IP = m_Conf.strip();
	astring Port = m_Conf.strport();
	astring User = m_Conf.struser();
	astring Password = m_Conf.strpasswd();
	astring OnvifAddress = m_Conf.stronvifaddress();
	astring OnvifCameraService = "http://" + IP + ":" + Port + OnvifAddress;
	astring url = "rtsp://" + IP + ":" + "554" + "/";
	astring urlSubStream = "rtsp://" + IP + ":" + "554" + "/";
	VVidOnvifProfileMap profileMap;

	if (VVidOnvifC::GetProfiles(User, Password, OnvifCameraService, profileMap) == true)
	{
		// VDC_DEBUG("%s %s %d start : %s Get Stream url\n", __FUNCTION__, __FILE__, __LINE__, OnvifCameraService.c_str());
	}
	else
	{
#ifdef AVOCADO_DEBUG
		VDC_DEBUG("ERROR: %s %s %d : %s Get Stream url failed\n", __FUNCTION__, __FILE__, __LINE__, OnvifCameraService.c_str());
#endif
		return false;
	}
	
	if (profileMap.size() > 0)
	{
		astring strToken;
		if (m_Conf.bprofiletoken() == true)
		{
			strToken = m_Conf.strprofiletoken1().c_str();
			// Find which token is in the OnvifProfileToken, and then use the token
		}
		else
		{
			strToken = profileMap.begin()->first;
			m_Conf.set_strprofiletoken1(strToken.c_str());
		}
		// VDC_DEBUG("%s %s %d strToken %s \n", __FUNCTION__, __FILE__, __LINE__, strToken.c_str());

		VVidOnvifProfileMap::iterator it;
		it = profileMap.find(strToken);
		if (it != profileMap.end())
		{
			if ((*it).second.bGotUrl == true)
			{
				m_strUrl = (*it).second.strRtspUrl;
				if (m_strUrl.size() > 0)
				{
					bGotUrl = TRUE;
				}
			}
		}
	}

	if (profileMap.size() > 1)
	{
		astring strToken;
		if (m_Conf.bprofiletoken() == true)
		{
			strToken = m_Conf.strprofiletoken2().c_str();
			// Find which token is in the OnvifProfileToken, and then use the token
		}
		else
		{
			VVidOnvifProfileMap::iterator it = profileMap.begin();
			it ++;
			strToken = (*it).first;
			m_Conf.set_strprofiletoken2(strToken.c_str());
		}
		// VDC_DEBUG("%s %s %d strToken %s \n", __FUNCTION__, __FILE__, __LINE__, strToken.c_str());

		VVidOnvifProfileMap::iterator it;
		it = profileMap.find(strToken);
		if (it != profileMap.end())
		{
			if ((*it).second.bGotUrl == true)
			{
				m_strUrlSubStream = (*it).second.strRtspUrl;
				if (m_strUrlSubStream.size() > 0)
				{
					bGotUrl = TRUE;
					m_bHasSubStream = TRUE;
				}
			}
		}
	}

	/* Cache the stream profile list */
	VVidOnvifProfileMap::iterator it = profileMap.begin();
	for (int i=0; i != profileMap.size(); ++i, ++it)
	{
		VidStream *pNewStream = m_cStreamList.add_cvidstream();
		
		pNewStream->set_strname((*it).second.strNameDisplay);
		pNewStream->set_strtoken((*it).first);
	}

	/* rtsp://admin:12345@192.168.1.1:554/Streaming/Channels/1?transportmode=unicast&profile=Profile_1 */
	// astring urlWithUser = "rtsp://" + User + ":" + Password + "@";
	// Replace(strUrl, "rtsp://", urlWithUser.c_str());
	if (bGotUrl == true)
	{
		m_bOnvifUrlGetted = TRUE;
	#ifdef AVOCADO_DEBUG
		VDC_DEBUG("%s %s %d end : success %s\n", __FUNCTION__, __FILE__, __LINE__, OnvifCameraService.c_str());
	#endif

	}
	else
	{
	#ifdef AVOCADO_DEBUG
		VDC_DEBUG("ERROR: %s %s %d end : fail %s\n", __FUNCTION__, __FILE__, __LINE__, OnvifCameraService.c_str());
	#endif
	}
	
	return bGotUrl;
}

BOOL CameraParam::UpdateUrl()
{
    // TODO RTSP ONVIF call onvif sdk to get a Stream URL
    if (m_Conf.ntype() == VID_FILE )
    {
        m_strUrl = m_Conf.strfile();
		m_bHasSubStream = FALSE;
    }

    if (m_Conf.ntype() == VID_RTSP || m_Conf.ntype() == VID_MJPEG)
    {
		Poco::URI rtspUrl(m_Conf.strrtspurl());
		astring strRtsp;

		if (rtspUrl.empty() != true)
		{
			strRtsp = rtspUrl.getScheme() + "://" + rtspUrl.getHost() + ":" + std::to_string(rtspUrl.getPort()) + rtspUrl.getPathAndQuery();
		}
		m_strUrl = strRtsp;
		m_bHasSubStream = FALSE;
    }

    if (m_Conf.ntype()== VID_ONVIF_S)
    {
        return UpdateUrlOnvif();
    }

#ifdef AVOCADO_DEBUG
	VDC_DEBUG("%s %s %d url %s\n", __FUNCTION__, __FILE__, __LINE__, m_strUrl.c_str());
#endif
    return TRUE;
}

CameraParam::CameraParam(const CameraParam &pParam)
{
	m_Conf = pParam.m_Conf;

	m_bOnvifUrlGetted = pParam.m_bOnvifUrlGetted;
	m_bHasSubStream = pParam.m_bHasSubStream;
	m_Online = pParam.m_Online;
	m_OnlineUrl = pParam.m_OnlineUrl;
}

CameraParam::~CameraParam()
{
}

Camera::Camera(ConfDB &pConfDB, VDB &pVdb, VHdfsDB &pVHdfsdb, const CameraParam &pParam, RecChangeFunctionPtr pCallback, void *pCallbackParam)
 : m_bStarted(FALSE), m_param(pParam), m_pVdb(pVdb), m_ptzInited(FALSE), 
m_bGotInfoData(FALSE), m_nDataRef(0), m_bGotInfoSubData(FALSE), m_nSubDataRef(0), 
m_pvPlay(new VPlay), m_pvPlaySubStream(new VPlay), m_vPlay(*m_pvPlay), m_vPlaySubStream(*m_pvPlaySubStream), 
m_pConfDB(pConfDB), m_cRecordWrapper(pVdb, pParam.m_Conf.strid(), m_cSchedMap, pCallback, pCallbackParam)
{
	m_param = pParam;
	m_param.UpdateDefaultUrl();
	UpdateRecSched(m_param.m_Conf);

	memset(&m_iFrameCache, 0, sizeof(m_iFrameCache));
	// m_nCnt = 0;
	
	return;
}

Camera::~Camera()
{
	/* stop all the data  */
	Lock();
	SubLock();
	CameraDelCallbackMap::iterator it = m_DelMap.begin();
	for(; it != m_DelMap.end(); ++it)
	{
	    void *pParam = (*it).first;
	    CameraDelCallbackFunctionPtr pFunc = (*it).second;
	    if (pFunc)
	    {
	        pFunc(pParam);
	    }
	}
	SubUnLock();
	UnLock();

	m_vPlay.StopGetData();
	m_vPlaySubStream.StopGetData();

	delete m_pvPlay;
	m_pvPlay = NULL;
	delete m_pvPlaySubStream;
	m_pvPlaySubStream = NULL;

	if (m_iFrameCache.dataBuf != NULL)
	{
		free(m_iFrameCache.dataBuf);
		m_iFrameCache.dataBuf = NULL;
	}
}

bool Camera::UpdateRecSched(VidCamera &pCam)
{
	m_cSchedMap.clear();

	astring strSched = pCam.strsched();

	if (RecordSchedWeek::CheckStringForSched(strSched) == false)
	{
	#ifdef AVOCADO_DEBUG
		VDC_DEBUG("ERROR: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
	#endif
		return false;
	}

	RecordSchedWeek sched(strSched);

	m_cSchedMap["default"] = sched;
	
	m_cRecordWrapper.UpdateSchedMap(m_cSchedMap);

	return true;	
}

BOOL Camera::GetCameraParam(CameraParam &pParam)
{
	pParam = m_param;

	return TRUE;
}

CameraStatus Camera::CheckCamera(astring strUrl, astring strUrlSubStream, BOOL bHasSubStream, BOOL bOnline, BOOL bOnlineUrl, VidStreamList cStreamlist)
{
    if (bOnline == TRUE)
    {
        /* Camera from offline to online */
        if (m_param.m_OnlineUrl == FALSE)
        {
			BOOL HWAccel = FALSE;
			if (m_param.m_Conf.bhwaccel() == TRUE)
			{
				HWAccel = TRUE;
			}
			if (bOnlineUrl == FALSE)
			{
			#ifdef AVOCADO_DEBUG
				VDC_DEBUG("ERROR: %s %s %d DEV_NO_CHANGE FALSE %s\n", __FUNCTION__, __FILE__, __LINE__, m_param.m_strUrl.c_str());
			#endif
				return  DEV_NO_CHANGE;
			}

			m_param.m_strUrl = strUrl;
			m_param.m_strUrlSubStream = strUrlSubStream;
			m_param.m_bHasSubStream = bHasSubStream;
			if (m_param.m_Conf.ntype() == VID_FILE)
			{
				m_vPlay.Init(m_param.m_strUrl, false, "fake", "fake", HWAccel);
			}
			else if (m_param.m_Conf.bmotionjpeg() == true || m_param.m_Conf.ntype() == VID_MJPEG)
			{
				/* Motion JPEG */
				m_vPlay.Init(m_param.m_strUrl, true, m_param.m_Conf.struser(), m_param.m_Conf.strpasswd(), HWAccel);
			}
			else
			{
				m_vPlay.Init(TRUE, m_param.m_strUrl, m_param.m_Conf.struser(),
									m_param.m_Conf.strpasswd(), HWAccel, (VSCConnectType)(m_param.m_Conf.nconnecttype()));
				// VDC_DEBUG("%s %s %d url %s\n", __FUNCTION__, __FILE__, __LINE__, m_param.m_strUrl.c_str());
				if (m_param.m_bHasSubStream == TRUE)
				{
					m_vPlaySubStream.Init(TRUE, m_param.m_strUrlSubStream, m_param.m_Conf.struser(),
												m_param.m_Conf.strpasswd(), HWAccel, (VSCConnectType)(m_param.m_Conf.nconnecttype()));
					// VDC_DEBUG("%s %s %d url %s\n", __FUNCTION__, __FILE__, __LINE__, m_param.m_strUrlSubStream.c_str());
				}
			}					
			m_param.m_OnlineUrl = TRUE;
			UpdatePTZConf();
			m_param.m_cStreamList = cStreamlist;

			/* Always start data */
			StartData();// 이것이 파일 받기임.
        }

        if (m_param.m_Online == FALSE)
        {
            m_param.m_Online = TRUE;
		#ifdef AVOCADO_DEBUG
			VDC_DEBUG("%s %s %d DEV_OFF2ON %s\n", __FUNCTION__, __FILE__, __LINE__, m_param.m_strUrl.c_str());
		#endif
            return DEV_OFF2ON;
        }
    }
	else
    {
        if (m_param.m_Online == TRUE)
        {
            m_param.m_Online = FALSE;
		#ifdef AVOCADO_DEBUG
			VDC_DEBUG("%s %s %d DEV_ON2OFF %s\n", __FUNCTION__, __FILE__, __LINE__, m_param.m_strUrl.c_str());
		#endif
            return DEV_ON2OFF;
        }
    }
#ifdef AVOCADO_DEBUG
	VDC_DEBUG("%s %s %d DEV_NO_CHANGE %s\n", __FUNCTION__, __FILE__, __LINE__, m_param.m_strUrl.c_str());
#endif

    return DEV_NO_CHANGE;    
}

BOOL Camera::UpdatePTZConf()
{
	if (m_param.m_Conf.ntype() != VID_ONVIF_S)
	{
		return TRUE;
	}

	astring IP = m_param.m_Conf.strip();
	astring Port = m_param.m_Conf.strport();
	astring User = m_param.m_Conf.struser();
	astring Password = m_param.m_Conf.strpasswd();
	astring OnvifAddress = m_param.m_Conf.stronvifaddress();
	astring OnvifCameraService = "http://" + IP + ":" + Port + OnvifAddress;

	m_ptz.Init(User, Password, OnvifCameraService);

	m_ptzInited = TRUE;
}

BOOL Camera::PtzAction(FPtzAction action, float speed)
{
	if (m_param.m_Online == FALSE || m_ptzInited == FALSE)
	{
#ifdef AVOCADO_DEBUG
		VDC_DEBUG("%s %s %d PTZ Camera is Offline\n", __FUNCTION__, __FILE__, __LINE__);
#endif
		return TRUE;
	}
#ifdef AVOCADO_DEBUG
	VDC_DEBUG("%s %s %d PTZ Camera is Offline\n", __FUNCTION__, __FILE__, __LINE__);
#endif
	m_ptz.PtzAction((VVidOnvifCPtzAct)action, speed);

	return true;
}

BOOL Camera::FireAlarm(s64 nStartTime)
{	
	return m_cRecordWrapper.FireAlarm(nStartTime);
}

BOOL Camera::GetInfoFrame(InfoFrame &pFrame)
{
	Lock();
	if (m_bGotInfoData == TRUE)
	{
		memcpy(&pFrame, &m_infoData, sizeof(InfoFrame));
		UnLock();

		return TRUE;
	}	
	UnLock();

#ifdef AVOCADO_DEBUG
	VDC_DEBUG("ERROR: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
	return FALSE;
}

BOOL Camera::GetSubInfoFrame(InfoFrame &pFrame)
{
	Lock();
	if (m_bGotInfoSubData == TRUE)
	{
		memcpy(&pFrame, &m_infoSubData, sizeof(InfoFrame));
		UnLock();

		return TRUE;
	}	
	UnLock();

#ifdef AVOCADO_DEBUG
	VDC_DEBUG("ERROR: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
	return FALSE;
}

BOOL Camera::RegDataCallback(CameraDataCallbackFunctionPtr pCallback, void * pParam)
{
	Lock();
	m_DataMap[pParam] = pCallback;
	UnLock();

	StartData();

#ifdef AVOCADO_DEBUG
	VDC_DEBUG("%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
	return TRUE;
}

BOOL Camera::UnRegDataCallback(void * pParam)
{
	Lock();
	m_DataMap.erase(pParam);
	UnLock();

	StopData();

#ifdef AVOCADO_DEBUG
	VDC_DEBUG("%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
	return TRUE;
}

BOOL Camera::RegSubDataCallback(CameraDataCallbackFunctionPtr pCallback, void * pParam)
{
	Lock();
	m_SubDataMap[pParam] = pCallback;
	UnLock();

	if (m_param.m_bHasSubStream == FALSE)
	{
		StartData();
	}
	else
	{
		StartSubData();
	}

#ifdef AVOCADO_DEBUG
	//VDC_DEBUG("%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
	return TRUE;
}

BOOL Camera::UnRegSubDataCallback(void * pParam)
{
	Lock();
	m_SubDataMap.erase(pParam);
	UnLock();

	if (m_param.m_bHasSubStream == FALSE)
	{
		StopData();
	}
	else
	{
		StopSubData();
	}

	return TRUE;
}

BOOL Camera::RegDelCallback(CameraDelCallbackFunctionPtr pCallback, void * pParam)
{
	Lock();
	m_DelMap[pParam] = pCallback;
	UnLock();

	return TRUE;
}

BOOL Camera::UnRegDelCallback(void * pParam)
{
	Lock();
	m_DelMap.erase(pParam);
	UnLock();

	return TRUE;
}

BOOL Camera::StartData()
{
	Lock();
	if (m_param.m_OnlineUrl == TRUE)
	{
		if (m_nDataRef == 0)
		{
			m_vPlay.StartGetData(this, (VPlayDataHandler)Camera::DataHandler);
			m_nDataRef++;
		}
		else
			m_nDataRef++;
	}
	UnLock();

	// printf("%s m_nDataRef %d\n", __FUNCTION__, m_nDataRef);
#ifdef AVOCADO_DEBUG
	//VDC_DEBUG("%s %s %d m_nDataRef %d\n", __FUNCTION__, __FILE__, __LINE__, m_nDataRef);
#endif
	return TRUE;
}

BOOL Camera::StopData()
{
	// printf("%s m_nDataRef %d\n", __FUNCTION__, m_nDataRef);
#ifdef AVOCADO_DEBUG
	//VDC_DEBUG("%s %s %d, m_nDataRef=%d\n", __FUNCTION__, __FILE__, __LINE__, m_nDataRef);
#endif
	
	Lock();
	m_nDataRef --;
	if (m_nDataRef <= 0)
	{
		m_nDataRef = 0;
		m_vPlay.StopGetData();
	}
	UnLock();
	
	// printf("%s m_nDataRef %d\n", __FUNCTION__, m_nDataRef);
#ifdef AVOCADO_DEBUG
	//VDC_DEBUG("%s %s %d, m_nDataRef=%d\n", __FUNCTION__, __FILE__, __LINE__, m_nDataRef);
#endif
	return TRUE;
}

BOOL Camera::StartSubData()
{
	Lock();
	if (m_param.m_OnlineUrl == TRUE)
	{
		if (m_nSubDataRef == 0)
		{
			m_vPlaySubStream.StartGetData(this, (VPlayDataHandler)Camera::SubDataHandler);
			m_nSubDataRef++;
		}
		else
			m_nSubDataRef ++;
 	}
	UnLock();

#ifdef AVOCADO_DEBUG
	VDC_DEBUG("%s %s %d, m_nSubDataRef=%d\n", __FUNCTION__, __FILE__, __LINE__, m_nSubDataRef);
#endif
	return TRUE;
}

BOOL Camera::StopSubData()
{
	Lock();
	m_nSubDataRef --;
	if (m_nSubDataRef <= 0)
	{
		m_nSubDataRef = 0;
		m_vPlaySubStream.StopGetData();
	}
	UnLock();

	// printf("%s m_nDataRef %d\n", __FUNCTION__, m_nDataRef);
#ifdef AVOCADO_DEBUG
	VDC_DEBUG("%s %s %d m_nSubDataRef=%d\n", __FUNCTION__, __FILE__, __LINE__, m_nSubDataRef);
#endif
	return TRUE;
}

/* the frame buffer is alloc here, need free by the application */
BOOL Camera::GetiFrame(VideoFrame& frame)
{
	Lock();
	if (m_iFrameCache.dataBuf == NULL)
	{
		UnLock();
#ifdef AVOCADO_DEBUG
		VDC_DEBUG("ERROR: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
		return false;
	}
	frame.dataBuf = (u8 *)malloc(m_iFrameCache.bufLen);
	frame.dataLen = m_iFrameCache.dataLen;
	frame.streamType = m_iFrameCache.streamType;
	frame.frameType = m_iFrameCache.frameType;
	frame.dataLen = m_iFrameCache.dataLen;
	memcpy(frame.dataBuf, m_iFrameCache.dataBuf, m_iFrameCache.dataLen);
	frame.secs = m_iFrameCache.secs;
	frame.msecs = m_iFrameCache.msecs;
	UnLock();

	return true;
}

BOOL Camera::DataHandler(void* pData, VideoFrame& frame)
{
    int dummy = errno;

	// 너무 많이 나옴
    LPCamera pThread = (LPCamera)pData;
	if (pThread)
    {
        return pThread->DataHandler1(frame);
    }
}

BOOL Camera::DataHandler1(VideoFrame& frame)
{
	Lock();
	/* Frist cache the info frame */
	if (frame.streamType == VIDEO_STREAM_INFO)
	{
		memcpy(&m_infoData, frame.dataBuf, sizeof(InfoFrame));
		m_bGotInfoData = TRUE;
	}

	/* Cache all the I frame */
	if (frame.frameType == VIDEO_FRM_I)
	{
		if (m_iFrameCache.bufLen < frame.dataLen)
		{
			if (m_iFrameCache.dataBuf)
			{
				free(m_iFrameCache.dataBuf);
				m_iFrameCache.dataBuf = NULL;
			}
			m_iFrameCache.dataBuf = (u8 *)malloc(frame.dataLen);
			m_iFrameCache.bufLen = frame.dataLen;
		}
		m_iFrameCache.streamType = frame.streamType;
		m_iFrameCache.frameType = frame.frameType;
		m_iFrameCache.dataLen = frame.dataLen;
		memcpy(m_iFrameCache.dataBuf, frame.dataBuf, frame.dataLen);
		m_iFrameCache.secs = frame.secs;
		m_iFrameCache.msecs = frame.msecs;
	}

	/* 1. Send to network client */
	CameraDataCallbackMap::iterator it = m_DataMap.begin();
	for(; it != m_DataMap.end(); ++it)
	{
	    void *pParam = (*it).first;
	    CameraDataCallbackFunctionPtr pFunc = (*it).second;
	    if (pFunc)
	    {
	        pFunc(frame, pParam);
		#ifdef AVOCADO_DEBUG
			// 너무 많이 나옴
			//VDC_DEBUG("%s %s %d waiting %d\n", __FUNCTION__, __FILE__, __LINE__, frame.dataLen);
		#endif
	    }
	}

	m_cRecordWrapper.PushAFrame(frame);

	/* Call the Sub DataHandler if there has no sub stream */
	if (m_param.m_bHasSubStream == FALSE)
	{
		SubDataHandler1(frame);
	}
	UnLock();

	return TRUE;
}

BOOL Camera::SubDataHandler(void* pData, VideoFrame& frame)
{
    int dummy = errno;

#ifdef AVOCADO_DEBUG
	VDC_DEBUG("%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
    LPCamera pThread = (LPCamera)pData;
    if (pThread)
    {
        return pThread->SubDataHandler1(frame);
    }
}

BOOL Camera::SubDataHandler1(VideoFrame& frame)
{
	SubLock();
	/* Frist cache the info frame */
	if (frame.streamType == VIDEO_STREAM_INFO)
	{
		memcpy(&m_infoSubData, frame.dataBuf, sizeof(InfoFrame));
		m_bGotInfoSubData = TRUE;
	}

	/* 1. Send to client */
	CameraDataCallbackMap::iterator it = m_SubDataMap.begin();
	for(; it != m_SubDataMap.end(); ++it)
	{
	    void *pParam = (*it).first;

	    CameraDataCallbackFunctionPtr pFunc = (*it).second;
	    if (pFunc)
	    {
	        pFunc(frame, pParam);
	    }
	}
	SubUnLock();

	return TRUE;
}

BOOL Camera::GetCameraOnline()
{
    BOOL online = true;

	return m_param.m_Online;
}

BOOL Camera::GetStreamInfo(VideoStreamInfo &pInfo)
{
    m_vPlay.GetStreamInfo(pInfo);

    return TRUE;
}

BOOL Camera::GetCamStreamList(VidStreamList &pList)
{
	pList =  m_param.m_cStreamList;

	return TRUE;
}

BOOL Camera::AttachPlayer(HWND hWnd, int w, int h)
{
    m_vPlay.AttachWidget(hWnd, w, h);

    return TRUE;
}

BOOL Camera::UpdateWidget(HWND hWnd, int w, int h)
{
    m_vPlay.UpdateWidget(hWnd, w, h);

    return TRUE;
}

BOOL Camera::DetachPlayer(HWND hWnd)
{
    m_vPlay.DetachWidget(hWnd);
    
    return TRUE;
}

BOOL Camera::ShowAlarm(HWND hWnd)
{
	m_vPlay.ShowAlarm(hWnd);

	return TRUE;
}
