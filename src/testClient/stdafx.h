// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include "common.h"

#include "util/configure.h"
#include "util/logger.h"
#include "util/stop_watch.h"
#include "util/common_pool.h"
#include "util/byte_order.h"
#include "util/memory_manager.h"
#include "handler/handler_manager.h"
#include "net/skeleton_server.h"
#include "net/header_factory.h"

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
