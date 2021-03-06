/* soapReplayBindingService.cpp
   Generated by gSOAP 2.7.17 from onvif.h
   Copyright(C) 2000-2010, Robert van Engelen, Genivia Inc. All Rights Reserved.
   This part of the software is released under one of the following licenses:
   GPL, the gSOAP public license, or Genivia's license for commercial use.
*/

#include "soapReplayBindingService.h"

ReplayBindingService::ReplayBindingService()
{	ReplayBindingService_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
}

ReplayBindingService::ReplayBindingService(const struct soap &_soap) :soap(_soap)
{ }

ReplayBindingService::ReplayBindingService(soap_mode iomode)
{	ReplayBindingService_init(iomode, iomode);
}

ReplayBindingService::ReplayBindingService(soap_mode imode, soap_mode omode)
{	ReplayBindingService_init(imode, omode);
}

ReplayBindingService::~ReplayBindingService()
{ }

void ReplayBindingService::ReplayBindingService_init(soap_mode imode, soap_mode omode)
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

void ReplayBindingService::destroy()
{	soap_destroy(this);
	soap_end(this);
}

ReplayBindingService *ReplayBindingService::copy()
{	ReplayBindingService *dup = SOAP_NEW_COPY(ReplayBindingService(*(struct soap*)this));
	return dup;
}

int ReplayBindingService::soap_close_socket()
{	return soap_closesock(this);
}

int ReplayBindingService::soap_senderfault(const char *string, const char *detailXML)
{	return ::soap_sender_fault(this, string, detailXML);
}

int ReplayBindingService::soap_senderfault(const char *subcodeQName, const char *string, const char *detailXML)
{	return ::soap_sender_fault_subcode(this, subcodeQName, string, detailXML);
}

int ReplayBindingService::soap_receiverfault(const char *string, const char *detailXML)
{	return ::soap_receiver_fault(this, string, detailXML);
}

int ReplayBindingService::soap_receiverfault(const char *subcodeQName, const char *string, const char *detailXML)
{	return ::soap_receiver_fault_subcode(this, subcodeQName, string, detailXML);
}

void ReplayBindingService::soap_print_fault(FILE *fd)
{	::soap_print_fault(this, fd);
}

#ifndef WITH_LEAN
void ReplayBindingService::soap_stream_fault(std::ostream& os)
{	::soap_stream_fault(this, os);
}

char *ReplayBindingService::soap_sprint_fault(char *buf, size_t len)
{	return ::soap_sprint_fault(this, buf, len);
}
#endif

void ReplayBindingService::soap_noheader()
{	header = NULL;
}

void ReplayBindingService::soap_header(char *wsa__MessageID, struct wsa__Relationship *wsa__RelatesTo, struct wsa__EndpointReferenceType *wsa__From, struct wsa__EndpointReferenceType *wsa__ReplyTo, struct wsa__EndpointReferenceType *wsa__FaultTo, char *wsa__To, char *wsa__Action, struct _wsse__Security *wsse__Security, char *wsa5__MessageID, struct wsa5__RelatesToType *wsa5__RelatesTo, struct wsa5__EndpointReferenceType *wsa5__From, struct wsa5__EndpointReferenceType *wsa5__ReplyTo, struct wsa5__EndpointReferenceType *wsa5__FaultTo, char *wsa5__To, char *wsa5__Action)
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

const SOAP_ENV__Header *ReplayBindingService::soap_header()
{	return this->header;
}

int ReplayBindingService::run(int port)
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

SOAP_SOCKET ReplayBindingService::bind(const char *host, int port, int backlog)
{	return soap_bind(this, host, port, backlog);
}

SOAP_SOCKET ReplayBindingService::accept()
{	return soap_accept(this);
}

int ReplayBindingService::serve()
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

static int serve___trp__GetServiceCapabilities(ReplayBindingService*);
static int serve___trp__GetReplayUri(ReplayBindingService*);
static int serve___trp__GetReplayConfiguration(ReplayBindingService*);
static int serve___trp__SetReplayConfiguration(ReplayBindingService*);

int ReplayBindingService::dispatch()
{	soap_peek_element(this);
	if (!soap_match_tag(this, this->tag, "trp:GetServiceCapabilities"))
		return serve___trp__GetServiceCapabilities(this);
	if (!soap_match_tag(this, this->tag, "trp:GetReplayUri"))
		return serve___trp__GetReplayUri(this);
	if (!soap_match_tag(this, this->tag, "trp:GetReplayConfiguration"))
		return serve___trp__GetReplayConfiguration(this);
	if (!soap_match_tag(this, this->tag, "trp:SetReplayConfiguration"))
		return serve___trp__SetReplayConfiguration(this);
	return this->error = SOAP_NO_METHOD;
}

