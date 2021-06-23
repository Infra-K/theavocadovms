/* soapReceiverBindingService.cpp
   Generated by gSOAP 2.7.17 from onvif.h
   Copyright(C) 2000-2010, Robert van Engelen, Genivia Inc. All Rights Reserved.
   This part of the software is released under one of the following licenses:
   GPL, the gSOAP public license, or Genivia's license for commercial use.
*/

#include "soapReceiverBindingService.h"

ReceiverBindingService::ReceiverBindingService()
{	ReceiverBindingService_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
}

ReceiverBindingService::ReceiverBindingService(const struct soap &_soap) :soap(_soap)
{ }

ReceiverBindingService::ReceiverBindingService(soap_mode iomode)
{	ReceiverBindingService_init(iomode, iomode);
}

ReceiverBindingService::ReceiverBindingService(soap_mode imode, soap_mode omode)
{	ReceiverBindingService_init(imode, omode);
}

ReceiverBindingService::~ReceiverBindingService()
{ }

void ReceiverBindingService::ReceiverBindingService_init(soap_mode imode, soap_mode omode)
{	soap_imode(this, imode);
	soap_omode(this, omode);
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
};

void ReceiverBindingService::destroy()
{	soap_destroy(this);
	soap_end(this);
}

ReceiverBindingService *ReceiverBindingService::copy()
{	ReceiverBindingService *dup = SOAP_NEW_COPY(ReceiverBindingService(*(struct soap*)this));
	return dup;
}

int ReceiverBindingService::soap_close_socket()
{	return soap_closesock(this);
}

int ReceiverBindingService::soap_senderfault(const char *string, const char *detailXML)
{	return ::soap_sender_fault(this, string, detailXML);
}

int ReceiverBindingService::soap_senderfault(const char *subcodeQName, const char *string, const char *detailXML)
{	return ::soap_sender_fault_subcode(this, subcodeQName, string, detailXML);
}

int ReceiverBindingService::soap_receiverfault(const char *string, const char *detailXML)
{	return ::soap_receiver_fault(this, string, detailXML);
}

int ReceiverBindingService::soap_receiverfault(const char *subcodeQName, const char *string, const char *detailXML)
{	return ::soap_receiver_fault_subcode(this, subcodeQName, string, detailXML);
}

void ReceiverBindingService::soap_print_fault(FILE *fd)
{	::soap_print_fault(this, fd);
}

#ifndef WITH_LEAN
void ReceiverBindingService::soap_stream_fault(std::ostream& os)
{	::soap_stream_fault(this, os);
}

char *ReceiverBindingService::soap_sprint_fault(char *buf, size_t len)
{	return ::soap_sprint_fault(this, buf, len);
}
#endif

void ReceiverBindingService::soap_noheader()
{	header = NULL;
}

void ReceiverBindingService::soap_header(char *wsa__MessageID, struct wsa__Relationship *wsa__RelatesTo, struct wsa__EndpointReferenceType *wsa__From, struct wsa__EndpointReferenceType *wsa__ReplyTo, struct wsa__EndpointReferenceType *wsa__FaultTo, char *wsa__To, char *wsa__Action, struct _wsse__Security *wsse__Security, char *wsa5__MessageID, struct wsa5__RelatesToType *wsa5__RelatesTo, struct wsa5__EndpointReferenceType *wsa5__From, struct wsa5__EndpointReferenceType *wsa5__ReplyTo, struct wsa5__EndpointReferenceType *wsa5__FaultTo, char *wsa5__To, char *wsa5__Action)
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

const SOAP_ENV__Header *ReceiverBindingService::soap_header()
{	return this->header;
}

int ReceiverBindingService::run(int port)
{	if (soap_valid_socket(bind(NULL, port, 100)))
	{	for (;;)
		{	if (!soap_valid_socket(accept()))
				return this->error;
			(void)serve();
			soap_destroy(this);
			soap_end(this);
		}
	}
	else
		return this->error;
	return SOAP_OK;
}

