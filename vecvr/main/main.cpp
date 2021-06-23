/** <!--
 *
 *  Copyright (C) 2017 veyesys support@veyesys.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published by the Free Software Foundation,
 *  either version 3 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License along with this program.
 *  If not, see <http://www.gnu.org/licenses/>.
 *
 *  If you would like this software to be made available to you under an 
 *  alternate commercial license please email support@veyesys.com for more information.
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
#include <QtWidgets/QApplication>
#include <QStyleFactory>
#include "server/factory.hpp"
#include <QPixmap>
#include <QSplashScreen>
#include <QtWidgets/QMainWindow>
#include <QTranslator>
#include <QTextCodec>
#include "vevent.hpp"
#include "webserver.hpp"
#include "vrtspserver.hpp"
#include "server/eventserver.hpp"
#include "vonvifdismgr.hpp"

Factory *gFactory = NULL;
astring gAppdir;

#include <Poco/AutoPtr.h>
#include <Poco/Util/SystemConfiguration.h>
#include <Poco/Format.h>
#include <Poco/Util/Application.h>

/* The media server only support linux & macOS */
#ifndef _WIN32
#include "mediaserver.hpp"
#endif

static BOOL WebServerUserChangeNotify(void* pParam, astring strUser, astring strPasswd)
{
	astring strPasswdPath = gAppdir + ".htpasswd";
	SimpleCrypt crypt;
	QString strDePasswd = strPasswd.c_str();

	//단순 로컬 파일에 내용저장
	mg_modify_passwords_file(strPasswdPath.c_str(), 
								"infra-k.net", 
								strUser.c_str(),
								crypt.decryptToString(strDePasswd).toStdString().c_str());

	return TRUE;
}

int main(int argc, char *argv[])
{
	Factory *pFactory = NULL;
	char *argv1[] = {"AvocadoStorage", "-platform", "offscreen", NULL};
	int argc1 = sizeof(argv1) / sizeof(char*) - 1;
    char *argv2[] = {"AvocadoStorage", NULL};
    int argc2 = sizeof(argv2) / sizeof(char*) - 1;
	VidEnv env;

	env.init(argc2, argv2);
	env.run();

	astring strLoggerPath = env.GetAppConfPath("logs");

	Poco::File file1(strLoggerPath);
	file1.createDirectories();
	astring strLoggerFile = strLoggerPath + "AvocadoStoragelog";
	Debug::init(9100, strLoggerFile);

	Debug::logger().info("AvocadoStorage started");
	//Debug::logger().info("vidstor started {} {}", __LINE__, __FUNCTION__);

	pFactory = new Factory(env);//카메라, 디스크, 검색을 수행
	gFactory = pFactory;

	pFactory->Init();
	QApplication a(argc1, argv1);

#if 0
	astring strIdTest = "c9d03a04-6ff0-4733-a5bc-6be41876f080";
	VidCamera camTest;
	pFactory->GetConfDB().GetCameraConf(strIdTest, camTest);
#endif
	
	gAppdir = env.GetAppDir();
	
	astring docRoot = env.GetAppDir() + "www";
	astring strPasswdPath = env.GetAppDir() + ".htpasswd";

	/* set htpasswd, when start a  */
	astring strPasswd = "admin";
	pFactory->GetAdminPasswd(strPasswd);
	WebServerUserChangeNotify(NULL, "admin", strPasswd);//로컬 파일에 정보 저장
	
	const char *options[] = {
							"document_root", docRoot.c_str(), 
							"listening_ports", PORT, 
							"global_auth_file", strPasswdPath.c_str(),
							"authentication_domain", "infra-k.net",
							0
							};
    
	std::vector<std::string> cpp_options;
	for (int i=0; i<(sizeof(options)/sizeof(options[0])-1); i++) {
	    cpp_options.push_back(options[i]);

		Debug::logger().info(options[i]);

		VDC_DEBUG("%s %s %d : %s", __FUNCTION__, __FILE__, __LINE__, options[i]);
	}
	
	/* Start RTSP server */
	VRTSPServer *pRTSPServer = new VRTSPServer(*pFactory); // 9554 포트

	/* Init Event Server */
	VEventServer *pEventServer = new VEventServer(*pFactory); // 9090 포트 이벤트 서버
	pEventServer->Init();

	/* Init Event framework */
	VEventMgr *pEventMgr = new VEventMgr(*pFactory, *pEventServer);
	pEventMgr->Init();

	VEWebServer *pWebServer = new VEWebServer(cpp_options, *pFactory, *pEventServer); // 8090 포트
	
	pFactory->RegUserChangeNotify(pWebServer, WebServerUserChangeNotify);
	pFactory->start();
	
#ifndef _WIN32
	VDC_DEBUG("Init media server 20210505 !\n");
	/* Init media server */
	VEMediaServer::InitMediaServer();
	/* Start Media server */
	VEMediaServer * pMediaServer = new VEMediaServer(*pFactory);
#endif

#if 0
	VONVIFDisMgr *pDisMgr = new VONVIFDisMgr();
	
	QList<QHostAddress> hostAddr = DeviceSearcher::getHostAddress();

	QList<QHostAddress>::iterator i;
	for(i=hostAddr.begin(); i!=hostAddr.end(); i++)
	{
		pDisMgr->AddHost(((*i).toString()).toStdString(), "9081", "Network_Video_Storage");
	}
#endif

	VDC_DEBUG("Start successfully 20210505 !\n");
	
	return a.exec();
}
