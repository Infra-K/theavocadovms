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
 이 프로그램은 무료 소프트웨어입니다. Free Software Foundation에서 게시 한 GNU Affero General Public License, 라이선스 버전 3 또는 이후 버전 (귀하의 선택에 따라)에 따라 재배포 및 / 또는 수정할 수 있습니다.
 *
 *이 프로그램은 유용 할 것이라는 희망으로 배포되지만 어떠한 보증도 없습니다. 상품성 또는 특정 목적에의 적합성에 대한 묵시적 보증도 없습니다. 자세한 내용은 GNU Affero General Public License를 참조하십시오.
 *
 *이 프로그램과 함께 GNU Affero General Public License 사본을 받았을 것입니다. 그렇지 않은 경우 <http://www.gnu.org/licenses/>를 참조하십시오.
 *
 이 소프트웨어를 대체 상용 라이선스로 사용할 수 있도록하려면 support@veyesys.com으로 이메일을 보내 자세한 내용을 확인하십시오.
 * -->
 */

#ifndef __VSC_FACTORY_H_
#define __VSC_FACTORY_H_

#include "config/confdb.hpp"
#include "server/camera.hpp"
#include "vdb.hpp"
#include "vhdfsdb.hpp"
#include "vplay.hpp"
#include "config/sysdb.hpp"
#include "server/hdddevice.hpp"
#include <QThread>
#include <qdebug.h>
#include "Poco/Path.h"
#include "Poco/File.h"
#include "config/vidconf.pb.h"
#include "XSDK/TimeUtils.h"
#include "config/videnv.hpp"

using namespace XSDK;
//using namespace std;

typedef enum
{
    FACTORY_CAMERA_ADD = 1,
    FACTORY_CAMERA_DEL,
    FACTORY_CAMERA_ONLINE,
    FACTORY_CAMERA_OFFLINE,
    FACTORY_CAMERA_REC_ON,
    FACTORY_CAMERA_REC_OFF,
    FACTORY_CAMERA_HDFS_REC_ON,
    FACTORY_CAMERA_HDFS_REC_OFF,

    FACTORY_CAMERA_LAST
} FactoryCameraChangeType;


class VE_LIBRARY_API FactoryCameraChangeData
{
public:
	FactoryCameraChangeType type;
	astring id;
	//VidCamera cCam;
};

typedef BOOL (*FactoryCameraChangeNotify)(void* pParam, FactoryCameraChangeData data);

typedef BOOL (*FactoryUserChangeNotify)(void* pParam, astring strUser, astring strPasswd);

typedef std::list<LPCamera> CameraList;
typedef std::list<CameraParam> CameraParamList;
typedef std::map<astring, LPCamera> CameraMap;
typedef std::map<astring, CameraParam> CameraParamMap;
typedef std::map<astring, bool> CameraOnlineMap;
typedef std::map<astring, bool> CameraRecMap;
typedef std::map<void *, FactoryCameraChangeNotify> CameraChangeNofityMap;
typedef std::map<void *, FactoryUserChangeNotify> UserChangeNofityMap;

class Factory;
class VE_LIBRARY_API FactoryHddTask:public QThread
{
	Q_OBJECT
public:
	FactoryHddTask(Factory &pFactory);
	~FactoryHddTask();
public:
	void run();
private:
	Factory &m_Factory;
};

/* Fatory is Qthread for callback in Qt GUI : Factory는 Qt GUI에서 콜백을 위한 Qthread입니다.*/
class VE_LIBRARY_API Factory: public QThread
{
    Q_OBJECT
public:
    Factory(VidEnv &pEnv);
    ~Factory();

public:
	/* Init function */
	BOOL Init();
	s32 InitAddCamera(CameraParam & pParam, astring strCamId);
	ConfDB &GetConfDB(){return m_Conf;};
	
public:
	BOOL RegCameraChangeNotify(void * pData, FactoryCameraChangeNotify callback);
	BOOL UnRegCameraChangeNotify(void * pData);
	BOOL CallCameraChange(FactoryCameraChangeData data);
	static BOOL RecChangeHandler(astring strId, bool bRec, void * pParam);
	BOOL RecChangeHandler1(astring strId, bool bRec);

public:
	BOOL RegUserChangeNotify(void * pData, FactoryUserChangeNotify callback);
	BOOL UnRegUserChangeNotify(void * pData);
	BOOL CallUserChange(astring strUser, astring strPasswd);
	
public:
	BOOL GetLicense(astring &strLicense, astring &strHostId, 
							int &ch, astring &type, astring &startTime, astring &expireTime);
	BOOL SetLicense(astring &strLicense);
	BOOL InitLicense();

