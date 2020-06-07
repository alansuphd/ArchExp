// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// task.h
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef AX_TASK_H_INCLUDED
#define AX_TASK_H_INCLUDED 1

#include <list>
#include <fstream>
#include "enum.h"
#include "type.h"
#include "util.h"

#ifndef AX_TASKGRAPH_H_INCLUDED
#include "taskgraph.h"
#endif /* AX_TASKGRAPH_H_INCLUDED */

#ifndef AX_TASKPORT_H_INCLUDED
#include "taskport.h"
#endif /* AX_TASKPORT_H_INCLUDED */

#ifndef AX_FUNCBLK_H_INCLUDED
#include "funcblk.h"
#endif /* AX_FUNCBLK_H_INCLUDED */

// Class Definition for TaskC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class AX_TaskC
{
private:

    std::string m_name;
    std::string m_function;
    int m_id;
    int m_db_idx; // database idx
    AX_TaskGraphCP m_parent_tgP; // pointer to parent task graph
    TaskTypeE m_type; // type of task
    int m_delay; // number of delays of a delay task
  
    int m_start; // start time which must be met by the schedule
    int m_finish; // finish time which must be met by the schedule
    AX_IterBlkSP m_iterblkP; // pointer to the relative iteration block
    int m_asap; // ASAP value
    int m_alap; // ALAP value
    AX_TaskCP m_backsuccP; // back sucessor pointer
    AX_TaskCP m_backpredP; // back sucessor pointer
    int m_lcd;  // back sucessor lcd
  
    AX_FuncBlkPLC m_fncblkPL; // list of pointers to implementations
    AX_TaskPortPLC m_inportPL; // list of input ports
    AX_TaskPortPLC m_outportPL; // list of output ports
    AX_TaskPLC m_predPL; // List of predecessor tasks
    AX_TaskPLC m_succPL; // list of sucessors tasks
  
    int m_temp; // a variable to be used within a procedure

public:
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructor
     AX_TaskC();
     AX_TaskC(const AX_TaskC & other);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Destructor
     ~AX_TaskC();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Assignator & Comparators
     AX_TaskC & operator= (const AX_TaskC & other);
     void copy(const AX_TaskC & other);
     void copyBasic(const AX_TaskC & other);
     bool operator== (const AX_TaskC & other);
     bool operator!= (const AX_TaskC & other) 
     {
        return(!(*this == other));
     }

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Sets & Gets
     void set_name(std::string & name){m_name = name;}
     std::string get_name(){return m_name;}

     void set_function(std::string function){m_function = function;}
     std::string get_function(){return m_function;}

     void set_id(int id){m_id = id;}
     int get_id(){return m_id;}

     void set_db_idx(int db_idx){m_db_idx = db_idx;}
     int get_db_idx(){return m_db_idx;}

     void set_parent_tg(AX_TaskGraphCP tgP){m_parent_tgP = tgP;}
     AX_TaskGraphCP get_parent_tg(){return m_parent_tgP;}

     void set_type(TaskTypeE type){m_type = type;}
     TaskTypeE get_type(){return m_type;}

     void set_delay(int delay){m_delay = delay;}
     int get_delay(){return m_delay;}

     void set_start(int start){m_start = start;}
     int get_start(){return m_start;}

     void set_finish(int finish){m_finish = finish;}
     int get_finish(){return m_finish;}

     void set_iterblk(AX_IterBlkSP iterblk){m_iterblkP = iterblk;}
     AX_IterBlkSP get_iterblk(){return m_iterblkP;}

     void set_asap(int asap){m_asap = asap;}
     int get_asap(){return m_asap;}

     void set_alap(int alap){m_alap = alap;}
     int get_alap(){return m_alap;}

     void set_backsucc(AX_TaskCP backsucc){m_backsuccP = backsucc;}
     AX_TaskCP get_backsucc(){return m_backsuccP;}

     void set_backpred(AX_TaskCP backpred){m_backpredP = backpred;}
     AX_TaskCP get_backpred(){return m_backpredP;}

     void set_lcd(int lcd){m_lcd = lcd;}
     int get_lcd(){return m_lcd;}

     void set_temp(int temp){m_temp = temp;}
     int get_temp(){return m_temp;}

  // ~~~~~~~~~~~~~~~~ Function Block Pointer List Manipulations
     AX_FuncBlkPLCIter add_new_fncblk(AX_FuncBlkCP fncblkP);

     AX_FuncBlkCP get_fncblk(int index);
     AX_FuncBlkPLCIter get_fncblk(std::string name);

     void set2_first_fncblk(AX_FuncBlkPLCIter & fIter){fIter = m_fncblkPL.begin();}
     AX_FuncBlkPLCIter get2_first_fncblk(){return m_fncblkPL.begin();}
     void set2_last_fncblk(AX_FuncBlkPLCIter & fIter){fIter = --m_fncblkPL.end();}
     AX_FuncBlkPLCIter get2_last_fncblk(){return --m_fncblkPL.end();}
     AX_FuncBlkPLCIter fncblk_list_end(){return m_fncblkPL.end();}

     int get_no_fncblks(){return m_fncblkPL.size();}

     void remove_fncblk(AX_FuncBlkPLCIter & iter){m_fncblkPL.erase(iter);}
     void remove_fncblk(AX_FuncBlkCP fncblkP);
     AX_FuncBlkPLCIter delete_fncblk(AX_FuncBlkPLCIter & iter);
     void remove_all_fncblks(){m_fncblkPL.clear();}

     AX_FuncBlkCP ran_pick_fncblk();

  // ~~~~~~~~~~~~~~~~~~~~ Input Port Pointer List Manipulations
     AX_TaskPortPLCIter add_new_inport(AX_TaskPortCP portP);

     AX_TaskPortCP get_inport(int index);
     AX_TaskPortPLCIter get_inport(std::string name);
     int get_inportIdx(std::string name);

     void set2_first_inport(AX_TaskPortPLCIter & pIter){pIter = m_inportPL.begin();}
     AX_TaskPortPLCIter get2_first_inport(){return m_inportPL.begin();}
     void set2_last_inport(AX_TaskPortPLCIter & pIter){pIter = --m_inportPL.end();}
     AX_TaskPortPLCIter get2_last_inport(){return --m_inportPL.end();}
     AX_TaskPortPLCIter inport_list_end(){return m_inportPL.end();}

     int get_no_inports(){return m_inportPL.size();}

     void remove_inport(AX_TaskPortPLCIter & iter){m_inportPL.erase(iter);}
     void remove_inport(AX_TaskPortCP portP);
     AX_TaskPortPLCIter delete_inport(AX_TaskPortPLCIter & iter);
     void remove_all_inports(){m_inportPL.clear();}

  // ~~~~~~~~~~~~~~~~~~~ Output Port Pointer List Manipulations
     AX_TaskPortPLCIter add_new_outport(AX_TaskPortCP portP);

     AX_TaskPortCP get_outport(int index);
     AX_TaskPortPLCIter get_outport(std::string name);
     int get_outportIdx(std::string name);

     void set2_first_outport(AX_TaskPortPLCIter & pIter){pIter = m_outportPL.begin();}
     AX_TaskPortPLCIter get2_first_outport(){return m_outportPL.begin();}
     void set2_last_outport(AX_TaskPortPLCIter & pIter){pIter = --m_outportPL.end();}
     AX_TaskPortPLCIter get2_last_outport(){return --m_outportPL.end();}
     AX_TaskPortPLCIter outport_list_end(){return m_outportPL.end();}

     int get_no_outports(){return m_outportPL.size();}

     void remove_outport(AX_TaskPortPLCIter & iter){m_outportPL.erase(iter);}
     void remove_outport(AX_TaskPortCP portP);
     AX_TaskPortPLCIter delete_outport(AX_TaskPortPLCIter & iter);
     void remove_all_outports(){m_outportPL.clear();}

  // ~~~~~~~~~~~~~~ Predecessor Task Pointer List Manipulations
     AX_TaskPLCIter add_new_pred(AX_TaskCP taskP);

     AX_TaskCP get_pred(int index);
     AX_TaskPLCIter get_pred(std::string name);

     void set2_first_pred(AX_TaskPLCIter & tIter){tIter = m_predPL.begin();}
     AX_TaskPLCIter get2_first_pred(){return m_predPL.begin();}
     void set2_last_pred(AX_TaskPLCIter & tIter){tIter = --m_predPL.end();}
     AX_TaskPLCIter get2_last_pred(){return --m_predPL.end();}
     AX_TaskPLCIter pred_list_end(){return m_predPL.end();}

     int get_no_preds(){return m_predPL.size();}

     void remove_pred(AX_TaskPLCIter & iter){m_predPL.erase(iter);}
     void remove_pred(AX_TaskCP taskP);
     AX_TaskPLCIter delete_pred(AX_TaskPLCIter & iter);
     void remove_all_preds(){m_predPL.clear();}

  // ~~~~~~~~~~~~~~~~ Successor Task Pointer List Manipulations
     AX_TaskPLCIter add_new_succ(AX_TaskCP taskP);

     AX_TaskCP get_succ(int index);
     AX_TaskPLCIter get_succ(std::string name);

     void set2_first_succ(AX_TaskPLCIter & tIter){tIter = m_succPL.begin();}
     AX_TaskPLCIter get2_first_succ(){return m_succPL.begin();}
     void set2_last_succ(AX_TaskPLCIter & tIter){tIter = --m_succPL.end();}
     AX_TaskPLCIter get2_last_succ(){return --m_succPL.end();}
     AX_TaskPLCIter succ_list_end(){return m_succPL.end();}

     int get_no_succs(){return m_succPL.size();}

     void remove_succ(AX_TaskPLCIter & iter){m_succPL.erase(iter);}
     void remove_succ(AX_TaskCP taskP);
     AX_TaskPLCIter delete_succ(AX_TaskPLCIter & iter);
     void remove_all_succs(){m_succPL.clear();}
     int get_max_sample_to_succ(AX_TaskCP succP);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Instance List Manipulations

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Structure List Manipulations

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Special Functions

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Miscellaneous Functions
     void clean_up();
     void dump();
     void write(std::fstream & File);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~End of Class Definition
};

// Typedef Definitions for TaskC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef AX_TaskC * AX_TaskCP;
typedef std::list<AX_TaskC> AX_TaskLC;
typedef std::list<AX_TaskC>::iterator AX_TaskLCIter;
typedef std::list<AX_TaskC *> AX_TaskPLC;
typedef std::list<AX_TaskC *>::iterator AX_TaskPLCIter;

#endif /* AX_TASK_H_INCLUDED */
