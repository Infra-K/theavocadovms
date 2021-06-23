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
#include "vdb.hpp"
#include "license.hpp"

VDB::VDB(astring & strPath) : m_IndexDB(strPath), /* m_EventDB(strPath), */ m_Mode(RECORDING_FULL_LOOP)
{
	this->m_pThread = new tthread::thread(VDB::Run, (void *)this);
	nCnt = 0;
}

VDB::~VDB()
{
	m_bExit = true;

	m_pThread->join();

	delete m_pThread;
	m_pThread = NULL;
}

BOOL VDB::SetRecordingMode(RecordingMode mode)
{
   m_Mode = mode;

   return TRUE;
}

void VDB::Lock()
{
	m_Lock.lock();
}

void VDB::UnLock()
{
	m_Lock.unlock();
}

BOOL VDB::GetDiskMap(VDBDiskMap &pMap)
{
	Lock();
	BOOL ret = m_IndexDB.GetDiskMap(pMap);
	UnLock();

	return ret;
}

BOOL VDB::GetDiskStatus(VDBDiskStatus &pStatus)
{
	Lock();
	BOOL ret = m_IndexDB.GetDiskStatus(pStatus);
	UnLock();

	return ret;
}

BOOL VDB::UpdateDiskStatusMap(VDBDiskStatus &pMap)
{
	Lock();
	BOOL ret = m_IndexDB.UpdateDiskStatusMap(pMap);
	UnLock();

	return ret;
}

BOOL VDB::AddHdd(astring &strHdd, astring &strPath, s64 nSize)
{
	Lock();
	BOOL ret = m_IndexDB.AddHdd(strHdd, strPath, nSize);
	UnLock();

	return ret;
}

BOOL VDB::DelHdd(astring &strHdd)
{
	Lock();
	BOOL ret = m_IndexDB.DelHdd(strHdd);
	UnLock();

	return ret;
}

BOOL VDB::HddUpdateSize(astring &strHdd, s64 nSize)
{
	Lock();
	BOOL ret = m_IndexDB.HddUpdateSize(strHdd, nSize);
	UnLock();

	return ret;
}

/* video search function, if startTime or endTime is 0, it mean all */
BOOL VDB::SearchItems(astring deviceId, u32 startTime, u32 endTime, u32 recordType, RecordItemMap &pMap)
{
	Lock();
	BOOL ret = m_IndexDB.SearchItems(deviceId, startTime, endTime, recordType, pMap);
	UnLock();

	return ret;
}

/* video search function, if startTime or endTime is 0, it mean all */
BOOL VDB::SearchHasItems(astring deviceId, u32 startTime, u32 endTime, u32 recordType)
{
	Lock();
	BOOL ret = m_IndexDB.SearchHasItems(deviceId, startTime, endTime, recordType);
	UnLock();

	return ret;
}

BOOL VDB::SearchAItem(astring deviceId, u32 Time, VdbRecordItem &pItem)
{
	Lock();
	BOOL ret = m_IndexDB.SearchAItem(deviceId, Time, pItem);
	UnLock();

	return ret;
}

BOOL VDB::SearchAItemNear(astring deviceId, u32 Time, VdbRecordItem &pItem)
{
	Lock();
	BOOL ret = m_IndexDB.SearchAItemNear(deviceId, Time, pItem);
	UnLock();

	return ret;
}

BOOL VDB::SearchNextItem(astring deviceId, s64 LastId, VdbRecordItem &pItem)
{
	Lock();
	BOOL ret = m_IndexDB.SearchNextItem(deviceId, LastId, pItem);
	UnLock();

	return ret;
}

BOOL VDB::RequestAMFRead(VdbRecordItem &pItem, astring &strPath)
{
	Lock();
	BOOL ret = FALSE;
	m_IndexDB.GetRecordFilePath(pItem, strPath);
	if (strPath.length() > 0)
	{
		ret = m_IndexDB.RecordRLock(strPath);
	}
	UnLock();

	return ret;	
}

BOOL VDB::FinishedAMFRead(VdbRecordItem &pItem, astring &strPath)
{
	Lock();
	BOOL ret = m_IndexDB.RecordRUnLock(strPath);
	UnLock();

	return ret;
}

void VDB::Run(void *pParam)
{
	int dummy = errno;
	VDC_DEBUG("%s %s %d \n", __FUNCTION__, __FILE__, __LINE__);

	VDB *pThread = (VDB *)pParam;
	if (pThread)
	{
	    pThread->Run1();
	}
}
 
void VDB::Run1()
{
	while(1)
	{
		int ret = m_IndexDB.MergeSingleStep();
		ve_sleep(ret);
	}
}

/* The Task will merge type with real items, and pre record is based on the file */
BOOL VDB::AddSchedItem(astring deviceId, s64 startTime, s64 endTime, RecordingType recordType)
{
	Lock();
	BOOL ret = m_IndexDB.AddSchedItem(deviceId, startTime, endTime, recordType);
	UnLock();

	return ret;
}

