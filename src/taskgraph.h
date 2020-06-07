// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// taskgraph.h
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef AX_TASKGRAPH_H_INCLUDED
#define AX_TASKGRAPH_H_INCLUDED 1

#include <list>
#include <fstream>
#include "type.h"
#include "enum.h"

#ifndef AX_TASK_H_INCLUDED
#include "task.h"
#endif /* AX_TASK_H_INCLUDED */

#ifndef AX_TASKPORT_H_INCLUDED
#include "taskport.h"
#endif /* AX_TASKPORT_H_INCLUDED */

#ifndef AX_CONN_H_INCLUDED
#include "conn.h"
#endif /* AX_CONN_H_INCLUDED */

// Forward Declarations ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

struct AX_CycleS
{
    AX_TaskCP m_headP;
    int m_cycle_latency;
    int m_lcd;
};

typedef AX_CycleS * AX_CycleSP;
typedef std::list<AX_CycleS> AX_CycleLS;
typedef std::list<AX_CycleS>::iterator AX_CycleLSIter;
typedef std::list<AX_CycleS>::const_iterator AX_CycleLSCnstIter;
typedef std::list<AX_CycleS *> AX_CyclePLS;
typedef std::list<AX_CycleS *>::iterator AX_CyclePLSIter;
typedef std::list<AX_CycleS *>::const_iterator AX_CyclePLSCnstIter;

