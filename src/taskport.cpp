// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// taskport.c
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <iostream>
#include "taskport.h"
using namespace std;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructor
// ------------------------------------------------------------
   AX_TaskPortC::AX_TaskPortC()
   {
      m_name.clear();
      m_id = -1;
      m_db_idx = -1;
      m_type = PORT_TYPE_UNKNOWN;
      m_parent_taskP = NULL;
      m_parent_tgP = NULL;
      m_no_samples = -1;
      m_sample_size = -1;
      m_temp = -1;
   }
// ------------------------------------------------------------
   AX_TaskPortC::AX_TaskPortC(const AX_TaskPortC & other)
   {
      copy(other);
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Destructor
// ------------------------------------------------------------
   AX_TaskPortC::~AX_TaskPortC()
   {
      remove_all_conns();
      clean_up();
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Assignator & Comparators
// ------------------------------------------------------------
   AX_TaskPortC & AX_TaskPortC::operator= (const AX_TaskPortC & other)
   {
      if (&other == this)
         return(*this);

      copy(other);

      return(*this);
   }
// ------------------------------------------------------------
   void AX_TaskPortC::copy(const AX_TaskPortC & other)
   {
      m_name = other.m_name;
      m_id = other.m_id;
      m_db_idx = other.m_db_idx;
      m_type = other.m_type;
      m_parent_taskP = other.m_parent_taskP;
      m_parent_tgP = other.m_parent_tgP;
      m_connPL = other.m_connPL;
      m_no_samples = other.m_no_samples;
      m_sample_size = other.m_sample_size;
      m_type = other.m_type;
      m_temp = other.m_temp;

      return;
   }
// ------------------------------------------------------------
   void AX_TaskPortC::copyBasic(const AX_TaskPortC & other)
   {
      m_name = other.m_name;
      m_id = other.m_id;
      m_db_idx = other.m_db_idx;
      m_type = other.m_type;
      m_parent_taskP = other.m_parent_taskP;
      m_parent_tgP = other.m_parent_tgP;
      m_no_samples = other.m_no_samples;
      m_sample_size = other.m_sample_size;
      m_type = other.m_type;
      m_temp = other.m_temp;

      return;
   }
// ------------------------------------------------------------
   bool AX_TaskPortC::operator== (const AX_TaskPortC & other)
   {
      if (m_name == other.m_name &&
          m_parent_taskP == other.m_parent_taskP &&
          m_parent_tgP == other.m_parent_tgP)
         return(true);

      return(false);
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Pointer List Manipulations
// ------------------------------------------------------------
   AX_ConnPLCIter AX_TaskPortC::add_new_conn(AX_ConnCP connP)
   {
      if (connP == NULL)
         return m_connPL.end();

      AX_ConnPLCIter cIter;

      for (cIter = m_connPL.begin();
           cIter != m_connPL.end(); cIter++)
          if (connP == (*cIter))
             return cIter;

      m_connPL.push_back(connP);

      return (--m_connPL.end());
   }
// ------------------------------------------------------------
   AX_ConnCP AX_TaskPortC::get_conn(int index)
   {
      AX_ConnCP connP;

      if ((index < 0)||((unsigned int)index > m_connPL.size()-1))
         connP = NULL;
      else
      {
         AX_ConnPLCIter cIter;
         int i;

         cIter = m_connPL.begin();
         for(i = 0; i < index; i++) cIter++;

         connP = (*cIter);
      }

      return connP;
   }
// ------------------------------------------------------------
   void AX_TaskPortC::remove_conn(AX_ConnCP connP)
   {
       AX_ConnPLCIter cIter;

       for (cIter = m_connPL.begin();
            cIter != m_connPL.end(); cIter++)
           if ((*cIter) == connP)
           {
               m_connPL.erase(cIter);
               return;
           }
       return;
   }
// ------------------------------------------------------------
   AX_ConnPLCIter AX_TaskPortC::delete_conn(AX_ConnPLCIter & iter)
   {
      AX_ConnPLCIter retIter;

      if (m_connPL.end() == iter)
         return iter;
      else if ((--m_connPL.end()) == iter)
      {
         m_connPL.erase(iter);
         retIter = m_connPL.end();
      }
      else
      {
         retIter = ++iter;
         iter--;
         m_connPL.erase(iter);
      }

      return retIter;
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Instance List Manipulations


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Structure List Manipulations


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Special Functions


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Miscellaneous Functions
// ------------------------------------------------------------
   void AX_TaskPortC::clean_up()
   {
      m_name.clear();
      m_id = -1;
      m_db_idx = -1;
      m_type = PORT_TYPE_UNKNOWN;
      m_parent_taskP = NULL;
      m_parent_tgP = NULL;
      m_no_samples = -1;
      m_sample_size = -1;
      m_temp = -1;
   }
// ------------------------------------------------------------
   void AX_TaskPortC::dump()
   {
     cerr << "    Name = " << m_name;
     cerr << ". Id = " << m_id;
     cerr << ". Db_Idx = " << m_db_idx;
     if (m_parent_tgP == NULL)
        cerr << ". Task = " << m_parent_taskP->get_name();
     else
        cerr << ". Task Graph = " << m_parent_tgP->get_name();
     cerr << ". Type = " << m_type << endl;
     cerr << "    No Sample = " << m_no_samples;
     cerr << ". Sample size = " << m_sample_size;
     cerr << endl;
     cerr << "    Conns:" << endl;

     AX_ConnPLCIter cIter;

     for (cIter = m_connPL.begin(); cIter != m_connPL.end(); cIter++)
         (*cIter)->dump_simple();
   }
// ------------------------------------------------------------
   void AX_TaskPortC::write(fstream & File)
   {
     File << "    Name = " << m_name;
     File << ". Id = " << m_id;
     File << ". Db_Idx = " << m_db_idx;
     if (m_parent_tgP == NULL)
        File << ". Task = " << m_parent_taskP->get_name();
     else
        File << ". Task Graph = " << m_parent_tgP->get_name();
     File << ". Type = " << m_type << endl;
     File << "    No Sample = " << m_no_samples;
     File << ". Sample size = " << m_sample_size;
     File << endl;
     File << "    Conns:" << endl;

     AX_ConnPLCIter cIter;

     for (cIter = m_connPL.begin(); cIter != m_connPL.end(); cIter++)
         (*cIter)->write_simple(File);
   }
// ------------------------------------------------------------

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~End of Function Definitions
