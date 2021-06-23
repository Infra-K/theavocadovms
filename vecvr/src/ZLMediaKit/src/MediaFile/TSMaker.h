/*
 * TSMaker.h
 *
 *  Created on: 2013-6-21
 *      Author: root
 */

#ifndef TSMAKER_H_
#define TSMAKER_H_

#include "crc32.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include "Util/File.h"

using namespace std;
using namespace ZL::Util;

#define TS_PACKET_SIZE                 188
#define TS_PACKET_HEADER               4
#define TS_SYNC_BYTE                   0x47
#define TS_PAT_PID                     0x00
#define TS_PMT_PID                     0xFFF
#define TS_H264_PID                    0x100
#define TS_AAC_PID                     0x101
#define TS_H264_STREAM_ID              0xE0
#define TS_AAC_STREAM_ID               0xC0
#define PMT_STREAM_TYPE_VIDEO          0x1B
#define PMT_STREAM_TYPE_AUDIO          0x0F

//#define ES_BUF_SIZE 256*1024

//ts 包头 //TS 바오 터우
typedef struct Tag_PacketHeader {
	unsigned char sync_byte :8;         //同步字节, 固定为0x47,表示后面的是一个TS分组 // 동기 바이트, 0x47로 고정, 다음이 TS 패킷임을 나타냅니다.
	unsigned char tras_error :1;        //传输误码指示符 // 전송 오류 표시기
	unsigned char play_init :1;         //有效荷载单元起始指示符 // 페이로드 단위의 시작 표시기
	unsigned char tras_prio :1;         //传输优先, 1表示高优先级,传输机制可能用到，解码用不着 // 전송 우선 순위, 1은 높은 우선 순위를 의미하며 전송 메커니즘을 사용할 수 있으며 디코딩이 필요하지 않습니다.
	unsigned int PID :13;         		//PID // PID
	unsigned char tras_scramb :2;       //传输加扰控制 // 전송 스크램블링 제어
	unsigned char ada_field_C :2; //自适应控制 01仅含有效负载，10仅含调整字段，11含有调整字段和有效负载，先调整字段然后有效负载。为00解码器不进行处理 // Adaptive control 01에는 페이로드 만 포함되고 10에는 조정 필드 만 포함되며 11에는 조정 필드와 페이로드가 포함됩니다. 필드가 먼저 조정 된 다음 페이로드가 조정됩니다. 00 디코더가 처리되지 않음
	unsigned char conti_cter :4;        //连续计数器 一个4bit的计数器，范围0-15 // 연속 카운터 4 비트 카운터, 범위 0-15
} TsPacketHeader;

//连续性计数器,也就是说 有多少个 pat包，几个pmt包 ，几个MP3 包，几个 h264包，0x00 - 0x0f ，然后折回到0x00
//연속성 카운터, 즉 몇 개의 pat 패키지, 여러 개의 pmt 패키지, 여러 개의 MP3 패키지, 여러 개의 h264 패키지, 0x00-0x0f, 그리고 다시 0x00으로 돌아갑니다.
typedef struct Tag_Continuity_Counter {
	unsigned char continuity_counter_pat;
	unsigned char continuity_counter_pmt;
	unsigned char continuity_counter_video;
	unsigned char continuity_counter_audio;
} Continuity_Counter;

//自适应段标志 //적응 형 세그먼트 플래그
typedef struct Tag_Ts_Adaptation_field {
	unsigned char discontinuty_indicator :1;       //1表明当前传送流分组的不连续状态为真 // 1은 현재 전송 스트림 패킷의 불연속 상태가 참임을 나타냅니다.
	unsigned char random_access_indicator :1; //表明下一个有相同PID的PES分组应该含有PTS字段和一个原始流访问点 // 같은 PID를 가진 다음 PES 패킷에 PTS 필드와 원래 스트림 액세스 포인트가 포함되어야 함을 나타냅니다.
	unsigned char elementary_stream_priority_indicator :1;  //优先级; // 우선 순위
	unsigned char PCR_flag :1;                              //包含pcr字段 // pcr 필드 포함
	unsigned char OPCR_flag :1;                             //包含opcr字段 // opcr 필드 포함
	unsigned char splicing_point_flag :1;                   //拼接点标志 // 스 플라이 싱 포인트 플래그
	unsigned char transport_private_data_flag :1;           //私用字节 // 전용 바이트
	unsigned char adaptation_field_extension_flag :1;       //调整字段有扩展 // Adjustment field has extension

	unsigned char adaptation_field_length;                 //自适应段长度 // 적응 세그먼트 길이
	unsigned long long pcr;                               //自适应段中用到的的pcr // 적응 세그먼트에서 사용되는 pcr
	unsigned long long opcr;                              //自适应段中用到的的opcr // 적응 형 세그먼트에 사용되는 opcr
	unsigned char splice_countdown;
	unsigned char private_data_len;
	unsigned char private_data[256];
} Ts_Adaptation_field;

