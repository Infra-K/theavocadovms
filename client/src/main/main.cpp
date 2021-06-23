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
#include <QtWidgets/QApplication>
#include <QStyleFactory>
#include "server/factory.hpp"
#include "client/clientfactory.hpp"
#include <QPixmap>
#include <QSplashScreen>
#include <QtWidgets/QMainWindow>
#include <QTranslator>
#include <QTextCodec>
#include "vstyle.hpp"
#include "vsclogin.h"
#include "vtaskmgr.hpp"
#include <QFileDialog>

Q_DECLARE_METATYPE (std::string) 

void LoadLangZH(QApplication &a)
{
	QTranslator *translator = new QTranslator(&a);
	bool ok = translator->load("opencvr_ko.qm", QCoreApplication::applicationDirPath() + "/translations");
	if (ok)
	{
		qDebug("Translation Files loaded.");
		a.installTranslator(translator);
	}

	QTranslator *translatorVe = new QTranslator(&a);
	ok = translatorVe->load("veuilib_ko.qm", QCoreApplication::applicationDirPath() + "/translations");
	if (ok)
	{
		qDebug("Translation Files loaded.");
		a.installTranslator(translatorVe);
	}

	return ;
}

void LoadLangKO(QApplication &a)
{
	QTranslator *translator = new QTranslator(&a);
	/*
	QString text1 = QCoreApplication::applicationDirPath() + "/translations";
	//QString MainPath = QCoreApplication::applicationDirPath() + "\\translations";
	char str1[1024] = { 0, };
	qsnprintf(str1, sizeof(str1), "%s", text1.toUtf8().constData());
	
	printf("m_lang %s\n", str1);
	 */

	bool ok = translator->load("opencvr_ko.qm", QCoreApplication::applicationDirPath() + "/translations");
	if (ok)
	{
		qDebug("NVR Translation Files loaded.");
		a.installTranslator(translator);
	}

	QTranslator *translatorVe = new QTranslator(&a);
	ok = translatorVe->load("veuilib_ko.qm", QCoreApplication::applicationDirPath() + "/translations");
	if (ok)
	{
		qDebug("LIB Translation Files loaded.");
		a.installTranslator(translatorVe);
	}
	
	return;
}

int main(int argc, char *argv[])
{
	int dummy = errno;

	ClientFactory *pFactory = NULL;

	char *argv1[] = {"avocadomonitor", NULL};
	int argc1 = sizeof(argv1) / sizeof(char*) - 1;

	char *argv2[] = {"avocadomonitor", NULL};
    int argc2 = sizeof(argv2) / sizeof(char*) - 1;

	VidEnv env;
	env.init(argc2, argv2);
	env.run();
	
	QApplication a(argc1, argv1);
#if 0
	QFileDialog *fileDialog = new QFileDialog();

	if (fileDialog->exec() == QDialog::Accepted) {
	}
#endif
	a.setStyle(new VStyle);
	QString style = "dlg::title {" " height: 24px;" " font-weight: bold;" " color: #000000;" " background: #ffffff;" " }";
	// a.setStyleSheet("background:rgb(0,120,120)");
	a.setStyleSheet(style);

	astring strLoggerPath = env.GetAppConfPath("logs");
	Poco::File file1(strLoggerPath);
	file1.createDirectories();
	astring strLoggerFile = strLoggerPath + "avocadomonitor";
	Debug::init(9200, strLoggerFile);

	Debug::logger().info("avocadomonitor started");
	// Debug::logger().info("opencvrclient started {} {}", __LINE__, __FUNCTION__);

	QPixmap pixmap(":/logo/resources/splash.png");
	QSplashScreen *splash = new QSplashScreen(pixmap);
	qRegisterMetaType<QVector<int> >("QVector<int>");
	qRegisterMetaType<std::string>("std::string");

	splash->setStyleSheet(QStringLiteral("color : white;"));    
	splash->show();
	
    pFactory = new ClientFactory(env);
	pFactory->Init();

#ifdef WIN32
	QFont font;

	font.setPointSize(10);
	font.setFamily(("Poppins"));//°úÀÏ
	font.setBold(false);

	a.setFont(font);
#else
	QFont font;
	font.setPointSize(10);
	font.setFamily(("Arial"));
	font.setBold(false);

	a.setFont(font);
#endif
	VidLanguage m_lang;

	pFactory->GetLang(m_lang);
	printf("m_lang : %d\n", m_lang);
	if (m_lang == VID_LANG_AUTO)
	{
		if (QLocale::system().name() == "ko_KR")
		{
			LoadLangKO(a);
		}
	}
	else if (m_lang == VID_KO_KR)
	{
		LoadLangKO(a);
	}
	
	splash->showMessage(QObject::tr("\n\n\n\n\n\n\n\n\n\n                                                          Starting ..."));
	ve_sleep(3000);

	VTaskMgr *pVTaskMgr = new VTaskMgr();

	VSCMainWindows *w = new VSCMainWindows(*pFactory);
	
	// w.showMaximized();
	w->hide();

	// w.showFullScreen();
	splash->finish(w);

	/* Auto  */
	if (pFactory->GetAutoLogin() == false)
	{
		w->ShowLogin();
	}
	else
	{
		//if (pFactory->GetAutoFullScreen() == false)
		//{
		//	w->showMaximized();
		//}
		//else
		//{
		//	w->showMinimized();
		//}
		w->showNormal();
	}

	delete splash;
	splash = NULL;

	/* Start Stor client to connect VidStor */
	pFactory->GetStorFactory().Start();

	VDC_DEBUG("Start successfully !\n");
	
	//infrak
	w->show();

	return a.exec();
}
