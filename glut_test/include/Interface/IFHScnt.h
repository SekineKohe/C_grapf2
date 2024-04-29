// -----------------------------------------------------------------------------
//
//		GPC/GPF-6320 Header file
//
//		File Name : IFHScnt.h
//		Version : 1.10
//
//		Copyright 2004, 2006 Interface Corporation. All rights reserved.
// -----------------------------------------------------------------------------
#if !defined( _IFHSCNT_H_ )
#define _IFHSCNT_H_

#ifdef __cplusplus
extern	"C" {
#endif

// -----------------------------------------------------------------------------
//	Error identifier
// -----------------------------------------------------------------------------
#define IFHSCNT_ERROR_SUCCESS				0x00000000
#define IFHSCNT_ERROR_NOT_DEVICE			0xC0000001
#define IFHSCNT_ERROR_NOT_OPEN				0xC0000002
#define IFHSCNT_ERROR_INVALID_HANDLE		0xC0000003
#define IFHSCNT_ERROR_ALREADY_OPEN			0xC0000004
#define IFHSCNT_ERROR_INSUFFICIENT_BUFFER	0xC0000007
#define IFHSCNT_ERROR_IO_PENDING			0xC0000008
#define IFHSCNT_ERROR_NOT_SUPPORTED			0xC0000009
#define IFHSCNT_ERROR_INVALID_PARAMETER		0xC0000010
#define IFHSCNT_ERROR_NOT_ALLOCATE_MEMORY	0xC0000011

#define IFHSCNT_ERROR_NULL_POINTER			0xC0001000
#define IFHSCNT_ERROR_INVALID_CHANNEL		0xC0001001
#define IFHSCNT_ERROR_INVALID_RESET_MODE	0xC0001002
#define IFHSCNT_ERROR_INVALID_COUNT_MODE	0xC0001003
#define IFHSCNT_ERROR_INVALID_MASK			0xC0001004
#define IFHSCNT_ERROR_INVALID_COMMAND		0xC0001005

#define IFHSCNT_ERROR_INVALID_OUT_ENABLE	0xC0001006
#define IFHSCNT_ERROR_INVALID_DO			0xC0001007
#define IFHSCNT_ERROR_INVALID_FILTER		0xC0001008
#define IFHSCNT_ERROR_INVALID_TIMER			0xC0001009

// -----------------------------------------------------------------------------
//	Reset mode identifier
// -----------------------------------------------------------------------------
#define IFHSCNT_RESET_DEVICE				0x00000001
#define IFHSCNT_RESET_COUNTER				0x00000002

// -----------------------------------------------------------------------------
//	Counter mode identifier
// -----------------------------------------------------------------------------
#define IFHSCNT_MODE_GATE					0x00000002
#define IFHSCNT_MODE_CLR					0x00000004

// -----------------------------------------------------------------------------
//	Counter control command identifier
// -----------------------------------------------------------------------------
#define	IFHSCNT_CMD_START					0x00000001
#define	IFHSCNT_CMD_STOP					0x00000004

// -----------------------------------------------------------------------------
//	Event channel identifier
// -----------------------------------------------------------------------------
#define	IFHSCNT_EVENT_CH_TIMER				0x00000007
#define	IFHSCNT_EVENT_CH_EDGE				0x00000008


// -----------------------------------------------------------------------------
//	Board information structure
// -----------------------------------------------------------------------------
typedef struct {
	DWORD	dwType;
	DWORD	dwBoardID;
} HSCNTDEVICEINFO, *PHSCNTDEVICEINFO;

// -----------------------------------------------------------------------------
//	Interrupt structure
// -----------------------------------------------------------------------------
#ifdef _WIN64
	typedef void (CALLBACK HSCNTCALLBACK)(INT nChannel, DWORD dwEvent, PVOID dwUser);
#else
	typedef void (CALLBACK HSCNTCALLBACK)(INT nChannel, DWORD dwEvent, DWORD dwUser);
#endif

typedef HSCNTCALLBACK FAR *PHSCNTCALLBACK;

#ifdef _WIN64
	typedef struct {
		HWND			hWnd;
		UINT			uMessage[8];
		HANDLE			hEvent;
		PHSCNTCALLBACK	pCallBackProc;
		PVOID			dwUser;
	} HSCNTEVENTREQ, *PHSCNTEVENTREQ;
#else
	typedef struct {
		HWND			hWnd;
		UINT			uMessage[8];
		HANDLE			hEvent;
		PHSCNTCALLBACK	pCallBackProc;
		DWORD			dwUser;
	} HSCNTEVENTREQ, *PHSCNTEVENTREQ;
#endif


// -----------------------------------------------------------------------------
//	Function definition
// -----------------------------------------------------------------------------
#ifndef _WIN64
#define _IF_HSCNT_DLL_
#endif

#if !defined(_IF_HSCNT_DLL_)
#define IFHSCNTAPI
#else
#define IFHSCNTAPI __declspec(dllexport)
#endif

// Initializing
#ifdef _WIN64
	IFHSCNTAPI HANDLE WINAPI HScntOpen(LPCSTR);
#else
	IFHSCNTAPI HANDLE WINAPI HScntOpen(LPCTSTR);
#endif

IFHSCNTAPI INT WINAPI HScntClose(HANDLE);
IFHSCNTAPI INT WINAPI HScntGetDeviceInfo(HANDLE, PHSCNTDEVICEINFO);
IFHSCNTAPI INT WINAPI HScntReset(HANDLE, DWORD, DWORD);

// Setting the mode
IFHSCNTAPI INT WINAPI HScntSetMode(HANDLE, INT, DWORD);
IFHSCNTAPI INT WINAPI HScntGetMode(HANDLE, INT, PDWORD);

// Retrieving the counter
IFHSCNTAPI INT WINAPI HScntReadCounter(HANDLE, DWORD, PDWORD);

// Retrieving the staus
IFHSCNTAPI INT WINAPI HScntGetStatus(HANDLE, DWORD, PDWORD);

// Controlling the counter
IFHSCNTAPI INT WINAPI HScntStartCount(HANDLE, DWORD, DWORD);
IFHSCNTAPI INT WINAPI HScntStopCount(HANDLE, DWORD, DWORD);

// Setting/retrieving the interrupt
IFHSCNTAPI INT WINAPI HScntSetEventMask(HANDLE, INT, DWORD);
IFHSCNTAPI INT WINAPI HScntGetEventMask(HANDLE, INT, PDWORD);
IFHSCNTAPI INT WINAPI HScntSetEvent(HANDLE, PHSCNTEVENTREQ);
IFHSCNTAPI INT WINAPI HScntKillEvent(HANDLE);

// General purpose digital input/output
IFHSCNTAPI INT WINAPI HScntSetDOEnable(HANDLE, DWORD);
IFHSCNTAPI INT WINAPI HScntGetDOEnable(HANDLE, PDWORD);
IFHSCNTAPI INT WINAPI HScntInputDI(HANDLE, PDWORD);
IFHSCNTAPI INT WINAPI HScntOutputDO(HANDLE, DWORD);

// Setting/retrieving the interval timer
IFHSCNTAPI INT WINAPI HScntSetTimerConfig(HANDLE, BYTE);
IFHSCNTAPI INT WINAPI HScntGetTimerConfig(HANDLE, PBYTE);
IFHSCNTAPI INT WINAPI HScntGetTimerCount(HANDLE, PBYTE);

// Setting/retrieving the filter
IFHSCNTAPI INT WINAPI HScntSetFilter(HANDLE, DWORD);
IFHSCNTAPI INT WINAPI HScntGetFilter(HANDLE, PDWORD);

#ifdef __cplusplus
}
#endif

#endif
