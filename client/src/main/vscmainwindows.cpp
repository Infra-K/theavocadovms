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
#include "vscmainwindows.h"

#include <QAction>
#include <QLayout>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QTextEdit>
#include <QFile>
#include <QDataStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QSignalMapper>
#include <QApplication>
#include <QPainter>
#include <QMouseEvent>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <qdebug.h>
#include <QDockWidget>
#include <QToolBar>
#include <QTabWidget>
#include <QListWidget>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QApplication>
#include <QMenu>

#include "server/factory.hpp"
#include "vsctoolbar.h"
#include "vscloading.hpp"
#include "vscuserstatus.h"
#include "vsclogin.h"
#include "vscabout.h"

Q_DECLARE_METATYPE(QDockWidget::DockWidgetFeatures)

VSCMainWindows::VSCMainWindows(ClientFactory &pFactory, QWidget *parent)
 : m_pFactory(pFactory), QMainWindow(parent), m_VidIdx(VSC_VID_IDX_LAST), m_pMainView(NULL), m_pDashBoard(NULL), m_pEventConsole(NULL), m_pToolBar(NULL)
{
	ui.setupUi(this);
#ifdef WIN32
	setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
#endif
	// QMenu menu(this);
	// menu.setStyleSheet("background-color: rgb(42, 42, 42);");
	// QMenu * popupMenu = new QMenu(this);
	// QString menuStyle = "QMenu { font-size:18px; color:blue; background-color:qlineargradient(x1:0, y1:0, x2:0, y2:1, stop: 0 #cccccc, stop: 1 #555555);}";
	// popupMenu->setStyleSheet(menuStyle);
	m_pToolBar = new VSCToolBar(this);
	SetupToolBar();

	VSCLoading *loading = new VSCLoading(NULL);
	loading->show();
	QDesktopWidget *desktop = QApplication::desktop();
	QRect rect = desktop->screenGeometry(0);
	loading->setGeometry(rect.width()/2, rect.height()/2, 64, 64);
	QCoreApplication::processEvents();

	// QDockWidget *m_pDockDevicelist = new QDockWidget(tr("Devices"), this);
	m_pDockDevicelist = new QDockWidget(this);
	m_pDockDevicelist->setFeatures(QDockWidget::DockWidgetMovable);

	m_pDockDevicelist->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	// m_pDockDevicelist->setAllowedAreas(Qt::LeftDockWidgetArea);
	m_pDockDevicelist->setTitleBarWidget(new QWidget(this));

	m_pVidList = new VSCVidList(m_pDockDevicelist);
	m_pDockDevicelist->setWidget(m_pVidList);

	addDockWidget(Qt::LeftDockWidgetArea, m_pDockDevicelist);
	// addDockWidget(Qt::RightDockWidgetArea, m_pDockDevicelist);

	m_pMainArea = new QTabWidget(this);

	m_pMainArea->setTabsClosable(true);
	m_pMainArea->setMovable(true);

	setCentralWidget(m_pMainArea);

	connect(m_pMainArea, SIGNAL(tabCloseRequested(int)), this, SLOT(MainCloseTab(int)));
	
	/* Show the Live default */
	m_pVidLive = new VSCVidLive(m_pFactory, *m_pMainArea, this);
	m_pMainView = m_pVidLive->GetMainView();
	m_pVidConf = new VSCVidConf(m_pFactory, *m_pMainArea, this);
	m_pVidPb = new VSCVidSearchPB(m_pFactory, *m_pMainArea, this);

	/* First hide all */
	m_pToolBar->ui.pbVidEventSearch->hide();
	//infrak 모션 아이콘 감추기
	m_pToolBar->ui.pbVidMotionSearch->hide();
	m_pVidLive->VidHide();
	m_pVidConf->VidHide();
	m_pVidPb->VidHide();
		
	ShowVidLive(); // 좌측 창 화면

	delete loading;
	loading = NULL;

	setWindowTitle(QApplication::translate("VSCMainWindowsClass", VE_INFO, 0));

	connect(m_pToolBar->ui.pbFullScreen, SIGNAL(clicked()), this, SLOT(SetFullScreen()));
	// connect(m_pToolBar->ui.pbAbout, SIGNAL(clicked()), this, SLOT(ShowAbout()));
	connect(m_pToolBar->ui.pbAbout, SIGNAL(clicked()), this, SLOT(about()));
	connect(m_pToolBar->ui.pbUser, SIGNAL(clicked()), this, SLOT(UserStatus()));
	connect(m_pToolBar->ui.pbVidLive, SIGNAL(clicked()), this, SLOT(ShowVidLive()));
	connect(m_pToolBar->ui.pbVidLiveView, SIGNAL(clicked()), m_pVidLive, SLOT(SlotNewLiveView()));
	connect(m_pToolBar->ui.pbVidPb, SIGNAL(clicked()), m_pVidLive, SLOT(SlotNewLivePB()));
	connect(m_pToolBar->ui.pbVidSearch, SIGNAL(clicked()), this, SLOT(ShowVidPb()));
	connect(m_pToolBar->ui.pbVidEventSearch, SIGNAL(clicked()), m_pVidPb, SLOT(SlotNewEventSearch()));
#if 0
	//infrak
	connect(m_pToolBar->ui.pbVidMotionSearch, SIGNAL(clicked()), m_pVidPb, SLOT(SlotNewMotionSearch()));
#endif
	connect(m_pToolBar->ui.pbVidConf, SIGNAL(clicked()), this, SLOT(ShowVidConf()));
	connect(m_pToolBar->ui.pbVidDashBoard, SIGNAL(clicked()), this, SLOT(ShowDashBoard()));
	connect(m_pToolBar->ui.pbAlarm, SIGNAL(clicked()), this, SLOT(ShowEventConsole()));
	connect(&(m_pFactory.GetStorFactory()), SIGNAL(SignalEvent1()), m_pToolBar, SLOT(NewAlarm()));

	m_pEventConsole = new VSCEventConsole(m_pFactory, this);
	m_pEventConsole->hide();
	
	// http://stackoverflow.com/questions/650889/qtoolbar-is-there-a-way-to-make-toolbar-unhidable
	setContextMenuPolicy(Qt::NoContextMenu);

	VDC_DEBUG("%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
}

VSCMainWindows::~VSCMainWindows()
{

}

void VSCMainWindows::MainCloseTab(int index)
{
    QWidget *wdgt = m_pMainArea->widget(index);

	m_pMainArea->setCurrentWidget(wdgt);

    /* Frist view can not be closed */
    if (wdgt == m_pMainView)
    {
        return;
    }
	
    if (wdgt == m_pDashBoard)	
    {
		m_pMainArea->removeTab(index);
		m_pDashBoard->hide();
		return;
    }

	if (wdgt == m_pEventConsole)
	{
		m_pMainArea->removeTab(index);
		m_pEventConsole->hide();
		return;
	}

	if (m_pVidLive->CheckClosed(wdgt) == true || m_pVidConf->CheckClosed(wdgt) == true || m_pVidPb->CheckClosed(wdgt) == true)
	{
		m_pMainArea->removeTab(index);
		wdgt->hide();
		return;
	}
	
	m_pMainArea->removeTab(index);
	if (wdgt)
	{
		delete wdgt;
		wdgt = NULL;
	}
}

void VSCMainWindows::ShowDashBoard()
{
	if (m_pDashBoard == NULL)
	{
		m_pDashBoard = new VSCDashBoard(this);
		// connect(m_pPanel, SIGNAL(AddVIPC()), this, SLOT(AddVIPC()));
	}

	m_pMainArea->addTab(m_pDashBoard, QIcon(tr(":/action/resources/dashboard.png")), tr("DASH BOARD"));  
	m_pMainArea->setCurrentWidget(m_pDashBoard);

	VDC_DEBUG("%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
}

void VSCMainWindows::ShowEventConsole()
{
	if (m_pEventConsole == NULL)
	{
		m_pEventConsole = new VSCEventConsole(m_pFactory, this);
		// connect(m_pPanel, SIGNAL(AddVIPC()), this, SLOT(AddVIPC()));
	}
	m_pEventConsole->show();

	m_pMainArea->addTab(m_pEventConsole, QIcon(tr(":/action/resources/alarmno.png")), tr("CONSOLE")); 
	m_pMainArea->setCurrentWidget(m_pEventConsole);

	VDC_DEBUG("%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
}

void VSCMainWindows::ShowVidLive()
{
	switch (m_VidIdx)
	{
		case VSC_VID_IDX_LIVE:
		{
			return;
			break;
		}
		case VSC_VID_IDX_CONF:
		{
			m_pVidConf->VidHide();
			break;
		}
		case VSC_VID_IDX_PB:
		{
			m_pVidPb->VidHide();
			m_pToolBar->ui.pbVidEventSearch->hide();
#if 0
			//infrak
			m_pToolBar->ui.pbVidMotionSearch->hide();
#endif
			break;
		}
		default:
		{
			break;
		}
	}

	m_pVidLive->VidShow();
	m_pToolBar->ui.pbVidLiveView->show();
	m_pToolBar->ui.pbVidPb->show();
	m_VidIdx = VSC_VID_IDX_LIVE;
	m_pVidList->SetCurrVidInf(m_pVidLive);

	VDC_DEBUG("%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
}

void VSCMainWindows::ShowVidConf()
{
	VDC_DEBUG("%s %s %d m_VidIdx=%d\n", __FUNCTION__, __FILE__, __LINE__, m_VidIdx);

	switch (m_VidIdx)
	{
		case VSC_VID_IDX_LIVE:
		{
			m_pVidLive->VidHide();
			m_pToolBar->ui.pbVidLiveView->hide();
			m_pToolBar->ui.pbVidPb->hide();
			break;
		}
		case VSC_VID_IDX_CONF:
		{
			return;
			break;
		}
		case VSC_VID_IDX_PB:
		{
			m_pToolBar->ui.pbVidEventSearch->hide();
#if 0
			//infrak
			m_pToolBar->ui.pbVidMotionSearch->hide();
#endif
			m_pVidPb->VidHide();
			break;
		}
		default:
		{
			break;
		}
	}

	m_pVidConf->VidShow();
	m_VidIdx = VSC_VID_IDX_CONF;
	m_pVidList->SetCurrVidInf(m_pVidConf);
}

void VSCMainWindows::ShowVidPb()
{
	switch (m_VidIdx)
	{
		case VSC_VID_IDX_LIVE:
		{
			m_pVidLive->VidHide();
			m_pToolBar->ui.pbVidLiveView->hide();
			m_pToolBar->ui.pbVidPb->hide();
			break;
		}
		case VSC_VID_IDX_CONF:
		{
			m_pVidConf->VidHide();
			break;
		}
		case VSC_VID_IDX_PB:
		{
			return;
			break;
		}
		default:
		{
			break;
		}
	}

	m_pVidPb->VidShow();
	m_pToolBar->ui.pbVidEventSearch->show();
#if 0
	//infrak
	m_pToolBar->ui.pbVidMotionSearch->show();
#endif
	m_VidIdx = VSC_VID_IDX_PB;
	m_pVidList->SetCurrVidInf(m_pVidPb);

	VDC_DEBUG("%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
}

void VSCMainWindows::ViewHideFocus()
{
	// m_pView->ViewHideFocus();
}

void VSCMainWindows::closeEvent(QCloseEvent *event)
{
	ExitOpenCVR();
	event->ignore(); 
}

void VSCMainWindows::ExitOpenCVR()
{
	QMessageBox msgBox(this);
	// Set text
	msgBox.setText(tr("AVOCADOVMS EXIT ..."));

	// Set predefined icon, icon is show on left side of text.
	msgBox.setIconPixmap(QPixmap(":/logo/resources/vsc32.png"));

	msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

	// Set focus of ok button
	msgBox.setDefaultButton(QMessageBox::Ok);

	// execute message box. method exec() return the button value of cliecke button
	int ret = msgBox.exec();

    switch (ret) {
    case QMessageBox::Ok:
		QApplication::quit();
		exit(0);
		break;
    default:
		// should never be reached
		break;
    }

	VDC_DEBUG("%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
    return;
}

void VSCMainWindows::SetupToolBar()
{
    m_pQToolBar = new QToolBar(this);
    m_pQToolBar->addWidget(m_pToolBar);
	// m_pQToolBar->setStyleSheet("background-color: lightgreen; border: 3px solid green");
    addToolBar(Qt::TopToolBarArea, m_pQToolBar);
}

void VSCMainWindows::about()
{
	//infrak 접속 경로
	QMessageBox::about(this, tr("ABOUT AVOCADOVMS"),
			tr("<b>License & Pricing</b> <br>"
			"<a href=\"https://github.com/infrak/avocadovms\">https://github.com/infrak/avocadovms</a>"
			"  <br><a href=\"http://www.infra-k.net/\">http://www.infra-k.net/</a>"
			"<br>"));
}

void VSCMainWindows::UserStatus()
{
	VSCUserStatus userStatus;

	userStatus.show();

    QDesktopWidget *desktop = QApplication::desktop();
	QRect rect = desktop->screenGeometry(0);
	userStatus.setGeometry(rect.width()/2 -userStatus.width()/2, rect.height()/2 - userStatus.height()/2, userStatus.width(), userStatus.height());
	userStatus.exec();

	if (userStatus.IsQuit() == TRUE)
	{
		ShowLogin();
	}
}

void VSCMainWindows::ShowAbout()
{
	VSCAbout about;

	about.show();
    QDesktopWidget *desktop = QApplication::desktop();
	QRect rect = desktop->screenGeometry(0);
	about.setGeometry(rect.width()/2 -about.width()/2, rect.height()/2 - about.height()/2, about.width(), about.height());
	about.exec();
}

void VSCMainWindows::ShowLogin()
{
	VSCLogin login;

	hide();

again:
	login.SetDefault();
	login.show();

    QDesktopWidget *desktop = QApplication::desktop();
	QRect rect = desktop->screenGeometry(0);

	login.exec();
	if (login.GetIsLogin() == TRUE)
	{	
		astring strUser;
		astring strPasswd;

		login.GetUserPasswd(strUser, strPasswd);
		if (m_pFactory.AuthUser(strUser, strPasswd) == TRUE)
		{
			if (m_pFactory.GetAutoFullScreen() == false)
			{
				showMaximized();//최대화면
			}
			else
			{
				showMinimized();//감추는 것			
			}
			return;
		}
		QMessageBox msgBox(this);
		//Set text
		msgBox.setText(tr("Username or Password not correct ..."));
		//Set predefined icon, icon is show on left side of text.
		msgBox.setIconPixmap(QPixmap(":/logo/resources/vsc32.png"));

		msgBox.setStandardButtons(QMessageBox::Ok);

		//Set focus of ok button
		msgBox.setDefaultButton(QMessageBox::Ok);

		//execute message box. method exec() return the button value of cliecke button
		int ret = msgBox.exec();
	}
	else
	{
		ExitOpenCVR();
	}

	goto again;	
}

//http://www.qtforum.org/article/520/mac-os-x-full-screen.html
void VSCMainWindows::SetFullScreen()
{
    if(isFullScreen()) {
        this->setWindowState(Qt::WindowMaximized);
    } 
	else {
#ifdef __APPLE__
		this->setWindowState(Qt::WindowMaximized);
#else
		this->setWindowState(Qt::WindowFullScreen);
#endif
    }
}

