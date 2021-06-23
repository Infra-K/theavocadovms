/* soapPausableSubscriptionManagerBindingProxy.cpp
   Generated by gSOAP 2.7.17 from onvif.h
   Copyright(C) 2000-2010, Robert van Engelen, Genivia Inc. All Rights Reserved.
   This part of the software is released under one of the following licenses:
   GPL, the gSOAP public license, or Genivia's license for commercial use.
*/

#include "soapPausableSubscriptionManagerBindingProxy.h"

PausableSubscriptionManagerBindingProxy::PausableSubscriptionManagerBindingProxy()
{	PausableSubscriptionManagerBindingProxy_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
}

PausableSubscriptionManagerBindingProxy::PausableSubscriptionManagerBindingProxy(const struct soap &_soap) : soap(_soap)
{ }

PausableSubscriptionManagerBindingProxy::PausableSubscriptionManagerBindingProxy(soap_mode iomode)
{	PausableSubscriptionManagerBindingProxy_init(iomode, iomode);
}

PausableSubscriptionManagerBindingProxy::PausableSubscriptionManagerBindingProxy(soap_mode imode, soap_mode omode)
{	PausableSubscriptionManagerBindingProxy_init(imode, omode);
}

void PausableSubscriptionManagerBindingProxy::PausableSubscriptionManagerBindingProxy_init(soap_mode imode, soap_mode omode)
{	soap_imode(this, imode);
	soap_omode(this, omode);
	soap_endpoint = NULL;
	static const struct Namespace namespaces[] =
{
	{"SOAP-ENV", "http://www.w3.org/2003/05/soap-envelope", "http://schemas.xmlsoap.org/soap/envelope/", NULL},
	{"SOAP-ENC", "http://www.w3.org/2003/05/soap-encoding", "http://schemas.xmlsoap.org/soap/encoding/", NULL},
	{"xsi", "http://www.w3.org/2001/XMLSchema-instance", "http://www.w3.org/*/XMLSchema-instance", NULL},
	{"xsd", "http://www.w3.org/2001/XMLSchema", "http://www.w3.org/*/XMLSchema", NULL},
	{"c14n", "http://www.w3.org/2001/10/xml-exc-c14n#", NULL, NULL},
	{"wsu", "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd", NULL, NULL},
	{"ds", "http://www.w3.org/2000/09/xmldsig#", NULL, NULL},
	{"wsse", "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd", "http://docs.oasis-open.org/wss/oasis-wss-wssecurity-secext-1.1.xsd", NULL},
	{"wsa5", "http://www.w3.org/2005/08/addressing", "http://schemas.xmlsoap.org/ws/2004/08/addressing", NULL},
	{"ns1", "http://schemas.xmlsoap.org/ws/2005/04/discovery", NULL, NULL},
	{"xmime", "http://tempuri.org/xmime.xsd", NULL, NULL},
	{"xop", "http://www.w3.org/2004/08/xop/include", NULL, NULL},
	{"tt", "http://www.onvif.org/ver10/schema", NULL, NULL},
	{"wsrfbf", "http://docs.oasis-open.org/wsrf/bf-2", NULL, NULL},
	{"wsa", "http://schemas.xmlsoap.org/ws/2004/08/addressing", NULL, NULL},
	{"wstop", "http://docs.oasis-open.org/wsn/t-1", NULL, NULL},
	{"wsrfr", "http://docs.oasis-open.org/wsrf/r-2", NULL, NULL},
	{"ns10", "http://www.onvif.org/ver10/network/wsdl/RemoteDiscoveryBinding", NULL, NULL},
	{"ns11", "http://www.onvif.org/ver10/network/wsdl/DiscoveryLookupBinding", NULL, NULL},
	{"tdn", "http://www.onvif.org/ver10/network/wsdl", NULL, NULL},
	{"ns2", "http://www.onvif.org/ver10/events/wsdl/PullPointSubscriptionBinding", NULL, NULL},
	{"ns3", "http://www.onvif.org/ver10/events/wsdl/EventBinding", NULL, NULL},
	{"tev", "http://www.onvif.org/ver10/events/wsdl", NULL, NULL},
	{"ns4", "http://www.onvif.org/ver10/events/wsdl/SubscriptionManagerBinding", NULL, NULL},
	{"ns5", "http://www.onvif.org/ver10/events/wsdl/NotificationProducerBinding", NULL, NULL},
	{"ns6", "http://www.onvif.org/ver10/events/wsdl/NotificationConsumerBinding", NULL, NULL},
	{"ns7", "http://www.onvif.org/ver10/events/wsdl/PullPointBinding", NULL, NULL},
	{"ns8", "http://www.onvif.org/ver10/events/wsdl/CreatePullPointBinding", NULL, NULL},
	{"ns9", "http://www.onvif.org/ver10/events/wsdl/PausableSubscriptionManagerBinding", NULL, NULL},
	{"wsnt", "http://docs.oasis-open.org/wsn/b-2", NULL, NULL},
	{"tds", "http://www.onvif.org/ver10/device/wsdl", NULL, NULL},
	{"timg", "http://www.onvif.org/ver20/imaging/wsdl", NULL, NULL},
	{"tls", "http://www.onvif.org/ver10/display/wsdl", NULL, NULL},
	{"tmd", "http://www.onvif.org/ver10/deviceIO/wsdl", NULL, NULL},
	{"tptz", "http://www.onvif.org/ver20/ptz/wsdl", NULL, NULL},
	{"trc", "http://www.onvif.org/ver10/recording/wsdl", NULL, NULL},
	{"trp", "http://www.onvif.org/ver10/replay/wsdl", NULL, NULL},
	{"trt", "http://www.onvif.org/ver10/media/wsdl", NULL, NULL},
	{"trv", "http://www.onvif.org/ver10/receiver/wsdl", NULL, NULL},
	{"tse", "http://www.onvif.org/ver10/search/wsdl", NULL, NULL},
	{NULL, NULL, NULL, NULL}
};
	this->namespaces = namespaces;
}

