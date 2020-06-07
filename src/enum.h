// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// enum.h
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef AX_ENUM_H_INCLUDED
#define AX_ENUM_H_INCLUDED 1

#include <list>
#include "type.h"

enum CategoryE
{
  NULLCATGRY  = 0,    // 0 null category
  PLATFORM,           // 1 platform
  INTERFACE,          // 2 interface
  FUNCBLK             // 3 functional block
};

enum UAttrTypeE
{
  T_VOID        = 0,    // 0 void
  T_INT,                // 1 int
  T_LONG,               // 2 long
  T_DOUBLE,             // 3 double
  T_FLOAT,              // 4 float
  T_CHAR,               // 5 char
  T_CHARP,              // 6 char *
  T_STRINGC,            // 7 string
  T_STRINGAC            // 8 StringAC
};

enum CompTypeE
{
  NULLTYPE      = 0,    // 0 null type
  HARDWARE,             // 1 hardware type
  SOFTWARE              // 2 software type
};

enum PlatformE            //PlatformC.m_level
{
  PLATFM_TYPE_UNKNOWN = 0,  // 0 = null type
  CABINET,                  // 1 = generic cabinet
  PCB,                      // 2 = generic pcb
  DIE,                      // 3 = unpackaged die
  CORE                      // 4 = prepackaged core
};

enum FuncTypeE
{
  FUNC_TYPE_UNKNOWN = 0,   // 0 null type
  FUNCTION,                // 1 general functional block
  PROCESSOR,               // 2 processor
  MEMORY,                  // 3 memory
  MEMORY_CORE,             // 4 memory core
  MEMREQ,                  // 5 memory required by funcblk
  FUNC_DISTRIBUTOR,        // 6 distributor
  FUNC_COLLECTOR,          // 7 collector
  FUNC_DELAY               // 8 delay
};

enum InterfaceE
{
  INTRFC_TYPE_UNKNOWN = 0,   // 0 null type
  INTRFC_BUS,                // 1 bus type
  INTRFC_WIRE                // 2 point to point type (wire)
};

enum TaskTypeE
{
  TASK_TYPE_UNKNOWN = 0,   // 0 = null type
  ATOMIC,                  // 1 = Basic task
  HIERARCHICAL,            // 2 = Hierarchical task
  DELAY,                   // 3 = delay
  DISTRIBUTOR,             // 4 = Multirate distributor
  COLLECTOR                // 5 = Multirate collector
};

enum PortTypeE
{
  PORT_TYPE_UNKNOWN = 0,   // 0 = null type
  PORT_IN,                 // 1 = input
  PORT_OUT,                // 2 = output
  PORT_BI                  // 3 = bi-directional
};

enum ConnTypeE
{
  CONN_TYPE_UNKNOWN = 0,   // 0 = null type
  DATA,                    // 1 = data type
  CONTROL                  // 2 = control type
};

enum ArchStatusE
{
  UNASSEMBLED        = 0, // unassembled and unscheduled
  UNSCHEDULED,            // aseembled but not sheduled
  REASSEMBLE,             // Needs to be re-assembled
  SCHEDULED,              // Valied and schedlued
  RANKED,                 // Ranked by fitness function
  PRUNED,                 // Pruned
  IN_GENE_POOL,           // In current gene pool
  IN_ELITE_POOL           // In elite gene pool
};

enum TabuE
{
   SETONEVAL = 0,
   SETLIMITS,
   COMPGT,
   COMPLT,
   COMPGE,
   COMPLE,
   MUST,
   NEVER
};

struct AX_IterBlkS
{
    std::string m_name;
    int m_itercnt;
    int m_ii;
    AX_TaskPLC m_taskPL;
    int m_latency;
    int m_basedelay;
    int m_temp;
};

typedef AX_IterBlkS * AX_IterBlkSP;
typedef std::list<AX_IterBlkS> AX_IterBlkLS;
typedef std::list<AX_IterBlkS>::iterator AX_IterBlkLSIter;
typedef std::list<AX_IterBlkS>::const_iterator AX_IterBlkLSCnstIter;
typedef std::list<AX_IterBlkS *> AX_IterBlkPLS;
typedef std::list<AX_IterBlkS *>::iterator AX_IterBlkPLSIter;
typedef std::list<AX_IterBlkS *>::const_iterator AX_IterBlkPLSCnstIter;

const int LARGEINTEGER = 999999999;

typedef std::list<int> AX_IntLC;
typedef std::list<int>::iterator AX_IntLCIter;
typedef std::list<int>::const_iterator AX_IntLCCnstIter;
typedef std::list<int *> AX_IntPLC;
typedef std::list<int *>::iterator AX_IntPLCIter;
typedef std::list<int *>::const_iterator AX_IntPLCCnstIter;

const std::string UATTRSIZE = "Size";
const std::string UATTRBASECOST = "BaseCost";
const std::string UATTRCODESIZE = "CodeSize";
const std::string UATTRDATASIZE = "DataSize";
const std::string UATTRDATACOMPDELAY = "DataCompDelay";
const std::string UATTRCTRLCOMPDELAY = "ControlCompDelay";
const std::string UATTRONCHIPMEM = "OnChipMem";
const std::string UATTRONCHIPDATAMEM = "OnChipDataMem";
const std::string UATTRONCHIPCODEMEM = "OnChipCodeMem";
const std::string UATTRARCHWIDTH = "ArchWidth";
// system use dynamic user attributes
const std::string UATTRGENCAPREM = "GenCapRem";
const std::string UATTRCODECAPREM = "CodeCapRem";
const std::string UATTRDATACAPREM = "DataCapRem";

#endif   /* AX_ENUM_H_INCLUDED */
