/*
Copyright (c) <2010> <Dreg aka David Reguera Garcia, dreg@fr33project.org>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef _VAD_H__
#define _VAD_H__

#include <Windows.h>
#include "others.h"
#include "driverusr.h"

typedef struct _DISPATCHER_HEADER                           // 30 elements, 0x10 bytes (sizeof)
{
	union                                                   // 2 elements, 0x4 bytes (sizeof)
	{
		struct                                              // 4 elements, 0x4 bytes (sizeof)
		{
			/*0x000*/             UINT8        Type;
			union                                           // 4 elements, 0x1 bytes (sizeof)
			{
				/*0x001*/                 UINT8        TimerControlFlags;
				struct                                      // 4 elements, 0x1 bytes (sizeof)
				{
					/*0x001*/                     UINT8        Absolute : 1;              // 0 BitPosition
					/*0x001*/                     UINT8        Coalescable : 1;           // 1 BitPosition
					/*0x001*/                     UINT8        KeepShifting : 1;          // 2 BitPosition
					/*0x001*/                     UINT8        EncodedTolerableDelay : 5; // 3 BitPosition
				};
				/*0x001*/                 UINT8        Abandoned;
				/*0x001*/                 UINT8        Signalling;
			};
			union                                           // 4 elements, 0x1 bytes (sizeof)
			{
				/*0x002*/                 UINT8        ThreadControlFlags;
				struct                                      // 4 elements, 0x1 bytes (sizeof)
				{
					/*0x002*/                     UINT8        CpuThrottled : 1;          // 0 BitPosition
					/*0x002*/                     UINT8        CycleProfiling : 1;        // 1 BitPosition
					/*0x002*/                     UINT8        CounterProfiling : 1;      // 2 BitPosition
					/*0x002*/                     UINT8        Reserved : 5;              // 3 BitPosition
				};
				/*0x002*/                 UINT8        Hand;
				/*0x002*/                 UINT8        Size;
			};
			union                                           // 5 elements, 0x1 bytes (sizeof)
			{
				/*0x003*/                 UINT8        TimerMiscFlags;
				struct                                      // 4 elements, 0x1 bytes (sizeof)
				{
					/*0x003*/                     UINT8        Index : 1;                 // 0 BitPosition
					/*0x003*/                     UINT8        Processor : 5;             // 1 BitPosition
					/*0x003*/                     UINT8        Inserted : 1;              // 6 BitPosition
					/*0x003*/                     UINT8        Expired : 1;               // 7 BitPosition
				};
				/*0x003*/                 UINT8        DebugActive;
				struct                                      // 5 elements, 0x1 bytes (sizeof)
				{
					/*0x003*/                     UINT8        ActiveDR7 : 1;             // 0 BitPosition
					/*0x003*/                     UINT8        Instrumented : 1;          // 1 BitPosition
					/*0x003*/                     UINT8        Reserved2 : 4;             // 2 BitPosition
					/*0x003*/                     UINT8        UmsScheduled : 1;          // 6 BitPosition
					/*0x003*/                     UINT8        UmsPrimary : 1;            // 7 BitPosition
				};
				/*0x003*/                 UINT8        DpcActive;
			};
		};
		/*0x000*/         LONG32       Lock;
	};
	/*0x004*/     LONG32       SignalState;
	/*0x008*/     struct _LIST_ENTRY WaitListHead;                        // 2 elements, 0x8 bytes (sizeof)
}DISPATCHER_HEADER, *PDISPATCHER_HEADER;


typedef struct _KEVENT                 
{
	struct _DISPATCHER_HEADER Header; 
} KEVENT, *PKEVENT;

typedef struct _FILE_OBJECT                                
{
	INT16        Type;
	INT16        Size;
	struct _DEVICE_OBJECT* DeviceObject;
	struct _VPB* Vpb;
	VOID*        FsContext;
	VOID*        FsContext2;
	VOID* SectionObjectPointer; /* struct _SECTION_OBJECT_POINTERS* SectionObjectPointer; */
	VOID*        PrivateCacheMap;
	LONG32       FinalStatus;
	struct _FILE_OBJECT* RelatedFileObject;
	UINT8        LockOperation;
	UINT8        DeletePending;
	UINT8        ReadAccess;
	UINT8        WriteAccess;
	UINT8        DeleteAccess;
	UINT8        SharedRead;
	UINT8        SharedWrite;
	UINT8        SharedDelete;
	ULONG32      Flags;
	struct _UNICODE_STRING FileName;                        
	union _LARGE_INTEGER CurrentByteOffset;                
	ULONG32      Waiters;
	ULONG32      Busy;
	VOID*        LastLock;
	struct _KEVENT Lock;                                    
	struct _KEVENT Event;                                  
	VOID* CompletionContext; /* struct _IO_COMPLETION_CONTEXT* CompletionContext; */
	ULONG32      IrpListLock;
	struct _LIST_ENTRY IrpList;                            // 2 elements, 0x8 bytes (sizeof)
	VOID*        FileObjectExtension;
} FILE_OBJECT, *PFILE_OBJECT;

