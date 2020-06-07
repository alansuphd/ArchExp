// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// conn.h
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef AX_CONN_H_INCLUDED
#define AX_CONN_H_INCLUDED 1

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

#ifndef AX_TASKGRAPH_H_INCLUDED
#include "taskgraph.h"
#endif /* AX_TASKGRAPH_H_INCLUDED */

// Class Definition for ConnC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class AX_ConnC
{
private:

    int m_id;
    int m_db_idx;
    ConnTypeE m_type;

    AX_TaskGraphCP m_src_tgP;
    AX_TaskCP m_src_taskP;
    AX_TaskPortCP m_src_portP;

    AX_TaskGraphCP m_sink_tgP;
    AX_TaskCP m_sink_taskP;
    AX_TaskPortCP m_sink_portP;

    int m_avg_activ; // average activation rate
    int m_pk_activ; // peak activation rate

    int m_temp;

public:
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructor
     AX_ConnC();
     AX_ConnC(const AX_ConnC & other);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Destructor
     ~AX_ConnC();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Assignator & Comparators
     AX_ConnC & operator= (const AX_ConnC & other);
     void copy(const AX_ConnC & other);
     void copyBasic(const AX_ConnC & other);
     bool operator== (const AX_ConnC & other);
     bool operator!= (const AX_ConnC & other) 
     {
        return(!(*this == other));
     }

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Sets & Gets
     void set_id(int id){m_id = id;}
     int get_id(){return m_id;}

     void set_db_idx(int db_idx){m_db_idx = db_idx;}
     int get_db_idx(){return m_db_idx;}

     void set_type(ConnTypeE type){m_type = type;}
     ConnTypeE get_type(){return m_type;}

     void set_src_port(AX_TaskPortCP portP){m_src_portP = portP;}
     AX_TaskPortCP get_src_port(){return m_src_portP;}

     void set_src_task(AX_TaskCP taskP){m_src_taskP = taskP;}
     AX_TaskCP get_src_task(){return m_src_taskP;}

     void set_src_tg(AX_TaskGraphCP tgP){m_src_tgP = tgP;}
     AX_TaskGraphCP get_src_tg(){return m_src_tgP;}

     void set_sink_port(AX_TaskPortCP portP){m_sink_portP = portP;}
     AX_TaskPortCP get_sink_port(){return m_sink_portP;}

     void set_sink_task(AX_TaskCP taskP){m_sink_taskP = taskP;}
     AX_TaskCP get_sink_task(){return m_sink_taskP;}

     void set_sink_tg(AX_TaskGraphCP tgP){m_sink_tgP = tgP;}
     AX_TaskGraphCP get_sink_tg(){return m_sink_tgP;}

     void set_avg_activ(int avg_activ){m_avg_activ = avg_activ;}
     int get_avg_activ(){return m_avg_activ;}

     void set_pk_activ(int pk_activ){m_pk_activ = pk_activ;}
     int get_pk_activ(){return m_pk_activ;}

     void set_temp(int temp){m_temp = temp;}
     int get_temp(){return m_temp;}

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Pointer List Manipulations

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Instance List Manipulations

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

// Typedef Definitions for ConnC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef AX_ConnC * AX_ConnCP;
typedef std::list<AX_ConnC> AX_ConnLC;
typedef std::list<AX_ConnC>::iterator AX_ConnLCIter;
typedef std::list<AX_ConnC>::const_iterator AX_ConnLCCnstIter;
typedef std::list<AX_ConnC *> AX_ConnPLC;
typedef std::list<AX_ConnC *>::iterator AX_ConnPLCIter;
typedef std::list<AX_ConnC *>::const_iterator AX_ConnPLCCnstIter;

#endif /* AX_CONN_H_INCLUDED */
