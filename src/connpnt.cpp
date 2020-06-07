// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// connpnt.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <iostream>
#include "connpnt.h"
using namespace std;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructor
// ------------------------------------------------------------ 
   AX_ConnPntC::AX_ConnPntC()
   {
      m_compP = NULL;
      m_agP = NULL;
      m_portP = NULL;
   }
// ------------------------------------------------------------
   AX_ConnPntC::AX_ConnPntC(const AX_ConnPntC & other)
   {
      m_compP = other.m_compP;
      m_agP = other.m_agP;
      m_portP = other.m_portP;
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Destructor
// ------------------------------------------------------------
   AX_ConnPntC::~AX_ConnPntC()
   {
      clean_up();
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Assignator & Comparators
// ------------------------------------------------------------
   AX_ConnPntC & AX_ConnPntC::operator= (const AX_ConnPntC & other)
   {
      if (&other == this)
         return(*this);

      m_compP = other.m_compP;
      m_agP = other.m_agP;
      m_portP = other.m_portP;

      return(*this);
   }
// ------------------------------------------------------------
   bool AX_ConnPntC::operator== (const AX_ConnPntC & other)
   {
      if (&other == this)
         return(true);

      if (other.m_compP == m_compP &&
          other.m_agP == m_agP &&
          other.m_portP == m_portP)
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
   void AX_ConnPntC::clean_up()
   {
      m_compP = NULL;
      m_agP = NULL;
      m_portP = NULL;
   }
// ------------------------------------------------------------
   void AX_ConnPntC::dump()
   {
     if (m_compP != NULL)
        cerr << "          " << m_compP->get_name() << ".";
     else
        cerr << "          " << m_agP->get_name() << ".";
     cerr << m_portP->get_name() << endl;
   }
// ------------------------------------------------------------
   void AX_ConnPntC::write(fstream & File)
   {
     if (m_compP != NULL)
        File << "          " << m_compP->get_name() << ".";
     else
        File << "          " << m_agP->get_name() << ".";
     File << m_portP->get_name() << endl;
   }
// ------------------------------------------------------------
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~End of Function Definitions