//PAT结构体：节目相关表 //PAT 구조 : 프로그램 관련 테이블
typedef struct Tag_TsPat {
	unsigned char table_id :8;                  //固定为0x00 ，标志是该表是PAT // 0x00으로 고정, 기호는 테이블이 PAT라는 것입니다.
	unsigned char section_syntax_indicator :1;   //段语法标志位，固定为1 // 세그먼트 구문 플래그, 1로 고정
	unsigned char zero :1;                      //0 // 0
	unsigned char reserved_1 :2;                //保留位 // 예약 된 비트
	unsigned int section_length :12;           //表示这个字节后面有用的字节数，包括CRC32 // CRC32를 포함하여이 바이트 이후의 유용한 바이트 수를 나타냅니다.
	unsigned int transport_stream_id :16;      //该传输流的ID，区别于一个网络中其它多路复用的流 // 네트워크의 다른 다중화 스트림과 다른 전송 스트림의 ID
	unsigned char reserved_2 :2;                //保留位 // 예약 된 비트
	unsigned char version_number :5;           //范围0-31，表示PAT的版本号 // 범위 0-31, PAT의 버전 번호를 나타냄
	unsigned char current_next_indicator :1;   //发送的PAT是当前有效还是下一个PAT有效 // 전송 된 PAT가 현재 유효하거나 다음 PAT가 유효합니까?
	unsigned char section_number :8; //分段的号码。PAT可能分为多段传输，第一段为00，以后每个分段加1，最多可能有256个分段 // 섹션의 번호. PAT는 다중 세그먼트 전송으로 나눌 수 있으며 첫 번째 세그먼트는 00이고 각 세그먼트는 1 씩 증가하며 최대 256 개의 세그먼트가있을 수 있습니다.
	unsigned char last_section_number :8;      //最后一个分段的号码 // 마지막 섹션의 번호
	unsigned int program_number :16;          //节目号 // 프로그램 번호
	unsigned char reserved_3 :3;             //保留位 // 예약 된 비트
	//unsigned int network_PID :13 ;             //网络信息表（NIT）的PID,节目号为0时对应的PID为network_PID,本例中不含有 networke_pid // 네트워크 정보 테이블 (NIT)의 PID, 프로그램 번호가 0 일 때 해당 PID는 network_PID,이 예제에는 networke_pid가 포함되지 않습니다.
	unsigned int program_map_PID :13;         //节目映射表的PID，节目号大于0时对应的PID，每个节目对应一个 // 프로그램 맵 테이블의 PID, 프로그램 번호가 0보다 클 때 해당 PID, 각 프로그램에 대해 하나씩
	unsigned long long CRC_32 :32;             //CRC32校验码 // CRC32 확인 코드
} TsPat;

