// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// bus.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <iostream>
#include "bus.h"
using namespace std;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructor
// ------------------------------------------------------------ 
   AX_BusC::AX_BusC()
   {
      m_id = -1;
      m_db_idx = -1;
      m_intrfcP = NULL;
      m_util = 0.0;
   }
// ------------------------------------------------------------
   AX_BusC::AX_BusC(const AX_BusC & other)
   {
      m_id = other.m_id;
      m_db_idx = other.m_db_idx;
      m_intrfcP = other.m_intrfcP;
      m_connpntL = other.m_connpntL;
      m_util = other.m_util;
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Destructor
// ------------------------------------------------------------
   AX_BusC::~AX_BusC()
   {
      remove_all_connrefs();
      clean_up();
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Assignator & Comparators
// ------------------------------------------------------------
   AX_BusC & AX_BusC::operator= (const AX_BusC & other)
   {
      if (&other == this)
         return(*this);

      m_id = other.m_id;
      m_db_idx = other.m_db_idx;
      m_intrfcP = other.m_intrfcP;
      m_util = other.m_util;
      m_connpntL = other.m_connpntL;

      AX_ConnRefLSCnstIter connrefI;
      remove_all_connrefs();

      for (connrefI = other.m_connrefL.begin();
           connrefI != other.m_connrefL.end(); connrefI++)
      {
          AX_ConnRefS nConnR;
          AX_ConnRefLSIter nConnI;

          m_connrefL.push_back(nConnR);
          nConnI = --m_connrefL.end();

          (*nConnI).m_connP = (*connrefI).m_connP;
          (*nConnI).m_start = (*connrefI).m_start;
          (*nConnI).m_finish = (*connrefI).m_finish;
      }

      return(*this);
   }
// ------------------------------------------------------------
   bool AX_BusC::operator== (const AX_BusC & other)
   {
      if (&other == this)
         return(true);

      if (other.m_id == m_id &&
          other.m_db_idx == m_db_idx &&
          other.m_intrfcP == m_intrfcP)
         return(true);

      return(false);
   }
// ------------------------------------------------------------

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Instance List Manipulations
// ------------------------------------------------------------
   AX_ConnPntLCIter AX_BusC::add_new_connpnt(AX_ConnPntC & connpnt)
   {
      AX_ConnPntLCIter cIter;

      for (cIter = m_connpntL.begin();
           cIter != m_connpntL.end(); cIter++)
          if (connpnt.get_ag() == (*cIter).get_ag() &&
              connpnt.get_comp() == (*cIter).get_comp() &&
              connpnt.get_port() == (*cIter).get_port())
             return cIter;

      m_connpntL.push_back(connpnt);
      cIter = --m_connpntL.end();

      return cIter;
   }
// ------------------------------------------------------------
   AX_ConnPntLCIter AX_BusC::add_connpnt(AX_ConnPntC & connpnt)
   {
      AX_ConnPntLCIter cIter;

      m_connpntL.push_back(connpnt);
      cIter = --m_connpntL.end();

      return cIter;
   }
// ------------------------------------------------------------
   AX_ConnPntCP AX_BusC::get_connpnt(int index)
   {
      AX_ConnPntCP connpntP;

      if ((index < 0)||((unsigned int)index > m_connpntL.size()-1))
         connpntP = NULL;
      else
      {
         AX_ConnPntLCIter cIter;
         int i;

         cIter = m_connpntL.begin();
         for(i = 0; i < index; i++) cIter++;

         connpntP = &(*cIter);
      }

      return connpntP;
   }
// ------------------------------------------------------------
   void AX_BusC::remove_connpnt(AX_ConnPntCP connpntP)
   {
      if (m_connpntL.size() == 0)
         return;

      AX_ConnPntLCIter cIter;

      for (cIter = m_connpntL.begin();
           cIter != m_connpntL.end(); cIter++)
          if (&(*cIter) == connpntP)
          {
             m_connpntL.erase(cIter);
             return;
          }
      return;
   }
// ------------------------------------------------------------
   AX_ConnPntLCIter AX_BusC::delete_connpnt(AX_ConnPntLCIter & iter)
   {
      if (m_connpntL.size() == 0)
         return m_connpntL.end();

      AX_ConnPntLCIter retIter;

      if (m_connpntL.end() == iter)
         return iter;
      else if ((--m_connpntL.end()) == iter)
      {
         m_connpntL.erase(iter);
         retIter = m_connpntL.end();
      }
      else
      {
         retIter = ++iter;
         iter--;
         m_connpntL.erase(iter);
      }

      return retIter;
   }
// ------------------------------------------------------------
   AX_ConnPntLCIter AX_BusC::delete_connpnt(AX_ConnPntCP funcP)
   {
      if (m_connpntL.size() == 0)
         return m_connpntL.end();

      AX_ConnPntLCIter cIter;

      if (&(*(--m_connpntL.end())) == funcP)
      {
         AX_ConnPntLCIter iter = --m_connpntL.end();
         m_connpntL.erase(iter);
         return m_connpntL.end();
      }

      for (cIter = m_connpntL.begin();
           cIter != m_connpntL.end(); cIter++)
          if (&(*cIter) == funcP)
          {
             AX_ConnPntLCIter retIter = ++cIter;
             cIter--;
             m_connpntL.erase(cIter);
             return retIter;
          }
      return cIter;
   }
// ------------------------------------------------------------
   AX_ConnPntLCIter AX_BusC::delete_connpnt(AX_ArchGraphCP agP,
                                              AX_CompCP compP,
                                              AX_CompPortCP portP)
   {
      if (m_connpntL.size() == 0)
         return m_connpntL.end();

      AX_ConnPntLCIter cIter;

      cIter = --m_connpntL.end();
      if ((*cIter).get_ag() == agP &&
          (*cIter).get_comp() == compP &&
          (*cIter).get_port() == portP)
      {
         AX_ConnPntLCIter iter = --m_connpntL.end();
         m_connpntL.erase(iter);
         return m_connpntL.end();
      }

      for (cIter = m_connpntL.begin();
           cIter != m_connpntL.end(); cIter++)
          if ((*cIter).get_ag() == agP &&
              (*cIter).get_comp() == compP &&
              (*cIter).get_port() == portP)
          {
             AX_ConnPntLCIter retIter = ++cIter;
             cIter--;
             m_connpntL.erase(cIter);
             return retIter;
          }
      return cIter;
   }
// ------------------------------------------------------------
   AX_ConnPntCP AX_BusC::find_connpnt_by_comp_port(AX_CompCP comp,
                                                     AX_CompPortCP port)
   {
      AX_ConnPntLCIter cIter;
      for (cIter = get2_first_connpnt();
           cIter != connpnt_list_end(); cIter++)
      {
         if ((*cIter).get_comp() == comp &&
             (*cIter).get_port())
            return &(*cIter);
      }

      return NULL;
   }
// ------------------------------------------------------------
   AX_ConnPntCP AX_BusC::find_connpnt_by_ag_port(AX_ArchGraphCP ag,
                                                   AX_CompPortCP port)
   {
      AX_ConnPntLCIter cIter;
      for (cIter = get2_first_connpnt();
           cIter != connpnt_list_end(); cIter++)
      {
         if ((*cIter).get_ag() == ag &&
             (*cIter).get_port())
            return &(*cIter);
      }

      return NULL;
   }

// ------------------------------------------------------------
   AX_ConnRefLSIter AX_BusC::add_new_connref(AX_ConnRefS connref)
   {
      AX_ConnRefLSIter nConnI;

      for (nConnI = m_connrefL.begin();
           nConnI != m_connrefL.end(); nConnI++)
          if (connref.m_connP == (*nConnI).m_connP)
             return nConnI;

      AX_ConnRefS nConnR;

      m_connrefL.push_back(nConnR);
      nConnI = --m_connrefL.end();
      (*nConnI).m_connP = connref.m_connP;
      (*nConnI).m_start = connref.m_start;
      (*nConnI).m_finish = connref.m_finish;

      return nConnI;
   }
// ------------------------------------------------------------
   AX_ConnRefLSIter AX_BusC::add_new_connref(AX_ConnCP connP)
   {
      AX_ConnRefLSIter nConnI;

      for (nConnI = m_connrefL.begin();
           nConnI != m_connrefL.end(); nConnI++)
          if (connP == (*nConnI).m_connP)
             return nConnI;

      AX_ConnRefS nConnR;

      m_connrefL.push_back(nConnR);
      nConnI = --m_connrefL.end();
      (*nConnI).m_connP = connP;
      (*nConnI).m_start = -1;
      (*nConnI).m_finish = -1;

      return nConnI;
   }
// ------------------------------------------------------------
   AX_ConnRefSP AX_BusC::get_connref(int index)
   {
      AX_ConnRefSP connrefP;

      if ((index < 0)||((unsigned int)index > m_connrefL.size()-1))
         connrefP = NULL;
      else
      {
         AX_ConnRefLSIter fIter;
         int i;

         fIter = m_connrefL.begin();
         for(i = 0; i < index; i++) fIter++;

         connrefP = &(*fIter);
      }

      return connrefP;
   }
// ------------------------------------------------------------
   void AX_BusC::remove_connref(AX_ConnRefSP connrefP)
   {
      AX_ConnRefLSIter fIter;

      for (fIter = m_connrefL.begin();
           fIter != m_connrefL.end(); fIter++)
          if (&(*fIter) == connrefP)
          {
             m_connrefL.erase(fIter);
             return;
          }
      return;
   }
// ------------------------------------------------------------
   void AX_BusC::remove_connref(AX_ConnCP connP)
   {
      AX_ConnRefLSIter fIter;

      for (fIter = m_connrefL.begin();
           fIter != m_connrefL.end(); fIter++)
          if ((*fIter).m_connP == connP)
          {
             m_connrefL.erase(fIter);
             return;
          }
      return;
   }
// ------------------------------------------------------------
   AX_ConnRefLSIter AX_BusC::delete_connref(AX_ConnRefLSIter & iter)
   {
      AX_ConnRefLSIter retIter;

      if (m_connrefL.end() == iter)
         return iter;
      else if ((--m_connrefL.end()) == iter)
      {
         m_connrefL.erase(iter);
         retIter = m_connrefL.end();
      }
      else
      {
         retIter = ++iter;
         iter--;
         (*iter).m_connP = NULL;
         (*iter).m_start = 0;
         (*iter).m_finish = 0;
         m_connrefL.erase(iter);
      }

      return retIter;
   }
// ------------------------------------------------------------
   AX_ConnRefLSIter AX_BusC::delete_connref(AX_ConnRefSP connrefP)
   {
      AX_ConnRefLSIter iter, retIter;

      if (connrefP == NULL)
         return m_connrefL.end();

      if (&(*(--m_connrefL.end())) == connrefP)
      {
         iter = --m_connrefL.end();
         m_connrefL.erase(iter);
         return m_connrefL.end();
      }
      else
      {
         for (iter = m_connrefL.begin();
              iter != m_connrefL.end(); iter++)
         {
            if (&(*iter) == connrefP)
            {
               retIter = ++iter;
               iter--;
               (*iter).m_connP = NULL;
               (*iter).m_start = 0;
               (*iter).m_finish = 0;
               m_connrefL.erase(iter);
               return retIter;
            }
         }
      }

      return retIter;
   }
// ------------------------------------------------------------
   AX_ConnRefLSIter AX_BusC::delete_connref(AX_ConnCP connP)
   {
      AX_ConnRefLSIter iter, retIter;

      if (connP == NULL)
         return m_connrefL.end();

      if (m_connrefL.size() == 0)
         return m_connrefL.end();
      else if ((*(--m_connrefL.end())).m_connP == connP)
      {
         iter = --m_connrefL.end();
         m_connrefL.erase(iter);
         return m_connrefL.end();
      }
      else
      {
         for (iter = m_connrefL.begin();
              iter != m_connrefL.end(); iter++)
         {
            if ((*iter).m_connP == connP)
            {
               retIter = ++iter;
               iter--;
               (*iter).m_connP = NULL;
               (*iter).m_start = 0;
               (*iter).m_finish = 0;
               m_connrefL.erase(iter);
               return retIter;
            }
         }
      }

      return retIter;
   }
// ------------------------------------------------------------
   AX_ConnRefLSIter AX_BusC::delete_connref(bool source,
                                              AX_TaskCP taskP,
                                              string pName)
   {
      AX_ConnRefLSIter iter, retIter;

      if (taskP == NULL || pName.empty())
         return m_connrefL.end();

      if (source &&
          (*(--m_connrefL.end())).m_connP->get_src_task() == taskP &&
          (*(--m_connrefL.end())).m_connP->get_src_port()->get_name() == pName)
      {
         iter = --m_connrefL.end();
         m_connrefL.erase(iter);
         return m_connrefL.end();
      }
      else if (!source &&
          (*(--m_connrefL.end())).m_connP->get_sink_task() == taskP &&
          (*(--m_connrefL.end())).m_connP->get_sink_port()->get_name() == pName)
      {
         iter = --m_connrefL.end();
         m_connrefL.erase(iter);
         return m_connrefL.end();
      }
      else
      {
         for (iter = m_connrefL.begin();
              iter != m_connrefL.end(); iter++)
         {
            if (source)
            {
               if ((*iter).m_connP->get_src_task() == taskP &&
                   (*iter).m_connP->get_src_port()->get_name() == pName)
               {
                  retIter = ++iter;
                  iter--;
                  (*iter).m_connP = NULL;
                  (*iter).m_start = 0;
                  (*iter).m_finish = 0;
                  m_connrefL.erase(iter);
                  return retIter;
               }
            }
            else
            {
               if ((*iter).m_connP->get_sink_task() == taskP &&
                   (*iter).m_connP->get_sink_port()->get_name() == pName)
               {
                  retIter = ++iter;
                  iter--;
                  (*iter).m_connP = NULL;
                  (*iter).m_start = 0;
                  (*iter).m_finish = 0;
                  m_connrefL.erase(iter);
                  return retIter;
               }
            }
         }
      }

      return iter;
   }
// ------------------------------------------------------------
   void AX_BusC::remove_all_connrefs()
   {
      if (m_connrefL.size() == 0)
         return;

      AX_ConnRefLSIter iter;
      while ((iter = m_connrefL.begin()) != m_connrefL.end())
      {
         (*iter).m_connP = NULL;
         (*iter).m_start = 0;
         (*iter).m_finish = 0;
         m_connrefL.erase(iter);
      }
   }
// ------------------------------------------------------------
   bool AX_BusC::is_mapped_conn(AX_ConnCP connP)
   {
      AX_ConnRefLSIter iter;
      for (iter = m_connrefL.begin();
           iter != m_connrefL.end(); iter++)
      {
         if ((*iter).m_connP == connP)
            return true;
      }

      return false;
   }
// ------------------------------------------------------------
   bool compare_conn_start(AX_ConnRefS const & r1,
                           AX_ConnRefS const & r2)
   {
/* Before using std list sort
      int arg1 = r1.m_start;
      int arg2 = r2.m_start;

      if (arg1 == arg2)
         return true;
      else if (arg1 < arg2)
         return false;
      else
         return true;
*/
      return (r1.m_start < r2.m_start);
   }
// ------------------------------------------------------------
   void AX_BusC::sort_connref_by_start()
   {
      if (m_connrefL.size() <= 0)
         return;
#if DEBUG >= 5
      cerr << "AX_BusC::sort_connref_by_start: before sort "
           << "size is " << m_connrefL.size() << endl;
      (*this).dump();
#endif
      m_connrefL.sort(compare_conn_start);
#if DEBUG >= 5
      cerr << "AX_BusC::sort_connref_by_start: after sort" << endl;
      (*this).dump();
#endif

      return;
   }
// ------------------------------------------------------------
   void AX_BusC::clean_connrefs()
   {
      AX_ConnRefLSIter iter;
      for (iter = m_connrefL.begin();
           iter != m_connrefL.end(); iter++)
      {
         (*iter).m_start = -1;
         (*iter).m_finish = -1;
      }
   }
// ------------------------------------------------------------
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Structure List Manipulations


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Special Functions


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Miscellaneous Functions
// ------------------------------------------------------------
   void AX_BusC::clean_up()
   {
      m_id = 0;
      m_db_idx = 0;
      m_intrfcP = NULL;
      m_util = 0.0;
      m_connpntL.clear();
   }
// ------------------------------------------------------------
   void AX_BusC::dump()
   {
     cerr.precision(4);
     cerr << "          Bus id: " << m_id
          << "  db_idx " << m_db_idx
          << "  utilization " << m_util
          << endl;
     if (m_intrfcP != NULL)
        cerr << "          interface: "
             <<  m_intrfcP->get_name() << endl;

     AX_ConnPntLCIter cIter;
     cerr << "          connpntL: " << endl;
     for (cIter = m_connpntL.begin();
          cIter != m_connpntL.end(); cIter++)
     {
        cerr << "  ";
        (*cIter).dump();
     }

     AX_ConnRefLSIter conRefI;
     cerr << "          connrefL: " << endl;
     for (conRefI = m_connrefL.begin();
          conRefI != m_connrefL.end(); conRefI++)
     {
        cerr << "      ";
        if ((*conRefI).m_connP != AX_ConnCP(1) &&
            (*conRefI).m_connP != AX_ConnCP(2))
           (*conRefI).m_connP->dump_simple();
        else
           cerr << "      Dummy connP" << endl;
        cerr << "            [start,finish]: "
             << "[" << (*conRefI).m_start << ","
             << (*conRefI).m_finish << "]" << endl;
     }
     cerr << endl;
   }
// ------------------------------------------------------------
   void AX_BusC::write(fstream & File)
   {
     File.precision(4);
     File << "          Bus id: " << m_id
          << "  db_idx " << m_db_idx
          << "  utilization " << m_util
          << endl;
     if (m_intrfcP != NULL)
        File << "          interface: "
             <<  m_intrfcP->get_name() << endl;

     AX_ConnPntLCIter cIter;
     File << "          connpntL: " << endl;
     for (cIter = m_connpntL.begin();
          cIter != m_connpntL.end(); cIter++)
     {
        File << "  ";
        (*cIter).write(File);
     }

     AX_ConnRefLSIter conRefI;
     File << "          connrefL: " << endl;
     for (conRefI = m_connrefL.begin();
          conRefI != m_connrefL.end(); conRefI++)
     {
        File << "      ";
        (*conRefI).m_connP->write_simple(File);
        File << "            [start,finish]: "
             << "[" << (*conRefI).m_start << ","
             << (*conRefI).m_finish << "]" << endl;
     }
     File << endl;
   }
// ------------------------------------------------------------
   void AX_BusC::dump_simple()
   {
     cerr.precision(4);
     cerr << "          Bus id: " << m_id
          << "  db_idx " << m_db_idx
          << "  utilization " << m_util
          << endl;
     if (m_intrfcP != NULL)
        cerr << "          interface: "
             <<  m_intrfcP->get_name() << endl;

     AX_ConnPntLCIter cIter;
     cerr << "          connpntL: " << endl;
     for (cIter = m_connpntL.begin();
          cIter != m_connpntL.end(); cIter++)
     {
        cerr << "  ";
        (*cIter).dump();
     }
   }
// ------------------------------------------------------------
   void AX_BusC::write_simple(fstream & File)
   {
     File.precision(4);
     File << "          Bus id: " << m_id
          << "  db_idx " << m_db_idx
          << "  utilization " << m_util
          << endl;
     if (m_intrfcP != NULL)
        File << "          interface: "
             <<  m_intrfcP->get_name() << endl;

     AX_ConnPntLCIter cIter;
     File << "          connpntL: " << endl;
     for (cIter = m_connpntL.begin();
          cIter != m_connpntL.end(); cIter++)
     {
        File << "  ";
        (*cIter).write(File);
     }
   }
// ------------------------------------------------------------
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~End of Function Definitions