	BOOL GetExportPath(astring &strPath);
	BOOL SetExportPath(astring &strPath);

	BOOL GetEventDBConf(VidEventDBConf &pConf);

public:
	bool AuthUser(astring &strUser, astring &strPasswd);
	bool GetAdminPasswd(astring &strPasswd);
	bool SetAdminPasswd(astring strPasswd);

public:
	BOOL GetCameraOnlineMap(CameraOnlineMap &pMap);
	BOOL GetCameraRecMap(CameraRecMap &pMap);
	bool GetCameraList(VidCameraList & pCameraList);

    /* Camera function */
	astring AddCamera(CameraParam & pParam);
	BOOL GetCameraRtspUrl(astring & strUrl, astring strCamId);
	BOOL DelCamera(astring strCamId);
	BOOL GetCamera(astring strId, VidCamera & pCam);
	BOOL PtzAction(astring strCamId, FPtzAction action, float speed);
	BOOL UpdateRecSched(astring strCamId, VidCamera &pCam);
	BOOL FireAlarm(astring strCamId, s64 nStartTime);

public:
	/* Disk function */
	BOOL AddHdd(astring strHdd, astring  strPath, s64 nSize);
	BOOL DelHdd(astring  strHdd);
	BOOL HddUpdateSize(astring strHdd, s64 nSize);
	BOOL GetDiskMap(VDBDiskMap &pMap);
	BOOL GetDiskStatus(VDBDiskStatus &pStatus);
	BOOL UpdateDiskStatusMap(VDBDiskStatus &pStatus);

	/* Search function */
	BOOL SearchItems(astring strCamId, u32 startTime, u32 endTime, u32 recordType, RecordItemMap &map);
	BOOL SearchHasItems(astring strCamId, u32 startTime, u32 endTime, u32 recordType);

	VDB & GetVdb();
	VidEnv & GetEnv(){return m_env;}
	
public:
	BOOL GetStreamInfo(astring strCamId, VideoStreamInfo &pInfo);
	BOOL GetCamStreamList(astring strCamId, VidStreamList &pList);
	/* Data */
	BOOL RegDataCallback(astring strCamId, CameraDataCallbackFunctionPtr pCallback, void * pParam);
	BOOL UnRegDataCallback(astring strCamId, void * pParam);
	BOOL GetInfoFrame(astring strCamId, InfoFrame &pFrame);
	BOOL GetiFrame(astring strCamId, VideoFrame& frame);
	BOOL RegSubDataCallback(astring strCamId, CameraDataCallbackFunctionPtr pCallback, void * pParam);
	BOOL UnRegSubDataCallback(astring strCamId, void * pParam);
	BOOL GetSubInfoFrame(astring strCamId, InfoFrame &pFrame);

	BOOL GetCameraOnline(astring strCamId, BOOL &bStatus);
	BOOL SetSystemPath(astring strPath);

public:
	void run();

private:
	CameraMap m_CameraMap;
	CameraOnlineMap m_CameraOnlineMap;
	CameraRecMap m_CameraRecMap;

	XMutex m_cMutex;
	std::mutex m_tinyMutex;
	
private:
	CameraChangeNofityMap m_CameraChange;
	UserChangeNofityMap m_UserChange;

private:
	VDB *m_pVdb;
	VHdfsDB *m_pVHdfsdb;
	FactoryHddTask *m_HddTask;

private:
	ConfDB m_Conf;
	//SysDB m_SysPath;
	VidEnv &m_env;
};

typedef Factory* LPFactory;


#endif // __VSC_FACTORY_H_
