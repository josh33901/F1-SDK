#pragma once
#include "baseHeaders.hh"

//#define LOGCONSOLE(X, ...) Log::Console(X, __VA_ARGS__)

#define XASSERT(x)                                    \
	if (!x) {                                         \
		Log::Fatal ("variable `%s' is nullptr!", #x); \
	}

class Log
{
public:
	static int               MaxQueueItems;
	static CUtlQueue<char *> LogQueue;
	static CThreadFastMutex  LogMutex;

	static void Init (HMODULE hModule);
	static void Debug (const char *fmt, ...);
	static void Msg (const char *fmt, ...);
	static void Error (const char *fmt, ...);
	static void Fatal (const char *fmt, ...);
	static void Console (const char *fmt, ...);
	static void RenderLog ();
};
