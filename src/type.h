// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// type.h
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef DEBUG
#define DEBUG -1
#endif

#ifndef AX_TYPE_H_INCLUDED
#define AX_TYPE_H_INCLUDED 1

#include <list>
#include <string.h>

//Class Declarations in Alphabetical Order

class AX_ArchGraphC;
typedef AX_ArchGraphC * AX_ArchGraphCP;
typedef std::list<AX_ArchGraphC> AX_ArchGraphLC;
typedef std::list<AX_ArchGraphC>::iterator AX_ArchGraphLCIter;
typedef std::list<AX_ArchGraphC>::const_iterator AX_ArchGraphLCCnstIter;
typedef std::list<AX_ArchGraphC *> AX_ArchGraphPLC;
typedef std::list<AX_ArchGraphC *>::iterator AX_ArchGraphPLCIter;
typedef std::list<AX_ArchGraphC *>::const_iterator AX_ArchGraphPLCCnstIter;

class AX_CompC;
typedef AX_CompC * AX_CompCP;
typedef std::list<AX_CompC> AX_CompLC;
typedef std::list<AX_CompC>::iterator AX_CompLCIter;
typedef std::list<AX_CompC>::const_iterator AX_CompLCCnstIter;
typedef std::list<AX_CompC *> AX_CompPLC;
typedef std::list<AX_CompC *>::iterator AX_CompPLCIter;
typedef std::list<AX_CompC *>::const_iterator AX_CompPLCCnstIter;

class AX_CompPortC;
typedef AX_CompPortC * AX_CompPortCP;
typedef std::list<AX_CompPortC> AX_CompPortLC;
typedef std::list<AX_CompPortC>::iterator AX_CompPortLCIter;
typedef std::list<AX_CompPortC>::const_iterator AX_CompPortLCCnstIter;
typedef std::list<AX_CompPortC *> AX_CompPortPLC;
typedef std::list<AX_CompPortC *>::iterator AX_CompPortPLCIter;
typedef std::list<AX_CompPortC *>::const_iterator AX_CompPortPLCCnstIter;

class AX_ConnC;
typedef AX_ConnC * AX_ConnCP;
typedef std::list<AX_ConnC> AX_ConnLC;
typedef std::list<AX_ConnC>::iterator AX_ConnLCIter;
typedef std::list<AX_ConnC>::const_iterator AX_ConnLCCnstIter;
typedef std::list<AX_ConnC *> AX_ConnPLC;
typedef std::list<AX_ConnC *>::iterator AX_ConnPLCIter;
typedef std::list<AX_ConnC *>::const_iterator AX_ConnPLCCnstIter;

class AX_DsnC;
typedef AX_DsnC * AX_DsnCP;
typedef std::list<AX_DsnC> AX_DsnLC;
typedef std::list<AX_DsnC>::iterator AX_DsnLCIter;
typedef std::list<AX_DsnC>::const_iterator AX_DsnLCCnstIter;
typedef std::list<AX_DsnC *> AX_DsnPLC;
typedef std::list<AX_DsnC *>::iterator AX_DsnPLCIter;
typedef std::list<AX_DsnC *>::const_iterator AX_DsnPLCCnstIter;

class AX_DsnInfoC;
typedef AX_DsnInfoC * AX_DsnInfoCP;
typedef std::list<AX_DsnInfoC> AX_DsnInfoLC;
typedef std::list<AX_DsnInfoC>::iterator AX_DsnInfoLCIter;
typedef std::list<AX_DsnInfoC>::const_iterator AX_DsnInfoLCCnstIter;
typedef std::list<AX_DsnInfoC *> AX_DsnInfoPLC;
typedef std::list<AX_DsnInfoC *>::iterator AX_DsnInfoPLCIter;
typedef std::list<AX_DsnInfoC *>::const_iterator AX_DsnInfoPLCCnstIter;

class AX_ExpResC;
typedef AX_ExpResC * AX_ExpResCP;
typedef std::list<AX_ExpResC> AX_ExpResLC;
typedef std::list<AX_ExpResC>::iterator AX_ExpResLCIter;
typedef std::list<AX_ExpResC>::const_iterator AX_ExpResLCCnstIter;
typedef std::list<AX_ExpResC *> AX_ExpResPLC;
typedef std::list<AX_ExpResC *>::iterator AX_ExpResPLCIter;
typedef std::list<AX_ExpResC *>::const_iterator AX_ExpResPLCCnstIter;

class AX_FuncBlkC;
typedef AX_FuncBlkC * AX_FuncBlkCP;
typedef std::list<AX_FuncBlkC> AX_FuncBlkLC;
typedef std::list<AX_FuncBlkC>::iterator AX_FuncBlkLCIter;
typedef std::list<AX_FuncBlkC>::const_iterator AX_FuncBlkLCCnstIter;
typedef std::list<AX_FuncBlkC *> AX_FuncBlkPLC;
typedef std::list<AX_FuncBlkC *>::iterator AX_FuncBlkPLCIter;
typedef std::list<AX_FuncBlkC *>::const_iterator AX_FuncBlkPLCCnstIter;

class AX_HistC;
typedef AX_HistC * AX_HistCP;
typedef std::list<AX_HistC> AX_HistLC;
typedef std::list<AX_HistC>::iterator AX_HistLCIter;
typedef std::list<AX_HistC>::const_iterator AX_HistLCCnstIter;
typedef std::list<AX_HistC *> AX_HistPLC;
typedef std::list<AX_HistC *>::iterator AX_HistPLCIter;
typedef std::list<AX_HistC *>::const_iterator AX_HistPLCCnstIter;

