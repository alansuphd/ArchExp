// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// dsnspec.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <iostream>
#include "dsnspec.h"
using namespace std;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructor
// ------------------------------------------------------------
   AX_DsnSpecC::AX_DsnSpecC()
   {
      m_name.clear();
      m_id = -1;
      m_dsnP = NULL;
      m_maxmemory = -1;
      m_maxlatency = -1;
      m_maxperiodicity = -1;
      m_maxpower = -1;
      m_maxarea = -1;
      m_maxcost = -1;
      m_temp = -1;
   }
// ------------------------------------------------------------
   AX_DsnSpecC::AX_DsnSpecC(const AX_DsnSpecC & other)
   {
      m_name = other.m_name;
      m_id = other.m_id;
      m_dsnP = other.m_dsnP;
      m_maxmemory = other.m_maxmemory;
      m_maxlatency = other.m_maxlatency;
      m_maxperiodicity = other.m_maxperiodicity;
      m_maxpower = other.m_maxpower;
      m_maxarea = other.m_maxarea;
      m_maxcost = other.m_maxcost;
      m_temp = -1;
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Destructor
// ------------------------------------------------------------
   AX_DsnSpecC::~AX_DsnSpecC()
   {
      clean_up();
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Assignator & Comparators
// ------------------------------------------------------------
   AX_DsnSpecC & AX_DsnSpecC::operator= (const AX_DsnSpecC & other)
   {
      if (&other == this)
         return(*this);

      m_name = other.m_name;
      m_id = other.m_id;
      m_dsnP = other.m_dsnP;
      m_maxmemory = other.m_maxmemory;
      m_maxlatency = other.m_maxlatency;
      m_maxperiodicity = other.m_maxperiodicity;
      m_maxpower = other.m_maxpower;
      m_maxarea = other.m_maxarea;
      m_maxcost = other.m_maxcost;
      m_temp = other.m_temp;
  
      return(*this);
   }
// ------------------------------------------------------------
   bool AX_DsnSpecC::operator== (const AX_DsnSpecC & other)
   {
      if (m_dsnP == other.m_dsnP &&
          m_maxmemory == other.m_maxmemory &&
          m_maxlatency == other.m_maxlatency &&
          m_maxperiodicity == other.m_maxperiodicity &&
          m_maxpower == other.m_maxpower &&
          m_maxarea == other.m_maxarea &&
          m_maxcost == other.m_maxcost)
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
   void AX_DsnSpecC::clean_up()
   {
      m_name.clear();
      m_id = -1;
      m_dsnP = NULL;
      m_maxmemory = -1;
      m_maxlatency = -1;
      m_maxperiodicity = -1;
      m_maxpower = -1;
      m_maxarea = -1;
      m_maxcost = -1;
      m_temp = -1;
   }
// ------------------------------------------------------------
   void AX_DsnSpecC::dump()
   {
      cerr << " Spec name: " << m_name << endl;
      cerr << " .ID: " << m_id << endl;
      cerr << " .Max Memory: " << m_maxmemory << endl;
      cerr << " .Max Latency: " << m_maxlatency << endl;
      cerr << " .Max Periodicity: " << m_maxperiodicity << endl;
      cerr << " .Max Power: " << m_maxpower << endl;
      cerr << " .Max Area: " << m_maxarea << endl;
      cerr << " .Max Cost: " << m_maxcost << endl;
      cerr << endl;
   }
// ------------------------------------------------------------
   void AX_DsnSpecC::write(fstream & File)
   {
      File << " Spec Infomation: " << m_name
           << " .ID: " << m_id << endl;
      File << " .Max Memory: " << m_maxmemory << endl;
      File << " .Max Latency: " << m_maxlatency << endl;
      File << " .Max Periodicity: " << m_maxperiodicity << endl;
      File << " .Max Power: " << m_maxpower << endl;
      File << " .Max Area: " << m_maxarea << endl;
      File << " .Max Cost: " << m_maxcost << endl;
      File << endl;
   }
// ------------------------------------------------------------

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~End of Function Definitions
