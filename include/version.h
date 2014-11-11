
// 버전 관리용 디파인 입니다.

#ifndef PUBLIC_VERSION_H_
#define PUBLIC_VERSION_H_

#include "common.h"

namespace public_
{
namespace version_
{
const int32_t SERVER_VER_BASE						=	100;		// 기본 100으로 시작한다. 큰업데이트 10단위, 작은업데이트 1단위로 업데이트 된다
const int32_t 		SYNCHONOUS_SESSION				=	110;		// Synchonous_Session add(동기연결세션 추가)
const int32_t 		CONNECTIONSESSION_POOL			=	120;		// ConnectionSessionPool add(연결유지 세션풀 추가)
const int32_t 		TIMEFUNC						=	121;		// timefuct
const int32_t 		LINUX_SYNC						=	130;		// linux sync
const int32_t 		PACKET_DELIVER					=	140;		// packetdeliver ( client --> session --> packetdeliver --> business --> packetdeliver --> session --> client )
const int32_t 		INIFILE_PARSER					=	141;		// window & linux ini parse class 
const int32_t 		ANALYSIS_PROCESS				=	142;		// analysis_process window & linux synchro
const int32_t 		CONNECTIONSESSION_POOL2			=	143;		// ConnectionSessionPool add(연결유지 세션풀 수정 작업)
const int32_t		MAKE_THREAD_BUSINESS			=	144;		// thread를 매번 생성하여 비즈니스 로직을 처리한다.


static int32_t 			SERVER_VER;		// current version check
}
}

#endif // PUBLIC_VERSION_H_


