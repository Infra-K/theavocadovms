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
#ifndef __VSC_VID_CONF_H__
#define __VSC_VID_CONF_H__

#include "common/vscvidinf.h"
#include "config/vscvidtreeconf.h"

class VSCVidConf : public VSCVidInf
{
    Q_OBJECT
public:
    VSCVidConf(ClientFactory &pFactory, QTabWidget &pTab, QMainWindow *parent);
    ~VSCVidConf();
	
public:
	virtual void VidShow();
	virtual void VidHide();

	// infrak 중복 표시 초기화 및 기능 
	BOOL GetIsStorAdd()
	{
		return m_isStorAdd;
	}
	BOOL SetStorAddDefault()
	{
		m_isStorAdd = FALSE;
		return TRUE;
	}

	BOOL GetIsCamAdd()
	{
		return m_isCamAdd;
	}
	BOOL SetCamAddDefault()
	{
		m_isCamAdd = FALSE;
		return TRUE;
	}

	BOOL GetIsClientConf()
	{
		return m_isClientConf;
	}
	BOOL SetClientConfDefault()
	{
		m_isClientConf = FALSE;
		return TRUE;
	}

	BOOL GetIsStorConf()
	{
		return m_isStorConf;
	}
	BOOL SetStorConfDefault()
	{
		m_isStorConf = FALSE;
		return TRUE;
	}

	BOOL GetIsDiskConf()
	{
		return m_isDiskConf;
	}
	BOOL SetDiskConfDefault()
	{
		m_isDiskConf = FALSE;
		return TRUE;
	}

	BOOL GetIsCamConf()
	{
		return m_isCamConf;
	}
	BOOL SetCamConfDefault()
	{
		m_isCamConf = FALSE;
		return TRUE;
	}

	BOOL GetIsViewConf()
	{
		return m_isViewConf;
	}
	BOOL SetViewConfDefault()
	{
		m_isViewConf = FALSE;
		return TRUE;
	}

	BOOL GetIsEmapConf()
	{
		return m_isEmapConf;
	}
	BOOL SetEmapConfDefault()
	{
		m_isEmapConf = FALSE;
		return TRUE;
	}

public:
	virtual VSCVidTreeInf *GetConfTree();

public slots:
	void SlotVidStorAdd();
	void SlotVidCamAdd(std::string strStorId);
	void SlotVidClientConf();
	void SlotVidStorConf(std::string strStorId);
	void SlotVidDiskConf(std::string strStorId);
	void SlotVidCamConf(std::string strStor, std::string strCam);
	void SlotVidViewConf();
	void SlotVidEmapConf();

private:
	VSCVidTreeInf *m_pConfTree;

	// infrak 중복 체크 변수 할당
	BOOL m_isStorAdd;
	BOOL m_isCamAdd;
	BOOL m_isClientConf;
	BOOL m_isStorConf;
	BOOL m_isDiskConf;
	BOOL m_isCamConf;
	BOOL m_isViewConf;
	BOOL m_isEmapConf;
};

#endif
	