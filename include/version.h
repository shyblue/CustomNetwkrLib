
// ���� ������ ������ �Դϴ�.

#ifndef PUBLIC_VERSION_H_
#define PUBLIC_VERSION_H_

#include "common.h"

namespace public_
{
namespace version_
{
const int32_t SERVER_VER_BASE						=	100;		// �⺻ 100���� �����Ѵ�. ū������Ʈ 10����, ����������Ʈ 1������ ������Ʈ �ȴ�
const int32_t 		SYNCHONOUS_SESSION				=	110;		// Synchonous_Session add(���⿬�Ἴ�� �߰�)
const int32_t 		CONNECTIONSESSION_POOL			=	120;		// ConnectionSessionPool add(�������� ����Ǯ �߰�)
const int32_t 		TIMEFUNC						=	121;		// timefuct
const int32_t 		LINUX_SYNC						=	130;		// linux sync
const int32_t 		PACKET_DELIVER					=	140;		// packetdeliver ( client --> session --> packetdeliver --> business --> packetdeliver --> session --> client )
const int32_t 		INIFILE_PARSER					=	141;		// window & linux ini parse class 
const int32_t 		ANALYSIS_PROCESS				=	142;		// analysis_process window & linux synchro
const int32_t 		CONNECTIONSESSION_POOL2			=	143;		// ConnectionSessionPool add(�������� ����Ǯ ���� �۾�)
const int32_t		MAKE_THREAD_BUSINESS			=	144;		// thread�� �Ź� �����Ͽ� ����Ͻ� ������ ó���Ѵ�.


static int32_t 			SERVER_VER;		// current version check
}
}

#endif // PUBLIC_VERSION_H_