// Class Definition for TaskGraphC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class AX_TaskGraphC
{
private:

    std::string m_name;
    int m_id;
    int m_db_idx;
    AX_DsnC * m_dsnP;

    // The following four variables are for flattened TG
    // Or for top-level TG of a hierarchical suite
    AX_TaskCP m_start_taskP;
    AX_TaskCP m_finish_taskP;

    // The following four variables are used for sub-taskgraphs   
    // Of a hierarchical suite
    AX_TaskPortLC m_tg_inportL; // List of TG input taskports
    AX_TaskPortLC m_tg_outportL; // List of TG output taskports

    AX_TaskLC m_taskL; // List of system tasks
    AX_ConnLC m_connL; // List of system conns
    AX_TaskPortLC m_taskportL; // List of taskports
    AX_IterBlkLS m_iterblkL; // list of iteration blocks
    AX_CycleLS m_cycleL; // list of cycle information
    int m_period; // periodicity

    int m_temp;

public:
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructor
     AX_TaskGraphC();
     AX_TaskGraphC(AX_TaskGraphC & other);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Destructor
     ~AX_TaskGraphC();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Assignator & Comparators
     AX_TaskGraphC & operator= (AX_TaskGraphC & other);
     bool copy(AX_TaskGraphC & other);
     bool operator== (const AX_TaskGraphC & other);
     bool operator!= (const AX_TaskGraphC & other) 
     {
        return(!(*this == other));
     }

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Sets & Gets
     void set_name(std::string & name){m_name = name;}
     std::string get_name(){return m_name;}

     void set_id(int id){m_id = id;}
     int get_id(){return m_id;}

     void set_db_idx(int idx){m_db_idx = idx;}
     int get_db_idx(){return m_db_idx;}

     void set_dsn(AX_DsnCP dsnP){m_dsnP = dsnP;}
     AX_DsnCP get_dsn(){return m_dsnP;}

     void set_start_task(AX_TaskCP taskP){m_start_taskP = taskP;}
     AX_TaskCP get_start_task(){return m_start_taskP;}

     void set_finish_task(AX_TaskCP taskP){m_finish_taskP = taskP;}
     AX_TaskCP get_finish_task(){return m_finish_taskP;}

     void set_period(int period){m_period = period;}
     int get_period(){return m_period;}

     void set_temp(int temp){m_temp = temp;}
     int get_temp(){return m_temp;}

  // ~~~~~~~~~~~~~~~~~~ In Taskport Instance List Manipulations
     AX_TaskPortLCIter create_new_tg_inport();
     AX_TaskPortLCIter create_new_tg_inport(std::string & name);
     AX_TaskPortLCIter add_new_tg_inport(AX_TaskPortC & taskport);

     AX_TaskPortCP get_tg_inport(int index);
     AX_TaskPortLCIter get_tg_inport(std::string name);
     AX_TaskPortLCIter get_tg_inport(AX_TaskPortCP taskportP);
     AX_TaskPortCP get_tg_inport_by_id(int id);

     void set2_first_tg_inport(AX_TaskPortLCIter & pIter)
     {
         pIter = m_tg_inportL.begin();
     }
     AX_TaskPortLCIter get2_first_tg_inport(){return m_tg_inportL.begin();}
     void set2_last_tg_inport(AX_TaskPortLCIter & pIter)
     {
         pIter = --m_tg_inportL.end();
     }
     AX_TaskPortLCIter get2_last_tg_inport(){return --m_tg_inportL.end();}
     AX_TaskPortLCIter tg_inport_list_end(){return m_tg_inportL.end();}

     int get_no_tg_inports(){return m_tg_inportL.size();}

     void remove_tg_inport(AX_TaskPortLCIter & iter){m_tg_inportL.erase(iter);}
     void remove_tg_inport(AX_TaskPortCP taskportP);
     AX_TaskPortLCIter delete_tg_inport(AX_TaskPortLCIter & iter);
     void remove_all_tg_inports();

  // ~~~~~~~~~~~~~~~~~ Out Taskport Instance List Manipulations
     AX_TaskPortLCIter create_new_tg_outport();
     AX_TaskPortLCIter create_new_tg_outport(std::string & name);
     AX_TaskPortLCIter add_new_tg_outport(AX_TaskPortC & taskport);

     AX_TaskPortCP get_tg_outport(int index);
     AX_TaskPortLCIter get_tg_outport(std::string name);
     AX_TaskPortLCIter get_tg_outport(AX_TaskPortCP taskportP);
     AX_TaskPortCP get_tg_outport_by_id(int id);

     void set2_first_tg_outport(AX_TaskPortLCIter & pIter)
     {
         pIter = m_tg_outportL.begin();
     }
     AX_TaskPortLCIter get2_first_tg_outport(){return m_tg_outportL.begin();}
     void set2_last_tg_outport(AX_TaskPortLCIter & pIter)
     {
         pIter = --m_tg_outportL.end();
     }
     AX_TaskPortLCIter get2_last_tg_outport(){return --m_tg_outportL.end();}
     AX_TaskPortLCIter tg_outport_list_end(){return m_tg_outportL.end();}

     int get_no_tg_outports(){return m_tg_outportL.size();}

     void remove_tg_outport(AX_TaskPortLCIter & iter){m_tg_outportL.erase(iter);}
     void remove_tg_outport(AX_TaskPortCP taskportP);
     AX_TaskPortLCIter delete_tg_outport(AX_TaskPortLCIter & iter);
     void remove_all_tg_outports();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~ Task Instance List Manipulations
     AX_TaskLCIter create_new_task();
     AX_TaskLCIter add_new_task(AX_TaskC & task);
     AX_TaskLCIter add_new_task(std::string name);

     AX_TaskCP get_task(int index);
     AX_TaskLCIter get_task(std::string name);
     AX_TaskLCIter get_task(AX_TaskCP taskP);
     AX_TaskCP get_task_by_id(int id);

     void set2_first_task(AX_TaskLCIter & tIter){tIter = m_taskL.begin();}
     AX_TaskLCIter get2_first_task(){return m_taskL.begin();}
     void set2_last_task(AX_TaskLCIter & tIter){tIter = --m_taskL.end();}
     AX_TaskLCIter get2_last_task(){return --m_taskL.end();}
     AX_TaskLCIter task_list_end(){return m_taskL.end();}

     int get_no_tasks(){return m_taskL.size();}

     void remove_task(AX_TaskLCIter & iter){m_taskL.erase(iter);}
     void remove_task(AX_TaskCP taskP);
     AX_TaskLCIter delete_task(AX_TaskLCIter & iter);
     void remove_all_tasks();

  // ~~~~~~~~~~ Internal Connection Instance List Manipulations
     AX_ConnLCIter create_new_conn();
     AX_ConnLCIter add_new_conn(AX_ConnC & conn);

     AX_ConnCP get_conn(int index);
     AX_ConnLCIter get_conn(std::string name);
     AX_ConnLCIter get_conn(AX_ConnCP connP);
     int get_connIdx(int id);

     void set2_first_conn(AX_ConnLCIter & cIter){cIter = m_connL.begin();}
     AX_ConnLCIter get2_first_conn(){return m_connL.begin();}
     void set2_last_conn(AX_ConnLCIter & cIter){cIter = --m_connL.end();}
     AX_ConnLCIter get2_last_conn(){return --m_connL.end();}
     AX_ConnLCIter conn_list_end(){return m_connL.end();}

     int get_no_conns(){return m_connL.size();}

     void remove_conn(AX_ConnLCIter & iter){m_connL.erase(iter);}
     void remove_conn(AX_ConnCP connP);
     AX_ConnLCIter delete_conn(AX_ConnLCIter & iter);
     void remove_all_conns();

  // ~~~~~~~~~~~~ Internal TaskPort Instance List Manipulations
     AX_TaskPortLCIter create_new_taskport();
     AX_TaskPortLCIter create_new_taskport(AX_TaskCP taskP, std::string & port);
     AX_TaskPortLCIter add_new_taskport(AX_TaskPortC & taskport);
     AX_TaskPortLCIter add_new_taskport(AX_TaskCP taskP, std::string port);

     AX_TaskPortCP get_taskport(int index);
     AX_TaskPortLCIter get_taskport(std::string name);
     AX_TaskPortLCIter get_taskport(AX_TaskPortCP taskportP);
     AX_TaskPortCP get_taskport_by_id(int id);

     void set2_first_taskport(AX_TaskPortLCIter & pIter)
     {
         pIter = m_taskportL.begin();
     }
     AX_TaskPortLCIter get2_first_taskport(){return m_taskportL.begin();}
     void set2_last_taskport(AX_TaskPortLCIter & pIter)
     {
         pIter = --m_taskportL.end();
     }
     AX_TaskPortLCIter get2_last_taskport(){return --m_taskportL.end();}
     AX_TaskPortLCIter taskport_list_end(){return m_taskportL.end();}

     int get_no_taskports(){return m_taskportL.size();}

     void remove_taskport(AX_TaskPortLCIter & iter){m_taskportL.erase(iter);}
     void remove_taskport(AX_TaskPortCP taskportP);
     AX_TaskPortLCIter delete_taskport(AX_TaskPortLCIter & iter);
     void remove_all_taskports();

  // ~~~~~~~~~~~~~~~~~~~~~~~~ Iteration Block List Manipulations
     AX_IterBlkLSIter create_new_iterblk();
     AX_IterBlkLSIter add_new_iterblk(AX_IterBlkS & iterblk);

     AX_IterBlkSP get_iterblk(int index);

     void set2_first_iterblk(AX_IterBlkLSIter & iIter)
     {
         iIter = m_iterblkL.begin();
     }
     AX_IterBlkLSIter get2_first_iterblk(){return m_iterblkL.begin();}
     void set2_last_iterblk(AX_IterBlkLSIter & iIter)
     {
         iIter = --m_iterblkL.end();
     }
     AX_IterBlkLSIter get2_last_iterblk(){return --m_iterblkL.end();}
     AX_IterBlkLSIter iterblk_list_end(){return m_iterblkL.end();}

     int get_no_iterblks(){return m_iterblkL.size();}

     void remove_iterblk(AX_IterBlkLSIter & iter){m_iterblkL.erase(iter);}
     void remove_iterblk(AX_IterBlkSP iterblkP);
     AX_IterBlkLSIter delete_iterblk(AX_IterBlkLSIter & iter);
     void remove_all_iterblks();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Cycle Head List Manipulations
     AX_CycleLSIter create_new_cycle();
     AX_CycleLSIter add_new_cycle(AX_CycleS cycle);

     AX_CycleSP get_cycle(int index);

     void set2_first_cycle(AX_CycleLSIter & iIter)
     {
         iIter = m_cycleL.begin();
     }
     AX_CycleLSIter get2_first_cycle(){return m_cycleL.begin();}
     void set2_last_cycle(AX_CycleLSIter & iIter)
     {
         iIter = --m_cycleL.end();
     }
     AX_CycleLSIter get2_last_cycle(){return --m_cycleL.end();}
     AX_CycleLSIter cycle_list_end(){return m_cycleL.end();}

     int get_no_cycles(){return m_cycleL.size();}

     void remove_cycle(AX_CycleLSIter & iter){m_cycleL.erase(iter);}
     void remove_cycle(AX_CycleSP cycleP);
     AX_CycleLSIter delete_cycle(AX_CycleLSIter & iter);
     void remove_all_cycles();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Structure List Manipulations

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Special Functions

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Miscellaneous Functions
     void clean_up();
     void dump();
     void dump_simple();
     void write(std::fstream & File);
     void write_simple(std::fstream & File);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~End of Class Definition
};

// Typedef Definitions for TaskGraphC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef AX_TaskGraphC * AX_TaskGraphCP;
typedef std::list<AX_TaskGraphC> AX_TaskGraphLC;
typedef std::list<AX_TaskGraphC>::iterator AX_TaskGraphLCIter;
typedef std::list<AX_TaskGraphC>::const_iterator AX_TaskGraphLCCnstIter;
typedef std::list<AX_TaskGraphC *> AX_TaskGraphPLC;
typedef std::list<AX_TaskGraphC *>::iterator AX_TaskGraphPLCIter;
typedef std::list<AX_TaskGraphC *>::const_iterator AX_TaskGraphPLCCnstIter;

#endif /* AX_TASKGRAPH_H_INCLUDED */
