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
#ifndef __VSC_VID_LIVE_H__
#define __VSC_VID_LIVE_H__

#include "common/vscvidinf.h"
#include "live/vscview.h"
#include <QDockWidget>

class VSCVidLive : public VSCVidInf
{
    Q_OBJECT
public:
    VSCVidLive(ClientFactory &pFactory, QTabWidget &pTab, QMainWindow *parent);
    ~VSCVidLive();

	//infrak
	BOOL GetIsLiveView()
	{
		return m_isLiveView;
	}
	BOOL SetLiveViewDefault()
	{
		m_isLiveView = FALSE;
		return TRUE;
	}

	BOOL GetIsPb()
	{
		return m_isPb;
	}
	BOOL SetPbDefault()
	{
		m_isPb = FALSE;
		return TRUE;
	}

	BOOL GetIsEmap()
	{
		return m_isEmap;
	}
	BOOL SetEmapDefault()
	{
		m_isEmap = FALSE;
		return TRUE;
	}

public:
	virtual void VidShow();
	virtual void VidHide();
	virtual void VidNewLiveView();
	virtual void VidNewLivePB();
	virtual void VidNewEmap(std::string strId);

	virtual VSCVidTreeInf *GetCameraTree();
	virtual VSCVidTreeInf *GetGroupTree();
	virtual VSCVidTreeInf *GetEmapTree();
	virtual VSCVidTreeInf *GetViewTree();
	virtual QWidget * GetMainView();

private:
	VSCView *m_pView;
	VSCVidTreeInf *m_pCameraTree;
	VSCVidTreeInf *m_pGroupTree;
	VSCVidTreeInf *m_pViewTree;
	VSCVidTreeInf *m_pEmapTree;

	//infrak
	BOOL m_isLiveView;
	BOOL m_isPb;
	BOOL m_isEmap;
};
	
#endif