PausableSubscriptionManagerBindingProxy::~PausableSubscriptionManagerBindingProxy()
{ }

void PausableSubscriptionManagerBindingProxy::destroy()
{	soap_destroy(this);
	soap_end(this);
}

void PausableSubscriptionManagerBindingProxy::soap_noheader()
{	header = NULL;
}

void PausableSubscriptionManagerBindingProxy::soap_header(char *wsa__MessageID, struct wsa__Relationship *wsa__RelatesTo, struct wsa__EndpointReferenceType *wsa__From, struct wsa__EndpointReferenceType *wsa__ReplyTo, struct wsa__EndpointReferenceType *wsa__FaultTo, char *wsa__To, char *wsa__Action, struct _wsse__Security *wsse__Security, char *wsa5__MessageID, struct wsa5__RelatesToType *wsa5__RelatesTo, struct wsa5__EndpointReferenceType *wsa5__From, struct wsa5__EndpointReferenceType *wsa5__ReplyTo, struct wsa5__EndpointReferenceType *wsa5__FaultTo, char *wsa5__To, char *wsa5__Action)
{	::soap_header(this);
	this->header->wsa__MessageID = wsa__MessageID;
	this->header->wsa__RelatesTo = wsa__RelatesTo;
	this->header->wsa__From = wsa__From;
	this->header->wsa__ReplyTo = wsa__ReplyTo;
	this->header->wsa__FaultTo = wsa__FaultTo;
	this->header->wsa__To = wsa__To;
	this->header->wsa__Action = wsa__Action;
	this->header->wsse__Security = wsse__Security;
	this->header->wsa5__MessageID = wsa5__MessageID;
	this->header->wsa5__RelatesTo = wsa5__RelatesTo;
	this->header->wsa5__From = wsa5__From;
	this->header->wsa5__ReplyTo = wsa5__ReplyTo;
	this->header->wsa5__FaultTo = wsa5__FaultTo;
	this->header->wsa5__To = wsa5__To;
	this->header->wsa5__Action = wsa5__Action;
}

const SOAP_ENV__Header *PausableSubscriptionManagerBindingProxy::soap_header()
{	return this->header;
}

const SOAP_ENV__Fault *PausableSubscriptionManagerBindingProxy::soap_fault()
{	return this->fault;
}

const char *PausableSubscriptionManagerBindingProxy::soap_fault_string()
{	return *soap_faultstring(this);
}

const char *PausableSubscriptionManagerBindingProxy::soap_fault_detail()
{	return *soap_faultdetail(this);
}

int PausableSubscriptionManagerBindingProxy::soap_close_socket()
{	return soap_closesock(this);
}

void PausableSubscriptionManagerBindingProxy::soap_print_fault(FILE *fd)
{	::soap_print_fault(this, fd);
}

#ifndef WITH_LEAN
void PausableSubscriptionManagerBindingProxy::soap_stream_fault(std::ostream& os)
{	::soap_stream_fault(this, os);
}

char *PausableSubscriptionManagerBindingProxy::soap_sprint_fault(char *buf, size_t len)
{	return ::soap_sprint_fault(this, buf, len);
}
#endif