//PMT结构体：节目映射表 //PMT 구조 : 프로그램 맵 테이블
typedef struct Tag_TsPmt {
	unsigned char table_id :8;                 //固定为0x02, 表示PMT表 //PMT 테이블을 의미하는 0x02로 고정
	unsigned char section_syntax_indicator :1;  //固定为0x01 //0x01로 고정
	unsigned char zero :1;                       //0x00
	unsigned char reserved_1 :2;                //0x03
	unsigned int section_length :12;     //首先两位bit置为00，它指示段的byte数，由段长度域开始，包含CRC。//첫째, 두 비트는 세그먼트 길이 필드에서 시작하여 CRC를 포함하여 세그먼트의 바이트 수를 나타내는 00으로 설정됩니다.
	unsigned int program_number :16;            // 指出该节目对应于可应用的Program map PID //프로그램이 해당 프로그램 맵 PID에 해당함을 지적하십시오.
	unsigned char reserved_2 :2;                 //0x03
	unsigned char version_number :5;             //指出TS流中Program map section的版本号 //TS 스트림에서 프로그램 맵 섹션의 버전 번호를 지적하십시오.
	unsigned char current_next_indicator :1; //当该位置1时，当前传送的Program map section可用；当该位置0时，指示当前传送的Program map section不可用，下一个TS流的Program map section有效。//위치가 1이면 현재 전송중인 프로그램 맵 섹션을 사용할 수 있고, 위치가 0이면 현재 전송중인 프로그램 맵 섹션을 사용할 수 없으며 다음 TS 스트림의 프로그램 맵 섹션이 유효 함을 나타냅니다.
	unsigned char section_number :8;            //固定为0x00 // 0x00으로 고정
	unsigned char last_section_number :8;        //固定为0x00 // 0x00으로 고정
	unsigned char reserved_3 :3;                //0x07 // 0x07
	unsigned int PCR_PID :13; //指明TS包的PID值，该TS包含有PCR域，该PCR值对应于由节目号指定的对应节目。如果对于私有数据流的节目定义与PCR无关，这个域的值将为0x1FFF。 // TS 패킷의 PID 값을 지정합니다. TS에는 PCR 필드가 포함됩니다. PCR 값은 프로그램 번호로 지정된 해당 프로그램에 해당합니다. 개인 데이터 스트림에 대한 프로그램 정의가 PCR과 관련이없는 경우이 필드의 값은 0x1FFF가됩니다.
	unsigned char reserved_4 :4;                //预留为0x0F // 0x0F로 예약 됨
	unsigned int program_info_length :12;    //前两位bit为00。该域指出跟随其后对节目信息的描述的byte数。 // 처음 두 비트는 00입니다. 이 필드는 프로그램 정보 설명 다음에 오는 바이트 수를 나타냅니다.
	unsigned char stream_type_video :8; //指示特定PID的节目元素包的类型。该处PID由elementary PID指定 // 특정 PID의 프로그램 요소 패키지 유형을 나타냅니다. 여기서 PID는 기본 PID로 지정됩니다.
	unsigned char reserved_5_video :3;           //0x07 // 0x07
	unsigned int elementary_PID_video :13;       //该域指示TS包的PID值。这些TS包含有相关的节目元素 //이 필드는 TS 패킷의 PID 값을 나타냅니다. 이 TS에는 관련 프로그램 요소가 포함되어 있습니다.
	unsigned char reserved_6_video :4;          //0x0F // 0x0F
	unsigned int ES_info_length_video :12;   //前两位bit为00。该域指示跟随其后的描述相关节目元素的byte数 // 처음 두 비트는 00입니다. 이 필드는 뒤에 오는 관련 프로그램 요소를 설명하는 바이트 수를 나타냅니다.
	unsigned char stream_type_audio :8; //指示特定PID的节目元素包的类型。该处PID由elementary PID指定 // 특정 PID의 프로그램 요소 패키지 유형을 나타냅니다. 여기서 PID는 기본 PID로 지정됩니다.
	unsigned char reserved_5_audio :3;           //0x07
	unsigned int elementary_PID_audio :13;       //该域指示TS包的PID值。这些TS包含有相关的节目元素 //이 필드는 TS 패킷의 PID 값을 나타냅니다. 이 TS에는 관련 프로그램 요소가 포함되어 있습니다.
	unsigned char reserved_6_audio :4;          //0x0F
	unsigned int ES_info_length_audio :12;   //前两位bit为00。该域指示跟随其后的描述相关节目元素的byte数 // 처음 두 비트는 00입니다. 이 필드는 뒤에 오는 관련 프로그램 요소를 설명하는 바이트 수를 나타냅니다.
	unsigned long long CRC_32 :32;                    //CRC32校验码 // CRC32 확인 코드
} TsPmt;

//PTS_DTS结构体：本程序设置都有 “11” //PTS_DTS 구조 : 프로그램 설정에 "11"이 있습니다.
typedef struct Tag_TsPtsDts {
	unsigned char reserved_1 :4;
	unsigned char pts_32_30 :3;                //显示时间戳 //타임 스탬프 표시
	unsigned char marker_bit1 :1;
	unsigned int pts_29_15 :15;
	unsigned char marker_bit2 :1;
	unsigned int pts_14_0 :15;
	unsigned char marker_bit3 :1;
	unsigned char reserved_2 :4;
	unsigned char dts_32_30 :3;                  //解码时间戳 //타임 스탬프 디코딩
	unsigned char marker_bit4 :1;
	unsigned int dts_29_15 :15;
	unsigned char marker_bit5 :1;
	unsigned int dts_14_0 :15;
	unsigned char marker_bit6 :1;
} TsPtsDts;

