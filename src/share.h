// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// share.h
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef AX_SHARE_H_INCLUDED
#define AX_SHARE_H_INCLUDED

#include <list>
#include <fstream>
#include "type.h"
#include "enum.h"

#ifndef AX_FUNCBLK_H_INCLUDED
#include "funcblk.h"
#endif /* AX_FUNCBLK_H_INCLUDED */

#ifndef AX_TASK_H_INCLUDED
#include "task.h"
#endif /* AX_TASK_H_INCLUDED */

// Class Definition for ShareC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class AX_ShareC
{
private:

  AX_FuncBlkCP m_fncblkP;
  AX_TaskPLC m_taskPL;
  AX_FuncBlkPLC m_fncblkPL;
  AX_FuncBlkPLC m_sw_fncblkPL;

public:
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructor
     AX_ShareC();
     AX_ShareC(const AX_ShareC & other);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Destructor
     ~AX_ShareC();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Assignator & Comparators
     AX_ShareC & operator= (const AX_ShareC & other);
     bool operator== (const AX_ShareC & other);
     bool operator!= (const AX_ShareC & other) 
     {
        return(!(*this == other));
     }

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Sets & Gets
     void set_fncblk(AX_FuncBlkCP fncblkP){m_fncblkP = fncblkP;}
     AX_FuncBlkCP get_fncblk(){return m_fncblkP;}

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Task List Manipulations
     AX_TaskPLCIter add_new_task(AX_TaskCP taskP);
     AX_TaskPLCIter add_task(AX_TaskCP taskP);

     AX_TaskCP get_task(int index);
     void set2_first_task(AX_TaskPLCIter & fIter)
     {
         fIter = m_taskPL.begin();
     }
     AX_TaskPLCIter get2_first_task(){return m_taskPL.begin();}
     void set2_last_task(AX_TaskPLCIter & fIter)
     {
         fIter = --m_taskPL.end();
     }
     AX_TaskPLCIter get2_last_task(){return --m_taskPL.end();}
     AX_TaskPLCIter task_list_end(){return m_taskPL.end();}

     int get_no_tasks(){return m_taskPL.size();}

     void remove_task(AX_TaskPLCIter & iter){m_taskPL.erase(iter);}
     void remove_task(AX_TaskCP taskP);
     AX_TaskPLCIter delete_task(AX_TaskPLCIter & iter);
     AX_TaskPLCIter delete_task(AX_TaskCP taskP);
     void remove_all_tasks(){m_taskPL.clear();};

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Fncblk List Manipulations
     AX_FuncBlkPLCIter add_new_fncblk(AX_FuncBlkCP fncblkP);
     AX_FuncBlkPLCIter add_fncblk(AX_FuncBlkCP fncblkP);

     AX_FuncBlkCP get_fncblk(int index);
     void set2_first_fncblk(AX_FuncBlkPLCIter & fIter)
     {
         fIter = m_fncblkPL.begin();
     }
     AX_FuncBlkPLCIter get2_first_fncblk(){return m_fncblkPL.begin();}
     void set2_last_fncblk(AX_FuncBlkPLCIter & fIter)
     {
         fIter = --m_fncblkPL.end();
     }
     AX_FuncBlkPLCIter get2_last_fncblk(){return --m_fncblkPL.end();}
     AX_FuncBlkPLCIter fncblk_list_end(){return m_fncblkPL.end();}

     int get_no_fncblks(){return m_fncblkPL.size();}

     void remove_fncblk(AX_FuncBlkPLCIter & iter){m_fncblkPL.erase(iter);}
     void remove_fncblk(AX_FuncBlkCP fncblkP);
     AX_FuncBlkPLCIter delete_fncblk(AX_FuncBlkPLCIter & iter);
     AX_FuncBlkPLCIter delete_fncblk(AX_FuncBlkCP fncblkP);
     void remove_all_fncblks(){m_fncblkPL.clear();};

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~SW Fncblk List Manipulations
     AX_FuncBlkPLCIter add_new_sw_fncblk(AX_FuncBlkCP fncblkP);
     AX_FuncBlkPLCIter add_sw_fncblk(AX_FuncBlkCP fncblkP);

     AX_FuncBlkCP get_sw_fncblk(int index);
     void set2_first_sw_fncblk(AX_FuncBlkPLCIter & fIter)
     {
         fIter = m_sw_fncblkPL.begin();
     }
     AX_FuncBlkPLCIter get2_first_sw_fncblk(){return m_sw_fncblkPL.begin();}
     void set2_last_sw_fncblk(AX_FuncBlkPLCIter & fIter)
     {
         fIter = --m_sw_fncblkPL.end();
     }
     AX_FuncBlkPLCIter get2_last_sw_fncblk(){return --m_sw_fncblkPL.end();}
     AX_FuncBlkPLCIter sw_fncblk_list_end(){return m_sw_fncblkPL.end();}

     int get_no_sw_fncblks(){return m_sw_fncblkPL.size();}

     void remove_sw_fncblk(AX_FuncBlkPLCIter & iter){m_sw_fncblkPL.erase(iter);}
     void remove_sw_fncblk(AX_FuncBlkCP fncblkP);
     AX_FuncBlkPLCIter delete_sw_fncblk(AX_FuncBlkPLCIter & iter);
     AX_FuncBlkPLCIter delete_sw_fncblk(AX_FuncBlkCP fncblkP);
     void remove_all_sw_fncblks(){m_sw_fncblkPL.clear();};

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Special Functions
     AX_FuncBlkCP get_mapping_fncblk(AX_TaskCP taskP);
     AX_TaskCP get_mapping_task(AX_FuncBlkCP fncblkP);
     AX_FuncBlkCP get_mapping_sw_fncblk(AX_TaskCP taskP);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Miscellaneous Functions
     void dump();
     void write(std::fstream & File);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~End of Class Definition
};

// Typedef Definitions for ShareC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef AX_ShareC * AX_ShareCP;
typedef std::list<AX_ShareC> AX_ShareLC;
typedef std::list<AX_ShareC>::iterator AX_ShareLCIter;
typedef std::list<AX_ShareC>::const_iterator AX_ShareLCCnstIter;
typedef std::list<AX_ShareC *> AX_SharePLC;
typedef std::list<AX_ShareC *>::iterator AX_SharePLCIter;
typedef std::list<AX_ShareC *>::const_iterator AX_SharePLCCnstIter;

#endif /* AX_SHARE_H_INCLUDED */
