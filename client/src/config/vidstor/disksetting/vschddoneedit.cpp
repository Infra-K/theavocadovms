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
#include "config/vidstor/disksetting/vschddoneedit.h"
#include "debug.hpp"
#include <QTableWidgetItem>
#include <QMessageBox>
#include "client/storsyncinf.hpp"
#include "vscloading.hpp"

static astring GetDiskSizeString(s64 size) 
{
	s8 DiskTitle[1024];

	if (size / 1024 > 0)
	{
		if (size / (1024 * 1024) > 0)
		{
	   		sprintf(DiskTitle, "%lld T",  size / (1024 * 1024));
		}
		else
		{
	   		sprintf(DiskTitle, "%lld G",  size / 1024);
		}
	}
	else
	{
	   sprintf(DiskTitle, "%lld M",  size);
	}

	astring strSize = DiskTitle;

	return strSize;
}

static astring GetDiskSizeString2(s64 size)
{
	s8 DiskTitle[1024];

	if (size / 1024 > 0)
	{
		sprintf(DiskTitle, "%lld G", size / 1024);
	}
	else
	{
		sprintf(DiskTitle, "%lld M", size);
	}

	astring strSize = DiskTitle;

	return strSize;
}

VSCHddOneEdit::VSCHddOneEdit(VidStor &stor, VidDisk disk, VidDiskList diskMap, QWidget *parent)
 : QWidget(parent), m_Disk(disk), m_DiskMap(diskMap), m_added(false), m_pStor(stor)
{
	s8 DiskTitle[1024];

#ifdef WIN32
	setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
#endif
	ui.setupUi(this);
#ifdef WIN32
	setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
	ui.tableWidget->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
#endif
	m_totalSize = m_Disk.ntotalsize();
	m_freeSize = m_Disk.nfreesize();

	astring strName = m_Disk.strpath();
	astring strHdd = m_Disk.strid();
	astring strLimit = "0 M";
	astring strRecording = "0 M";
	VidDisk addedDisk;

	s64 nstoragelimit = 0;
	s64 nstorageused = 0;
	s64 nused = 0;
	s64 nlimit = 0;
	
	VidCameraList camList;
	StorSyncInf syncInf(m_pStor, 5 * 1000);

	if (syncInf.Connect() == false)
	{
#ifdef AVOCADO_DEBUG
		VDC_DEBUG("ERROR: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
	}
	camList = syncInf.GetVidCameraList();

	int storSize = camList.cvidcamera_size();

#ifdef AVOCADO_DEBUG
	for (s32 i = 0; i < storSize; i++)
	{
		VidCamera pCam = camList.cvidcamera(i);
		VDC_DEBUG("%s %s %d, pCam=%s\n", __FUNCTION__, __FILE__, __LINE__, pCam.strname().c_str());
		//VSCHddOneEdit::VSCHddOneEdit ..\src\config\vidstor\disksetting\vschddoneedit.cpp 97, pCam = 172.30.1.20
	}
#endif

#ifdef AVOCADO_DEBUG
	VDC_DEBUG("%s, m_totalSize=%lld, m_freeSize=%lld, strName=%s, strHdd=%s, camCount=%d\n", __FUNCTION__, m_totalSize, m_freeSize, strName.c_str(), strHdd.c_str(), storSize);
	//VSCHddOneEdit::VSCHddOneEdit, m_totalSize=2861570, m_freeSize=1493784, strName=D:/, strHdd=\\?\Volume{a1e4b6ac-e723-4fee-ad8a-291119a6eca2}\, camCount=3
	//VSCHddOneEdit::VSCHddOneEdit, m_totalSize = 953866, m_freeSize = 22618, strName = E: / , strHdd = \\ ? \Volume{ 3868fc61 - 0000 - 0000 - 0000 - 100000000000 }\, camCount = 3
#endif

	for (s32 i = 0; i < m_DiskMap.cviddisk_size(); i ++)
	{
		VidDisk disk = m_DiskMap.cviddisk(i);
		if (disk.strid() == m_Disk.strid())
		{
			m_added = true;
			addedDisk = disk;
			break;
		}
	}

	ui.tableWidget->setItem(0, 0, new QTableWidgetItem(strName.c_str()));
	ui.tableWidget->setItem(0, 1, new QTableWidgetItem(GetDiskSizeString2(m_totalSize).c_str()));
	ui.tableWidget->setItem(0, 2, new QTableWidgetItem(GetDiskSizeString2(m_freeSize).c_str()));
	ui.checkBox->setChecked(m_added);

	ui.horizontalSlider->setMinimum(0); 
	ui.horizontalSlider->setMaximum(m_totalSize/1024); 
	if (m_added == true)
	{
		nstoragelimit = addedDisk.nstorlimit();
		nstorageused = addedDisk.nstorused();
		strLimit = GetDiskSizeString2(nstoragelimit);
		strRecording = GetDiskSizeString2(nstorageused);
		//ui.horizontalSlider->setValue(nstoragelimit/1024);		
		switch (storSize)
		{
		case 1:
			nused = 102400 - nstorageused;
			if (nused <= 0)
				nused = 102400;
			nlimit = m_totalSize - m_freeSize + nused;
			if (nlimit > 0)
				ui.horizontalSlider->setValue(nlimit / 1024);
			else
				ui.horizontalSlider->setValue(nstoragelimit / 1024);
#ifdef AVOCADO_DEBUG
			//100G = 102,400, 200G = 204,800, 300G = 307,200
			VDC_DEBUG("%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
			break;
		case 2:
			nused = 204800 - nstorageused;
			if (nused <= 0)
				nused = 204800;
			nlimit = m_totalSize - m_freeSize + nused;
			if (nlimit > 0)
				ui.horizontalSlider->setValue(nlimit / 1024);
			else
				ui.horizontalSlider->setValue(nstoragelimit / 1024);
#ifdef AVOCADO_DEBUG
			VDC_DEBUG("%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
			break;
		case 3:
			nused = 307200 - nstorageused;
			if (nused <= 0)
				nused = 307200;
			nlimit = m_totalSize - m_freeSize + nused;
			if (nlimit > 0)
				ui.horizontalSlider->setValue(nlimit / 1024);
			else
				ui.horizontalSlider->setValue(nstoragelimit / 1024);
#ifdef AVOCADO_DEBUG
			VDC_DEBUG("%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
			break;
		case 4:
			nused = 409600 - nstorageused;
			if (nused <= 0)
				nused = 409600;
			nlimit = m_totalSize - m_freeSize + nused;
			if (nlimit > 0)
				ui.horizontalSlider->setValue(nlimit / 1024);
			else
				ui.horizontalSlider->setValue(nstoragelimit / 1024);
#ifdef AVOCADO_DEBUG
			VDC_DEBUG("%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
			break;
		default:
			nlimit = m_totalSize - m_freeSize;
			ui.horizontalSlider->setValue(nlimit / 1024);
#ifdef AVOCADO_DEBUG
			VDC_DEBUG("%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
			break;
		}
	}
	else
	{
		strLimit = GetDiskSizeString(m_totalSize - 1024);
		strRecording = "0 M";
		ui.horizontalSlider->setValue(m_totalSize/1024); 
	}

	astring barToolTip;
	if (nlimit > 0)
		barToolTip = GetDiskSizeString2(nlimit);
	else
		barToolTip = GetDiskSizeString(m_totalSize - 1024);
	ui.horizontalSlider->setToolTip(barToolTip.c_str());
#ifdef AVOCADO_DEBUG
	VDC_DEBUG("%s, strLimit=%s:nstoragelimit=%lld, strRecording=%s:nstorageused=%lld, nlimit=%lld\n", __FUNCTION__, strLimit.c_str(), nstoragelimit, strRecording.c_str(), nstorageused, nlimit);
	//VSCHddOneEdit::VSCHddOneEdit, strLimit = 2 T:nstoragelimit = 2613248, strRecording = 520 G : nstorageused = 532656, nlimit = 1674986
	//VSCHddOneEdit::VSCHddOneEdit, strLimit = 930 G:nstoragelimit = 0, strRecording = 0 M : nstorageused = 0
#endif

	if (nlimit > 0)
		ui.tableWidget->setItem(0, 3, new QTableWidgetItem(barToolTip.c_str()));
	else
		ui.tableWidget->setItem(0, 3, new QTableWidgetItem(strLimit.c_str()));
	ui.tableWidget->setItem(0, 4, new QTableWidgetItem(strRecording.c_str()));

#if 0
	//infrak 사용 못하도록함
	connect(ui.horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(setFreeValue(int)));
#endif
	connect(ui.checkBox, SIGNAL( toggled(bool) ), this, SLOT(diskSelected(bool)));
}

void VSCHddOneEdit::diskSelected(bool value)
{
	VSCLoading * pLoading = VSCLoading::Create();
	StorSyncInf syncInf(m_pStor);
	astring pVer;
	astring strInfo;

	syncInf.Connect();
	
	if (m_added == true && value != true)
	{
		QMessageBox msgBox(this);
		// Set text
		msgBox.setText("Delete the Hdd, The Data on this disk will be lost ... ");
		msgBox.setIcon(QMessageBox::Information);
		msgBox.setIconPixmap(QPixmap(":/logo/resources/vsc32.png"));
		msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Ok);

		int ret = msgBox.exec();
		switch (ret) {
		case QMessageBox::Ok:
			break;
		default:
			ui.checkBox->setChecked(m_added);
			return;
			break;
		}
		syncInf.DeleteVidDisk(m_Disk.strid());
		m_added = false;
		delete pLoading;
#ifdef AVOCADO_DEBUG
		VDC_DEBUG("%s Del Hdd strHdd %s %s\n", __FUNCTION__, m_Disk.strid().c_str(), m_Disk.strpath().c_str());
#endif
		return;
	}
	else if (m_added == false && value == true)
	{
		s64 limitSize = ui.horizontalSlider->value() * 1024;
		m_Disk.set_nstorlimit(limitSize);
		
		syncInf.AddVidDisk(m_Disk);
		m_added = true;
#ifdef AVOCADO_DEBUG
		VDC_DEBUG("%s Add Hdd strHdd %s %s\n", __FUNCTION__, m_Disk.strid().c_str(), m_Disk.strpath().c_str());
#endif
	}

	delete pLoading;

	return;
}

void VSCHddOneEdit::setFreeValue(int value)
{
	astring strLimit = "0 M";
	astring strRecording = "0 M";
	s64 usedSize = (m_totalSize - m_freeSize) / 1024;

#ifdef AVOCADO_DEBUG
	VDC_DEBUG("%s %s %d, value=%d, usedSize=%lld\n", __FUNCTION__, __FILE__, __LINE__, value, usedSize);
	//VSCHddOneEdit::setFreeValue ..\src\config\vidstor\disksetting\vschddoneedit.cpp 179, value=2794, restSize=1324
#endif
	/* The limit can be less than 2G */
	if (value <= (usedSize + 1))
	{
		ui.horizontalSlider->setValue(usedSize + 1);
#ifdef AVOCADO_DEBUG
		VDC_DEBUG("%s, strid=%s, strpath=%s, usedSize=%lld\n", __FUNCTION__, m_Disk.strid().c_str(), m_Disk.strpath().c_str(), usedSize);		
#endif
	}
	else
	{		
		if (m_added)
		{
			VSCLoading * pLoading = VSCLoading::Create();
			StorSyncInf syncInf(m_pStor);
			astring pVer;
			astring strInfo;
			syncInf.Connect();
			s64 limitSize = ui.horizontalSlider->value() * 1024;

			ui.tableWidget->setItem(0, 3, new QTableWidgetItem(GetDiskSizeString(limitSize).c_str()));
			syncInf.UpdateVidDiskLimit(m_Disk.strid(), limitSize);

			delete pLoading;
#ifdef AVOCADO_DEBUG
			VDC_DEBUG("%s HddUpdateSize strHdd %s %s limit %lld\n", __FUNCTION__, m_Disk.strid().c_str(), m_Disk.strpath().c_str(), limitSize);
			//VSCHddOneEdit::setFreeValue HddUpdateSize strHdd \\ ? \Volume{ a1e4b6ac - e723 - 4fee - ad8a - 291119a6eca2 }\ D: / limit 2861056
#endif	
		}
		else
		{
			s64 limitSize = (value * 1024);
			ui.tableWidget->setItem(0, 3, new QTableWidgetItem(GetDiskSizeString(limitSize).c_str()));
#ifdef AVOCADO_DEBUG
			VDC_DEBUG("%s limit %lld\n", __FUNCTION__, limitSize);
#endif
		}		
	}
}