//PES包结构体，包括PES包头和ES数据 ,头 19 个字节 //PES 패킷 헤더 및 ES 데이터를 포함한 PES 패킷 구조, 처음 19 바이트
typedef struct Tag_TsPes {
	unsigned int packet_start_code_prefix :24;                  //起始：0x000001 //시작 : 0x000001
	unsigned char stream_id :8;                //基本流的类型和编号 // 기본 스트림의 유형과 수
	unsigned int PES_packet_length :16; //包长度,就是帧数据的长度，可能为0,要自己算,做多16位，如果超出则需要自己算 // Packet length, is the length of the frame data, it may be 0, you have yourself 계산해야하고 16 비트 이상을 수행하고 초과하면 직접 계산해야합니다.
	unsigned char marker_bit :2;                 //必须是：'10' // must be : '10'
	unsigned char PES_scrambling_control :2;     //pes包有效载荷的加扰方式 // pes 패킷 페이로드의 스크램블링 방법
	unsigned char PES_priority :1;               //有效负载的优先级 // 페이로드 우선 순위
	unsigned char data_alignment_indicator :1; //如果设置为1表明PES包的头后面紧跟着视频或音频syncword开始的代码。 // 1로 설정하면 PES 패킷의 헤더 바로 뒤에 비디오 또는 오디오 싱크 워드의 시작 코드가옵니다.
	unsigned char copyright :1;                  //1:靠版权保护，0：不靠 // 1 : 저작권 보호에 의존, 0 : 아니오
	unsigned char original_or_copy :1;           //1;有效负载是原始的，0：有效负载时拷贝的 // 1; 페이로드가 원본, 0 : 페이로드가 복사 됨
	unsigned char PTS_DTS_flags :2; //'10'：PTS字段存在，‘11’：PTD和DTS都存在，‘00’：都没有，‘01’：禁用。 // '10': PTS 필드 존재, '11': PTD 및 DTS 모두 존재, '00': 없음, '01': 비활성화 됨.
	unsigned char ESCR_flag :1;         //1:escr基准字段 和 escr扩展字段均存在，0：无任何escr字段存在 // 1 : escr 참조 필드와 escr 확장 필드가 모두 존재합니다. 0 : escr 필드가 없습니다.
	unsigned char ES_rate_flag :1;               //1:es_rate字段存在，0 ：不存在 // 1 : es_rate 필드 존재, 0 : 존재하지 않음
	unsigned char DSM_trick_mode_flag :1;        //1;8比特特接方式字段存在，0 ：不存在 // 1; 8 비트 특수 연결 모드 필드 존재, 0 : 존재하지 않음
	unsigned char additional_copy_info_flag :1; //1:additional_copy_info存在，0: 不存在 // 1 : additional_copy_info 존재, 0 : 존재하지 않음
	unsigned char PES_CRC_flag :1;               //1:crc字段存在，0：不存在 // 1 : crc 필드 존재, 0 : 존재하지 않음
	unsigned char PES_extension_flag :1;         //1：扩展字段存在，0：不存在 // 1 : 확장 필드 존재, 0 : 존재하지 않음
	unsigned char PES_header_data_length :8;    //后面数据的长度， // 다음 데이터의 길이,
	//TsPtsDts tsptsdts;                     //ptsdts结构体对象，10个字节 // ptsdts 구조체 객체, 10 바이트
	char *ES;
	unsigned long ESlen;
} TsPes;

