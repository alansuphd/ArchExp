// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// archgraph.h
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef AX_ARCHGRAPH_H_INCLUDED
#define AX_ARCHGRAPH_H_INCLUDED 1

#include <list>
#include <fstream>
#include <string.h>
#include "type.h"
#include "enum.h"

#ifndef AX_BUS_H_INCLUDED
#include "bus.h"
#endif /* AX_BUS_H_INCLUDED */

#ifndef AX_COMPPORT_H_INCLUDED
#include "compport.h"
#endif /* AX_COMPPORT_H_INCLUDED */

#ifndef AX_FUNCBLK_H_INCLUDED
#include "funcblk.h"
#endif /* AX_FUNCBLK_H_INCLUDED */

#ifndef AX_INTERFACE_H_INCLUDED
#include "interface.h"
#endif /* AX_INTERFACE_H_INCLUDED */

#ifndef AX_PLATFORM_H_INCLUDED
#include "platform.h"
#endif /* AX_PLATFORM_H_INCLUDED */

#ifndef AX_TASK_H_INCLUDED
#include "task.h"
#endif /* AX_TASK_H_INCLUDED */

#ifndef AX_SHARE_H_INCLUDED
#include "share.h"
#endif /* AX_SHARE_H_INCLUDED */

#ifndef AX_UTIL_H_INCLUDED
#include "util.h"
#endif /* AX_UTIL_H_INCLUDED */

using namespace std;

enum CandidateTypeE
{
    GET_ANY_TYPE = 0,      // get any types of interface
    GET_BUS_ONLY,          // want bus only
    GET_WIRE_ONLY
};

struct AX_SchdTimeS
{
    AX_TaskCP m_taskP;         // task
    AX_FuncBlkCP m_fncblkP;    // fncblk the task execute on
    AX_ConnCP m_connP;         // connection
    AX_BusCP m_busP;           // bus this connection maps to
    int m_start;                // start time
    int m_finish;               // finish time
    int m_temp;
                                // {m_taskP,m_fncblkP} and
                                // {m_connP,m_busP} are mutual exclusive
};

typedef AX_SchdTimeS * AX_SchdTimeSP;
typedef std::list<AX_SchdTimeS> AX_SchdTimeLS;
typedef std::list<AX_SchdTimeS>::iterator AX_SchdTimeLSIter;
typedef std::list<AX_SchdTimeS>::const_iterator AX_SchdTimeLSCnstIter;
typedef std::list<AX_SchdTimeS *> AX_SchdTimePLS;
typedef std::list<AX_SchdTimeS *>::iterator AX_SchdTimePLSIter;
typedef std::list<AX_SchdTimeS *>::const_iterator AX_SchdTimePLSCnstIter;

// Forward Declarations ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class AX_ArchGraphC;

// Typedef Definitions for ArchGraphC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef AX_ArchGraphC * AX_ArchGraphCP;
typedef std::list<AX_ArchGraphC> AX_ArchGraphLC;
typedef std::list<AX_ArchGraphC>::iterator AX_ArchGraphLCIter;
typedef std::list<AX_ArchGraphC>::const_iterator AX_ArchGraphLCCnstIter;
typedef std::list<AX_ArchGraphC *> AX_ArchGraphPLC;
typedef std::list<AX_ArchGraphC *>::iterator AX_ArchGraphPLCIter;
typedef std::list<AX_ArchGraphC *>::const_iterator AX_ArchGraphPLCCnstIter;