SOAP_SOCKET ReceiverBindingService::bind(const char *host, int port, int backlog)
{	return soap_bind(this, host, port, backlog);
}

SOAP_SOCKET ReceiverBindingService::accept()
{	return soap_accept(this);
}

int ReceiverBindingService::serve()
{
#ifndef WITH_FASTCGI
	unsigned int k = this->max_keep_alive;
#endif
	do
	{	soap_begin(this);
#ifdef WITH_FASTCGI
		if (FCGI_Accept() < 0)
		{
			this->error = SOAP_EOF;
			return soap_send_fault(this);
		}
#endif

		soap_begin(this);

#ifndef WITH_FASTCGI
		if (this->max_keep_alive > 0 && !--k)
			this->keep_alive = 0;
#endif

		if (soap_begin_recv(this))
		{	if (this->error < SOAP_STOP)
			{
#ifdef WITH_FASTCGI
				soap_send_fault(this);
#else 
				return soap_send_fault(this);
#endif
			}
			soap_closesock(this);

			continue;
		}

		if (soap_envelope_begin_in(this)
		 || soap_recv_header(this)
		 || soap_body_begin_in(this)
		 || dispatch() || (this->fserveloop && this->fserveloop(this)))
		{
#ifdef WITH_FASTCGI
			soap_send_fault(this);
#else
			return soap_send_fault(this);
#endif
		}

#ifdef WITH_FASTCGI
		soap_destroy(this);
		soap_end(this);
	} while (1);
#else
	} while (this->keep_alive);
#endif
	return SOAP_OK;
}

static int serve___trv__GetServiceCapabilities(ReceiverBindingService*);
static int serve___trv__GetReceivers(ReceiverBindingService*);
static int serve___trv__GetReceiver(ReceiverBindingService*);
static int serve___trv__CreateReceiver(ReceiverBindingService*);
static int serve___trv__DeleteReceiver(ReceiverBindingService*);
static int serve___trv__ConfigureReceiver(ReceiverBindingService*);
static int serve___trv__SetReceiverMode(ReceiverBindingService*);
static int serve___trv__GetReceiverState(ReceiverBindingService*);

int ReceiverBindingService::dispatch()
{	soap_peek_element(this);
	if (!soap_match_tag(this, this->tag, "trv:GetServiceCapabilities"))
		return serve___trv__GetServiceCapabilities(this);
	if (!soap_match_tag(this, this->tag, "trv:GetReceivers"))
		return serve___trv__GetReceivers(this);
	if (!soap_match_tag(this, this->tag, "trv:GetReceiver"))
		return serve___trv__GetReceiver(this);
	if (!soap_match_tag(this, this->tag, "trv:CreateReceiver"))
		return serve___trv__CreateReceiver(this);
	if (!soap_match_tag(this, this->tag, "trv:DeleteReceiver"))
		return serve___trv__DeleteReceiver(this);
	if (!soap_match_tag(this, this->tag, "trv:ConfigureReceiver"))
		return serve___trv__ConfigureReceiver(this);
	if (!soap_match_tag(this, this->tag, "trv:SetReceiverMode"))
		return serve___trv__SetReceiverMode(this);
	if (!soap_match_tag(this, this->tag, "trv:GetReceiverState"))
		return serve___trv__GetReceiverState(this);
	return this->error = SOAP_NO_METHOD;
}

