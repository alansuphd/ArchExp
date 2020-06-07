// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// task.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <iostream>
#include "task.h"
using namespace std;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructor
// ------------------------------------------------------------
   AX_TaskC::AX_TaskC()
   {
      m_name.clear();
      m_function.clear();
      m_id = -1;
      m_db_idx = -1;
      m_delay = -1;
      m_start = -1;
      m_finish = -1;
      m_asap = -1;
      m_alap = -1;
      m_iterblkP = NULL;
      m_backsuccP = NULL;
      m_backpredP = NULL;
      m_lcd = -1;
      m_parent_tgP = NULL;
      m_type = TASK_TYPE_UNKNOWN;
      m_temp = -1;
   }
// ------------------------------------------------------------
   AX_TaskC::AX_TaskC(const AX_TaskC & other)
   {
      copy(other);
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Destructor
// ------------------------------------------------------------
   AX_TaskC::~AX_TaskC()
   {
      remove_all_fncblks();
      remove_all_inports();
      remove_all_outports();
      remove_all_preds();
      remove_all_succs();
      clean_up();
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Assignator & Comparators
// ------------------------------------------------------------
   AX_TaskC & AX_TaskC::operator= (const AX_TaskC & other)
   {
      if (&other == this)
         return(*this);

      copy(other);

      return(*this);
   }
// ------------------------------------------------------------
   void AX_TaskC::copy(const AX_TaskC & other)
   {
      m_name = other.m_name;
      m_function = other.m_function;
      m_id = other.m_id;
      m_db_idx = other.m_db_idx;
      m_delay = other.m_delay;
      m_start = other.m_start;
      m_finish = other.m_finish;
      m_iterblkP = other.m_iterblkP;
      m_backsuccP = other.m_backsuccP;
      m_backpredP = other.m_backpredP;
      m_lcd = other.m_lcd;
      m_asap = other.m_asap;
      m_alap = other.m_alap;
      m_parent_tgP = other.m_parent_tgP;
      m_type = other.m_type;
      m_fncblkPL = other.m_fncblkPL;
      m_inportPL = other.m_inportPL;
      m_outportPL = other.m_outportPL;
      m_predPL = other.m_predPL;
      m_succPL = other.m_succPL;
      m_temp = other.m_temp;

      return;
   }
// ------------------------------------------------------------
   void AX_TaskC::copyBasic(const AX_TaskC & other)
   {
      m_name = other.m_name;
      m_function = other.m_function;
      m_id = other.m_id;
      m_db_idx = other.m_db_idx;
      m_delay = other.m_delay;
      m_start = other.m_start;
      m_finish = other.m_finish;
      m_asap = other.m_asap;
      m_alap = other.m_alap;
      m_lcd = other.m_lcd;
      m_parent_tgP = other.m_parent_tgP;
      m_type = other.m_type;
      m_fncblkPL = other.m_fncblkPL;
      m_temp = other.m_temp;

      return;
   }
// ------------------------------------------------------------
   bool AX_TaskC::operator== (const AX_TaskC & other)
   {
      if (m_name == other.m_name)
         return(true);

      return(false);
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Pointer List Manipulations
// ------------------------------------------------------------
   AX_FuncBlkPLCIter AX_TaskC::add_new_fncblk(AX_FuncBlkCP fncblkP)
   {
      AX_FuncBlkPLCIter fIter;

      for (fIter = m_fncblkPL.begin();
           fIter != m_fncblkPL.end(); fIter++)
          if (fncblkP == (*fIter))
             return fIter;

      m_fncblkPL.push_back(fncblkP);
      fIter = --m_fncblkPL.end();

    return fIter;
   }
// ------------------------------------------------------------
   AX_FuncBlkCP AX_TaskC::get_fncblk(int index)
   {
      AX_FuncBlkCP fncblkP;

      if ((index < 0)||((unsigned int)index > m_fncblkPL.size()-1))
         fncblkP = NULL;
      else
      {
         AX_FuncBlkPLCIter fIter;
         int i;

         fIter = m_fncblkPL.begin();
         for(i = 0; i < index; i++) fIter++;

         fncblkP = (*fIter);
      }

      return fncblkP;
   }
// ------------------------------------------------------------
   AX_FuncBlkPLCIter AX_TaskC::get_fncblk(string name)
   {
      if (name.empty()) 
         return m_fncblkPL.end();

      AX_FuncBlkPLCIter fIter;

      for (fIter = m_fncblkPL.begin();
           fIter != m_fncblkPL.end(); fIter++)
          if ((*fIter)->get_name() == name)
             return fIter;

      return fIter;
   }
// ------------------------------------------------------------
   void AX_TaskC::remove_fncblk(AX_FuncBlkCP fncblkP)
   {
      AX_FuncBlkPLCIter fIter;

      for (fIter = m_fncblkPL.begin();
           fIter != m_fncblkPL.end(); fIter++)
          if ((*fIter) == fncblkP)
          {
             m_fncblkPL.erase(fIter);
             return;
          }
      return;
   }
// ------------------------------------------------------------
   AX_FuncBlkPLCIter AX_TaskC::delete_fncblk(AX_FuncBlkPLCIter & iter)
   {
      AX_FuncBlkPLCIter retIter;

      if (m_fncblkPL.end() == iter)
         return iter;
      else if ((--m_fncblkPL.end()) == iter)
      {
         m_fncblkPL.erase(iter);
         retIter = m_fncblkPL.end();
      }
      else
      {
         retIter = ++iter;
         iter--;
         m_fncblkPL.erase(iter);
      }

      return retIter;
   }
// ------------------------------------------------------------
   AX_FuncBlkCP AX_TaskC::ran_pick_fncblk()
   {
      AX_FuncBlkCP funcP = NULL;

      int iNum = my_rand(m_fncblkPL.size());
      funcP = get_fncblk(iNum);

      return funcP;
   }
// ------------------------------------------------------------
   AX_TaskPortPLCIter AX_TaskC::add_new_inport(AX_TaskPortCP portP)
   {
      AX_TaskPortPLCIter pIter;

      for (pIter = m_inportPL.begin();
           pIter != m_inportPL.end(); pIter++)
          if (portP == (*pIter))
             return pIter;

      m_inportPL.push_back(portP);
      pIter = --m_inportPL.end();

      return pIter;
   }
// ------------------------------------------------------------
   AX_TaskPortCP AX_TaskC::get_inport(int index)
   {
      AX_TaskPortCP portP;

      if ((index < 0)||((unsigned int)index > m_inportPL.size()-1))
         portP = NULL;
      else
      {
         AX_TaskPortPLCIter pIter;
         int i;

         pIter = m_inportPL.begin();
         for(i = 0; i < index; i++) pIter++;

         portP = (*pIter);
      }

      return portP;
   }
// ------------------------------------------------------------
   AX_TaskPortPLCIter AX_TaskC::get_inport(string name)
   {
      if (name.empty())
         return m_inportPL.end();

      AX_TaskPortPLCIter pIter;

      for (pIter = m_inportPL.begin();
           pIter != m_inportPL.end(); pIter++)
          if ((*pIter)->get_name() == name)
             return pIter;

      return pIter;
   }
// ------------------------------------------------------------
   int AX_TaskC::get_inportIdx(string name)
   {
      if (name.empty())
         return -1;

      AX_TaskPortPLCIter pIter;
      int idx = 0;

      for (pIter = m_inportPL.begin();
           pIter != m_inportPL.end(); pIter++)
      {
          if ((*pIter)->get_name() == name)
             return idx;
          idx++;
      }

      return -1;
   }
// ------------------------------------------------------------
   void AX_TaskC::remove_inport(AX_TaskPortCP portP)
   {
      AX_TaskPortPLCIter pIter;

      for (pIter = m_inportPL.begin();
           pIter != m_inportPL.end(); pIter++)
          if ((*pIter) == portP)
          {
             m_inportPL.erase(pIter);
             return;
          }
      return;
   }
// ------------------------------------------------------------
   AX_TaskPortPLCIter AX_TaskC::delete_inport(AX_TaskPortPLCIter & iter)
   {
      AX_TaskPortPLCIter retIter;

      if (m_inportPL.end() == iter)
         return iter;
      else if ((--m_inportPL.end()) == iter)
      {
         m_inportPL.erase(iter);
         retIter = m_inportPL.end();
      }
      else
      {
         retIter = ++iter;
         iter--;
         m_inportPL.erase(iter);
      }

      return retIter;
   }
// ------------------------------------------------------------
   AX_TaskPortPLCIter AX_TaskC::add_new_outport(AX_TaskPortCP portP)
   {
      AX_TaskPortPLCIter pIter;

      for (pIter = m_outportPL.begin();
           pIter != m_outportPL.end(); pIter++)
          if (portP == (*pIter))
             return pIter;

      m_outportPL.push_back(portP);
      pIter = --m_outportPL.end();

      return pIter;
   }
// ------------------------------------------------------------
   AX_TaskPortCP AX_TaskC::get_outport(int index)
   {
      AX_TaskPortCP portP;

      if ((index < 0)||((unsigned int)index > m_outportPL.size()-1))
         portP = NULL;
      else
      {
         AX_TaskPortPLCIter pIter;
         int i;

         pIter = m_outportPL.begin();
         for(i = 0; i < index; i++) pIter++;

         portP = (*pIter);
      }

      return portP;
   }
// ------------------------------------------------------------
   AX_TaskPortPLCIter AX_TaskC::get_outport(string name)
   {
      if (name.empty())
         return m_outportPL.end();

      AX_TaskPortPLCIter pIter;

      for (pIter = m_outportPL.begin();
           pIter != m_outportPL.end(); pIter++)
          if ((*pIter)->get_name() == name)
             return pIter;

      return pIter;
   }
// ------------------------------------------------------------
   int AX_TaskC::get_outportIdx(string name)
   {
      if (name.empty())
         return -1;

      AX_TaskPortPLCIter pIter;
      int idx = 0;

      for (pIter = m_outportPL.begin();
           pIter != m_outportPL.end(); pIter++)
      {
          if ((*pIter)->get_name() == name)
             return idx;
          idx++;
      }

      return -1;
   }
// ------------------------------------------------------------
   void AX_TaskC::remove_outport(AX_TaskPortCP portP)
   {
      AX_TaskPortPLCIter pIter;

      for (pIter = m_outportPL.begin();
           pIter != m_outportPL.end(); pIter++)
          if ((*pIter) == portP)
          {
             m_outportPL.erase(pIter);
             return;
          }
      return;
   }
// ------------------------------------------------------------
   AX_TaskPortPLCIter AX_TaskC::delete_outport(AX_TaskPortPLCIter & iter)
   {
      AX_TaskPortPLCIter retIter;

      if (m_outportPL.end() == iter)
         return iter;
      else if ((--m_outportPL.end()) == iter)
      {
         m_outportPL.erase(iter);
         retIter = m_outportPL.end();
      }
      else
      {
         retIter = ++iter;
         iter--;
         m_outportPL.erase(iter);
      }

      return retIter;
   }
// ------------------------------------------------------------
   AX_TaskPLCIter AX_TaskC::add_new_pred(AX_TaskCP taskP)
   {
      AX_TaskPLCIter tIter;

      for (tIter = m_predPL.begin();
           tIter != m_predPL.end(); tIter++)
          if (taskP == (*tIter))
             return tIter;

      m_predPL.push_back(taskP);
      tIter = --m_predPL.end();

      return tIter;
   }
// ------------------------------------------------------------
   AX_TaskCP AX_TaskC::get_pred(int index)
   {
      AX_TaskCP taskP;

      if ((index < 0)||((unsigned int)index > m_predPL.size()-1))
         taskP = NULL;
      else
      {
         AX_TaskPLCIter tIter;
         int i;

         tIter = m_predPL.begin();
         for(i = 0; i < index; i++) tIter++;

         taskP = (*tIter);
      }

      return taskP;
   }
// ------------------------------------------------------------
   AX_TaskPLCIter AX_TaskC::get_pred(string name)
   {
      if (name.empty())
         return m_predPL.end();

      AX_TaskPLCIter tIter;

      for (tIter = m_predPL.begin();
           tIter != m_predPL.end(); tIter++)
          if ((*tIter)->get_name() == name)
             return tIter;

      return tIter;
   }
// ------------------------------------------------------------
   void AX_TaskC::remove_pred(AX_TaskCP taskP)
   {
      AX_TaskPLCIter tIter;

      for (tIter = m_predPL.begin();
           tIter != m_predPL.end(); tIter++)
          if ((*tIter) == taskP)
          {
             m_predPL.erase(tIter);
             return;
          }
      return;
   }
// ------------------------------------------------------------
   AX_TaskPLCIter AX_TaskC::delete_pred(AX_TaskPLCIter & iter)
   {
      AX_TaskPLCIter retIter;

      if (m_predPL.end() == iter)
         return iter;
      else if ((--m_predPL.end()) == iter)
      {
         m_predPL.erase(iter);
         retIter = m_predPL.end();
      }
      else
      {
         retIter = ++iter;
         iter--;
         m_predPL.erase(iter);
      }

      return retIter;
   }
// ------------------------------------------------------------
   AX_TaskPLCIter AX_TaskC::add_new_succ(AX_TaskCP taskP)
   {
      AX_TaskPLCIter tIter;

      for (tIter = m_succPL.begin();
           tIter != m_succPL.end(); tIter++)
          if (taskP == (*tIter))
             return tIter;

      m_succPL.push_back(taskP);
      tIter = --m_succPL.end();

      return tIter;
   }
// ------------------------------------------------------------
   AX_TaskCP AX_TaskC::get_succ(int index)
   {
      AX_TaskCP taskP;

      if ((index < 0)||((unsigned int)index > m_succPL.size()-1))
         taskP = NULL;
      else
      {
         AX_TaskPLCIter tIter;
         int i;

         tIter = m_succPL.begin();
         for(i = 0; i < index; i++) tIter++;

         taskP = (*tIter);
      }

      return taskP;
   }
// ------------------------------------------------------------
   AX_TaskPLCIter AX_TaskC::get_succ(string name)
   {
      if (name.empty())
         return m_succPL.end();

      AX_TaskPLCIter tIter;

      for (tIter = m_succPL.begin();
           tIter != m_succPL.end(); tIter++)
          if ((*tIter)->get_name() == name)
             return tIter;

      return tIter;
   }
// ------------------------------------------------------------
   void AX_TaskC::remove_succ(AX_TaskCP taskP)
   {
      AX_TaskPLCIter tIter;

      for (tIter = m_succPL.begin();
           tIter != m_succPL.end(); tIter++)
          if ((*tIter) == taskP)
          {
             m_succPL.erase(tIter);
             return;
          }
      return;
   }
// ------------------------------------------------------------
   AX_TaskPLCIter AX_TaskC::delete_succ(AX_TaskPLCIter & iter)
   {
      AX_TaskPLCIter retIter;

      if (m_succPL.end() == iter)
         return iter;
      else if ((--m_succPL.end()) == iter)
      {
         m_succPL.erase(iter);
         retIter = m_succPL.end();
      }
      else
      {
         retIter = ++iter;
         iter--;
         m_succPL.erase(iter);
      }

      return retIter;
   }
// ------------------------------------------------------------
   int AX_TaskC::get_max_sample_to_succ(AX_TaskCP succP)
   {
      if (succP == NULL)
         return -1;
      int max = -1;
      AX_TaskPortPLCIter portPI;
      for (portPI = m_outportPL.begin();
           portPI != m_outportPL.end(); portPI++)
      {
         AX_ConnPLCIter connPI;
         for (connPI = (*portPI)->get2_first_conn();
              connPI != (*portPI)->conn_list_end(); connPI++)
         {
            if ((*connPI)->get_sink_task() == succP)
               if ((*portPI)->get_sample_size() > max)
                  max = (*portPI)->get_sample_size();
         }  // end conn for
      } // end outport for

      return max;
   }
// ------------------------------------------------------------

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Instance List Manipulations


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Structure List Manipulations


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Special Functions


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Miscellaneous Functions
// ------------------------------------------------------------
   void AX_TaskC::clean_up()
   {
      m_name.clear();
      m_function.clear();
      m_id = 0;
      m_db_idx = 0;
      m_delay = 0;
      m_start = 0;
      m_finish = 0;
      m_iterblkP = NULL;
      m_backsuccP = NULL;
      m_backpredP = NULL;
      m_lcd = 0;
      m_parent_tgP = NULL;
      m_type = TASK_TYPE_UNKNOWN;
      m_temp = 0;
   }
// ------------------------------------------------------------
   void AX_TaskC::dump()
   {
     cerr << "  Name: " << m_name;
     cerr << ". id: " << m_id;
     cerr << ". Db_Idx: " << m_db_idx;
     cerr << ". delay: " << m_delay << endl;
     cerr << "  Start: " << m_start;
     cerr << ". Finish: " << m_finish;
     cerr << ". Parent TG: " << m_parent_tgP->get_name() << endl;
     cerr << "  In IterBlk: ";
     if (m_iterblkP == NULL)
        cerr << "No";
     else
        cerr << "Yes";
     cerr << "  backsuccP: ";
     if (m_backsuccP == NULL)
        cerr << "null";
     else
        cerr << m_backsuccP->get_name();
     cerr << "  lcd: " << m_lcd
          << "  asap: " << m_asap
          << "  alap: " << endl;

     cerr << "  fncblkPL: ";
     AX_FuncBlkPLCIter fncblkPI;
     for (fncblkPI = m_fncblkPL.begin();
          fncblkPI != m_fncblkPL.end(); fncblkPI++)
        cerr << (*fncblkPI)->get_name() << " ";
     cerr << endl;

     cerr << "  Preds:";
     AX_TaskPLCIter iter;
     for (iter = m_predPL.begin(); iter != m_predPL.end(); iter++)
       {
         if ((*iter) == NULL)
           continue;
         cerr << " " << (*iter)->get_name();
       }

     cerr << "\n  Succs:";
     for (iter = m_succPL.begin(); iter != m_succPL.end(); iter++)
       {
         if ((*iter) == NULL)
           continue;
         cerr << " " << (*iter)->get_name();
       }

     cerr << "\n  InPorts:" << endl;
     AX_TaskPortPLCIter pIter;
     for (pIter = m_inportPL.begin(); pIter != m_inportPL.end(); pIter++)
         (*pIter)->dump();
   
     cerr << "  OutPorts:" << endl;
     for (pIter = m_outportPL.begin(); pIter != m_outportPL.end(); pIter++)
         (*pIter)->dump();
     cerr << endl;
   }
// ------------------------------------------------------------
   void AX_TaskC::write(fstream & File)
   {
     File << "  Name: " << m_name;
     File << ". id: " << m_id;
     File << ". Db_Idx: " << m_db_idx;
     File << ". delay: " << m_delay << endl;
     File << "  Start: " << m_start;
     File << ". Finish: " << m_finish;
     File << ". Parent TG: " << m_parent_tgP->get_name() << endl;
     File << "  In IterBlk: ";
     if (m_iterblkP == NULL)
        File << "No";
     else
        File << "Yes";
     File << "  backsuccP: ";
     if (m_backsuccP == NULL)
        File << "null";
     else
        File << m_backsuccP->get_name();

     File << "  lcd: " << m_lcd
          << "  asap: " << m_asap
          << "  alap: " << endl;

     File << "  Preds:";
     AX_TaskPLCIter iter;
     for (iter = m_predPL.begin(); iter != m_predPL.end(); iter++)
       {
         if ((*iter) == NULL)
           continue;
         File << " " << (*iter)->get_name();
       }

     File << "\n  Succs:";
     for (iter = m_succPL.begin(); iter != m_succPL.end(); iter++)
       {
         if ((*iter) == NULL)
           continue;
         File << " " << (*iter)->get_name();
       }

     File << "\n  InPorts:" << endl;
     AX_TaskPortPLCIter pIter;
     for (pIter = m_inportPL.begin(); pIter != m_inportPL.end(); pIter++)
         (*pIter)->write(File);
   
     File << "  OutPorts:" << endl;
     for (pIter = m_outportPL.begin(); pIter != m_outportPL.end(); pIter++)
         (*pIter)->write(File);
     File << endl;
   }
// ------------------------------------------------------------

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~End of Function Definitions
