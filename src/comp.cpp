// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// comp.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <iostream>
#include "comp.h"
using namespace std;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructor
// ------------------------------------------------------------
  AX_CompC::AX_CompC()
  { 
     m_category = NULLCATGRY;
     m_name.clear();
     m_id = -1; 
     m_db_idx = -1; 
     m_tot_size = -1;
  }
// ------------------------------------------------------------
  AX_CompC::AX_CompC(const AX_CompC & other)
  {
     m_category = other.m_category;
     m_name = other.m_name;
     m_id = other.m_id;
     m_db_idx = other.m_db_idx;
     m_tot_size = other.m_tot_size;
  }
// ------------------------------------------------------------
  AX_CompC::AX_CompC(const CategoryE cat)
  {
     m_category = cat;
     m_name.clear();
     m_id = -1;
     m_db_idx = -1;
     m_tot_size = -1;
  }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Destructor
// ------------------------------------------------------------
  AX_CompC::~AX_CompC()
  {
     //m_name.clear();
     m_category = NULLCATGRY;
     m_id = 0;
     m_db_idx = 0;
  }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Assignator & Comparators
// ------------------------------------------------------------
  AX_CompC & AX_CompC::operator= (const AX_CompC & other)
  {
     if (&other == this)
        return(*this);

     m_category = other.m_category;
     m_name = other.m_name;
     m_id = other.m_id;
     m_db_idx = other.m_db_idx;
     m_tot_size = other.m_tot_size;

     return(*this);
  }
// ------------------------------------------------------------
  void AX_CompC::copy(const AX_CompC & other)
  {
     m_category = other.m_category;
     m_name = other.m_name;
     m_id = other.m_id;
     m_db_idx = other.m_db_idx;
     m_tot_size = other.m_tot_size;
  }
// ------------------------------------------------------------
  bool AX_CompC::operator== (const AX_CompC & other)
  {
     if (&other == this)
        return(true);

     if (m_name == other.m_name &&
         m_category == other.m_category)
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
   void AX_CompC::clean_up()
   {
      m_name.clear();
      m_category = NULLCATGRY;
      m_id = 0;
      m_db_idx = 0;
   }

// ------------------------------------------------------------
   void AX_CompC::dump()
   {
      cerr << "    " << m_name << ":\n"
           << "      Category " << m_category
           << "  id " << m_id
           << "  db_idx " << m_db_idx
           << "  tot_size " << m_tot_size << endl;
   }
// ------------------------------------------------------------
   void AX_CompC::write(std::fstream & File)
   {
      File << "    " << m_name << ":\n"
           << "      Category " << m_category
           << "  id " << m_id
           << "  db_idx " << m_db_idx << endl;
   }
// ------------------------------------------------------------
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~End of Function Definitions