static int serve___trv__GetServiceCapabilities(ReceiverBindingService *soap)
{	struct __trv__GetServiceCapabilities soap_tmp___trv__GetServiceCapabilities;
	_trv__GetServiceCapabilitiesResponse trv__GetServiceCapabilitiesResponse;
	trv__GetServiceCapabilitiesResponse.soap_default(soap);
	soap_default___trv__GetServiceCapabilities(soap, &soap_tmp___trv__GetServiceCapabilities);
	soap->encodingStyle = NULL;
	if (!soap_get___trv__GetServiceCapabilities(soap, &soap_tmp___trv__GetServiceCapabilities, "-trv:GetServiceCapabilities", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->GetServiceCapabilities(soap_tmp___trv__GetServiceCapabilities.trv__GetServiceCapabilities, &trv__GetServiceCapabilitiesResponse);
	if (soap->error)
		return soap->error;
	soap_serializeheader(soap);
	trv__GetServiceCapabilitiesResponse.soap_serialize(soap);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || trv__GetServiceCapabilitiesResponse.soap_put(soap, "trv:GetServiceCapabilitiesResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || trv__GetServiceCapabilitiesResponse.soap_put(soap, "trv:GetServiceCapabilitiesResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve___trv__GetReceivers(ReceiverBindingService *soap)
{	struct __trv__GetReceivers soap_tmp___trv__GetReceivers;
	_trv__GetReceiversResponse trv__GetReceiversResponse;
	trv__GetReceiversResponse.soap_default(soap);
	soap_default___trv__GetReceivers(soap, &soap_tmp___trv__GetReceivers);
	soap->encodingStyle = NULL;
	if (!soap_get___trv__GetReceivers(soap, &soap_tmp___trv__GetReceivers, "-trv:GetReceivers", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->GetReceivers(soap_tmp___trv__GetReceivers.trv__GetReceivers, &trv__GetReceiversResponse);
	if (soap->error)
		return soap->error;
	soap_serializeheader(soap);
	trv__GetReceiversResponse.soap_serialize(soap);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || trv__GetReceiversResponse.soap_put(soap, "trv:GetReceiversResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || trv__GetReceiversResponse.soap_put(soap, "trv:GetReceiversResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve___trv__GetReceiver(ReceiverBindingService *soap)
{	struct __trv__GetReceiver soap_tmp___trv__GetReceiver;
	_trv__GetReceiverResponse trv__GetReceiverResponse;
	trv__GetReceiverResponse.soap_default(soap);
	soap_default___trv__GetReceiver(soap, &soap_tmp___trv__GetReceiver);
	soap->encodingStyle = NULL;
	if (!soap_get___trv__GetReceiver(soap, &soap_tmp___trv__GetReceiver, "-trv:GetReceiver", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->GetReceiver(soap_tmp___trv__GetReceiver.trv__GetReceiver, &trv__GetReceiverResponse);
	if (soap->error)
		return soap->error;
	soap_serializeheader(soap);
	trv__GetReceiverResponse.soap_serialize(soap);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || trv__GetReceiverResponse.soap_put(soap, "trv:GetReceiverResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || trv__GetReceiverResponse.soap_put(soap, "trv:GetReceiverResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve___trv__CreateReceiver(ReceiverBindingService *soap)
{	struct __trv__CreateReceiver soap_tmp___trv__CreateReceiver;
	_trv__CreateReceiverResponse trv__CreateReceiverResponse;
	trv__CreateReceiverResponse.soap_default(soap);
	soap_default___trv__CreateReceiver(soap, &soap_tmp___trv__CreateReceiver);
	soap->encodingStyle = NULL;
	if (!soap_get___trv__CreateReceiver(soap, &soap_tmp___trv__CreateReceiver, "-trv:CreateReceiver", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->CreateReceiver(soap_tmp___trv__CreateReceiver.trv__CreateReceiver, &trv__CreateReceiverResponse);
	if (soap->error)
		return soap->error;
	soap_serializeheader(soap);
	trv__CreateReceiverResponse.soap_serialize(soap);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || trv__CreateReceiverResponse.soap_put(soap, "trv:CreateReceiverResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || trv__CreateReceiverResponse.soap_put(soap, "trv:CreateReceiverResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve___trv__DeleteReceiver(ReceiverBindingService *soap)
{	struct __trv__DeleteReceiver soap_tmp___trv__DeleteReceiver;
	_trv__DeleteReceiverResponse trv__DeleteReceiverResponse;
	trv__DeleteReceiverResponse.soap_default(soap);
	soap_default___trv__DeleteReceiver(soap, &soap_tmp___trv__DeleteReceiver);
	soap->encodingStyle = NULL;
	if (!soap_get___trv__DeleteReceiver(soap, &soap_tmp___trv__DeleteReceiver, "-trv:DeleteReceiver", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->DeleteReceiver(soap_tmp___trv__DeleteReceiver.trv__DeleteReceiver, &trv__DeleteReceiverResponse);
	if (soap->error)
		return soap->error;
	soap_serializeheader(soap);
	trv__DeleteReceiverResponse.soap_serialize(soap);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || trv__DeleteReceiverResponse.soap_put(soap, "trv:DeleteReceiverResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || trv__DeleteReceiverResponse.soap_put(soap, "trv:DeleteReceiverResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve___trv__ConfigureReceiver(ReceiverBindingService *soap)
{	struct __trv__ConfigureReceiver soap_tmp___trv__ConfigureReceiver;
	_trv__ConfigureReceiverResponse trv__ConfigureReceiverResponse;
	trv__ConfigureReceiverResponse.soap_default(soap);
	soap_default___trv__ConfigureReceiver(soap, &soap_tmp___trv__ConfigureReceiver);
	soap->encodingStyle = NULL;
	if (!soap_get___trv__ConfigureReceiver(soap, &soap_tmp___trv__ConfigureReceiver, "-trv:ConfigureReceiver", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->ConfigureReceiver(soap_tmp___trv__ConfigureReceiver.trv__ConfigureReceiver, &trv__ConfigureReceiverResponse);
	if (soap->error)
		return soap->error;
	soap_serializeheader(soap);
	trv__ConfigureReceiverResponse.soap_serialize(soap);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || trv__ConfigureReceiverResponse.soap_put(soap, "trv:ConfigureReceiverResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || trv__ConfigureReceiverResponse.soap_put(soap, "trv:ConfigureReceiverResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve___trv__SetReceiverMode(ReceiverBindingService *soap)
{	struct __trv__SetReceiverMode soap_tmp___trv__SetReceiverMode;
	_trv__SetReceiverModeResponse trv__SetReceiverModeResponse;
	trv__SetReceiverModeResponse.soap_default(soap);
	soap_default___trv__SetReceiverMode(soap, &soap_tmp___trv__SetReceiverMode);
	soap->encodingStyle = NULL;
	if (!soap_get___trv__SetReceiverMode(soap, &soap_tmp___trv__SetReceiverMode, "-trv:SetReceiverMode", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->SetReceiverMode(soap_tmp___trv__SetReceiverMode.trv__SetReceiverMode, &trv__SetReceiverModeResponse);
	if (soap->error)
		return soap->error;
	soap_serializeheader(soap);
	trv__SetReceiverModeResponse.soap_serialize(soap);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || trv__SetReceiverModeResponse.soap_put(soap, "trv:SetReceiverModeResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || trv__SetReceiverModeResponse.soap_put(soap, "trv:SetReceiverModeResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve___trv__GetReceiverState(ReceiverBindingService *soap)
{	struct __trv__GetReceiverState soap_tmp___trv__GetReceiverState;
	_trv__GetReceiverStateResponse trv__GetReceiverStateResponse;
	trv__GetReceiverStateResponse.soap_default(soap);
	soap_default___trv__GetReceiverState(soap, &soap_tmp___trv__GetReceiverState);
	soap->encodingStyle = NULL;
	if (!soap_get___trv__GetReceiverState(soap, &soap_tmp___trv__GetReceiverState, "-trv:GetReceiverState", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->GetReceiverState(soap_tmp___trv__GetReceiverState.trv__GetReceiverState, &trv__GetReceiverStateResponse);
	if (soap->error)
		return soap->error;
	soap_serializeheader(soap);
	trv__GetReceiverStateResponse.soap_serialize(soap);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || trv__GetReceiverStateResponse.soap_put(soap, "trv:GetReceiverStateResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || trv__GetReceiverStateResponse.soap_put(soap, "trv:GetReceiverStateResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}
/* End of server object code */
