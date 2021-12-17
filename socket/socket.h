#pragma once
#ifdef _WIN32
#include "includes/windows_sock.h"
#define Socket WinSock
#endif
#ifdef __UNIX__
#include "includes/linsock.h"
#define Socket LinSock
#endif
