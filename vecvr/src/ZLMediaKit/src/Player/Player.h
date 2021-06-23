/*
 * Player.h
 *
 *  Created on: 2016年12月2日
 *      Author: xzl
 */

#ifndef SRC_PLAYER_PLAYER_H_
#define SRC_PLAYER_PLAYER_H_

#include <string>
using namespace std;

typedef struct {
	uint16_t sequence;
	uint32_t timeStamp;
	unsigned char type;
	string data;
} H264Frame;

//ADTS 头中相对有用的信息 采样率、声道数、帧长度 //ADTS 헤더 샘플링 속도, 채널 수, 프레임 길이에서 상대적으로 유용한 정보
typedef struct {
	unsigned int syncword; //12 bslbf 同步字The bit string ‘1111 1111 1111’，说明一个ADTS帧的开始 // 12 bslbf ADTS 프레임의 시작을 나타내는 비트 문자열‘1111 1111 1111’
	unsigned int id;        //1 bslbf   MPEG 标示符, 设置为1 // 1 bslbf MPEG 식별자, 1로 설정
	unsigned int layer;    //2 uimsbf Indicates which layer is used. Set to ‘00’ // 2 uimsbf 사용되는 레이어를 나타냅니다. '00'으로 설정
	unsigned int protection_absent;  //1 bslbf  表示是否误码校验 // 1 bslbf는 오류 코드 확인 여부를 나타냅니다.
	unsigned int profile; //2 uimsbf  表示使用哪个级别的AAC，如01 Low Complexity(LC)--- AACLC // 2 uimsbf는 01 LC (Low Complexity)와 같이 사용되는 AAC 수준을 나타냅니다 .--- AACLC
	unsigned int sf_index;           //4 uimsbf  表示使用的采样率下标 // 4 uimsbf는 사용 된 샘플링 속도 인덱스를 나타냅니다.
	unsigned int private_bit;        //1 bslbf // 1 bslbf
	unsigned int channel_configuration;  //3 uimsbf  表示声道数 // 3 uimsbf는 채널 수를 나타냅니다.
	unsigned int original;               //1 bslbf
	unsigned int home;                   //1 bslbf
										 //下面的为改变的参数即每一帧都不同 // 다음은 변경된 매개 변수입니다. 즉, 각 프레임이 다릅니다.
	unsigned int copyright_identification_bit;   //1 bslbf
	unsigned int copyright_identification_start; //1 bslbf
	unsigned int aac_frame_length; // 13 bslbf  一个ADTS帧的长度包括ADTS头和raw data block  // 13 bslbf ADTS 프레임의 길이에는 ADTS 헤더와 원시 데이터 블록이 포함됩니다.
	unsigned int adts_buffer_fullness;           //11 bslbf     0x7FF 说明是码率可变的码流  // 11 bslbf 0x7FF는 비트 전송률이 가변적 인 스트림임을 의미합니다.
//no_raw_data_blocks_in_frame 表示ADTS帧中有number_of_raw_data_blocks_in_frame + 1个AAC原始帧.// no_raw_data_blocks_in_frame은 ADTS 프레임에 number_of_raw_data_blocks_in_frame + 1 AAC 원본 프레임이 있음을 의미합니다.
//所以说number_of_raw_data_blocks_in_frame == 0// 그래서 number_of_raw_data_blocks_in_frame == 0
//表示说ADTS帧中有一个AAC数据块并不是说没有。(一个AAC原始帧包含一段时间内1024个采样及相关数据)// ADTS 프레임에 AAC 데이터 블록이 있음을 의미하며 없음을 의미하지는 않습니다. (AAC 원본 프레임에는 일정 기간 동안 1024 개의 샘플 및 관련 데이터가 포함됩니다.)
	unsigned int no_raw_data_blocks_in_frame;    //2 uimsfb
	unsigned char data[2 * 1024 + 7];
	uint16_t sequence;
	uint32_t timeStamp;
} AdtsFrame;

void	makeAdtsHeader(const string &strAudioCfg,AdtsFrame &adts);
void 	writeAdtsHeader(const AdtsFrame &adts, uint8_t *pcAdts) ;
string 	makeAdtsConfig(const uint8_t *pcAdts);
void 	getAACInfo(const AdtsFrame &adts,int &iSampleRate,int &iChannel);
bool 	getAVCInfo(const string &strSps,int &iVideoWidth, int &iVideoHeight, float  &iVideoFps);

#endif /* SRC_PLAYER_PLAYER_H_ */