RecordSession *VDB::StartRecord(astring deviceId, u32 startTime, u32 recordType)
{
	// 녹화시 무지 많이 출력됨
	/*
	if (nCnt > 1000)
	{
		VDC_DEBUG("%s %s %d start deviceId=%s \n", __FUNCTION__, __FILE__, __LINE__, deviceId.c_str());
		nCnt = 0;
	}
	else
	{
		nCnt++;
	}
	VDB::StartRecord ..\src\vdb\vdb.cpp 221 start deviceId = 2c4fe182 - e2f0 - 44f1 - a256 - f0e7c40f4f69
	VDB::StartRecord ..\src\vdb\vdb.cpp 221 start deviceId = a79860f9 - 188d - 416d - 8426 - ea9734eac7fc
	VDB::StartRecord ..\src\vdb\vdb.cpp 221 start deviceId = 2c4fe182 - e2f0 - 44f1 - a256 - f0e7c40f4f69
	VDB::StartRecord ..\src\vdb\vdb.cpp 221 start deviceId = 4fa850ed - 3374 - 4379 - bcc0 - 6b334e922c26
	VDB::StartRecord ..\src\vdb\vdb.cpp 221 start deviceId = 2c4fe182 - e2f0 - 44f1 - a256 - f0e7c40f4f69
	VDB::StartRecord ..\src\vdb\vdb.cpp 221 start deviceId = 4fa850ed - 3374 - 4379 - bcc0 - 6b334e922c26
	VDB::StartRecord ..\src\vdb\vdb.cpp 221 start deviceId = 2c4fe182 - e2f0 - 44f1 - a256 - f0e7c40f4f69
	VDB::StartRecord ..\src\vdb\vdb.cpp 221 start deviceId = 4fa850ed - 3374 - 4379 - bcc0 - 6b334e922c26
	VDB::StartRecord ..\src\vdb\vdb.cpp 221 start deviceId = d0a32e62 - c547 - 4125 - 8ce7 - 90d2300ad703
	VDB::StartRecord ..\src\vdb\vdb.cpp 221 start deviceId = 4fa850ed - 3374 - 4379 - bcc0 - 6b334e922c26
	VDB::StartRecord ..\src\vdb\vdb.cpp 221 start deviceId = 2c4fe182 - e2f0 - 44f1 - a256 - f0e7c40f4f69
	VDB::StartRecord ..\src\vdb\vdb.cpp 221 start deviceId = 4fa850ed - 3374 - 4379 - bcc0 - 6b334e922c26
	*/
	// 너무 많이 나옴
	// VDC_DEBUG("%s %s %d start deviceId=%s\n", __FUNCTION__, __FILE__, __LINE__, deviceId.c_str());
	// VDB::StartRecord ..\src\vdb\vdb.cpp 215 start deviceId = 1e717cf6 - 7118 - 4443 - a589 - 7ac623d749ea
	// VDB::StartRecord ..\src\vdb\vdb.cpp 215 start deviceId = 7f4c92cb - cd65 - 49d4 - 91ea - 8d866cb3b9ac

	astring strBlockPath;

	Lock();
	if (m_IndexDB.RequestABlockFile(strBlockPath) != TRUE) // DB, 디스크 또는 텅빈 초기 파일 생성 점검
	{
		if (m_Mode == RECORDING_FULL_STOP)
		{
			VDC_DEBUG("ERROR: %s %s %d Can not RequestABlockFile \n", __FUNCTION__, __FILE__, __LINE__);
			UnLock();
			return NULL;
		}
		else
		{
			// VDC_DEBUG("%s %s %d Recording loop mode \n", __FUNCTION__, __FILE__, __LINE__);
			if (m_IndexDB.GetAOldRecord(strBlockPath) == false) // DB
			{
				// 너무 많이 나옴
				// VDC_DEBUG("ERROR: %s %s %d Can not Start Record \n", __FUNCTION__, __FILE__, __LINE__); ERROR: VDB::StartRecord ..\src\vdb\vdb.cpp 230 Can not Start Record
				UnLock();
				return NULL;
			}
		}
	}
	if (LicRecordRef() == false)
	{
		VDC_DEBUG("ERROR: %s %s %d Need License !\n", __FUNCTION__, __FILE__, __LINE__);
		UnLock();
		return NULL;
	}
	s64 recordId = m_IndexDB.AddRecord(deviceId, recordType, startTime, strBlockPath); // DB
	m_IndexDB.RecordWLock(strBlockPath); // DB
	UnLock();
	// VDC_DEBUG("%s %s %d end deviceId=%s\n", __FUNCTION__, __FILE__, __LINE__, deviceId.c_str());

	return new RecordSession(deviceId, strBlockPath, startTime, recordId);
}

BOOL VDB::FinishRecord(RecordSession * pRs)
{
	s64 recordId = pRs->GetRecordId();
	u32 endTime = pRs->GetEndTime();

	LicRecordUnRef();

	// VDC_DEBUG("%s %s %d start\n", __FUNCTION__, __FILE__, __LINE__);
   	Lock();		
	m_IndexDB.UpdateRecordEndtime(recordId, endTime);
	m_IndexDB.RecordWUnLock(pRs->GetBlockPath());
	UnLock();

	m_IndexDB.UpdateRecordEndtime(recordId, endTime);
	VDC_DEBUG("%s %s %d recordId %lld, time %llu\n", __FUNCTION__, __FILE__, __LINE__, recordId, endTime);

	return TRUE;
}

/* VDBPrivate is thread from Qt, it timely get QStorageInfo list and set to m_IndexDB m_IndexDB will use this from request block that disk is OK,
for disk is not OK, just SIKP for the block is not is the HDD list, just remove. 

Delete the disk: just delete the HDD from the list, and then all the write and read next time will skip this disk.

all the block is alloc when used. different with current add a new class for hadoop recording.
 */