#pragma pack(push,1)
typedef struct _EX_FAST_REF
{
	union
	{
		PVOID Object;
		ULONG_PTR RefCnt:3;
		ULONG_PTR Value;
	};
} EX_FAST_REF, *PEX_FAST_REF;
#pragma pack(pop)

typedef struct _CONTROL_AREA // 0x50
{
	PVOID Segment; //struct _SEGMENT* Segment; // +0x0(0x4)
	struct _LIST_ENTRY DereferenceList; // +0x4(0x8)
	ULONG NumberOfSectionReferences; // +0xc(0x4)
	ULONG NumberOfPfnReferences; // +0x10(0x4)
	ULONG NumberOfMappedViews; // +0x14(0x4)
	ULONG NumberOfUserReferences; // +0x18(0x4)
	ULONG u; // +0x1c(0x4)
	ULONG FlushInProgressCount; // +0x20(0x4)
	union
	{
		struct _EX_FAST_REF fast_ref; // +0x24(0x4)
		PFILE_OBJECT pfile_obj;
	} FilePointer;
} CONTROL_AREA,*PCONTROL_AREA;


typedef struct _MMVAD_XP
{
	UINT32 StartingVpn;
	UINT32 EndingVpn;
	struct _MMVAD* Parent;
	struct _MMVAD* LeftChild;
	struct _MMVAD* RightChild;
	ULONG32 u;
	PCONTROL_AREA ControlArea;
	struct _MMPTE* FirstPrototypePte;
	struct _MMPTE* LastContiguousPte;
	ULONG32 u2;
} MMVAD_XP, *PMMVAD_XP;

typedef struct _EX_PUSH_LOCK                
{
	union                                    
	{
		struct                                
		{
			ULONG32      Locked : 1;          
			ULONG32      Waiting : 1;        
			ULONG32      Waking : 1;          
			ULONG32      MultipleShared : 1;  
			ULONG32      Shared : 28;         
		};
		ULONG32      Value;
		VOID*        Ptr;
	};

} EX_PUSH_LOCK, *PEX_PUSH_LOCK;

typedef struct _SUBSECTION                          
{
	struct _CONTROL_AREA* ControlArea;
	PVOID SubsectionBase; /* struct _MMPTE* SubsectionBase; */
	struct _SUBSECTION* NextSubsection;
	ULONG32      PtesInSubsection;                                       
	ULONG32      UnusedPtes;
	ULONG32      LongFlags;
	ULONG32      StartingSector;
	ULONG32      NumberOfFullSectors;

} SUBSECTION, *PSUBSECTION;

typedef struct _MMVAD_VISTA                        
{
	union                                      
	{
		LONG32       Balance : 2;            
		struct _MMVAD_VISTA* Parent;
	}u1;
	struct _MMVAD_VISTA* LeftChild;
	struct _MMVAD_VISTA* RightChild;
	ULONG32      StartingVpn;
	ULONG32      EndingVpn;
	ULONG32      LongFlags;
	struct _EX_PUSH_LOCK PushLock;             
	ULONG32      LongFlags3;
	ULONG32      LongFlags2;
	struct _SUBSECTION* Subsection;

	PVOID FirstPrototypePte; /* struct _MMPTE* FirstPrototypePte; */
	PVOID LastContiguousPte; /* struct _MMPTE* LastContiguousPte; */
} MMVAD_VISTA, *PMMVAD_VISTA;

typedef struct VAD_USEFULL_s
{
	SLIST_ENTRY SingleListEntry;

	DWORD starting_vpn;
	DWORD ending_vpn;
	char dll_name[(MAX_PATH * 2) + 2];

} VAD_USEFULL_t;

typedef struct _OBJECT_NAME_INFORMATION {
	UNICODE_STRING Name;
} OBJECT_NAME_INFORMATION, *POBJECT_NAME_INFORMATION;



STATUS_t CheckVAD( HANDLE, DWORD, PSLIST_HEADER * vad_usefull_head );
VOID _CheckVAD2KXP( HANDLE, void *, PSLIST_HEADER, STATUS_t * );
VOID _CheckVADVista7( HANDLE device, void *, PSLIST_HEADER, STATUS_t * );


#endif /* _VAD_H__ */
