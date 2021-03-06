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
#ifndef __VIDEO_VIDCAMADD_H__
#define __VIDEO_VIDCAMADD_H__

#include <QWidget>
#include <QGridLayout>
#include <QScrollArea>
#include <QMimeData>
#include <QDrag>

#define NOMINMAX

#include "utility.hpp"
#include <QTimer>
#include <QMovie>
#include <QtWidgets/QMainWindow>
#include "client/clientfactory.hpp"
#include "ui_vidcamadd.h"
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QApplication>
#include "client/storsyncinf.hpp"
#include "XSDK/XMutex.h"
#include "XSDK/XGuard.h"

class VidCamTableItem : public QTableWidgetItem
{
public:
	VidCamTableItem(VidCamera &pCam, bool bNew)
	: m_sCam(pCam), m_bNew(bNew)
	{
	}
	~VidCamTableItem(){}

public:
	bool IsNew(){return m_bNew;}
	VidCamera GetCam(){return m_sCam;}

private:
	VidCamera m_sCam;
	bool m_bNew;
};

class VidCamAdd : public QWidget
{
    Q_OBJECT

public:
	VidCamAdd(VidStor &pStor, ClientFactory &pFactory, QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~VidCamAdd();

public:
	void TreeWidgetUpdate();
	void SetCamUI(VidCamera pCam);
	void GetCamUI(VidCamera &pCam);
	void updateParamValue(QTableWidgetItem *item, s8 * pParam);	
	bool CheckIPPort(s8 * ipAddr, s8 * Port);

signals:
	void SignalSectionClicked(int row, int column);

public slots:
	void SlotNewCam();
	void SlotDeleteCam();
	void SlotApplyCam();
	void SlotCancelCam();
	void SlotSectionClicked(int row, int column);
	void SlotRadioButtonClicked();
	void SlotSearchRecv();
    void SlotStartSearch();    
	void SlotStopSearch();    
	void SlotAddAll();	
	void SlotSelectAll();

public:
	Ui::VidCamAdd ui;

private:
	ClientFactory &m_pFactory;
	VidStor m_sStor;
	QTimer *m_Timer;
	bool m_bStarted;
	StorSyncInf *m_syncInfSearch;
	bool m_bSelectedAll;
	XMutex m_cMutex;
};

#endif // __VIDEO_VIDCAMADD_H__
