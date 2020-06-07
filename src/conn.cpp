// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// conn.c
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <iostream>
#include "conn.h"
using namespace std;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructor
// ------------------------------------------------------------
   AX_ConnC::AX_ConnC()
   {
      m_id = -1;
      m_db_idx = -1;
      m_type = CONN_TYPE_UNKNOWN;
      m_src_tgP = NULL;
      m_sink_tgP = NULL;
      m_src_portP = NULL;
      m_src_taskP = NULL;
      m_sink_portP = NULL;
      m_sink_taskP = NULL;
      m_avg_activ = -1;
      m_pk_activ = -1;
      m_temp = -1;
   }
// ------------------------------------------------------------
   AX_ConnC::AX_ConnC(const AX_ConnC & other)
   {
      copy(other);
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Destructor
// ------------------------------------------------------------
   AX_ConnC::~AX_ConnC()
   {
      clean_up();
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Assignator & Comparators
// ------------------------------------------------------------
   AX_ConnC & AX_ConnC::operator= (const AX_ConnC & other)
   {
      if (&other == this)
         return(*this);

      copy(other);

      return(*this);
   }
// ------------------------------------------------------------
   void AX_ConnC::copy(const AX_ConnC & other)
   {
      m_id = other.m_id;
      m_db_idx = other.m_db_idx;
      m_type = other.m_type;
      m_src_tgP = other.m_src_tgP;
      m_sink_tgP = other.m_sink_tgP;
      m_src_portP = other.m_src_portP;
      m_src_taskP = other.m_src_taskP;
      m_sink_portP = other.m_sink_portP;
      m_sink_taskP = other.m_sink_taskP;
      m_avg_activ = other.m_avg_activ;
      m_pk_activ = other.m_pk_activ;
      m_temp = other.m_temp;

      return;
   }
// ------------------------------------------------------------
   void AX_ConnC::copyBasic(const AX_ConnC & other)
   {
      m_id = other.m_id;
      m_db_idx = other.m_db_idx;
      m_type = other.m_type;
      m_avg_activ = other.m_avg_activ;
      m_pk_activ = other.m_pk_activ;
      m_temp = other.m_temp;

      return;
   }
// ------------------------------------------------------------
   bool AX_ConnC::operator== (const AX_ConnC & other)
   {
      if (&other == this)
         return(true);

      if (m_src_portP == other.m_src_portP &&
          m_sink_portP == other.m_sink_portP)
         return(true);

      return(false);
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Pointer List Manipulations


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Instance List Manipulations


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Structure List Manipulations


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Special Functions


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Miscellaneous Functions
// ------------------------------------------------------------
   void AX_ConnC::clean_up()
   {
      m_id = 0;
      m_db_idx = 0;
      m_type = CONN_TYPE_UNKNOWN;
      m_src_tgP = NULL;
      m_sink_tgP = NULL;
      m_src_portP = NULL;
      m_src_taskP = NULL;
      m_sink_portP = NULL;
      m_sink_taskP = NULL;
      m_avg_activ = 0;
      m_pk_activ = 0;
      m_temp = 0;
   }
// ------------------------------------------------------------
   void AX_ConnC::dump()
   {
      cerr << "      Id: " << m_id
           << ". Db_idx: " << m_db_idx << endl;
      cerr << "      Avg. activa: " << m_avg_activ;
      cerr << ". Pk_activ: " << m_pk_activ << endl;

      if (m_src_tgP != NULL)
         cerr << "      " << m_src_tgP->get_name() << ":";
      else
         cerr << "      " << m_src_taskP->get_name() << ":";
      cerr << m_src_portP->get_name();

      cerr << " --> ";

      if (m_sink_tgP != NULL)
         cerr << m_sink_tgP->get_name() << ":";
      else
         cerr << m_sink_taskP->get_name() << ":";
      cerr << m_sink_portP->get_name();

      cerr << endl;
   }
// ------------------------------------------------------------
   void AX_ConnC::dump_simple()
   {
      if (m_src_tgP != NULL)
         cerr << "      " << m_src_tgP->get_name() << ":";
      else
         cerr << "      " << m_src_taskP->get_name() << ":";
      cerr << m_src_portP->get_name();

      cerr << " --> ";

      if (m_sink_tgP != NULL)
         cerr << m_sink_tgP->get_name() << ":";
      else
         cerr << m_sink_taskP->get_name() << ":";
      cerr << m_sink_portP->get_name();

      cerr << endl;
   }
// ------------------------------------------------------------
   void AX_ConnC::write(fstream & File)
   {
      File << "      Id: " << m_id
           << ". Db_idx: " << m_db_idx << endl;
      File << "      Avg. activa: " << m_avg_activ;
      File << ". Pk_activ: " << m_pk_activ << endl;

      if (m_src_tgP != NULL)
         File << "      " << m_src_tgP->get_name() << ":";
      else
         File << "      " << m_src_taskP->get_name() << ":";
      File << m_src_portP->get_name();

      File << " --> ";

      if (m_sink_tgP != NULL)
         File << m_sink_tgP->get_name() << ":";
      else
         File << m_sink_taskP->get_name() << ":";
      File << m_sink_portP->get_name();

      File << endl;
   }
// ------------------------------------------------------------
   void AX_ConnC::write_simple(fstream & File)
   {
      if (m_src_tgP != NULL)
         File << "      " << m_src_tgP->get_name() << ":";
      else
         File << "      " << m_src_taskP->get_name() << ":";
      File << m_src_portP->get_name();

      File << " --> ";

      if (m_sink_tgP != NULL)
         File << m_sink_tgP->get_name() << ":";
      else
         File << m_sink_taskP->get_name() << ":";
      File << m_sink_portP->get_name();

      File << endl;
   }
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~End of Function Definitions
