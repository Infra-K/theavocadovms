/** <!--
 *
 *  Copyright (C) 2021 infrak infrak@naver.com
 *  <http://infra-k.net>
 *
 * -->
 */
#include "client/storwsclient.hpp"

StorWebSocketClient::StorWebSocketClient(std::string strHost, std::string strPort, std::string strPath)
											:m_wsConn(NULL), m_bConnected(false), m_strHost(strHost), m_strPort(strPort),
											m_strPath(strPath), m_msgId(0), m_bOnline(false), m_pThread(NULL), m_bExit(false), 
											m_bLogined(false)
{
	memset(m_ebuf, 0, WS_EBUF_LEN);
}

StorWebSocketClient::~StorWebSocketClient()
{
	Disconnect();
}

bool StorWebSocketClient::StartKeepThread()
{
	/* start keepalive thread */
	m_pThread = new std::thread(StorWebSocketClient::Thread, this);
		
	return true;
}

bool StorWebSocketClient::Disconnect()
{
	//infrak
	// std::lock_guard<std::mutex> guard(m_ConnectLock);
	if (m_bExit == false)
	{
		m_bExit = true;
		if (m_pThread)
		{
			m_pThread->join();
			delete m_pThread;
			m_pThread = NULL;
		}

		if (m_bConnected == true)
		{
			if (m_wsConn)
			{
				// 함수 내부에서 메모리 에러로 정지할때 있음
				mg_close_connection(m_wsConn);
				m_wsConn = NULL;
			}
		}
	}
#ifdef AVOCADO_DEBUG
	R_LOG(logRINFO, "%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif

	return true;
}

bool StorWebSocketClient::SendDeviceListRequest()
{
	if (Connected() == false)
	{
		if (Connect() == false)
		{
		#ifdef AVOCADO_DEBUG
			R_LOG(logRINFO, "ERROR: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
		#endif
			return false;
		}
	}

	Link::LinkCmd cmd;
	std::string strMsg;

	cmd.set_type(Link::LINK_CMD_CAM_LIST_REQ);
	LinkListCamReq * req = new LinkListCamReq;
	req->set_ball(true);
	
	cmd.set_allocated_camlistreq(req);
	::google::protobuf::util::Status status = ::google::protobuf::util::MessageToJsonString(cmd, &strMsg);
	if (!status.ok())
	{
#ifdef AVOCADO_DEBUG
		R_LOG(logRINFO, "ERROR: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
		return false;
	}

	if (SendMsg(strMsg) == false)
	{
#ifdef AVOCADO_DEBUG
		R_LOG(logRINFO, "ERROR: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
		return false;
	}
	//R_LOG(logRINFO, "%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);

	return true;
}

bool StorWebSocketClient::SendRegNotifyRequest()
{
	if (Connected() == false)
	{
		if (Connect() == false)
		{
#ifdef AVOCADO_DEBUG
			R_LOG(logRINFO, "ERROR: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif			
			return false;
		}
	}

	Link::LinkCmd cmd;
	std::string strMsg;

	cmd.set_type(Link::LINK_CMD_REG_NOTIFY_REQ);
	LinkRegNotifyReq * req = new LinkRegNotifyReq;	
	cmd.set_allocated_regnotifyreq(req);

	::google::protobuf::util::Status status = ::google::protobuf::util::MessageToJsonString(cmd, &strMsg);
	if (!status.ok())
	{
#ifdef AVOCADO_DEBUG
		R_LOG(logRINFO, "ERROR: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
		return false;
	}

	if (SendMsg(strMsg) == false)
	{
#ifdef AVOCADO_DEBUG
		R_LOG(logRINFO, "ERROR: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
		return false;
	}

	return true;
}

bool StorWebSocketClient::RegRealEvent()
{
	return true;
}

bool StorWebSocketClient::UnRegRealEvent()
{
	return true;
}

bool StorWebSocketClient::ProcessLoginResp(Link::LinkCmd &cmd, astring strUser, astring strPasswd)
{
	// R_LOG(logRINFO, "%s %s %d User=%s\n", __FUNCTION__, __FILE__, __LINE__, strUser.c_str());
	if (!cmd.has_loginresp())
	{		
#ifdef AVOCADO_DEBUG
		R_LOG(logRINFO, "ERROR: %s %s %d User=%s\n", __FUNCTION__, __FILE__, __LINE__, strUser.c_str());
#endif
		return false;
	}
	const LinkLoginResp &pResp = cmd.loginresp();

	/* Password error need relogin */
	if (pResp.bretnonce() == true)
	{
		//infrak
#ifdef AVOCADO_DEBUG
		R_LOG(logRINFO, "%s %s %d Password error need relogin User=%s\n", __FUNCTION__, __FILE__, __LINE__, strUser.c_str());
#endif
		return Login(strUser, strPasswd, pResp.strnonce());
	}

	if (pResp.bret() == true)
	{
		m_bLogined = true;
		ProcessLogined();
#ifdef AVOCADO_DEBUG
		R_LOG(logRINFO, "%s %s %d ProcessLogined User=%s\n", __FUNCTION__, __FILE__, __LINE__, strUser.c_str());
#endif
	}

	if (pResp.bret() == true && IsKeep() == true)
	{
		SendDeviceListRequest();
		SendRegNotifyRequest();
		RegRealEvent();
#ifdef AVOCADO_DEBUG
		R_LOG(logRINFO, "%s %s %d SendDeviceListRequest/SendRegNotifyRequest/RegRealEvent User=%s\n", __FUNCTION__, __FILE__, __LINE__, strUser.c_str());
#endif
	}

	return true;
}

bool StorWebSocketClient::Login(astring strUser, astring strPasswd, astring strNonce)
{
	// R_LOG(logRINFO, "%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
	if (Connected() == false)
	{
		if (Connect() == false)
		{
#ifdef AVOCADO_DEBUG
			R_LOG(logRINFO, "ERROR: %s %s %d User=%s\n", __FUNCTION__, __FILE__, __LINE__, strUser.c_str());
#endif
			return false;
		}
	}

	Link::LinkCmd cmd;
	XMD5 md5Check;
	std::string strMsg;

	cmd.set_type(Link::LINK_CMD_LOGIN_REQ);
	LinkLoginReq * req = new LinkLoginReq;
	req->set_strusername(strUser);

	std::string pass = strNonce + strPasswd;

	md5Check.Update((const uint8_t *)(pass.c_str()), pass.length());
	md5Check.Finalize();
	std::string md5Output = md5Check.GetAsString().c_str();
	req->set_strpasswd(md5Output);
	
	cmd.set_allocated_loginreq(req);
	::google::protobuf::util::Status status = ::google::protobuf::util::MessageToJsonString(cmd, &strMsg);
	if (!status.ok())
	{
#ifdef AVOCADO_DEBUG
		R_LOG(logRINFO, "ERROR: %s %s %d User=%s\n", __FUNCTION__, __FILE__, __LINE__, strUser.c_str());
#endif
		return false;
	}

	// long long lastMsgId = 0;	
	if (SendMsg(strMsg) == false)
	{
#ifdef AVOCADO_DEBUG
		R_LOG(logRINFO, "ERROR: %s %s %d User=%s\n", __FUNCTION__, __FILE__, __LINE__, strUser.c_str());
#endif
		return false;
	}
	
	return true;	
}

int StorWebSocketClient::WSDataHandler(struct mg_connection *conn,
														  int flags,
														  char *data,
														  size_t data_len,
														  void *user_data)
{
	// R_LOG(logRINFO, "%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
	StorWebSocketClient *pWS = (StorWebSocketClient *)user_data;
	if (pWS)
	{
	    return pWS->WSDataHandler1(conn, flags, data, data_len);
	}

	return true;
}

/* handle the data */
int StorWebSocketClient::WSDataHandler1(struct mg_connection *conn,
														  int flags,
														  char *data,
														  size_t data_len)
{
	// R_LOG(logRINFO,"%s %s %d %d\n",__FUNCTION__, __FILE__, __LINE__, data_len);
	return ProcessRecvMsg(data, data_len);
}

void StorWebSocketClient::WSCloseHandler(struct mg_connection *conn, void *user_data)
{	
	StorWebSocketClient *pWS = (StorWebSocketClient *)user_data;
	if (pWS)
	{
#ifdef AVOCADO_DEBUG
		R_LOG(logRINFO, "%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
	    return pWS->WSCloseHandler1(conn);
	}

#ifdef AVOCADO_DEBUG
	R_LOG(logRINFO, "ERROR: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif	
	return;
}

/* handle close */
void StorWebSocketClient::WSCloseHandler1(struct mg_connection *conn)
{
#ifdef AVOCADO_DEBUG
	R_LOG(logRINFO, "%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
	ProcessOffline();

	std::lock_guard<std::mutex> guard(m_ConnectLock);

	m_bConnected = false;
	m_bLogined = false;
}

bool StorWebSocketClient::Connect()
{
	if (m_bConnected == true)
	{
		return m_bConnected;
	}
	// 끊어지면 매2초마다 접속시도
	// R_LOG(logRINFO, "request: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);

	std::lock_guard<std::mutex> guard(m_ConnectLock);

	if (m_wsConn)
	{
		mg_close_connection(m_wsConn);
#ifdef AVOCADO_DEBUG
		R_LOG(logRINFO, "%s %s %d mg_close_connection()\n", __FUNCTION__, __FILE__, __LINE__);
#endif
		m_wsConn = NULL;
	}
	m_wsConn = mg_connect_websocket_client(m_strHost.c_str(),
										   atoi(m_strPort.c_str()),
										   0,
										   m_ebuf,
										   sizeof(m_ebuf),
										   m_strPath.c_str(),
										   NULL,
										   (mg_websocket_data_handler)StorWebSocketClient::WSDataHandler,
										   (mg_websocket_close_handler)StorWebSocketClient::WSCloseHandler,
										   this);
	if (m_wsConn != NULL)
	{
#ifdef AVOCADO_DEBUG
		R_LOG(logRINFO, "%s %s %d mg_connect_websocket_client success\n", __FUNCTION__, __FILE__, __LINE__);
#endif
		m_bConnected = true;
	}
	// client_res = mg_get_response(m_wsConn, m_ebuf, sizeof(m_ebuf), 10000);
	// ck_assert_int_ge(client_res, 0);

	return m_bConnected;
}

bool StorWebSocketClient::SendMsg(std::string &strMsg)
{
	std::lock_guard<std::mutex> guard(m_ConnectLock); // infrak

	if (m_bConnected == false)
	{
#ifdef AVOCADO_DEBUG
		R_LOG(logRINFO, "ERROR: %s %s %d m_bConnected(false) SendMsg=%s\n", __FUNCTION__, __FILE__, __LINE__, strMsg.c_str());
#endif
		return false;
	}
	
	int ret = mg_websocket_client_write(m_wsConn, WEBSOCKET_OPCODE_TEXT, strMsg.c_str(), strMsg.length());
	if (ret == strMsg.length())
	{
#ifdef AVOCADO_DEBUG
		if (strstr(strMsg.c_str(), "Keepalive") == NULL && strstr(strMsg.c_str(), "LINK_CMD_HAS_RECORD_REQ") == NULL) // Keepalive 로그만 생략함
			R_LOG(logRINFO, "%s %s %d SendMsg=%s, (%d:%d)\n", __FUNCTION__, __FILE__, __LINE__, strMsg.c_str(), strMsg.length(), ret);
#endif
		return true;
	}

#ifdef AVOCADO_DEBUG
	if (strMsg.length())
		R_LOG(logRINFO, "ERROR: %s %s %d strMsg=%s, (%d:%d)\n", __FUNCTION__, __FILE__, __LINE__, strMsg.c_str(), strMsg.length(), ret);
#endif
	return false;
}

bool StorWebSocketClient::Connected()
{
	// 매 초마다
	// R_LOG(logRINFO, "%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
	std::lock_guard<std::mutex> guard(m_ConnectLock);

	return m_bConnected;
}

bool StorWebSocketClient::Thread(void* pData)
{
	StorWebSocketClient *pThread = (StorWebSocketClient *)pData;
	if (pThread)
	{
		//R_LOG(logRINFO, "%s %s %d, id=%ld\n", __FUNCTION__, __FILE__, __LINE__, pThread->m_pThread->get_id());
		return pThread->ThreadRun();
	}

#ifdef AVOCADO_DEBUG
	R_LOG(logRINFO, "ERROR: %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
	return false;
}

bool StorWebSocketClient::ThreadRun()
{
#ifdef AVOCADO_DEBUG
	R_LOG(logRINFO, "%s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
	while(m_bExit == false)
	{		
		/* loop to send keepalive & reconnect */
		{
			if (Connected() == false) // 연결되어 있지 않을 확인함
			{
				if (Connect() == true) // 연결되어 있는 상태는 true
				{
					// R_LOG(logRINFO, "ProcessOnline %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
					ProcessOnline();
				}
			}
			Link::LinkCmd cmd;
			std::string strMsg;

			cmd.set_type(Link::LINK_CMD_KEEPALIVE_REQ);
			LinkKeepaliveReq *req = new LinkKeepaliveReq;
			req->set_nkeepalive(100);
			cmd.set_allocated_keepalivereq(req);
			
			// 정보 가져오기
			::google::protobuf::util::Status status = ::google::protobuf::util::MessageToJsonString(cmd, &strMsg);
			if (!status.ok())
			{
#ifdef AVOCADO_DEBUG
				R_LOG(logRINFO, "ThreadRun status.ok %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
				continue;
			}
			// 살아있는지 보내는 것
			SendMsg(strMsg);
		}

		int i = 40;
		while(m_bExit == false && i > 0)
		{
			i --;
			ve_sleep(50);
		}

		// 접속 확인
		if (NeedReconnect() == true)
		{
			// std::lock_guard<std::mutex> guard(m_ConnectLock);
			if (m_bConnected == true)
			{
				if (m_wsConn)
				{
#ifdef AVOCADO_DEBUG
					R_LOG(logRINFO, "mg_close_connection %s %s %d\n", __FUNCTION__, __FILE__, __LINE__);
#endif
					mg_close_connection(m_wsConn);
					m_wsConn = NULL;
				}
				m_bConnected = false;
			}
		}
	}

	return true;
}