typedef int * AX_IntCP;
typedef std::list<int> AX_IntLC;
typedef std::list<int>::iterator AX_IntLCIter;
typedef std::list<int>::const_iterator AX_IntLCCnstIter;
typedef std::list<int *> AX_IntPLC;
typedef std::list<int *>::iterator AX_IntPLCIter;
typedef std::list<int *>::const_iterator AX_IntPLCCnstIter;

class AX_InterfaceC;
typedef AX_InterfaceC * AX_InterfaceCP;
typedef std::list<AX_InterfaceC> AX_InterfaceLC;
typedef std::list<AX_InterfaceC>::iterator AX_InterfaceLCIter;
typedef std::list<AX_InterfaceC>::const_iterator AX_InterfaceLCCnstIter;
typedef std::list<AX_InterfaceC *> AX_InterfacePLC;
typedef std::list<AX_InterfaceC *>::iterator AX_InterfacePLCIter;
typedef std::list<AX_InterfaceC *>::const_iterator AX_InterfacePLCCnstIter;

class AX_PlatformC;
typedef AX_PlatformC * AX_PlatformCP;
typedef std::list<AX_PlatformC> AX_PlatformLC;
typedef std::list<AX_PlatformC>::iterator AX_PlatformLCIter;
typedef std::list<AX_PlatformC>::const_iterator AX_PlatformLCCnstIter;
typedef std::list<AX_PlatformC *> AX_PlatformPLC;
typedef std::list<AX_PlatformC *>::iterator AX_PlatformPLCIter;
typedef std::list<AX_PlatformC *>::const_iterator AX_PlatformPLCCnstIter;

class AX_TaskC;
typedef AX_TaskC * AX_TaskCP;
typedef std::list<AX_TaskC> AX_TaskLC;
typedef std::list<AX_TaskC>::iterator AX_TaskLCIter;
typedef std::list<AX_TaskC>::const_iterator AX_TaskLCCnstIter;
typedef std::list<AX_TaskC *> AX_TaskPLC;
typedef std::list<AX_TaskC *>::iterator AX_TaskPLCIter;
typedef std::list<AX_TaskC *>::const_iterator AX_TaskPLCCnstIter;

class AX_TaskGraphC;
typedef AX_TaskGraphC * AX_TaskGraphCP;
typedef std::list<AX_TaskGraphC> AX_TaskGraphLC;
typedef std::list<AX_TaskGraphC>::iterator AX_TaskGraphLCIter;
typedef std::list<AX_TaskGraphC>::const_iterator AX_TaskGraphLCCnstIter;
typedef std::list<AX_TaskGraphC *> AX_TaskGraphPLC;
typedef std::list<AX_TaskGraphC *>::iterator AX_TaskGraphPLCIter;
typedef std::list<AX_TaskGraphC *>::const_iterator AX_TaskGraphPLCCnstIter;

class AX_TaskPortC;
typedef AX_TaskPortC * AX_TaskPortCP;
typedef std::list<AX_TaskPortC> AX_TaskPortLC;
typedef std::list<AX_TaskPortC>::iterator AX_TaskPortLCIter;
typedef std::list<AX_TaskPortC>::const_iterator AX_TaskPortLCCnstIter;
typedef std::list<AX_TaskPortC *> AX_TaskPortPLC;
typedef std::list<AX_TaskPortC *>::iterator AX_TaskPortPLCIter;
typedef std::list<AX_TaskPortC *>::const_iterator AX_TaskPortPLCCnstIter;

class AX_UAttrC;
typedef AX_UAttrC * AX_UAttrCP;
typedef std::list<AX_UAttrC> AX_UAttrLC;
typedef std::list<AX_UAttrC>::iterator AX_UAttrLCIter;
typedef std::list<AX_UAttrC>::const_iterator AX_UAttrLCCnstIter;
typedef std::list<AX_UAttrC *> AX_UAttrPLC;
typedef std::list<AX_UAttrC *>::iterator AX_UAttrPLCIter;
typedef std::list<AX_UAttrC *>::const_iterator AX_UAttrPLCCnstIter;

class AX_BusC;
typedef AX_BusC * AX_BusCP;
typedef std::list<AX_BusC> AX_BusLC;
typedef std::list<AX_BusC>::iterator AX_BusLCIter;
typedef std::list<AX_BusC>::const_iterator AX_BusLCCnstIter;
typedef std::list<AX_BusC *> AX_BusPLC;
typedef std::list<AX_BusC *>::iterator AX_BusPLCIter;
typedef std::list<AX_BusC *>::const_iterator AX_BusPLCCnstIter;

class AX_ConnPntC;
typedef AX_ConnPntC * AX_ConnPntCP;
typedef std::list<AX_ConnPntC> AX_ConnPntLC;
typedef std::list<AX_ConnPntC>::iterator AX_ConnPntLCIter;
typedef std::list<AX_ConnPntC>::const_iterator AX_ConnPntLCCnstIter;
typedef std::list<AX_ConnPntC *> AX_ConnPntPLC;
typedef std::list<AX_ConnPntC *>::iterator AX_ConnPntPLCIter;
typedef std::list<AX_ConnPntC *>::const_iterator AX_ConnPntPLCCnstIter;

class AX_ShareC;
typedef AX_ShareC * AX_ShareCP;
typedef std::list<AX_ShareC> AX_ShareLC;
typedef std::list<AX_ShareC>::iterator AX_ShareLCIter;
typedef std::list<AX_ShareC>::const_iterator AX_ShareLCCnstIter;
typedef std::list<AX_ShareC *> AX_SharePLC;
typedef std::list<AX_ShareC *>::iterator AX_SharePLCIter;
typedef std::list<AX_ShareC *>::const_iterator AX_SharePLCCnstIter;

#endif /* AX_TYPE_H_INCLUDED */