int PausableSubscriptionManagerBindingProxy::Renew(_wsnt__Renew *wsnt__Renew, _wsnt__RenewResponse *wsnt__RenewResponse)
{	struct soap *soap = this;
	struct __ns9__Renew soap_tmp___ns9__Renew;
	const char *soap_action = NULL;
	soap_action = "http://docs.oasis-open.org/wsn/bw-2/PausableSubscriptionManager/RenewRequest";
	soap->encodingStyle = NULL;
	soap_tmp___ns9__Renew.wsnt__Renew = wsnt__Renew;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___ns9__Renew(soap, &soap_tmp___ns9__Renew);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___ns9__Renew(soap, &soap_tmp___ns9__Renew, "-ns9:Renew", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put___ns9__Renew(soap, &soap_tmp___ns9__Renew, "-ns9:Renew", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!wsnt__RenewResponse)
		return soap_closesock(soap);
	wsnt__RenewResponse->soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	wsnt__RenewResponse->soap_get(soap, "wsnt:RenewResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

int PausableSubscriptionManagerBindingProxy::Unsubscribe(_wsnt__Unsubscribe *wsnt__Unsubscribe, _wsnt__UnsubscribeResponse *wsnt__UnsubscribeResponse)
{	struct soap *soap = this;
	struct __ns9__Unsubscribe soap_tmp___ns9__Unsubscribe;
	const char *soap_action = NULL;
	soap_action = "http://docs.oasis-open.org/wsn/bw-2/PausableSubscriptionManager/UnsubscribeRequest";
	soap->encodingStyle = NULL;
	soap_tmp___ns9__Unsubscribe.wsnt__Unsubscribe = wsnt__Unsubscribe;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___ns9__Unsubscribe(soap, &soap_tmp___ns9__Unsubscribe);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___ns9__Unsubscribe(soap, &soap_tmp___ns9__Unsubscribe, "-ns9:Unsubscribe", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put___ns9__Unsubscribe(soap, &soap_tmp___ns9__Unsubscribe, "-ns9:Unsubscribe", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!wsnt__UnsubscribeResponse)
		return soap_closesock(soap);
	wsnt__UnsubscribeResponse->soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	wsnt__UnsubscribeResponse->soap_get(soap, "wsnt:UnsubscribeResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

int PausableSubscriptionManagerBindingProxy::PauseSubscription(_wsnt__PauseSubscription *wsnt__PauseSubscription, _wsnt__PauseSubscriptionResponse *wsnt__PauseSubscriptionResponse)
{	struct soap *soap = this;
	struct __ns9__PauseSubscription soap_tmp___ns9__PauseSubscription;
	const char *soap_action = NULL;
	soap_action = "http://docs.oasis-open.org/wsn/bw-2/PausableSubscriptionManager/PauseSubscriptionRequest";
	soap->encodingStyle = NULL;
	soap_tmp___ns9__PauseSubscription.wsnt__PauseSubscription = wsnt__PauseSubscription;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___ns9__PauseSubscription(soap, &soap_tmp___ns9__PauseSubscription);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___ns9__PauseSubscription(soap, &soap_tmp___ns9__PauseSubscription, "-ns9:PauseSubscription", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put___ns9__PauseSubscription(soap, &soap_tmp___ns9__PauseSubscription, "-ns9:PauseSubscription", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!wsnt__PauseSubscriptionResponse)
		return soap_closesock(soap);
	wsnt__PauseSubscriptionResponse->soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	wsnt__PauseSubscriptionResponse->soap_get(soap, "wsnt:PauseSubscriptionResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

int PausableSubscriptionManagerBindingProxy::ResumeSubscription(_wsnt__ResumeSubscription *wsnt__ResumeSubscription, _wsnt__ResumeSubscriptionResponse *wsnt__ResumeSubscriptionResponse)
{	struct soap *soap = this;
	struct __ns9__ResumeSubscription soap_tmp___ns9__ResumeSubscription;
	const char *soap_action = NULL;
	soap_action = "http://docs.oasis-open.org/wsn/bw-2/PausableSubscriptionManager/ResumeSubscriptionRequest";
	soap->encodingStyle = NULL;
	soap_tmp___ns9__ResumeSubscription.wsnt__ResumeSubscription = wsnt__ResumeSubscription;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___ns9__ResumeSubscription(soap, &soap_tmp___ns9__ResumeSubscription);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___ns9__ResumeSubscription(soap, &soap_tmp___ns9__ResumeSubscription, "-ns9:ResumeSubscription", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put___ns9__ResumeSubscription(soap, &soap_tmp___ns9__ResumeSubscription, "-ns9:ResumeSubscription", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!wsnt__ResumeSubscriptionResponse)
		return soap_closesock(soap);
	wsnt__ResumeSubscriptionResponse->soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	wsnt__ResumeSubscriptionResponse->soap_get(soap, "wsnt:ResumeSubscriptionResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}
/* End of client proxy code */
