// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// share.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <iostream>
#include "share.h"
using namespace std;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructor
// ------------------------------------------------------------ 
   AX_ShareC::AX_ShareC()
   {
      m_fncblkP = NULL;
      m_taskPL.clear();
      m_fncblkPL.clear();
   }
// ------------------------------------------------------------
   AX_ShareC::AX_ShareC(const AX_ShareC & other)
   {
      m_fncblkP = other.m_fncblkP;
      AX_TaskPLCCnstIter taskPI;
      AX_FuncBlkPLCCnstIter fncblkPI;

      m_taskPL.clear();
      for (taskPI = other.m_taskPL.begin();
           taskPI != other.m_taskPL.end(); taskPI++)
         m_taskPL.push_back(*taskPI);

      m_fncblkPL.clear();
      for (fncblkPI = other.m_fncblkPL.begin();
           fncblkPI != other.m_fncblkPL.end(); fncblkPI++)
         m_fncblkPL.push_back(*fncblkPI);
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Destructor
// ------------------------------------------------------------
   AX_ShareC::~AX_ShareC()
   {
      m_fncblkP = NULL;
      m_taskPL.clear();
      m_fncblkPL.clear();
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Assignator & Comparators
// ------------------------------------------------------------
   AX_ShareC & AX_ShareC::operator= (const AX_ShareC & other)
   {
      if (&other == this)
         return(*this);

      m_fncblkP = other.m_fncblkP;
      AX_TaskPLCCnstIter taskPI;
      AX_FuncBlkPLCCnstIter fncblkPI;

      m_taskPL.clear();
      for (taskPI = other.m_taskPL.begin();
           taskPI != other.m_taskPL.end(); taskPI++)
         m_taskPL.push_back(*taskPI);

      m_fncblkPL.clear();
      for (fncblkPI = other.m_fncblkPL.begin();
           fncblkPI != other.m_fncblkPL.end(); fncblkPI++)
         m_fncblkPL.push_back(*fncblkPI);

      return(*this);
   }
// ------------------------------------------------------------
   bool AX_ShareC::operator== (const AX_ShareC & other)
   {
      if (&other == this)
         return(true);

      if (other.m_fncblkP == m_fncblkP)
         return(true);

      return(false);
   }

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Task List Manipulations
// ------------------------------------------------------------
   AX_TaskPLCIter AX_ShareC::add_new_task(AX_TaskCP taskP)
   {
      AX_TaskPLCIter pIter;

      for (pIter = m_taskPL.begin();
           pIter != m_taskPL.end(); pIter++)
          if (taskP == (*pIter))
             return pIter;

      m_taskPL.push_back(taskP);
      pIter = --m_taskPL.end();

      return pIter;
   }
// ------------------------------------------------------------
   AX_TaskPLCIter AX_ShareC::add_task(AX_TaskCP taskP)
   {
      AX_TaskPLCIter pIter;

      m_taskPL.push_back(taskP);
      pIter = --m_taskPL.end();

      return pIter;
   }
// ------------------------------------------------------------
   AX_TaskCP AX_ShareC::get_task(int index)
   {
      AX_TaskCP taskP;

      if ((index < 0)||((unsigned int)index > m_taskPL.size()-1))
         taskP = NULL;
      else
      {
         AX_TaskPLCIter pIter;
         int i;

         pIter = m_taskPL.begin();
         for(i = 0; i < index; i++) pIter++;

         taskP = (*pIter);  
      }

      return taskP;
   }
// ------------------------------------------------------------
   void AX_ShareC::remove_task(AX_TaskCP taskP)
   {
      AX_TaskPLCIter pIter;

      for (pIter = m_taskPL.begin();
           pIter != m_taskPL.end(); pIter++)
          if ((*pIter) == taskP)
          {
             m_taskPL.erase(pIter);
             return;
          }
      return;
   }
// ------------------------------------------------------------
   AX_TaskPLCIter AX_ShareC::delete_task(AX_TaskPLCIter & iter)
   {
      AX_TaskPLCIter retIter;

      if (m_taskPL.end() == iter)
         return iter;
      else if ((--m_taskPL.end()) == iter)
      {
         m_taskPL.erase(iter);
         retIter = m_taskPL.end();
      }
      else
      {
         retIter = ++iter;
         iter--;
         m_taskPL.erase(iter);
      }

      return retIter;
   }
// ------------------------------------------------------------
   AX_TaskPLCIter AX_ShareC::delete_task(AX_TaskCP taskP)
   {
      AX_TaskPLCIter retIter, iter;

      if (taskP == NULL)
         return m_taskPL.end();

      iter = --m_taskPL.end();
      if ((*iter) == taskP)
      {
         m_taskPL.erase(iter);
         return m_taskPL.end();
      }
      retIter = m_taskPL.end();
      for (iter = m_taskPL.begin();
           iter != m_taskPL.end(); iter++)
         if ((*iter) == taskP)
         {
            retIter = ++iter;
            iter--;
            m_taskPL.erase(iter);
         }

      return retIter;
   }
// ------------------------------------------------------------
   AX_FuncBlkCP AX_ShareC::get_mapping_fncblk(AX_TaskCP taskP)
   {
      AX_TaskPLCIter tIter;
      AX_FuncBlkPLCIter fIter = m_fncblkPL.begin();

      for (tIter = m_taskPL.begin();
           tIter != m_taskPL.end(); tIter++)
      {
         if ((*tIter) == taskP)
            return (*fIter);
         fIter++;
      }

      return NULL;
   }

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Fncblk List Manipulations
// ------------------------------------------------------------
   AX_FuncBlkPLCIter AX_ShareC::add_new_fncblk(AX_FuncBlkCP fncblkP)
   {
      AX_FuncBlkPLCIter pIter;

      for (pIter = m_fncblkPL.begin();
           pIter != m_fncblkPL.end(); pIter++)
          if (fncblkP == (*pIter))
             return pIter;

      m_fncblkPL.push_back(fncblkP);
      pIter = --m_fncblkPL.end();

      return pIter;
   }
// ------------------------------------------------------------
   AX_FuncBlkPLCIter AX_ShareC::add_fncblk(AX_FuncBlkCP fncblkP)
   {
      AX_FuncBlkPLCIter pIter;

      m_fncblkPL.push_back(fncblkP);
      pIter = --m_fncblkPL.end();

      return pIter;
   }
// ------------------------------------------------------------
   AX_FuncBlkCP AX_ShareC::get_fncblk(int index)
   {
      AX_FuncBlkCP fncblkP;

      if ((index < 0)||((unsigned int)index > m_fncblkPL.size()-1))
         fncblkP = NULL;
      else
      {
         AX_FuncBlkPLCIter pIter;
         int i;

         pIter = m_fncblkPL.begin();
         for(i = 0; i < index; i++) pIter++;

         fncblkP = (*pIter);  
      }

      return fncblkP;
   }
// ------------------------------------------------------------
   void AX_ShareC::remove_fncblk(AX_FuncBlkCP fncblkP)
   {
      AX_FuncBlkPLCIter pIter;

      for (pIter = m_fncblkPL.begin();
           pIter != m_fncblkPL.end(); pIter++)
          if ((*pIter) == fncblkP)
          {
             m_fncblkPL.erase(pIter);
             return;
          }
      return;
   }
// ------------------------------------------------------------
   AX_FuncBlkPLCIter AX_ShareC::delete_fncblk(AX_FuncBlkPLCIter & iter)
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
   AX_FuncBlkPLCIter AX_ShareC::delete_fncblk(AX_FuncBlkCP fncblkP)
   {
      AX_FuncBlkPLCIter retIter, iter;

      if (fncblkP == NULL)
         return m_fncblkPL.end();

      iter = --m_fncblkPL.end();
      if ((*iter) == fncblkP)
      {
         m_fncblkPL.erase(iter);
         return m_fncblkPL.end();
      }
      retIter = m_fncblkPL.end();
      for (iter = m_fncblkPL.begin();
           iter != m_fncblkPL.end(); iter++)
         if ((*iter) == fncblkP)
         {
            retIter = ++iter;
            iter--;
            m_fncblkPL.erase(iter);
         }

      return retIter;
   }
// ------------------------------------------------------------
   AX_TaskCP AX_ShareC::get_mapping_task(AX_FuncBlkCP fncblkP)
   {
      AX_FuncBlkPLCIter fIter;
      AX_TaskPLCIter tIter = m_taskPL.begin();

      for (fIter = m_fncblkPL.begin();
           fIter != m_fncblkPL.end(); fIter++)
      {
          if ((*fIter) == fncblkP)
            return (*tIter);
          tIter++;
      }

      return NULL;
   }

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~SW Fncblk List Manipulations
// ------------------------------------------------------------
   AX_FuncBlkPLCIter AX_ShareC::add_new_sw_fncblk(AX_FuncBlkCP fncblkP)
   {
      AX_FuncBlkPLCIter pIter;

      for (pIter = m_sw_fncblkPL.begin();
           pIter != m_sw_fncblkPL.end(); pIter++)
          if (fncblkP == (*pIter))
             return pIter;

      m_sw_fncblkPL.push_back(fncblkP);
      pIter = --m_sw_fncblkPL.end();

      return pIter;
   }
// ------------------------------------------------------------
   AX_FuncBlkPLCIter AX_ShareC::add_sw_fncblk(AX_FuncBlkCP fncblkP)
   {
      AX_FuncBlkPLCIter pIter;

      m_sw_fncblkPL.push_back(fncblkP);
      pIter = --m_sw_fncblkPL.end();

      return pIter;
   }
// ------------------------------------------------------------
   AX_FuncBlkCP AX_ShareC::get_sw_fncblk(int index)
   {
      AX_FuncBlkCP fncblkP;

      if ((index < 0)||((unsigned int)index > m_sw_fncblkPL.size()-1))
         fncblkP = NULL;
      else
      {
         AX_FuncBlkPLCIter pIter;
         int i;

         pIter = m_sw_fncblkPL.begin();
         for(i = 0; i < index; i++) pIter++;

         fncblkP = (*pIter);  
      }

      return fncblkP;
   }
// ------------------------------------------------------------
   void AX_ShareC::remove_sw_fncblk(AX_FuncBlkCP fncblkP)
   {
      AX_FuncBlkPLCIter pIter;

      for (pIter = m_sw_fncblkPL.begin();
           pIter != m_sw_fncblkPL.end(); pIter++)
          if ((*pIter) == fncblkP)
          {
             m_sw_fncblkPL.erase(pIter);
             return;
          }
      return;
   }
// ------------------------------------------------------------
   AX_FuncBlkPLCIter AX_ShareC::delete_sw_fncblk(AX_FuncBlkPLCIter & iter)
   {
      AX_FuncBlkPLCIter retIter;

      if (m_sw_fncblkPL.end() == iter)
         return iter;
      else if ((--m_sw_fncblkPL.end()) == iter)
      {
         m_sw_fncblkPL.erase(iter);
         retIter = m_sw_fncblkPL.end();
      }
      else
      {
         retIter = ++iter;
         iter--;
         m_sw_fncblkPL.erase(iter);
      }

      return retIter;
   }
// ------------------------------------------------------------
   AX_FuncBlkPLCIter AX_ShareC::delete_sw_fncblk(AX_FuncBlkCP fncblkP)
   {
      AX_FuncBlkPLCIter retIter, iter;

      if (fncblkP == NULL)
         return m_sw_fncblkPL.end();

      iter = --m_sw_fncblkPL.end();
      if ((*iter) == fncblkP)
      {
         m_sw_fncblkPL.erase(iter);
         return m_sw_fncblkPL.end();
      }
      retIter = m_sw_fncblkPL.end();
      for (iter = m_sw_fncblkPL.begin();
           iter != m_sw_fncblkPL.end(); iter++)
         if ((*iter) == fncblkP)
         {
            retIter = ++iter;
            iter--;
            m_sw_fncblkPL.erase(iter);
         }

      return retIter;
   }
// ------------------------------------------------------------
   AX_FuncBlkCP AX_ShareC::get_mapping_sw_fncblk(AX_TaskCP taskP)
   {
      AX_TaskPLCIter tIter;
      AX_FuncBlkPLCIter fIter = m_sw_fncblkPL.begin();

      for (tIter = m_taskPL.begin();
           tIter != m_taskPL.end(); tIter++)
      {
         if ((*tIter) == taskP)
            return (*fIter);
         fIter++;
      }

      return NULL;
   }

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Instance List Manipulations


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Structure List Manipulations


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Special Functions


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Miscellaneous Functions
// ------------------------------------------------------------
   void AX_ShareC::dump()
   {
      cerr << "\n    Sharing component: "
           << m_fncblkP->get_name() << endl;
      cerr << "    Sharing list: task -> implementation\n";
      AX_TaskPLCIter taskPI;
      AX_FuncBlkPLCIter swfncblkPI = m_sw_fncblkPL.begin();;
      AX_FuncBlkPLCIter fncblkPI = m_fncblkPL.begin();

      for (taskPI = m_taskPL.begin();
           taskPI != m_taskPL.end(); taskPI++)
      {
         cerr << "      " << (*taskPI)->get_name()
              << ": " << (*swfncblkPI)->get_name()
              << " -> " << (*fncblkPI)->get_name()
              << endl;
         swfncblkPI++;
         fncblkPI++;
      }
   }
// ------------------------------------------------------------
   void AX_ShareC::write(fstream & File)
   {
      File << "\n    Sharing component: "
           << m_fncblkP->get_name() << endl;
      File << "    Sharing list: task -> implementation\n";
      AX_TaskPLCIter taskPI;
      AX_FuncBlkPLCIter swfncblkPI = m_sw_fncblkPL.begin();;
      AX_FuncBlkPLCIter fncblkPI = m_fncblkPL.begin();

      for (taskPI = m_taskPL.begin();
           taskPI != m_taskPL.end(); taskPI++)
      {
         File << "      " << (*taskPI)->get_name()
              << ": " << (*swfncblkPI)->get_name()
              << " -> " << (*fncblkPI)->get_name()
              << endl;
         swfncblkPI++;
         fncblkPI++;
      }
   }
// ------------------------------------------------------------
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~End of Function Definitions
