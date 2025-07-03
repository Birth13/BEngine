// include를 모아놓은 헤더

#pragma once
#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외

// debug
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

// Windows
#include <windows.h>

// C
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// C++
#include <string>
#include <vector>
#include <unordered_map>
#include <thread>

// resource
#include "resource.h"