// Class Definition for ArchGraphC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class AX_ArchGraphC
{
private:

    std::string m_name;
    int m_id;
    AX_DsnCP m_dsnP;
    ArchStatusE m_status;

    AX_CompPortLC m_ag_inportL; // List of graph input ports
    AX_CompPortLC m_ag_outportL; // List of graph output ports

    AX_FuncBlkLC m_fncblkL; // List of pointers to functional blocks
    AX_InterfaceLC m_intrfcL; // List of pointers to interfaces
    AX_PlatformLC m_platfmL; // List of pointers to platforms

    AX_Gen_SystemCPLC m_gsccL; // List of pointers to Gen SystemC codes

    AX_CompPortLC m_compportL; // List of instances of component ports
    AX_BusLC m_busL; // List of buses

    AX_SchdTimeLS m_schdtimeL; // list of schedules
    int m_latency;      // overall latency
    int m_area;         // overall area
    int m_power;        // overall power
    float m_cost;        // overall cost
    float m_fitness;    // fitness value

    int m_temp;

public:
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructor
  // Had to remove the const descriptor from the operator=.
  // Stupid C++ does not allow const_iterator to be used
  // to access the class instance member data and functions
  // it refers to.
     AX_ArchGraphC();
     AX_ArchGraphC(const AX_ArchGraphC & other);
     AX_ArchGraphC(AX_ArchGraphC & other);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Destructor
     ~AX_ArchGraphC();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Assignator & Comparators
  // Had to remove the const descriptor from the operator=.
  // Stupid C++ does not allow const_iterator to be used
  // to access the class instance member data and functions
  // it refers to.
     AX_ArchGraphC & operator= (const AX_ArchGraphC & other);
     AX_ArchGraphC & operator= (AX_ArchGraphC & other);
     bool operator== (const AX_ArchGraphC & other);
     bool operator!= (const AX_ArchGraphC & other) 
     {
        return(!(*this == other));
     }

     void copy(AX_ArchGraphC & ag2);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Sets & Gets
     void set_name(std::string & name){m_name = name;}
     std::string get_name(){return m_name;}

     void set_id(int id){m_id = id;}
     int get_id(){return m_id;}

     void set_dsn(AX_DsnCP dsnP){m_dsnP = dsnP;}
     AX_DsnCP get_dsn(){return m_dsnP;}

     void set_status(ArchStatusE status){m_status = status;}
     ArchStatusE get_status(){return m_status;}

     void set_latency(int latency){m_latency = latency;};
     int get_latency(){return m_latency;};

     void set_area(int area){m_area = area;};
     int get_area(){return m_area;};

     void set_power(int power){m_power = power;};
     int get_power(){return m_power;};

     void set_cost(float cost){m_cost = cost;};
     float get_cost(){return m_cost;};

     void set_fitness(float fitness){m_fitness = fitness;};
     float get_fitness(){return m_fitness;};

     void set_temp(int temp){m_temp = temp;}
     int get_type(){return m_temp;}

  // ~~~~~~~~~~~~~~~~ Function Block Pointer List Manipulations
     AX_FuncBlkLCIter add_new_fncblk(AX_FuncBlkC & fncblk);
     AX_FuncBlkLCIter add_fncblk(AX_FuncBlkC & fncblk);

     AX_FuncBlkCP get_fncblk(int index);
     AX_FuncBlkCP get_fncblk(std::string name);
     AX_FuncBlkCP get_fncblk_by_id(int id);
     AX_FuncBlkCP get_fncblk_by_task(AX_TaskCP taskP);
     void set2_first_fncblk(AX_FuncBlkLCIter & fIter)
     {
        fIter = m_fncblkL.begin();
     }
     AX_FuncBlkLCIter get2_first_fncblk(){return m_fncblkL.begin();}
     void set2_last_fncblk(AX_FuncBlkLCIter & fIter)
     {
        fIter = --m_fncblkL.end();
     }
     AX_FuncBlkLCIter get2_last_fncblk(){return --m_fncblkL.end();}
     AX_FuncBlkLCIter fncblk_list_end(){return m_fncblkL.end();}

     int get_no_fncblks(){return m_fncblkL.size();}

     void remove_fncblk(AX_FuncBlkLCIter & iter){m_fncblkL.erase(iter);}
     void remove_fncblk(AX_FuncBlkCP fncblkP);
     AX_FuncBlkLCIter delete_fncblk(AX_FuncBlkLCIter & iter);
     AX_FuncBlkLCIter delete_fncblk(AX_FuncBlkCP fncblkP);
     void remove_all_fncblks();

     AX_FuncBlkCP find_fncblk_by_task(AX_TaskCP task);
     void erase_dep_fm_fncblk(AX_FuncBlkCP funcP, AX_TaskCP taskP);

     AX_FuncBlkCP pick_largest_mem(int & capRem);
     int get_latency_by_task(AX_TaskCP taskP);
     bool fncblk_exists(AX_FuncBlkCP funcP);
     void clean_taskref_in_proc();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Interface List Manipulations
     AX_InterfaceLCIter add_new_intrfc(AX_InterfaceC & intrfc);
     AX_InterfaceLCIter add_intrfc(AX_InterfaceC & intrfc);

     AX_InterfaceCP get_intrfc(int index);
     AX_InterfaceCP get_intrfc(std::string name);
     AX_InterfaceCP get_intrfc_by_id(int id);
     void set2_first_intrfc(AX_InterfaceLCIter & iIter)
     {
        iIter = m_intrfcL.begin();
     }
     AX_InterfaceLCIter get2_first_intrfc(){return m_intrfcL.begin();}
     void set2_last_intrfc(AX_InterfaceLCIter & iIter)
     {
        iIter = --m_intrfcL.end();
     }
     AX_InterfaceLCIter get2_last_intrfc(){return --m_intrfcL.end();}
     AX_InterfaceLCIter intrfc_list_end(){return m_intrfcL.end();}

     AX_InterfaceLC & get_intrfcL(){return m_intrfcL;};
     int get_no_intrfcs(){return m_intrfcL.size();}

     void remove_intrfc(AX_InterfaceLCIter & iter){m_intrfcL.erase(iter);}
     void remove_intrfc(AX_InterfaceCP intrfcP);
     AX_InterfaceLCIter delete_intrfc(AX_InterfaceLCIter & iter);
     void remove_all_intrfcs();

     int get_intrfc_candidates(AX_CompPortCP srcPortP,
                               AX_CompPortCP sinkPortP,
                               AX_InterfacePLC & intrfcPL,
                               CandidateTypeE cType);
     bool get_intrfcPL_fm_port(AX_CompPortCP portP,
                                  AX_InterfacePLC & intrfcPL,
                                  bool isSrc, CandidateTypeE cType);
     bool get_intrfcPL_fm_port(AX_CompPortCP srcPortP,
                                  AX_CompPortCP sinkPortP,
                                  AX_InterfacePLC & intrfcPL,
                                  CandidateTypeE cType);

     bool collect_all_bus_intrfcs(AX_InterfacePLC & collectPL);
     bool collect_all_wire_intrfcs(AX_InterfacePLC & collectPL);

  // ~~~~~~~~~~~~~~~~~~~~~~ Platform Pointer List Manipulations
     AX_PlatformLCIter add_new_platfm(AX_PlatformC & platfm);
     AX_PlatformLCIter add_platfm(AX_PlatformC & platfm);

     AX_PlatformCP get_platfm(int index);
     AX_PlatformCP get_platfm(std::string name);
     AX_PlatformCP get_platfm_by_id(int id);
     void set2_first_platfm(AX_PlatformLCIter & pIter)
     {
        pIter = m_platfmL.begin();
     }
     AX_PlatformLCIter get2_first_platfm(){return m_platfmL.begin();}
     void set2_last_platfm(AX_PlatformLCIter & pIter)
     {
        pIter = --m_platfmL.end();
     }
     AX_PlatformLCIter get2_last_platfm(){return --m_platfmL.end();}
     AX_PlatformLCIter platfm_list_end(){return m_platfmL.end();}

     int get_no_platfms(){return m_platfmL.size();}

     void remove_platfm(AX_PlatformLCIter & iter){m_platfmL.erase(iter);}
     void remove_platfm(AX_PlatformCP platfmP);
     AX_PlatformLCIter delete_platfm(AX_PlatformLCIter & iter);
     void remove_all_platfms();

  // ~~~~~~~~~~~~~~~~ AG Input Port Instance List Manipulations
     AX_CompPortLCIter create_new_ag_inport();
     AX_CompPortLCIter add_new_ag_inport(AX_CompPortC & compport);
     AX_CompPortLCIter add_new_ag_inport(std::string);

     AX_CompPortCP get_ag_inport(int index);
     AX_CompPortCP get_ag_inport(std::string name);
     void set2_first_ag_inport(AX_CompPortLCIter & pIter)
     {
        pIter = m_ag_inportL.begin();
     }
     AX_CompPortLCIter get2_first_ag_inport(){return m_ag_inportL.begin();}
     void set2_last_ag_inport(AX_CompPortLCIter & pIter)
     {
        pIter = --m_ag_inportL.end();
     }
     AX_CompPortLCIter get2_last_ag_inport(){return --m_ag_inportL.end();}
     AX_CompPortLCIter ag_inport_list_end(){return m_ag_inportL.end();}

     int get_no_ag_inports(){return m_ag_inportL.size();}

     void remove_ag_inport(AX_CompPortLCIter & iter)
     {
        m_ag_inportL.erase(iter);
     }
     void remove_ag_inport(AX_CompPortCP compportP);
     AX_CompPortLCIter delete_ag_inport(AX_CompPortLCIter & iter);
     void remove_all_ag_inports();
     void reset_ag_inport_temp();

  // ~~~~~~~~~~~~~~~~ AG Input Port Instance List Manipulations
     AX_CompPortLCIter create_new_ag_outport();
     AX_CompPortLCIter add_new_ag_outport(AX_CompPortC & compport);
     AX_CompPortLCIter add_new_ag_outport(std::string);

     AX_CompPortCP get_ag_outport(int index);
     AX_CompPortCP get_ag_outport(std::string name);
     void set2_first_ag_outport(AX_CompPortLCIter & pIter)
     {
        pIter = m_ag_outportL.begin();
     }
     AX_CompPortLCIter get2_first_ag_outport(){return m_ag_outportL.begin();}
     void set2_last_ag_outport(AX_CompPortLCIter & pIter)
     {
        pIter = --m_ag_outportL.end();
     }
     AX_CompPortLCIter get2_last_ag_outport(){return --m_ag_outportL.end();}
     AX_CompPortLCIter ag_outport_list_end(){return m_ag_outportL.end();}

     int get_no_ag_outports(){return m_ag_outportL.size();}

     void remove_ag_outport(AX_CompPortLCIter & iter)
     {
        m_ag_outportL.erase(iter);
     }
     void remove_ag_outport(AX_CompPortCP compportP);
     AX_CompPortLCIter delete_ag_outport(AX_CompPortLCIter & iter);
     void remove_all_ag_outports();
     void reset_ag_outport_temp();

  // ~~~~~~~~~~~~~ AG Internal Port Instance List Manipulations
     AX_CompPortLCIter create_new_compport();
     AX_CompPortLCIter add_new_compport(AX_CompPortC & compport);
     AX_CompPortLCIter add_new_compport(std::string name);
     AX_CompPortLCIter add_compport(AX_CompPortC & compport);
     void add_compports_by_fncblk(AX_FuncBlkCP oFuncP,
                                  AX_FuncBlkCP nFuncP, int & portId);
     void add_compports_by_task(AX_TaskCP taskP, AX_FuncBlkCP oFuncP,
                                  AX_FuncBlkCP nFuncP, int & portId);
     bool change_compports_owner(AX_FuncBlkCP oFuncP,
                                    AX_FuncBlkCP nFuncP,
                                    AX_FuncBlkCP pFuncP,
                                    AX_TaskCP taskP);
     void collect_oth_ports(AX_TaskPortCP portP, AX_ConnPntLC & connPntL,
                            AX_ConnPLC & connL, bool input);

     AX_CompPortCP get_compport(int index);
     AX_CompPortCP get_compport(AX_FuncBlkCP funcP, std::string name);
     AX_CompPortCP get_compport(AX_TaskCP taskP, std::string name);
     AX_CompPortCP get_compport_by_id(int id);
     void set2_first_compport(AX_CompPortLCIter & pIter)
     {
        pIter = m_compportL.begin();
     }
     AX_CompPortLCIter get2_first_compport(){return m_compportL.begin();}
     void set2_last_compport(AX_CompPortLCIter & pIter)
     {
        pIter = --m_compportL.end();
     }
     AX_CompPortLCIter get2_last_compport(){return --m_compportL.end();}
     AX_CompPortLCIter compport_list_end(){return m_compportL.end();}

     int get_no_compports(){return m_compportL.size();}

     void remove_compport(AX_CompPortLCIter & iter){m_compportL.erase(iter);}
     void remove_compport(AX_CompPortCP compportP);
     AX_CompPortLCIter delete_compport(AX_CompPortLCIter & iter);
     void delete_compports_by_fncblk(AX_FuncBlkCP funcP);
     void delete_compports_by_fncblk(AX_FuncBlkCP funcP,
                                     AX_FuncBlkCP orgFuncP,
                                     AX_TaskCP taskP);
     void remove_all_compports();
     void reset_compport_temp();
     void adjust_refCnt(AX_CompPortCP portP);

  // ~~~~~~~~~~~~~ AG Bus Instance List Manipulations
     AX_BusLCIter create_new_bus();
     AX_BusLCIter add_new_bus(AX_BusC & bus);

     AX_BusCP get_bus(int index);
     AX_BusCP get_bus_by_id(int id);
     AX_BusCP get_bus_by_intrfc(int id, AX_InterfaceCP intrP);
     AX_BusCP get_bus_by_conn(AX_ConnCP connP);
     void set2_first_bus(AX_BusLCIter & wIter)
     {
        wIter = m_busL.begin();
     }
     AX_BusLCIter get2_first_bus(){return m_busL.begin();}
     void set2_last_bus(AX_BusLCIter & wIter)
     {
        wIter = --m_busL.end();
     }
     AX_BusLCIter get2_last_bus(){return --m_busL.end();}
     AX_BusLCIter bus_list_end(){return m_busL.end();}

     int get_no_buses(){return m_busL.size();}
     int get_no_buses_by_intrfc(AX_InterfaceCP intrP);
     AX_BusCP ran_pick_bus(AX_InterfaceCP intrP, int & Cnt);

     void remove_bus(AX_BusLCIter & iter){m_busL.erase(iter);}
     void remove_bus(AX_BusCP busP);
     AX_BusLCIter delete_bus(AX_BusLCIter & iter);
     AX_BusLCIter delete_bus(AX_BusCP busP);
     void remove_all_buses();
     void clean_all_bus_connrefs();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~ Schedule Time List Manipulations
     AX_SchdTimeLSIter create_new_schdtime();
     AX_SchdTimeLSIter add_new_schdtime(AX_SchdTimeS schdtime);

     AX_SchdTimeSP get_schdtime(int index);
     AX_SchdTimeLSIter get_schdtime_by_task(AX_TaskCP taskP);
     AX_SchdTimeLSIter get_schdtime_by_fncblk(AX_FuncBlkCP funcP);
     AX_SchdTimeLSIter get_schdtime_by_conn(AX_ConnCP connP);
     AX_SchdTimeLSIter get_schdtime_by_bus(AX_BusCP busP);

     void set2_first_schdtime(AX_SchdTimeLSIter & iIter)
     {
         iIter = m_schdtimeL.begin();
     }
     AX_SchdTimeLSIter get2_first_schdtime(){return m_schdtimeL.begin();}
     void set2_last_schdtime(AX_SchdTimeLSIter & iIter)
     {
         iIter = --m_schdtimeL.end();
     }
     AX_SchdTimeLSIter get2_last_schdtime(){return --m_schdtimeL.end();}
     AX_SchdTimeLSIter schdtime_list_end(){return m_schdtimeL.end();}

     int get_no_schdtimes(){return m_schdtimeL.size();}

     void remove_schdtime(AX_SchdTimeLSIter & iter){m_schdtimeL.erase(iter);}
     void remove_schdtime(AX_SchdTimeSP schdtimeP);
     AX_SchdTimeLSIter delete_schdtime(AX_SchdTimeLSIter & iter);
     void remove_all_schdtimes();
     void schdtimes_dump();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Sorting Functions
     void sort_fncblk_by_id();
     void sort_compport_by_id();
     void sort_bus_by_id();

     void renum_fncblk_id();
     void renum_compport_id();
     void renum_bus_id();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Special Functions

     void delete_bus_fm_ports(AX_BusCP busP);
     int get_sharable_lists(AX_TaskGraphC & tg, AX_ShareLC & shareL);
     int get_arch_latency(AX_TaskGraphC & tg);
     void compute_utilization(AX_TaskGraphC & tg);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Miscellaneous Functions
     void clean_up();
     void dump();
     void dump_simple();
     void dump_small();
     void write(std::fstream & agFile);
     void write_small(std::fstream & agFile);
     void write_simple(std::fstream & agFile);
     void write_SystemC( string folder );

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~End of Class Definition
};

#endif /* AX_ARCHGRAPH_H_INCLUDED */