/*
//H264一帧数据的结构体 //H264 데이터의 한 프레임 구조
 typedef struct Tag_NALU_t {
 unsigned char forbidden_bit;           //! Should always be FALSE //! 항상 FALSE 여야합니다.
 unsigned char nal_reference_idc;       //! NALU_PRIORITY_xxxx //! NALU_PRIORITY_xxxx
 unsigned char nal_unit_type;           //! NALU_TYPE_xxxx //! NALU_TYPE_xxxx
 unsigned int startcodeprefix_len;      //! 前缀字节数 //! 접두사 바이트 수
 unsigned int len;            //! 包含nal 头的nal 长度，从第一个00000001到下一个000000001的长度 //! 처음 00000001부터 다음 000000001까지 nal 헤더를 포함하는 nal의 길이
 unsigned int max_size;                 //! 做多一个nal 的长度 //! 끝 길이를 하나 더 만듭니다.
 unsigned char * buf;                   //! 包含nal 头的nal 数据 //! nal 헤더의 nal 데이터를 포함합니다.
 unsigned int lost_packets;             //! 预留 //! 예약 됨
 } NALU_t;*/

//nal类型 날 유형
typedef enum {
	NALU_TYPE_SLICE = 1,
	NALU_TYPE_DPA = 2,
	NALU_TYPE_DPB = 3,
	NALU_TYPE_DPC = 4,
	NALU_TYPE_IDR = 5,
	NALU_TYPE_SEI = 6,
	NALU_TYPE_SPS = 7,
	NALU_TYPE_PPS = 8,
	NALU_TYPE_AUD = 9,
	NALU_TYPE_EOSEQ = 10,
	NALU_TYPE_EOSTREAM = 11,
	NALU_TYPE_FILL = 12,
#if (MVC_EXTENSION_ENABLE)
NALU_TYPE_PREFIX = 14,
NALU_TYPE_SUB_SPS = 15,
NALU_TYPE_SLC_EXT = 20,
NALU_TYPE_VDRD = 24  // View and Dependency Representation Delimiter NAL Unit
#endif
} NaluType;

/*//MP3头结构体 //MP3 헤더 구조
 typedef struct Tag_Mp3_Header {
 unsigned int sync :11;                  //同步信息 // 정보 동기화
 unsigned char version :2;                //版本 // version
 unsigned char layer :2;                  //层 // layer
 unsigned char error_protection :1;        //CRC校验 // CRC 검사
 unsigned char bitrate_index :4;          //位率 // 비트 전송률
 unsigned char sampling_frequency :2;     //采样频率 // 샘플링 주파수
 unsigned char padding :1;                //帧长调节 // 프레임 길이 조정
 unsigned char private_t :1;              //保留字 // 예약어
 unsigned char mode :2;                   //声道模式 // 채널 모드
 unsigned char mode_extension :2;         //扩展模式 // 확장 모드
 unsigned char copyright :1;              //版权 // 저작권
 unsigned char original :1;               //原版标志 // 원래 로고
 unsigned char emphasis :2;               //强调模式 // 강조 모드
 } Mp3_Header;*/


class TSMaker {
public:
	TSMaker();
	virtual ~TSMaker();
	bool init(const string &strFilename, uint32_t ui32BufSize);
	int inputH264(const char *pcData, uint32_t ui32Len, uint64_t ui64Time);
	int inputAAC(const char *pcData, uint32_t ui32Len, uint64_t ui64Time);
	void clear();
private:
	string m_strFilename;
	FILE *m_pOutVideoTs;
	Continuity_Counter m_continuityCounter;
	TsPes *m_pVideo_pes;
	TsPes *m_pAudio_pes;
	unsigned int m_uiWritePacketNum;
	char *m_pcFileBuf;

	void flush();
	void CreateTsHeader(TsPacketHeader * pTsHeader, unsigned int uiPID, unsigned char ucPlayInit, unsigned char ucAdaFieldC);
	void TsHeader2buffer(TsPacketHeader * pTsHeader, unsigned char *pucBuffer);
	void CreatePAT();
	void CreatePMT();

	void WriteAdaptive_flags_Head(Ts_Adaptation_field * pAdaptationField, uint64_t ui64VideoPts);
	void WriteAdaptive_flags_Tail(Ts_Adaptation_field * pAdaptationField); //填写自适应段标志帧尾的 //프레임의 끝을 표시하기 위해 적응 형 세그먼트를 채우십시오.

	void PES2TS(TsPes * pPes, unsigned int uiPID, Ts_Adaptation_field * pAdaptationField, uint64_t ui64Pts);
	void CreateAdaptive_Ts(Ts_Adaptation_field * pAdaptationField, unsigned char * pcTs, unsigned int uiAdaptiveLength);
};

#endif /* TSMAKER_H_ */
