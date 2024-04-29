// -----------------------------------------------------------------------------
//
//		GPC/GPF-6320 Header file
//
//		File Name: IFUcnt.h
//		Version		:	1.10
//
//					Copyright 2004, 2012 Interface Corporation. All rights reserved.
// -----------------------------------------------------------------------------
#if !defined( _IFUCNT_H_ )
#define _IFUCNT_H_

#ifdef __cplusplus
extern	"C" {
#endif

// -----------------------------------------------------------------------------
//	Error identifier
// -----------------------------------------------------------------------------
#define IFUCNT_ERROR_SUCCESS				0x00000000
#define IFUCNT_ERROR_NOT_DEVICE				0xC0000001
#define IFUCNT_ERROR_NOT_OPEN				0xC0000002
#define IFUCNT_ERROR_INVALID_HANDLE			0xC0000003
#define IFUCNT_ERROR_ALREADY_OPEN			0xC0000004
#define IFUCNT_ERROR_INSUFFICIENT_BUFFER	0xC0000007
#define IFUCNT_ERROR_IO_PENDING				0xC0000008
#define IFUCNT_ERROR_NOT_SUPPORTED			0xC0000009
#define IFUCNT_ERROR_INVALID_PARAMETER		0xC0000010
#define IFUCNT_ERROR_NOT_ALLOCATE_MEMORY	0xC0000021

#define IFUCNT_ERROR_NOW_SAMPLING			0xC0001000
#define IFUCNT_ERROR_NOW_STOP				0xC0001001
#define IFUCNT_ERROR_NULL_POINTER			0xC0001002
#define IFUCNT_ERROR_WRITE_FAILED			0xC0001003
#define IFUCNT_ERROR_READ_FAILED			0xC0001004
#define	IFUCNT_ERROR_NOBUFFER				0xC0001005
#define IFUCNT_ERROR_INVALID_OFFSET			0xC0001006

#define IFUCNT_ERROR_INVALID_FUNC_MODE		0xC0001010
#define IFUCNT_ERROR_INVALID_CHANNEL		0xC0001011
#define IFUCNT_ERROR_INVALID_RESET_MODE		0xC0001012
#define IFUCNT_ERROR_INVALID_COUNT_MODE		0xC0001013
#define IFUCNT_ERROR_INVALID_LOAD			0xC0001014
#define IFUCNT_ERROR_INVALID_LATCH			0xC0001015
#define IFUCNT_ERROR_INVALID_CLOCK			0xC0001016
#define IFUCNT_ERROR_INVALID_TYPE			0xC0001017
#define IFUCNT_ERROR_INVALID_DIVISOR		0xC0001018
#define IFUCNT_ERROR_INVALID_PWIDE			0xC0001019
#define IFUCNT_ERROR_INVALID_LOGIC			0xC000101A
#define IFUCNT_ERROR_INVALID_OUTB			0xC000101B
#define IFUCNT_ERROR_INVALID_CYCLE			0xC000101C
#define IFUCNT_ERROR_INVALID_HPULSE			0xC000101D
#define IFUCNT_ERROR_INVALID_TRG_START		0xC000101E
#define IFUCNT_ERROR_INVALID_TRG_STOP		0xC000101F
#define IFUCNT_ERROR_INVALID_TRG_CHANNEL	0xC0001020
#define IFUCNT_ERROR_INVALID_TRG_MODE		0xC0001021
#define IFUCNT_ERROR_INVALID_CMP_CHANNEL	0xC0001022
#define IFUCNT_ERROR_INVALID_CMP_OUTA		0xC0001023
#define IFUCNT_ERROR_INVALID_CMP_OUTB		0xC0001024
#define IFUCNT_ERROR_INVALID_FILTER			0xC0001025
#define IFUCNT_ERROR_INVALID_COUNTER		0xC0001026
#define IFUCNT_ERROR_INVALID_CMP_SELECT		0xC0001027
#define IFUCNT_ERROR_INVALID_COMPARATOR		0xC0001028
#define IFUCNT_ERROR_INVALID_BUFFER_SIZE	0xC0001029
#define IFUCNT_ERROR_INVALID_DATA_NUM		0xC000102A
#define IFUCNT_ERROR_INVALID_COMMAND		0xC000102B
#define IFUCNT_ERROR_INVALID_OUT_ENABLE		0xC000102C
#define IFUCNT_ERROR_INVALID_DO				0xC000102D
#define IFUCNT_ERROR_INVALID_MASK			0xC000102E

#define IFUCNT_ERROR_NO_DATA				0xC0001030
#define IFUCNT_ERROR_NO_MEMORY				0xC0001031

#define IFUCNT_ERROR_NO_DMAMEMORY			0xC0001032
#define IFUCNT_ERROR_FAILED_MEMMAP			0xC0001033


// -----------------------------------------------------------------------------
//	Reset mode identifier
// -----------------------------------------------------------------------------
#define IFUCNT_RESET_DEVICE					0x00000001
#define IFUCNT_RESET_COUNTER				0x00000002

// -----------------------------------------------------------------------------
//	Counter mode identifier
// -----------------------------------------------------------------------------
#define IFUCNT_COUNT_P_DIR_1				0x00000000
#define IFUCNT_COUNT_P_DIR_2				0x00000001
#define IFUCNT_COUNT_U_D_1					0x00000004
#define IFUCNT_COUNT_U_D_2					0x00000005
#define IFUCNT_COUNT_PHASE_1				0x00000008
#define IFUCNT_COUNT_PHASE_2				0x00000009
#define IFUCNT_COUNT_PHASE_4				0x0000000A
#define IFUCNT_COUNT_INTERNAL_CLOCK			0x0000000C

#define IFUCNT_DIR_NORMAL					0x00000000
#define IFUCNT_DIR_REVERS					0x00000010

// -----------------------------------------------------------------------------
//	Cycle measurement mode identifier
// -----------------------------------------------------------------------------
#define IFUCNT_CYCLE_NORMAL					0x00000001
#define IFUCNT_CYCLE_HIGH					0x00000002
#define IFUCNT_CYCLE_LOW					0x00000003
#define IFUCNT_CYCLE_HIGH_LOW				0x00000004

// -----------------------------------------------------------------------------
//	Phase-shifted measurement mode identifier
// -----------------------------------------------------------------------------
#define IFUCNT_PHASE_DIFF_NORMAL			0x00000001
#define IFUCNT_PHASE_DIFF_ALL				0x00000002

// -----------------------------------------------------------------------------
//	OUTB output mode identifier
// -----------------------------------------------------------------------------
#define	IFUCNT_OUTB_STOP					0x00000001
#define	IFUCNT_OUTB_OUT						0x00000002
#define	IFUCNT_OUTB_REV						0x00000003

// -----------------------------------------------------------------------------
//	Trigger setting identifier
// -----------------------------------------------------------------------------
#define	IFUCNT_LA_R							0x00000001
#define	IFUCNT_LA_F							0x00000002
#define	IFUCNT_LB_R							0x00000004
#define	IFUCNT_LB_F							0x00000008

// -----------------------------------------------------------------------------
//	Trigger mode identifier
// -----------------------------------------------------------------------------
#define	IFUCNT_TRG_NORMAL					0x00000000
#define	IFUCNT_TRG_SINGLE					0x00000001

// -----------------------------------------------------------------------------
//	Status information setting identifier
// -----------------------------------------------------------------------------
#define IFUCNT_NO_STATUS					0x00000001
#define IFUCNT_ADD_STATUS					0x00000002

// -----------------------------------------------------------------------------
//	Sampling error setting identifier
// -----------------------------------------------------------------------------
#define	IFUCNT_FREERUN						0x00000001
#define	IFUCNT_STOP_ORER					0x00000002

// -----------------------------------------------------------------------------
// Sampling status
// -----------------------------------------------------------------------------
#define	IFUCNT_SMPL_START					0x00000001
#define	IFUCNT_SMPL_STOP					0x00000002
#define	IFUCNT_SMPL_STOP_ERR				0x00000003

// -----------------------------------------------------------------------------
//	Counter control command identifier
// -----------------------------------------------------------------------------
#define	IFUCNT_CMD_START					0x00000001
#define	IFUCNT_CMD_EXWAIT					0x00000002
#define	IFUCNT_CMD_STOP						0x00000004
#define	IFUCNT_CMD_SAMPLING					0x00000100

// -----------------------------------------------------------------------------
//	General purpose digital input/output control channel identifier
// -----------------------------------------------------------------------------
#define	IFUCNT_CH_1							0x00000001
#define	IFUCNT_CH_2							0x00000002
#define	IFUCNT_CH_3							0x00000003
#define	IFUCNT_CH_4							0x00000004
#define	IFUCNT_CH_HS						0x00000100

// -----------------------------------------------------------------------------
//	Internal Sync identifier
// -----------------------------------------------------------------------------
#define	IFUCNT_SYNC1						0x00000001
#define	IFUCNT_SYNC2						0x00000002

#define	IFUCNT_DISABLE						0x00000000
#define	IFUCNT_UPDN							0x00000001

// -----------------------------------------------------------------------------
//	OS Architecture
// -----------------------------------------------------------------------------
#define IFUCNT_OS_ARCHITECTURE_x64			0x00000001
#define IFUCNT_OS_ARCHITECTURE_x86			0x00000002

// -----------------------------------------------------------------------------
//	Board information strucutre
// -----------------------------------------------------------------------------
typedef struct {
	DWORD	dwType;
	DWORD	dwBoardID;
} UCNTDEVICEINFO, *PUCNTDEVICEINFO;

// -----------------------------------------------------------------------------
//	Sampling setting information structure
// -----------------------------------------------------------------------------
typedef struct {
	DWORD	dwSmplNum;
	DWORD	dwSmplEventNum;
	DWORD	dwSmplRepeat;
	DWORD	dwStatusMode;
	DWORD	dwErrCtrl;
} UCNTSMPLCONFIG, *PUCNTSMPLCONFIG;

// -----------------------------------------------------------------------------
//	Interrupt structure
// -----------------------------------------------------------------------------
#ifdef _WIN64
	typedef void (CALLBACK UCNTCALLBACK)(INT nChannel, DWORD dwEvent, PVOID dwUser);
#else
	typedef void (CALLBACK UCNTCALLBACK)(INT nChannel, DWORD dwEvent, DWORD dwUser);
#endif

typedef UCNTCALLBACK FAR *PUCNTCALLBACK;

#ifdef _WIN64
	typedef struct {
		HWND			hWnd;
		UINT			uMessage[8];
		HANDLE			hEvent;
		PUCNTCALLBACK	pCallBackProc;
		PVOID			dwUser;
	} UCNTEVENTREQ, *PUCNTEVENTREQ;
#else
	typedef struct {
		HWND			hWnd;
		UINT			uMessage[8];
		HANDLE			hEvent;
		PUCNTCALLBACK	pCallBackProc;
		DWORD			dwUser;
	} UCNTEVENTREQ, *PUCNTEVENTREQ;
#endif


// -----------------------------------------------------------------------------
//	Function definition
// -----------------------------------------------------------------------------
#ifndef _WIN64
#define _IF_UCNT_DLL_
#endif

#if !defined(_IF_UCNT_DLL_)
#define IFUCNTAPI
#else
#define IFUCNTAPI __declspec(dllexport)
#endif

// Initializing
#ifdef _WIN64
	IFUCNTAPI HANDLE WINAPI UcntOpen(LPCSTR);
#else
	IFUCNTAPI HANDLE WINAPI UcntOpen(LPCTSTR);
#endif
IFUCNTAPI INT WINAPI UcntClose(HANDLE);
IFUCNTAPI INT WINAPI UcntGetDeviceInfo(HANDLE, PUCNTDEVICEINFO);
IFUCNTAPI INT WINAPI UcntReset(HANDLE, DWORD, DWORD);

// Setting/retrieving the motion mode
IFUCNTAPI INT WINAPI UcntSetPulseCountMode(HANDLE, INT, DWORD, DWORD, DWORD);
IFUCNTAPI INT WINAPI UcntGetPulseCountMode(HANDLE, INT, PDWORD, PDWORD, PDWORD);
IFUCNTAPI INT WINAPI UcntSetFreqAvgMode(HANDLE, INT, DWORD, DWORD);
IFUCNTAPI INT WINAPI UcntGetFreqAvgMode(HANDLE, INT, PDWORD, PDWORD);
IFUCNTAPI INT WINAPI UcntSetCycleMode(HANDLE, INT, DWORD, DWORD);
IFUCNTAPI INT WINAPI UcntGetCycleMode(HANDLE, INT, PDWORD, PDWORD);
IFUCNTAPI INT WINAPI UcntSetPhaseDiffMode(HANDLE, INT, DWORD, DWORD);
IFUCNTAPI INT WINAPI UcntGetPhaseDiffMode(HANDLE, INT, PDWORD, PDWORD);
IFUCNTAPI INT WINAPI UcntSetTimerMode(HANDLE, INT, DWORD);
IFUCNTAPI INT WINAPI UcntGetTimerMode(HANDLE, INT, PDWORD);
IFUCNTAPI INT WINAPI UcntSetFreqDividerMode(HANDLE, INT, DWORD, DWORD, DWORD, DWORD);
IFUCNTAPI INT WINAPI UcntGetFreqDividerMode(HANDLE, INT, PDWORD, PDWORD, PDWORD, PDWORD);
IFUCNTAPI INT WINAPI UcntSetPulseGeneratorMode(HANDLE, INT, DWORD, DWORD, DWORD, DWORD);
IFUCNTAPI INT WINAPI UcntGetPulseGeneratorMode(HANDLE, INT, PDWORD, PDWORD, PDWORD, PDWORD);

// Setting/retrieving the function
IFUCNTAPI INT WINAPI UcntSetLoadData(HANDLE, INT, DWORD);
IFUCNTAPI INT WINAPI UcntGetLoadData(HANDLE, INT, PDWORD);
IFUCNTAPI INT WINAPI UcntSetBaseClock(HANDLE, INT, DWORD);
IFUCNTAPI INT WINAPI UcntGetBaseClock(HANDLE, INT, PDWORD);
IFUCNTAPI INT WINAPI UcntSetTriggerConfig(HANDLE, INT, DWORD, DWORD);
IFUCNTAPI INT WINAPI UcntGetTriggerConfig(HANDLE, INT, PDWORD, PDWORD);
IFUCNTAPI INT WINAPI UcntSetTriggerChannel(HANDLE, INT, INT, DWORD);
IFUCNTAPI INT WINAPI UcntGetTriggerChannel(HANDLE, INT, PINT, PDWORD);
IFUCNTAPI INT WINAPI UcntSetComparatorConfig(HANDLE, INT, DWORD, DWORD, DWORD);
IFUCNTAPI INT WINAPI UcntGetComparatorConfig(HANDLE, INT, PDWORD, PDWORD, PDWORD);
IFUCNTAPI INT WINAPI UcntSetFilter(HANDLE, INT, DWORD);
IFUCNTAPI INT WINAPI UcntGetFilter(HANDLE, INT, PDWORD);

// Setting/retrieving the counter
IFUCNTAPI INT WINAPI UcntSetCounter(HANDLE, DWORD, PDWORD);
IFUCNTAPI INT WINAPI UcntReadCounter(HANDLE, DWORD, PDWORD);
IFUCNTAPI INT WINAPI UcntReadLatchCounter(HANDLE, DWORD, PDWORD);
IFUCNTAPI INT WINAPI UcntSetComparator(HANDLE, DWORD, DWORD, PDWORD, PDWORD);
IFUCNTAPI INT WINAPI UcntGetComparator(HANDLE, DWORD, PDWORD, PDWORD);

// Retrieving the status
IFUCNTAPI INT WINAPI UcntGetStatus(HANDLE, DWORD, PDWORD);

// Setting/retrieving the sampling
IFUCNTAPI INT WINAPI UcntSetSamplingConfig(HANDLE, INT, PUCNTSMPLCONFIG);
IFUCNTAPI INT WINAPI UcntGetSamplingConfig(HANDLE, INT, PUCNTSMPLCONFIG);
IFUCNTAPI INT WINAPI UcntGetSamplingData(HANDLE, INT, PVOID, PDWORD);
IFUCNTAPI INT WINAPI UcntClearSamplingData(HANDLE, INT);
IFUCNTAPI INT WINAPI UcntGetSamplingStatus(HANDLE, INT, PDWORD, PDWORD, PDWORD);

// Controlling the coutner
IFUCNTAPI INT WINAPI UcntStartCount(HANDLE, DWORD, DWORD);
IFUCNTAPI INT WINAPI UcntStopCount(HANDLE, DWORD, DWORD);
IFUCNTAPI INT WINAPI UcntLatchCount(HANDLE, DWORD);

// General purpose digital input/output
IFUCNTAPI INT WINAPI UcntSetDOEnable(HANDLE, DWORD);
IFUCNTAPI INT WINAPI UcntGetDOEnable(HANDLE, PDWORD);
IFUCNTAPI INT WINAPI UcntInputDI(HANDLE, INT, PDWORD);
IFUCNTAPI INT WINAPI UcntOutputDO(HANDLE, INT, DWORD);

// Setting/retrieving the interrupt
IFUCNTAPI INT WINAPI UcntSetEventMask(HANDLE, INT, DWORD);
IFUCNTAPI INT WINAPI UcntGetEventMask(HANDLE, INT, PDWORD);
IFUCNTAPI INT WINAPI UcntSetEvent(HANDLE, PUCNTEVENTREQ);
IFUCNTAPI INT WINAPI UcntKillEvent(HANDLE);

IFUCNTAPI INT WINAPI UcntOutputSync(HANDLE, INT, INT, DWORD);

IFUCNTAPI INT WINAPI UcntSetLatchMode(HANDLE, INT, DWORD);
IFUCNTAPI INT WINAPI UcntGetLatchMode(HANDLE, INT, PDWORD);

IFUCNTAPI INT WINAPI UcntSetPhysicalMemorysSize (HANDLE, INT, DWORD);
IFUCNTAPI INT WINAPI UcntGetDriverInfo (HANDLE, PDWORD, PDWORD);

#ifdef __cplusplus
}
#endif

#endif
