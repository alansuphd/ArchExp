// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// taskport.h
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef AX_TASKPORT_H_INCLUDED
#define AX_TASKPORT_H_INCLUDED 1

#include <list>
#include <fstream>
#include "type.h"
#include "enum.h"

#ifndef AX_CONN_H_INCLUDED
#include "conn.h"
#endif /* AX_CONN_H_INCLUDED */

#ifndef AX_TASKGRAPH_H_INCLUDED
#include "taskgraph.h"
#endif /* AX_TASKGRAPH_H_INCLUDED */

#ifndef AX_TASK_H_INCLUDED
#include "task.h"
#endif /* AX_TASK_H_INCLUDED */

// Class Definition for TaskPortC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class AX_TaskPortC
{
private:

    std::string m_name;
    int m_id;
    int m_db_idx;
    PortTypeE m_type;

    // Of the following two members, only one must be valid.
    AX_TaskCP m_parent_taskP; // the task to which this port belongs
    AX_TaskGraphCP m_parent_tgP; // the task graph to which this belongs to

    AX_ConnPLC m_connPL; // list of edges connected to this port
    int m_no_samples; // number of samples per execution
    int m_sample_size; // number of bits per sample

    int m_temp;

public:
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructor
     AX_TaskPortC();
     AX_TaskPortC(const AX_TaskPortC & other);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Destructor
     ~AX_TaskPortC();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Assignator & Comparators
     AX_TaskPortC & operator= (const AX_TaskPortC & other);
     void copy(const AX_TaskPortC & other);
     void copyBasic(const AX_TaskPortC & other);
     bool operator== (const AX_TaskPortC & other);
     bool operator!= (const AX_TaskPortC & other) 
     {
        return(!(*this == other));
     }

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Sets & Gets
     void set_name(std::string & name){m_name = name;}
     std::string get_name(){return m_name;}

     void set_id(int id){m_id = id;}
     int get_id(){return m_id;}

     void set_db_idx(int db_idx){m_db_idx = db_idx;}
     int get_db_idx(){return m_db_idx;}

     void set_type(PortTypeE type){m_type = type;}
     PortTypeE get_type(){return m_type;}

     void set_parent_task(AX_TaskCP taskP){m_parent_taskP = taskP;}
     AX_TaskCP get_parent_task(){return m_parent_taskP;}

     void set_parent_tg(AX_TaskGraphCP tgP){m_parent_tgP = tgP;}
     AX_TaskGraphCP get_parent_tg(){return m_parent_tgP;}

     void set_no_samples(int i){m_no_samples = i;}
     int get_no_samples(){return m_no_samples;}

     void set_sample_size(int i){m_sample_size = i;}
     int get_sample_size(){return m_sample_size;}

     void set_temp(int temp){m_temp = temp;}
     int get_temp(){return m_temp;}

  // ~~~~~~~~~~~~~~~~~~~~ Connection Pointer List Manipulations
     AX_ConnPLCIter add_new_conn(AX_ConnCP connP);

     AX_ConnCP get_conn(int index);
     void set2_first_conn(AX_ConnPLCIter & iter){iter = m_connPL.begin();}
     AX_ConnPLCIter get2_first_conn(){return m_connPL.begin();}
     void set2_last_conn(AX_ConnPLCIter & iter){iter = --m_connPL.end();}
     AX_ConnPLCIter get2_last_conn(){return --m_connPL.end();}
     AX_ConnPLCIter conn_list_end(){return m_connPL.end();}

     int get_no_conns(){return m_connPL.size();}

     void remove_conn(AX_ConnPLCIter & iter){m_connPL.erase(iter);}
     void remove_conn(AX_ConnCP connP);
     AX_ConnPLCIter delete_conn(AX_ConnPLCIter & iter);
     void remove_all_conns(){m_connPL.clear();}

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Instance List Manipulations

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Structure List Manipulations

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Special Functions

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Miscellaneous Functions
     void clean_up();
     void dump();
     void write(std::fstream & File);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~End of Class Definition
};

// Typedef Definitions for TaskPortC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef AX_TaskPortC * AX_TaskPortCP;
typedef std::list<AX_TaskPortC> AX_TaskPortLC;
typedef std::list<AX_TaskPortC>::iterator AX_TaskPortLCIter;
typedef std::list<AX_TaskPortC>::const_iterator AX_TaskPortLCCnstIter;
typedef std::list<AX_TaskPortC *> AX_TaskPortPLC;
typedef std::list<AX_TaskPortC *>::iterator AX_TaskPortPLCIter;
typedef std::list<AX_TaskPortC *>::const_iterator AX_TaskPortPLCCnstIter;

#endif /* AX_TASKPORT_H_INCLUDED */
