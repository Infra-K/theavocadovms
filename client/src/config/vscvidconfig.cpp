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
#include "config/vscvidconfig.h"
#include "config/vidstor/vidstoradd.h"
#include "config/vidstor/vidcamadd.h"
#include "config/vidstor/vidstorsetting.h"
#include "config/vidstor/vidcamsetting.h"
#include "config/vidclientsetting.h"
#include "config/vidstor/disksetting/vschddedit.h"
#include "config/vidview/vidviewconf.h"
#include "config/videmap/videmapconf.h"


VSCVidConf::VSCVidConf(ClientFactory &pFactory, QTabWidget &pTab, QMainWindow *parent)
 : VSCVidInf(pFactory, pTab, parent)
{
	m_pConfTree = new VSCVidTreeConf(m_pFactory, parent);
	m_pConfTree->Init();
	m_pConfTree->hide();

	connect(m_pConfTree, SIGNAL(SignalStorAddSelectd()), this, SLOT(SlotVidStorAdd()));
	connect(m_pConfTree, SIGNAL(SignalCamAddSelectd(std::string)), this, SLOT(SlotVidCamAdd(std::string)));
						
	connect(m_pConfTree, SIGNAL(SignalClientConfSelectd()), this, SLOT(SlotVidClientConf()));
	connect(m_pConfTree, SIGNAL(SignalStorConfSelectd(std::string)), this, SLOT(SlotVidStorConf(std::string)));
	connect(m_pConfTree, SIGNAL(SignalDiskConfSelectd(std::string)), this, SLOT(SlotVidDiskConf(std::string)));
	connect(m_pConfTree, SIGNAL(SignalCamConfSelectd(std::string, std::string)), this, SLOT(SlotVidCamConf(std::string, std::string)));
								
	connect(m_pConfTree, SIGNAL(SignalViewConfSelectd()), this, SLOT(SlotVidViewConf()));
	connect(m_pConfTree, SIGNAL(SignalEmapConfSelectd()), this, SLOT(SlotVidEmapConf()));

	SetStorAddDefault();
	SetCamAddDefault();
	SetClientConfDefault();
	SetStorConfDefault();
	SetDiskConfDefault();
	SetCamConfDefault();
	SetViewConfDefault();
	SetEmapConfDefault();
#ifdef AVOCADO_DEBUG
	VDC_DEBUG("%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
}

VSCVidConf::~VSCVidConf()
{
	SetStorAddDefault();
	SetCamAddDefault();
	SetClientConfDefault();
	SetStorConfDefault();
	SetDiskConfDefault();
	SetCamConfDefault();
	SetViewConfDefault();
	SetEmapConfDefault();
}

VSCVidTreeInf *VSCVidConf::GetConfTree()
{
	return m_pConfTree;
}

void VSCVidConf::VidShow()
{

}

void VSCVidConf::VidHide()
{

}

void VSCVidConf::SlotVidStorAdd()
{
#ifdef AVOCADO_DEBUG
	VDC_DEBUG("%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif	
	//infrak 탭 중복표시 방지 기능
	BOOL bTab = false;
	int tabNum = m_pMainArea.count();

	if (GetIsStorAdd() == TRUE)
	{
#ifdef AVOCADO_DEBUG
		VDC_DEBUG("%s %s %d, tabNum=%d\n", __FUNCTION__, __FILE__, __LINE__, tabNum);
#endif
		for (int i = 0; i < tabNum; i++) {
			if (m_pMainArea.tabText(i) == tr("STORAGE ADD"))
			{
				m_pMainArea.setCurrentIndex(i);
				bTab = true;
				break;
			}
		}
	}
	if (bTab)
		return;
	m_isStorAdd = TRUE;
	//infrak
	
	VidStorAdd *pAdd = new VidStorAdd(m_pFactory, &m_pMainArea);
	// pAdd->setWindowFlags(Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);
	// connect(m_pDeviceList, SIGNAL(CameraDoubleClicked(int)), m_pView, SLOT(CameraDoubleClicked(int)));

	m_pMainArea.addTab(pAdd, QIcon(tr(":/device/resources/addstor.png")), tr("STORAGE ADD"));
	m_pMainArea.setCurrentWidget(pAdd);

#ifdef AVOCADO_DEBUG
	VDC_DEBUG("%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
}

void VSCVidConf::SlotVidCamAdd(std::string strStorId)
{
#ifdef AVOCADO_DEBUG
	VDC_DEBUG("%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif	
	//infrak 탭 중복표시 방지 기능
	BOOL bTab = false;
	int tabNum = m_pMainArea.count();

	if (GetIsCamAdd() == TRUE)
	{
	#ifdef AVOCADO_DEBUG
		VDC_DEBUG("%s %s %d, tabNum=%d\n", __FUNCTION__, __FILE__, __LINE__, tabNum);
	#endif
		for (int i = 0; i < tabNum; i++)
		{
			if (m_pMainArea.tabText(i) == tr("CAMERA ADD"))
			{
				m_pMainArea.setCurrentIndex(i);
				bTab = true;
				break;
			}
		}
	}
	if (bTab)
		return;
	m_isCamAdd = TRUE;
	
	VidStor pStor;
	m_pFactory.GetConfDB().GetStorConf(strStorId, pStor);
	VidCamAdd *pAdd = new VidCamAdd(pStor, m_pFactory, &m_pMainArea);
	// pAdd->setWindowFlags(Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);
	// connect(m_pDeviceList, SIGNAL(CameraDoubleClicked(int)), m_pView, SLOT(CameraDoubleClicked(int)));

	m_pMainArea.addTab(pAdd, QIcon(tr(":/device/resources/addcam.png")), tr("CAMERA ADD"));
	m_pMainArea.setCurrentWidget(pAdd);

#ifdef AVOCADO_DEBUG
	VDC_DEBUG("%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
}

void VSCVidConf::SlotVidClientConf()
{
#ifdef AVOCADO_DEBUG
	VDC_DEBUG("%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif	
	//infrak 탭 중복표시 방지 기능
	BOOL bTab = false;
	int tabNum = m_pMainArea.count();

	if (GetIsClientConf() == TRUE)
	{
	#ifdef AVOCADO_DEBUG
		VDC_DEBUG("%s %s %d, tabNum=%d\n", __FUNCTION__, __FILE__, __LINE__, tabNum);
	#endif
		for (int i = 0; i < tabNum; i++)
		{
			if (m_pMainArea.tabText(i) == tr("CLIENT"))
			{
				m_pMainArea.setCurrentIndex(i);
				bTab = true;
				break;
			}
		}
	}
	if (bTab)
		return;
	m_isClientConf = TRUE;
	
	VidClientSetting *pConf = new VidClientSetting(m_pFactory, &m_pMainArea);

	m_pMainArea.addTab(pConf, QIcon(tr(":/action/resources/control_panel.png")), tr("CLIENT"));
	m_pMainArea.setCurrentWidget(pConf);

#ifdef AVOCADO_DEBUG
	VDC_DEBUG("%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
}

void VSCVidConf::SlotVidViewConf()
{
#ifdef AVOCADO_DEBUG
	VDC_DEBUG("%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif	
	//infrak 탭 중복표시 방지 기능
	BOOL bTab = false;
	int tabNum = m_pMainArea.count();

	if (GetIsViewConf() == TRUE)
	{
	#ifdef AVOCADO_DEBUG
		VDC_DEBUG("%s %s %d, tabNum=%d\n", __FUNCTION__, __FILE__, __LINE__, tabNum);
	#endif
		for (int i = 0; i < tabNum; i++)
		{
			if (m_pMainArea.tabText(i) == tr("VIEW"))
			{
				m_pMainArea.setCurrentIndex(i);
				bTab = true;
				break;
			}
		}
	}
	if (bTab)
		return;
	m_isViewConf = TRUE;
	
	VidViewConf *pConf = new VidViewConf(m_pFactory, &m_pMainArea);
	
	m_pMainArea.addTab(pConf, QIcon(tr(":/device/resources/view.png")), tr("VIEW"));
	m_pMainArea.setCurrentWidget(pConf);

#ifdef AVOCADO_DEBUG
	VDC_DEBUG("%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
}

void VSCVidConf::SlotVidEmapConf()
{
#ifdef AVOCADO_DEBUG
	VDC_DEBUG("%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif	
	//infrak 탭 중복표시 방지 기능
	BOOL bTab = false;
	int tabNum = m_pMainArea.count();

	if (GetIsEmapConf() == TRUE)
	{
	#ifdef AVOCADO_DEBUG
		VDC_DEBUG("%s %s %d, tabNum=%d\n", __FUNCTION__, __FILE__, __LINE__, tabNum);
	#endif
		for (int i = 0; i < tabNum; i++)
		{
			if (m_pMainArea.tabText(i) == tr("EMAP"))
			{
				m_pMainArea.setCurrentIndex(i);
				bTab = true;
				break;
			}
		}
	}
	if (bTab)
		return;
	m_isEmapConf = TRUE;
	
	VidEmapConf *pConf = new VidEmapConf(m_pFactory, &m_pMainArea);

	m_pMainArea.addTab(pConf, QIcon(tr(":/action/resources/map.png")), tr("EMAP"));
	m_pMainArea.setCurrentWidget(pConf);

#ifdef AVOCADO_DEBUG
	VDC_DEBUG("%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
}

void VSCVidConf::SlotVidStorConf(std::string strStorId)
{
#ifdef AVOCADO_DEBUG
	VDC_DEBUG("%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
	//infrak 탭 중복표시 방지 기능
	BOOL bTab = false;
	int tabNum = m_pMainArea.count();

	if (GetIsStorConf() == TRUE)
	{
#ifdef AVOCADO_DEBUG
		VDC_DEBUG("%s %s %d, tabNum=%d\n", __FUNCTION__, __FILE__, __LINE__, tabNum);
#endif
		for (int i = 0; i < tabNum; i++)
		{
			if (m_pMainArea.tabText(i) == tr("STORAGE"))
			{
				m_pMainArea.setCurrentIndex(i);
				bTab = true;
				break;
			}
		}
	}
	if (bTab)
		return;
	m_isStorConf = TRUE;
	
	VidStor pStor;
	m_pFactory.GetConfDB().GetStorConf(strStorId, pStor);
	VidStorSetting *pConf = new VidStorSetting(pStor, m_pFactory, &m_pMainArea);
	m_pMainArea.addTab(pConf, QIcon(tr(":/action/resources/control_panel.png")), tr("STORAGE"));
	m_pMainArea.setCurrentWidget(pConf);

#ifdef AVOCADO_DEBUG
	VDC_DEBUG("%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
}

void VSCVidConf::SlotVidDiskConf(std::string strStorId)
{
#ifdef AVOCADO_DEBUG
	VDC_DEBUG("%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif	
	//infrak 탭 중복표시 방지 기능
	BOOL bTab = false;
	int tabNum = m_pMainArea.count();

	if (GetIsDiskConf() == TRUE)
	{
	#ifdef AVOCADO_DEBUG
		VDC_DEBUG("%s %s %d, tabNum=%d\n", __FUNCTION__, __FILE__, __LINE__, tabNum);
	#endif
		for (int i = 0; i < tabNum; i++)
		{
			if (m_pMainArea.tabText(i) == tr("DISK"))
			{
				m_pMainArea.setCurrentIndex(i);
				bTab = true;
				break;
			}
		}
	}
	if (bTab)
		return;
	m_isDiskConf = TRUE;
	
	VidStor pStor;
	m_pFactory.GetConfDB().GetStorConf(strStorId, pStor);
	VSCHddEdit *pConf = new VSCHddEdit(pStor, &m_pMainArea);
	m_pMainArea.addTab(pConf, QIcon(tr(":/action/resources/disksetting.png")), tr("DISK"));
	m_pMainArea.setCurrentWidget(pConf);

#ifdef AVOCADO_DEBUG
	VDC_DEBUG("%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
}

void VSCVidConf::SlotVidCamConf(std::string strStor, std::string strCam)
{
#ifdef AVOCADO_DEBUG
	VDC_DEBUG("%s %s %d, strStor=%s, strCam=%s\n", __FUNCTION__, __FILE__, __LINE__, strStor.c_str(), strCam.c_str());
#endif
	//infrak 탭 중복표시 방지 기능
	BOOL bTab = false;
	int tabNum = m_pMainArea.count();

	if (GetIsCamConf() == TRUE)
	{
		for (int i = 0; i < tabNum; i++)
		{
			QString tabText = m_pMainArea.tabText(i);
			char tabStr[1024] = { 0, };
			qsnprintf(tabStr, sizeof(tabStr), "%s", tabText.toUtf8().constData());

#ifdef AVOCADO_DEBUG
			VDC_DEBUG("%s %s %d, tabText=%s\n", __FUNCTION__, __FILE__, __LINE__, tabStr);
#endif
			if (m_pMainArea.tabText(i) == tr("CAMERA"))
			{
				m_pMainArea.setCurrentIndex(i);
				bTab = true;
				break;
			}
		}
	}
	if (bTab)
		return;
	m_isCamConf = TRUE;

	VidStor pStor;
	m_pFactory.GetConfDB().GetStorConf(strStor, pStor);
	VidCamSetting *pConf = new VidCamSetting(pStor, strCam, m_pFactory, &m_pMainArea);
#if 0
	//카메라 넘버표시 기능 제거
	m_pMainArea.addTab(pConf, QIcon(tr(":/device/resources/camera.png")), tr("CAMERA %1").arg(currentNum + 1));
#endif
	m_pMainArea.addTab(pConf, QIcon(tr(":/device/resources/camera.png")), tr("CAMERA"));
	m_pMainArea.setCurrentWidget(pConf);

#ifdef AVOCADO_DEBUG
	VDC_DEBUG("%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
}