static int serve___trp__GetServiceCapabilities(ReplayBindingService *soap)
{	struct __trp__GetServiceCapabilities soap_tmp___trp__GetServiceCapabilities;
	_trp__GetServiceCapabilitiesResponse trp__GetServiceCapabilitiesResponse;
	trp__GetServiceCapabilitiesResponse.soap_default(soap);
	soap_default___trp__GetServiceCapabilities(soap, &soap_tmp___trp__GetServiceCapabilities);
	soap->encodingStyle = NULL;
	if (!soap_get___trp__GetServiceCapabilities(soap, &soap_tmp___trp__GetServiceCapabilities, "-trp:GetServiceCapabilities", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->GetServiceCapabilities(soap_tmp___trp__GetServiceCapabilities.trp__GetServiceCapabilities, &trp__GetServiceCapabilitiesResponse);
	if (soap->error)
		return soap->error;
	soap_serializeheader(soap);
	trp__GetServiceCapabilitiesResponse.soap_serialize(soap);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || trp__GetServiceCapabilitiesResponse.soap_put(soap, "trp:GetServiceCapabilitiesResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || trp__GetServiceCapabilitiesResponse.soap_put(soap, "trp:GetServiceCapabilitiesResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve___trp__GetReplayUri(ReplayBindingService *soap)
{	struct __trp__GetReplayUri soap_tmp___trp__GetReplayUri;
	_trp__GetReplayUriResponse trp__GetReplayUriResponse;
	trp__GetReplayUriResponse.soap_default(soap);
	soap_default___trp__GetReplayUri(soap, &soap_tmp___trp__GetReplayUri);
	soap->encodingStyle = NULL;
	if (!soap_get___trp__GetReplayUri(soap, &soap_tmp___trp__GetReplayUri, "-trp:GetReplayUri", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->GetReplayUri(soap_tmp___trp__GetReplayUri.trp__GetReplayUri, &trp__GetReplayUriResponse);
	if (soap->error)
		return soap->error;
	soap_serializeheader(soap);
	trp__GetReplayUriResponse.soap_serialize(soap);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || trp__GetReplayUriResponse.soap_put(soap, "trp:GetReplayUriResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || trp__GetReplayUriResponse.soap_put(soap, "trp:GetReplayUriResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve___trp__GetReplayConfiguration(ReplayBindingService *soap)
{	struct __trp__GetReplayConfiguration soap_tmp___trp__GetReplayConfiguration;
	_trp__GetReplayConfigurationResponse trp__GetReplayConfigurationResponse;
	trp__GetReplayConfigurationResponse.soap_default(soap);
	soap_default___trp__GetReplayConfiguration(soap, &soap_tmp___trp__GetReplayConfiguration);
	soap->encodingStyle = NULL;
	if (!soap_get___trp__GetReplayConfiguration(soap, &soap_tmp___trp__GetReplayConfiguration, "-trp:GetReplayConfiguration", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->GetReplayConfiguration(soap_tmp___trp__GetReplayConfiguration.trp__GetReplayConfiguration, &trp__GetReplayConfigurationResponse);
	if (soap->error)
		return soap->error;
	soap_serializeheader(soap);
	trp__GetReplayConfigurationResponse.soap_serialize(soap);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || trp__GetReplayConfigurationResponse.soap_put(soap, "trp:GetReplayConfigurationResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || trp__GetReplayConfigurationResponse.soap_put(soap, "trp:GetReplayConfigurationResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

static int serve___trp__SetReplayConfiguration(ReplayBindingService *soap)
{	struct __trp__SetReplayConfiguration soap_tmp___trp__SetReplayConfiguration;
	_trp__SetReplayConfigurationResponse trp__SetReplayConfigurationResponse;
	trp__SetReplayConfigurationResponse.soap_default(soap);
	soap_default___trp__SetReplayConfiguration(soap, &soap_tmp___trp__SetReplayConfiguration);
	soap->encodingStyle = NULL;
	if (!soap_get___trp__SetReplayConfiguration(soap, &soap_tmp___trp__SetReplayConfiguration, "-trp:SetReplayConfiguration", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = soap->SetReplayConfiguration(soap_tmp___trp__SetReplayConfiguration.trp__SetReplayConfiguration, &trp__SetReplayConfigurationResponse);
	if (soap->error)
		return soap->error;
	soap_serializeheader(soap);
	trp__SetReplayConfigurationResponse.soap_serialize(soap);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || trp__SetReplayConfigurationResponse.soap_put(soap, "trp:SetReplayConfigurationResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || trp__SetReplayConfigurationResponse.soap_put(soap, "trp:SetReplayConfigurationResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}
/* End of server object code */
