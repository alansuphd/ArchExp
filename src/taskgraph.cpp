// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// taskgraph.c
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <iostream>
#include "taskgraph.h"
using namespace std;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructor
// ------------------------------------------------------------
   AX_TaskGraphC::AX_TaskGraphC()
   {
      m_name.clear();
      m_id = -1;
      m_db_idx = -1;
      m_dsnP = NULL;
      m_start_taskP = NULL;
      m_finish_taskP = NULL;
      m_period = 0;
      m_temp = -1;
   }
// ------------------------------------------------------------
   AX_TaskGraphC::AX_TaskGraphC(AX_TaskGraphC & other)
   {
      copy(other);
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Destructor
// ------------------------------------------------------------
   AX_TaskGraphC::~AX_TaskGraphC()
   {
      remove_all_tg_inports();
      remove_all_tg_outports();
      remove_all_tasks();
      remove_all_taskports();
      remove_all_conns();
      remove_all_iterblks();
      remove_all_cycles();
      clean_up();
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Assignator & Comparators
// ------------------------------------------------------------
   AX_TaskGraphC & AX_TaskGraphC::operator= (AX_TaskGraphC & other)
   {
      if (&other == this)
         return(*this);

      copy(other);

      return(*this);
   }
// ------------------------------------------------------------
   bool AX_TaskGraphC::copy(AX_TaskGraphC & other)
   {
      m_name = other.m_name;
      m_id = other.m_id;
      m_db_idx = other.m_db_idx;
      m_dsnP = other.m_dsnP;

      // reconstruct start_task
      m_start_taskP = NULL;
      if (other.m_start_taskP != NULL)
      {
         AX_TaskC nTask;
         AX_TaskLCIter nTaskI;
         m_taskL.push_back(nTask);
         nTaskI = --m_taskL.end();
         (*nTaskI).copyBasic(*other.m_start_taskP);
         (*nTaskI).set_parent_tg(this);
         m_start_taskP = &(*nTaskI);
      }

      // reconstruct finish_task
      m_finish_taskP = NULL;
      if (other.m_finish_taskP != NULL)
      {
         AX_TaskC nTask;
         AX_TaskLCIter nTaskI;
         m_taskL.push_back(nTask);
         nTaskI = --m_taskL.end();
         (*nTaskI).copyBasic(*other.m_start_taskP);
         (*nTaskI).set_parent_tg(this);
         m_finish_taskP = &(*nTaskI);
      }

      // reconstruct tg_inportL
      AX_TaskPortLCIter tPortI;
      m_tg_inportL.clear();
      for (tPortI = other.m_tg_inportL.begin();
           tPortI != other.m_tg_inportL.end(); tPortI++)
      {
         AX_TaskPortC nPort;
         AX_TaskPortLCIter nPortI;
         nPortI = add_new_tg_inport(nPort);
         (*nPortI).copyBasic(*tPortI);
         (*nPortI).set_parent_tg(this);
         (*nPortI).set_parent_task(NULL);
      }

      // reconstruct tg_outportL
      m_tg_outportL.clear();
      for (tPortI = other.m_tg_outportL.begin();
           tPortI != other.m_tg_outportL.end(); tPortI++)
      {
         AX_TaskPortC nPort;
         AX_TaskPortLCIter nPortI;
         nPortI = add_new_tg_outport(nPort);
         (*nPortI).copyBasic(*tPortI);
         (*nPortI).set_parent_tg(this);
         (*nPortI).set_parent_task(NULL);
      }

      // reconstruct taskL
      AX_TaskLCIter taskI;
      m_taskL.clear();
      for (taskI = other.m_taskL.begin();
           taskI != other.m_taskL.end(); taskI++)
      {
         AX_TaskC nTask;
         AX_TaskLCIter nTaskI;
         nTaskI = add_new_task((*taskI).get_name());
         (*nTaskI).copyBasic(*taskI);
         (*nTaskI).set_parent_tg(this);
      }

      // set up the m_backsuccP
      for (taskI = other.m_taskL.begin();
           taskI != other.m_taskL.end(); taskI++)
      {
         if ((*taskI).get_backsucc() == NULL)
            continue;
         AX_TaskCP bTaskP, cTaskP;
         cTaskP = get_task_by_id((*taskI).get_id());
         bTaskP = get_task_by_id((*taskI).get_backsucc()->get_id());
         cTaskP->set_backsucc(bTaskP);
      }

      // reconstruct taskportL
      m_taskportL.clear();
      for (tPortI = other.m_taskportL.begin();
           tPortI != other.m_taskportL.end(); tPortI++)
      {
         AX_TaskPortLCIter nPortI;
         AX_TaskCP oTaskP;
         oTaskP = get_task_by_id((*tPortI).get_parent_task()->get_id());
         if (oTaskP == NULL)
         {
#if DEBUG >= 2
            cerr << "Cannot find task "
                 << (*tPortI).get_parent_task()->get_name()
                 << endl;
#endif
            return false;
         }
         nPortI = add_new_taskport(oTaskP, (*tPortI).get_name());
         (*nPortI).copyBasic(*tPortI);
         (*nPortI).set_parent_tg(NULL);
         (*nPortI).set_parent_task(oTaskP);
         if ((*nPortI).get_type() == PORT_IN)
            oTaskP->add_new_inport(&(*nPortI));
         else if ((*nPortI).get_type() == PORT_OUT)
            oTaskP->add_new_outport(&(*nPortI));
         else
         {
#if DEBUG >=2
            cerr << "Incorrect task port type:"
                 << (*tPortI).get_parent_task()->get_name()
                 << "." << (*tPortI).get_name() << endl;
#endif
            return false;
         }
      }

      // reconstruct connL
      AX_ConnLCIter connI;
      m_connL.clear();
      for (connI = other.m_connL.begin();
           connI != other.m_connL.end(); connI++)
      {
         AX_ConnC nConn;
         AX_ConnLCIter nConnI;
         AX_TaskPortCP osrPortP = NULL, oskPortP = NULL;
         AX_TaskCP osrTaskP = NULL, oskTaskP = NULL;

         nConnI = add_new_conn(nConn);
         (*nConnI).copyBasic(*connI);

         // reconstruct src node
         if ((*connI).get_src_tg() != NULL)
         {
            (*nConnI).set_src_tg(this);
            (*nConnI).set_src_task(NULL);
            osrPortP = get_tg_inport_by_id((*connI).get_src_port()->get_id());
            if (osrPortP == NULL)
            {
#if DEBUG >=2
               cerr << "Cannot find src taskport "
                    << m_name << "."
                    << (*nConnI).get_src_port()->get_name()
                    << endl;
#endif
               return false;
            }
            (*nConnI).set_src_port(osrPortP);
         } // end if src_tg != NULL
         if ((*connI).get_src_tg() == NULL)
         {
            (*nConnI).set_src_tg(NULL);
            osrTaskP = get_task_by_id((*connI).get_src_task()->get_id());
            if (osrTaskP == NULL)
            {
#if DEBUG >= 2
               cerr << "Cannot find src task "
                    << (*nConnI).get_src_task()->get_name()
                    << endl;
#endif
               return false;
            }

            osrPortP = get_taskport_by_id((*connI).get_src_port()->get_id());
            if (osrPortP == NULL)
            {
#if DEBUG >= 2
               cerr << "Cannot find src taskport "
                    << osrTaskP->get_name() << "."
                    << (*nConnI).get_src_port()->get_name()
                    << endl;
#endif
               return false;
            }
            (*nConnI).set_src_task(osrTaskP);
            (*nConnI).set_src_port(osrPortP);
         } // end if src_tg == NULL
         osrPortP->add_new_conn(&(*nConnI));

         // reconstruct sink node
         if ((*connI).get_sink_tg() != NULL)
         {
            (*nConnI).set_sink_tg(this);
            (*nConnI).set_sink_task(NULL);
            oskPortP = get_tg_outport_by_id((*connI).get_sink_port()->get_id());
            if (oskPortP == NULL)
            {
#if DEBUG >= 2
               cerr << "ConnL: cannot find sink taskport "
                    << m_name << "."
                    << (*nConnI).get_sink_port()->get_name()
                    << endl;
#endif
               return false;
            }
            (*nConnI).set_sink_port(oskPortP);
         } // end if sink_tg != NULL
         if ((*connI).get_sink_tg() == NULL)
         {
            (*nConnI).set_sink_tg(NULL);
            oskTaskP = get_task_by_id((*connI).get_sink_task()->get_id());
            if (oskTaskP == NULL)
            {
#if DEBUG >= 2
               cerr << "ConnL: cannot find sink task "
                    << (*nConnI).get_sink_task()->get_name()
                    << endl;
#endif
               return false;
            }

            oskPortP = get_taskport_by_id((*connI).get_sink_port()->get_id());
            if (oskPortP == NULL)
            {
#if DEBUG >= 2
               cerr << "Conn: cannot find sink taskport "
                    << oskTaskP->get_name() << "."
                    << (*nConnI).get_sink_port()->get_name()
                    << endl;
#endif
               return false;
            }
            (*nConnI).set_sink_task(oskTaskP);
            (*nConnI).set_sink_port(oskPortP);
         } // end if sink_tg == NULL
         oskPortP->add_new_conn(&(*nConnI));

         if (osrTaskP != NULL && oskTaskP != NULL)
         {
            osrTaskP->add_new_succ(oskTaskP);
            oskTaskP->add_new_pred(osrTaskP);
         }
      } // end connL loop

      // reconstruct iterblkL
      AX_IterBlkLSIter iBlkI;
      m_iterblkL.clear();
      for (iBlkI = other.m_iterblkL.begin(); 
           iBlkI != other.m_iterblkL.end(); iBlkI++)
      {
         AX_IterBlkS nBlk;
         AX_IterBlkLSIter nBlkI;
         m_iterblkL.push_back(nBlk);
         nBlkI = --m_iterblkL.end();
         (*nBlkI).m_itercnt = (*iBlkI).m_itercnt;
         (*nBlkI).m_latency = (*iBlkI).m_latency;

         AX_TaskPLCIter taskPI;
         (*nBlkI).m_taskPL.clear();
         for (taskPI = (*iBlkI).m_taskPL.begin();
              taskPI != (*iBlkI).m_taskPL.end(); taskPI++)
         {
            AX_TaskCP ooTaskP;
            ooTaskP = get_task_by_id((*taskPI)->get_id());
            if (ooTaskP == NULL)
            {
#if DEBUG >= 2
               cerr << "IterblkL: cannot find task "
                    << (*taskPI)->get_name()
                    << endl;
#endif
               return false;
            }
            (*nBlkI).m_taskPL.push_back(ooTaskP);
            ooTaskP->set_iterblk(&(*nBlkI));
         }
      }

      // reconstruct cycleL
      AX_CycleLSIter cycleI;
      m_cycleL.clear();
      for (cycleI = other.m_cycleL.begin();
           cycleI != other.m_cycleL.end(); cycleI++)
      {
         AX_CycleS nCycle;
         AX_CycleLSIter nCycleI;
         m_cycleL.push_back(nCycle);
         nCycleI = --m_cycleL.end();
         (*nCycleI).m_cycle_latency = (*cycleI).m_cycle_latency;
         (*nCycleI).m_lcd = (*cycleI).m_lcd;

         AX_TaskCP ooTaskP;
         ooTaskP = get_task_by_id((*cycleI).m_headP->get_id());
         if (ooTaskP == NULL)
         {
#if DEBUG >= 2
            cerr << "CycleL: cannot find task "
                 << (*cycleI).m_headP->get_name()
                 << endl;
#endif
            return false;
         }
         (*nCycleI).m_headP = ooTaskP;
      }

      m_period = other.m_period;
      m_temp = other.m_temp;

      return true;
   }
// ------------------------------------------------------------
   bool AX_TaskGraphC::operator== (const AX_TaskGraphC & other)
   {
      if (&other == this)
         return(true);

      return(false);
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Pointer List Manipulations


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Instance List Manipulations
// ------------------------------------------------------------
   AX_TaskPortLCIter AX_TaskGraphC::create_new_tg_inport()
   {
      AX_TaskPortC taskport;

      m_tg_inportL.push_back(taskport);

      return(--m_tg_inportL.end());
   }
// ------------------------------------------------------------
   AX_TaskPortLCIter AX_TaskGraphC::create_new_tg_inport(string & name)
   {
      if (name.empty())
         return m_tg_inportL.end();

      AX_TaskPortLCIter pIter;
      for (pIter = get2_first_tg_inport();
           pIter != tg_inport_list_end(); pIter++)
         if ((*pIter).get_name() == name)
            return pIter;

      AX_TaskPortC nPort;
      m_tg_inportL.push_back(nPort);
      pIter = --m_tg_inportL.end();
      (*pIter).set_name(name);
      (*pIter).set_parent_tg(&(*this));
      (*pIter).set_type(PORT_OUT);

      return(pIter);

   }
// ------------------------------------------------------------
   AX_TaskPortLCIter AX_TaskGraphC::add_new_tg_inport(AX_TaskPortC & taskport)
   {
      AX_TaskPortLCIter tIter;

      for (tIter = m_tg_inportL.begin();
           tIter != m_tg_inportL.end(); tIter++)
          if (taskport == (*tIter))
             return tIter;

      m_tg_inportL.push_back(taskport);

      return (--m_tg_inportL.end());
   }
// ------------------------------------------------------------
   AX_TaskPortCP AX_TaskGraphC::get_tg_inport(int index)
   {
      AX_TaskPortCP taskportP;

      if ((index < 0)||((unsigned int)index > m_tg_inportL.size()-1))
         taskportP = NULL;
      else
      {
         AX_TaskPortLCIter tIter;
         int i;

         tIter = m_tg_inportL.begin();
         for(i = 0; i < index; i++) tIter++;

         taskportP = &(*tIter);
      }

      return taskportP;
   }
// ------------------------------------------------------------
   AX_TaskPortLCIter AX_TaskGraphC::get_tg_inport(string name)
   {
      if (name.empty())
         return m_tg_inportL.end();

      AX_TaskPortLCIter pIter;

      for (pIter = m_tg_inportL.begin();
           pIter != m_tg_inportL.end(); pIter++)
          if ((*pIter).get_name() == name)
             return pIter;

      return pIter;
   }
// ------------------------------------------------------------
   AX_TaskPortLCIter AX_TaskGraphC::get_tg_inport(AX_TaskPortCP taskportP)
   {
      AX_TaskPortLCIter pIter;
 
      for (pIter = m_tg_inportL.begin();
           pIter != m_tg_inportL.end(); pIter++)
          if (&(*pIter) == taskportP)
          {
             return pIter;
          }

      return pIter;
   }
// ------------------------------------------------------------
   AX_TaskPortCP AX_TaskGraphC::get_tg_inport_by_id(int id)
   {
      AX_TaskPortLCIter pIter;
 
      for (pIter = m_tg_inportL.begin();
           pIter != m_tg_inportL.end(); pIter++)
          if ((*pIter).get_id() == id)
          {
             return &(*pIter);
          }

      return NULL;
   }
// ------------------------------------------------------------
   void AX_TaskGraphC::remove_tg_inport(AX_TaskPortCP taskportP)
   {
       AX_TaskPortLCIter tIter;

       for (tIter = m_tg_inportL.begin();
            tIter != m_tg_inportL.end(); tIter++)
           if (&(*tIter) == taskportP)
           {
               m_tg_inportL.erase(tIter);
               return;
           }
       return;
   }
// ------------------------------------------------------------
   AX_TaskPortLCIter AX_TaskGraphC::delete_tg_inport(AX_TaskPortLCIter & iter)
   {
      AX_TaskPortLCIter tIter;

      if (m_tg_inportL.end() == iter)
         return iter;
      else if ((--m_tg_inportL.end()) == iter)
      {
         AX_TaskPortC tTmp;
         (*iter) = tTmp;
         m_tg_inportL.erase(iter);
         tIter = m_tg_inportL.end();
      }
      else
      {
         tIter = ++iter;
         iter--;
         AX_TaskPortC tTmp;
         (*iter) = tTmp;
         m_tg_inportL.erase(iter);
      }

      return tIter;
   }
// ------------------------------------------------------------
   void AX_TaskGraphC::remove_all_tg_inports()
   {
      AX_TaskPortLCIter tIter;

      AX_TaskPortC tTmp;
      while ((tIter = m_tg_inportL.begin()) != m_tg_inportL.end())
      {
         (*tIter) = tTmp;
         m_tg_inportL.erase(tIter);
      }

      m_tg_inportL.clear();
   }
// ------------------------------------------------------------
   AX_TaskPortLCIter AX_TaskGraphC::create_new_tg_outport()
   {
      AX_TaskPortC taskport;

      m_tg_outportL.push_back(taskport);

      return(--m_tg_outportL.end());
   }
// ------------------------------------------------------------
   AX_TaskPortLCIter AX_TaskGraphC::create_new_tg_outport(string & name)
   {
      if (name.empty())
         return m_tg_outportL.end();

      AX_TaskPortLCIter pIter;
      for (pIter = get2_first_tg_outport();
           pIter != tg_outport_list_end(); pIter++)
         if ((*pIter).get_name() == name)
            return pIter;

      AX_TaskPortC nPort;
      m_tg_outportL.push_back(nPort);
      pIter = --m_tg_outportL.end();
      (*pIter).set_name(name);
      (*pIter).set_parent_tg(&(*this));
      (*pIter).set_type(PORT_IN);

      return(pIter);

   }
// ------------------------------------------------------------
   AX_TaskPortLCIter AX_TaskGraphC::add_new_tg_outport(AX_TaskPortC & taskport)
   {
      AX_TaskPortLCIter tIter;

      for (tIter = m_tg_outportL.begin();
           tIter != m_tg_outportL.end(); tIter++)
          if (taskport == (*tIter))
             return tIter;

      m_tg_outportL.push_back(taskport);

      return (--m_tg_outportL.end());
   }
// ------------------------------------------------------------
   AX_TaskPortCP AX_TaskGraphC::get_tg_outport(int index)
   {
      AX_TaskPortCP taskportP;

      if ((index < 0)||((unsigned int)index > m_tg_outportL.size()-1))
         taskportP = NULL;
      else
      {
         AX_TaskPortLCIter tIter;
         int i;

         tIter = m_tg_outportL.begin();
         for(i = 0; i < index; i++) tIter++;

         taskportP = &(*tIter);
      }

      return taskportP;
   }
// ------------------------------------------------------------
   AX_TaskPortLCIter AX_TaskGraphC::get_tg_outport(string name)
   {
      if (name.empty())
         return m_tg_outportL.end();

      AX_TaskPortLCIter pIter;

      for (pIter = m_tg_outportL.begin();
           pIter != m_tg_outportL.end(); pIter++)
          if ((*pIter).get_name() == name)
             return pIter;

      return pIter;
   }
// ------------------------------------------------------------
   AX_TaskPortLCIter AX_TaskGraphC::get_tg_outport(AX_TaskPortCP taskportP)
   {
      AX_TaskPortLCIter pIter;
 
      for (pIter = m_tg_outportL.begin();
           pIter != m_tg_outportL.end(); pIter++)
          if (&(*pIter) == taskportP)
          {
             return pIter;
          }

      return pIter;
   }
// ------------------------------------------------------------
   AX_TaskPortCP AX_TaskGraphC::get_tg_outport_by_id(int id)
   {
      AX_TaskPortLCIter pIter;
 
      for (pIter = m_tg_outportL.begin();
           pIter != m_tg_outportL.end(); pIter++)
          if ((*pIter).get_id() == id)
          {
             return &(*pIter);
          }

      return NULL;
   }
// ------------------------------------------------------------
   void AX_TaskGraphC::remove_tg_outport(AX_TaskPortCP taskportP)
   {
       AX_TaskPortLCIter tIter;

       for (tIter = m_tg_outportL.begin();
            tIter != m_tg_outportL.end(); tIter++)
           if (&(*tIter) == taskportP)
           {
               m_tg_outportL.erase(tIter);
               return;
           }
       return;
   }
// ------------------------------------------------------------
   AX_TaskPortLCIter AX_TaskGraphC::delete_tg_outport(AX_TaskPortLCIter & iter)
   {
      AX_TaskPortLCIter tIter;

      if (m_tg_outportL.end() == iter)
         return iter;
      else if ((--m_tg_outportL.end()) == iter)
      {
         AX_TaskPortC tTmp;
         (*iter) = tTmp;
         m_tg_outportL.erase(iter);
         tIter = m_tg_outportL.end();
      }
      else
      {
         tIter = ++iter;
         iter--;
         AX_TaskPortC tTmp;
         (*iter) = tTmp;
         m_tg_outportL.erase(iter);
      }

      return tIter;
   }
// ------------------------------------------------------------
   void AX_TaskGraphC::remove_all_tg_outports()
   {
      AX_TaskPortLCIter tIter;

      AX_TaskPortC tTmp;
      while ((tIter = m_tg_outportL.begin()) != m_tg_outportL.end())
      {
         (*tIter) = tTmp;
         m_tg_outportL.erase(tIter);
      }

      m_tg_outportL.clear();
   }
// ------------------------------------------------------------
   AX_TaskLCIter AX_TaskGraphC::create_new_task()
   {
      AX_TaskC task;

      m_taskL.push_back(task);

      return(--m_taskL.end());
   }
// ------------------------------------------------------------
   AX_TaskLCIter AX_TaskGraphC::add_new_task(AX_TaskC & task)
   {
      AX_TaskLCIter tIter;

      for (tIter = m_taskL.begin();
           tIter != m_taskL.end(); tIter++)
          if (task == (*tIter))
             return tIter;

      m_taskL.push_back(task);

      return (--m_taskL.end());
   }
// ------------------------------------------------------------
   AX_TaskLCIter AX_TaskGraphC::add_new_task(string name)
   {
      AX_TaskLCIter tIter;

      for (tIter = m_taskL.begin();
           tIter != m_taskL.end(); tIter++)
          if (name== (*tIter).get_name())
             return tIter;

      AX_TaskC task;
      m_taskL.push_back(task);
      tIter = --m_taskL.end();
      (*tIter).set_name(name);

      return (tIter);
   }
// ------------------------------------------------------------
   AX_TaskCP AX_TaskGraphC::get_task(int index)
   {
      AX_TaskCP taskP;

      if ((index < 0)||((unsigned int)index > m_taskL.size()-1))
         taskP = NULL;
      else
      {
         AX_TaskLCIter tIter;
         int i;

         tIter = m_taskL.begin();
         for(i = 0; i < index; i++) tIter++;

         taskP = &(*tIter);
      }

      return taskP;
   }
// ------------------------------------------------------------
   AX_TaskLCIter AX_TaskGraphC::get_task(string name)
   {
      if (name.empty())
         return m_taskL.end();

      AX_TaskLCIter tIter;

      for (tIter = m_taskL.begin();
           tIter != m_taskL.end(); tIter++)
          if ((*tIter).get_name() == name)
             return tIter;

      return tIter;
   }
// ------------------------------------------------------------
   AX_TaskLCIter AX_TaskGraphC::get_task(AX_TaskCP taskP)
   {
      AX_TaskLCIter tIter;
  
      for (tIter = m_taskL.begin();
           tIter != m_taskL.end(); tIter++)
          if (&(*tIter) == taskP)
          {
             return tIter;
          }

      return tIter;
   }
// ------------------------------------------------------------
   AX_TaskCP AX_TaskGraphC::get_task_by_id(int id)
   {
      AX_TaskLCIter tIter;
  
      for (tIter = m_taskL.begin();
           tIter != m_taskL.end(); tIter++)
          if ((*tIter).get_id() == id)
          {
             return &(*tIter);
          }

      return NULL;
   }
// ------------------------------------------------------------
   void AX_TaskGraphC::remove_task(AX_TaskCP taskP)
   {
       AX_TaskLCIter tIter;

       for (tIter = m_taskL.begin();
            tIter != m_taskL.end(); tIter++)
           if (&(*tIter) == taskP)
           {
               m_taskL.erase(tIter);
               return;
           }
       return;
   }
// ------------------------------------------------------------
   AX_TaskLCIter AX_TaskGraphC::delete_task(AX_TaskLCIter & iter)
   {
      AX_TaskLCIter tIter;

      if (m_taskL.end() == iter)
         return iter;
      else if ((--m_taskL.end()) == iter)
      {
         AX_TaskC tTmp;
         (*iter) = tTmp;
         m_taskL.erase(iter);
         tIter = m_taskL.end();
      }
      else
      {
         tIter = ++iter;
         iter--;
         AX_TaskC tTmp;
         (*iter) = tTmp;
         m_taskL.erase(iter);
      }

      return tIter;
   }
// ------------------------------------------------------------
   void AX_TaskGraphC::remove_all_tasks()
   {
      AX_TaskLCIter tIter;

      AX_TaskC tTmp;
      while ((tIter = m_taskL.begin()) != m_taskL.end())
      {
         (*tIter) = tTmp;
         m_taskL.erase(tIter);
      }

      m_taskL.clear();
   }
// ------------------------------------------------------------
   AX_ConnLCIter AX_TaskGraphC::create_new_conn()
   {
      AX_ConnC conn;

      m_connL.push_back(conn);

      return(--m_connL.end());
   }
// ------------------------------------------------------------
   AX_ConnLCIter AX_TaskGraphC::add_new_conn(AX_ConnC & conn)
   {
      AX_ConnLCIter cIter;

      for (cIter = m_connL.begin();
           cIter != m_connL.end(); cIter++)
          if (conn == (*cIter))
             return cIter;

      m_connL.push_back(conn);

      return (--m_connL.end());
   }
// ------------------------------------------------------------
   AX_ConnCP AX_TaskGraphC::get_conn(int index)
   {
      AX_ConnCP connP;

      if ((index < 0)||((unsigned int)index > m_connL.size()-1))
         connP = NULL;
      else
      {
         AX_ConnLCIter cIter;
         int i;

         cIter = m_connL.begin();
         for(i = 0; i < index; i++) cIter++;

         connP = &(*cIter);
      }

      return connP;
   }
// ------------------------------------------------------------
   AX_ConnLCIter AX_TaskGraphC::get_conn(AX_ConnCP connP)
   {
      AX_ConnLCIter cIter;
 
      for (cIter = m_connL.begin();
           cIter != m_connL.end(); cIter++)
          if (&(*cIter) == connP)
          {
             return cIter;
          }

      return cIter;
   }
// ------------------------------------------------------------
   int AX_TaskGraphC::get_connIdx(int id)
   {
      AX_ConnLCIter cIter;
      int idx = 0;
 
      for (cIter = m_connL.begin();
           cIter != m_connL.end(); cIter++)
      {
          if ((*cIter).get_id() == id)
             return idx;
          idx++;
      }

      return -1;
   }
// ------------------------------------------------------------
   void AX_TaskGraphC::remove_conn(AX_ConnCP connP)
   {
       AX_ConnLCIter cIter;

       for (cIter = m_connL.begin();
            cIter != m_connL.end(); cIter++)
           if (&(*cIter) == connP)
           {
               m_connL.erase(cIter);
               return;
           }
       return;
   }
// ------------------------------------------------------------
   AX_ConnLCIter AX_TaskGraphC::delete_conn(AX_ConnLCIter & iter)
   {
      AX_ConnLCIter cIter;

      if (m_connL.end() == iter)
         return iter;
      else if ((--m_connL.end()) == iter)
      {
         AX_ConnC cTmp;
         (*iter) = cTmp;
         m_connL.erase(iter);
         cIter = m_connL.end();
      }
      else
      {
         cIter = ++iter;
         iter--;
         AX_ConnC cTmp;
         (*iter) = cTmp;
         m_connL.erase(iter);
      }

      return cIter;
   }
// ------------------------------------------------------------
   void AX_TaskGraphC::remove_all_conns()
   {
      AX_ConnLCIter cIter;

      AX_ConnC cTmp;
      while ((cIter = m_connL.begin()) != m_connL.end())
      {
         (*cIter) = cTmp;
         m_connL.erase(cIter);
      }

      m_connL.clear();
   }
// ------------------------------------------------------------
   AX_TaskPortLCIter AX_TaskGraphC::create_new_taskport()
   {
      AX_TaskPortC taskport;

      m_taskportL.push_back(taskport);

      return(--m_taskportL.end());
   }
// ------------------------------------------------------------
   AX_TaskPortLCIter AX_TaskGraphC::create_new_taskport(AX_TaskCP taskP,
                                                        string & port)
   {
      if (taskP == NULL || port.empty())
         return m_taskportL.end();

      AX_TaskPortLCIter pIter;
      for (pIter = get2_first_taskport();
           pIter != taskport_list_end(); pIter++)
          if((taskP == (*pIter).get_parent_task()) &&
             (port == (*pIter).get_name()))
           return pIter;

      AX_TaskPortC nPort;
      m_taskportL.push_back(nPort);
      pIter = --m_taskportL.end();
      (*pIter).set_name(port);
      (*pIter).set_parent_task(taskP);

      return(pIter);
   }
// ------------------------------------------------------------
   AX_TaskPortLCIter AX_TaskGraphC::add_new_taskport(AX_TaskPortC & taskport)
   {
      AX_TaskPortLCIter tIter;

      for (tIter = m_taskportL.begin();
           tIter != m_taskportL.end(); tIter++)
          if (taskport == (*tIter))
             return tIter;

      m_taskportL.push_back(taskport);

      return (--m_taskportL.end());
   }
// ------------------------------------------------------------
   AX_TaskPortLCIter AX_TaskGraphC::add_new_taskport(AX_TaskCP taskP,
                                                       string port)
   {
      if (taskP == NULL || port.empty())
         return m_taskportL.end();

      AX_TaskPortLCIter pIter;
      for (pIter = get2_first_taskport();
           pIter != taskport_list_end(); pIter++)
          if((taskP == (*pIter).get_parent_task()) &&
             (port == (*pIter).get_name()))
           return pIter;

      AX_TaskPortC nPort;
      nPort.set_name(port);
      nPort.set_parent_task(taskP);
      m_taskportL.push_back(nPort);

      return(--m_taskportL.end());
   }
// ------------------------------------------------------------
   AX_TaskPortCP AX_TaskGraphC::get_taskport(int index)
   {
      AX_TaskPortCP taskportP;

      if ((index < 0)||((unsigned int)index > m_taskportL.size()-1))
         taskportP = NULL;
      else
      {
         AX_TaskPortLCIter tIter;
         int i;

         tIter = m_taskportL.begin();
         for(i = 0; i < index; i++) tIter++;

         taskportP = &(*tIter);
      }

      return taskportP;
   }
// ------------------------------------------------------------
   AX_TaskPortLCIter AX_TaskGraphC::get_taskport(string name)
   {
      if (name.empty())
         return m_taskportL.end();

      AX_TaskPortLCIter tIter;

      for (tIter = m_taskportL.begin();
           tIter != m_taskportL.end(); tIter++)
          if ((*tIter).get_name() == name)
             return tIter;

      return tIter;
   }
// ------------------------------------------------------------
   AX_TaskPortCP AX_TaskGraphC::get_taskport_by_id(int id)
   {
      AX_TaskPortLCIter tIter;

      for (tIter = m_taskportL.begin();
           tIter != m_taskportL.end(); tIter++)
          if ((*tIter).get_id() == id)
             return &(*tIter);

      return NULL;
   }
// ------------------------------------------------------------
   void AX_TaskGraphC::remove_taskport(AX_TaskPortCP taskportP)
   {
       AX_TaskPortLCIter tIter;

       for (tIter = m_taskportL.begin();
            tIter != m_taskportL.end(); tIter++)
           if (&(*tIter) == taskportP)
           {
               m_taskportL.erase(tIter);
               return;
           }
       return;
   }
// ------------------------------------------------------------
   AX_TaskPortLCIter AX_TaskGraphC::get_taskport(AX_TaskPortCP taskportP)
   {
      AX_TaskPortLCIter pIter;
 
      for (pIter = m_taskportL.begin();
           pIter != m_taskportL.end(); pIter++)
          if (&(*pIter) == taskportP)
          {
             return pIter;
          }

      return pIter;
   }
// ------------------------------------------------------------
   AX_TaskPortLCIter AX_TaskGraphC::delete_taskport(AX_TaskPortLCIter & iter)
   {
      AX_TaskPortLCIter tIter;

      if (m_taskportL.end() == iter)
         return iter;
      else if ((--m_taskportL.end()) == iter)
      {
         AX_TaskPortC tTmp;
         (*iter) = tTmp;
         m_taskportL.erase(iter);
         tIter = m_taskportL.end();
      }
      else
      {
         tIter = ++iter;
         iter--;
         AX_TaskPortC tTmp;
         (*iter) = tTmp;
         m_taskportL.erase(iter);
      }

      return tIter;
   }
// ------------------------------------------------------------
   void AX_TaskGraphC::remove_all_taskports()
   {
      AX_TaskPortLCIter tIter;

      AX_TaskPortC tTmp;
      while ((tIter = m_taskportL.begin()) != m_taskportL.end())
      {
         (*tIter) = tTmp;
         m_taskportL.erase(tIter);
      }

      m_taskportL.clear();
   }
// ------------------------------------------------------------
   AX_IterBlkLSIter AX_TaskGraphC::create_new_iterblk()
   {
      AX_IterBlkS iterblk;

      iterblk.m_itercnt = -1;
      iterblk.m_basedelay = -1;
      iterblk.m_taskPL.clear();
      m_iterblkL.push_back(iterblk);

      return(--m_iterblkL.end());
   }
// ------------------------------------------------------------
   AX_IterBlkLSIter AX_TaskGraphC::add_new_iterblk(AX_IterBlkS & iterblk)
   {
      AX_IterBlkLSIter tIter;

      for (tIter = m_iterblkL.begin();
           tIter != m_iterblkL.end(); tIter++)
          if (&iterblk == &(*tIter))
             return tIter;

      m_iterblkL.push_back(iterblk);

      return (--m_iterblkL.end());
   }
// ------------------------------------------------------------
   AX_IterBlkSP AX_TaskGraphC::get_iterblk(int index)
   {
      AX_IterBlkSP iterblkP = NULL;

      if ((index < 0)||((unsigned int)index > m_iterblkL.size()-1))
         iterblkP = NULL;
      else
      {
         AX_IterBlkLSIter tIter;
         int i;

         tIter = m_iterblkL.begin();
         for(i = 0; i < index; i++) tIter++;

         iterblkP = &(*tIter);
      }

      return iterblkP;
   }
// ------------------------------------------------------------
   void AX_TaskGraphC::remove_iterblk(AX_IterBlkSP iterblkP)
   {
       AX_IterBlkLSIter tIter;

       for (tIter = m_iterblkL.begin();
            tIter != m_iterblkL.end(); tIter++)
           if (&(*tIter) == iterblkP)
           {
               (*tIter).m_itercnt = 0;
               (*tIter).m_basedelay = 0;
               (*tIter).m_taskPL.clear();
               m_iterblkL.erase(tIter);
               return;
           }
       return;
   }
// ------------------------------------------------------------
   AX_IterBlkLSIter AX_TaskGraphC::delete_iterblk(AX_IterBlkLSIter & iter)
   {
      AX_IterBlkLSIter tIter;

      if (m_iterblkL.end() == iter)
         return iter;
      else if ((--m_iterblkL.end()) == iter)
      {
         (*iter).m_name.clear();
         (*iter).m_taskPL.clear();
         m_iterblkL.erase(iter);
         tIter = m_iterblkL.end();
      }
      else
      {
         tIter = ++iter;
         iter--;
         (*iter).m_name.clear();
         (*iter).m_taskPL.clear();
         m_iterblkL.erase(iter);
      }

      return tIter;
   }
// ------------------------------------------------------------
   void AX_TaskGraphC::remove_all_iterblks()
   {
      AX_IterBlkLSIter tIter;

      while ((tIter = m_iterblkL.begin()) != m_iterblkL.end())
      {
         (*tIter).m_name.clear();
         (*tIter).m_taskPL.clear();
         m_iterblkL.erase(tIter);
      }

      m_iterblkL.clear();
   }
// ------------------------------------------------------------
   AX_CycleLSIter AX_TaskGraphC::create_new_cycle()
   {
      AX_CycleS cycle;

      cycle.m_headP = NULL;
      cycle.m_cycle_latency = -1;
      cycle.m_lcd = -1;
      m_cycleL.push_back(cycle);

      return(--m_cycleL.end());
   }
// ------------------------------------------------------------
   AX_CycleLSIter AX_TaskGraphC::add_new_cycle(AX_CycleS cycle)
   {
      AX_CycleLSIter tIter;
      
      for (tIter = m_cycleL.begin();
           tIter != m_cycleL.end(); tIter++)
          if (&cycle == &(*tIter))
             return tIter;

      m_cycleL.push_back(cycle);

      return (--m_cycleL.end());
   }
// ------------------------------------------------------------
   AX_CycleSP AX_TaskGraphC::get_cycle(int index)
   {
      AX_CycleSP cycleP = NULL;

      if ((index < 0)||((unsigned int)index > m_cycleL.size()-1))
         cycleP = NULL;
      else
      {
         AX_CycleLSIter tIter;
         int i;

         tIter = m_cycleL.begin();
         for(i = 0; i < index; i++) tIter++;

         cycleP = &(*tIter);
      }

      return cycleP;
   }
// ------------------------------------------------------------
   void AX_TaskGraphC::remove_cycle(AX_CycleSP cycleP)
   {
       AX_CycleLSIter tIter;

       for (tIter = m_cycleL.begin();
            tIter != m_cycleL.end(); tIter++)
           if (&(*tIter) == cycleP)
           {
               (*tIter).m_headP = NULL;
               (*tIter).m_cycle_latency = 0;
               (*tIter).m_lcd = 0;
               m_cycleL.erase(tIter);
               return;
           }
       return;
   }
// ------------------------------------------------------------
   AX_CycleLSIter AX_TaskGraphC::delete_cycle(AX_CycleLSIter & iter)
   {
      AX_CycleLSIter tIter;

      if (m_cycleL.end() == iter)
         return iter;
      else if ((--m_cycleL.end()) == iter)
      {
         m_cycleL.erase(iter);
         tIter = m_cycleL.end();
      }
      else
      {
         tIter = ++iter;
         iter--;
         (*iter).m_headP = NULL;
         (*iter).m_cycle_latency = 0;
         (*iter).m_lcd = 0;
         m_cycleL.erase(iter);
      }

      return tIter;
   }
// ------------------------------------------------------------
   void AX_TaskGraphC::remove_all_cycles()
   {
      AX_CycleLSIter tIter;

      while ((tIter = m_cycleL.begin()) != m_cycleL.end())
      {
         (*tIter).m_headP = NULL;
         (*tIter).m_cycle_latency = 0;
         (*tIter).m_lcd = 0;
         m_cycleL.erase(tIter);
      }

      m_cycleL.clear();
   }
// ------------------------------------------------------------

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Structure List Manipulations


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Special Functions


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Miscellaneous Functions
// ------------------------------------------------------------
   void AX_TaskGraphC::clean_up()
   {
      m_name.clear();
      m_id = -1;
      m_db_idx = -1;
      m_dsnP = NULL;
      m_start_taskP = NULL;
      m_finish_taskP = NULL;
      m_period = 0;
      m_temp = -1;
   }
// ------------------------------------------------------------
   void AX_TaskGraphC::dump()
   {
      AX_TaskLCIter tIter;
      AX_TaskPortLCIter tpIter;
      AX_ConnLCIter cIter;

      cerr << "Name: " << m_name
           << "ID: " << m_id << endl
           << "  period: " << m_period
           << "  temp: " << m_temp << endl;
 
      cerr << "Tasks :" << endl;
      for (tIter = m_taskL.begin();
           tIter != m_taskL.end(); tIter++)
          (*tIter).dump();
      
      if ((m_tg_inportL.size() != 0) || (m_tg_outportL.size() != 0))
      {
         cerr << "TaskGraph Input Task Ports :" << endl;
         for (tpIter = m_tg_inportL.begin();
              tpIter != m_tg_inportL.end(); tpIter++)
         {
             (*tpIter).dump();
             cerr << endl;
         }

         cerr << "TaskGraph Output Task Ports :" << endl;
         for (tpIter = m_tg_outportL.begin();
              tpIter != m_tg_outportL.end(); tpIter++)
         {
             (*tpIter).dump();
             cerr << endl;
         }
      }
 
      cerr << "Internal Task Ports :" << endl;
      for (tpIter = m_taskportL.begin();
           tpIter != m_taskportL.end(); tpIter++)
      {
          (*tpIter).dump();
          cerr << endl;
      }

      cerr << "Task Connections :" << endl;
      for (cIter = m_connL.begin();
           cIter != m_connL.end(); cIter++)
      {
          (*cIter).dump();
          cerr << endl;
      }

      cerr << "Iteration Blocks: " << endl;
      AX_IterBlkLSIter iBlkI;
      int bCnt = 0;
      for (iBlkI = m_iterblkL.begin();
           iBlkI != m_iterblkL.end(); iBlkI++)
      {
         cerr << "  Block " << bCnt++ << ": "
              << "itercnt: " << (*iBlkI).m_itercnt
              << "  latency: " << (*iBlkI).m_latency
              << endl << "    ";

         AX_TaskPLCIter tPI;
         for (tPI = (*iBlkI).m_taskPL.begin();
              tPI != (*iBlkI).m_taskPL.end(); tPI++)
            cerr << (*tPI)->get_name() << " ";

         cerr << endl;
      }

      cerr << "\nCycle information: " << endl;
      AX_CycleLSIter iCylI;
      int cCnt = 0;
      for (iCylI = m_cycleL.begin();
           iCylI != m_cycleL.end(); iCylI++)
      {
         cerr << "  Cycle " << cCnt++ << ": "
              << "head: " << (*iCylI).m_headP->get_name()
              << "  cycle_latency: " << (*iCylI).m_cycle_latency
              << "  lcd: " << (*iCylI).m_lcd << endl;
      }

      cerr << "\nDone TaskGraph printing\n";
      cerr << endl;
   }
// ------------------------------------------------------------
   void AX_TaskGraphC::dump_simple()
   {
      AX_TaskLCIter tIter;
      AX_TaskPortLCIter tpIter;
      AX_ConnLCIter cIter;

      cerr << "Name: " << m_name
           << "ID: " << m_id << endl
           << "  period: " << m_period
           << "  temp: " << m_temp << endl;
 
      cerr << "Tasks :" << endl;
      for (tIter = m_taskL.begin();
           tIter != m_taskL.end(); tIter++)
          (*tIter).dump();
   }
// ------------------------------------------------------------
   void AX_TaskGraphC::write(fstream & File)
   {
      AX_TaskLCIter tIter;
      AX_TaskPortLCIter tpIter;
      AX_ConnLCIter cIter;

      File << "Name: " << m_name
           << "ID: " << m_id << endl
           << "  period: " << m_period
           << "  temp: " << m_temp << endl;
 
      File << "Tasks :" << endl;
      for (tIter = m_taskL.begin();
           tIter != m_taskL.end(); tIter++)
          (*tIter).write(File);
      
      if ((m_tg_inportL.size() != 0) || (m_tg_outportL.size() != 0))
      {
         File << "TaskGraph Input Task Ports :" << endl;
         for (tpIter = m_tg_inportL.begin();
              tpIter != m_tg_inportL.end(); tpIter++)
         {
             (*tpIter).write(File);
             File << endl;
         }

         File << "TaskGraph Output Task Ports :" << endl;
         for (tpIter = m_tg_outportL.begin();
              tpIter != m_tg_outportL.end(); tpIter++)
         {
             (*tpIter).write(File);
             File << endl;
         }
      }
 
      File << "Internal Task Ports :" << endl;
      for (tpIter = m_taskportL.begin();
           tpIter != m_taskportL.end(); tpIter++)
      {
          (*tpIter).write(File);
          File << endl;
      }

      File << "Task Connections :" << endl;
      for (cIter = m_connL.begin();
           cIter != m_connL.end(); cIter++)
      {
          (*cIter).write(File);
          File << endl;
      }

      File << "Iteration Blocks: " << endl;
      AX_IterBlkLSIter iBlkI;
      int bCnt = 0;
      for (iBlkI = m_iterblkL.begin();
           iBlkI != m_iterblkL.end(); iBlkI++)
      {
         File << "  Block " << bCnt++ << ": "
              << "itercnt: " << (*iBlkI).m_itercnt
              << "  latency: " << (*iBlkI).m_latency
              << endl << "    ";

         AX_TaskPLCIter tPI;
         for (tPI = (*iBlkI).m_taskPL.begin();
              tPI != (*iBlkI).m_taskPL.end(); tPI++)
            File << (*tPI)->get_name() << " ";

         File << endl;
      }

      File << "\nCycle information: " << endl;
      AX_CycleLSIter iCylI;
      int cCnt = 0;
      for (iCylI = m_cycleL.begin();
           iCylI != m_cycleL.end(); iCylI++)
      {
         File << "  Cycle " << cCnt++ << ": "
              << "head: " << (*iCylI).m_headP->get_name()
              << "  cycle_latency: " << (*iCylI).m_cycle_latency
              << "  lcd: " << (*iCylI).m_lcd << endl;
      }

      File << "\nDone TaskGraph printing\n";
      File << endl;
   }
// ------------------------------------------------------------
   void AX_TaskGraphC::write_simple(fstream & File)
   {
      AX_TaskLCIter tIter;
      AX_TaskPortLCIter tpIter;
      AX_ConnLCIter cIter;

      File << "Name: " << m_name
           << "ID: " << m_id << endl
           << "  period: " << m_period
           << "  temp: " << m_temp << endl;
 
      File << "Tasks :" << endl;
      for (tIter = m_taskL.begin();
           tIter != m_taskL.end(); tIter++)
          (*tIter).write(File);
   }

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~End of Function Definitions
