// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// comp.h
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef AX_COMP_H_INCLUDED
#define AX_COMP_H_INCLUDED 1

#include <fstream>
#include <list>
#include "type.h"
#include "enum.h"

// Class Definition for CompC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class AX_CompC
{
public:
    int m_id; // ID number

private:
    CategoryE m_category; // {PLATFORM, INTERFACE, FUNCBLK, NULLCATGRY}
    std::string m_name; // component name. Unique within a library
    int m_db_idx; // DB index
    float m_tot_size; // size used by memory core, platform and interface

public:
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructor
  AX_CompC();
  AX_CompC(const AX_CompC & other);
  AX_CompC(const CategoryE cat);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Destructor
  ~AX_CompC();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Assignator & Comparators
  AX_CompC & operator= (const AX_CompC & other);
  void copy(const AX_CompC & other);
  bool operator== (const AX_CompC & other);
  bool operator!= (const AX_CompC & other) 
  {
     return(!(*this == other));
  }
  bool operator== (const std::string name)
  {
     return(name == m_name);
  }
  bool operator!= (const std::string name)
  {
     return(name != m_name);
  }

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Sets & Gets
  void set_category(CategoryE category){m_category = category;};
  CategoryE get_category(){return m_category;};

  void set_name(std::string & name){m_name = name;};
  std::string get_name(){return m_name;};

  void set_id(int id){m_id = id;};
  int get_id(){return m_id;};

  void set_db_idx(int db_idx){m_db_idx = db_idx;};
  int get_db_idx(){return m_db_idx;};

  void set_tot_size(float size){m_tot_size = size;};
  float get_tot_size(){return m_tot_size;};

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Pointer List Manipulations

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Instance List Manipulations

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Structure List Manipulations

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Special Functions

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Miscellaneous Functions
     void clean_up();
     void dump();
     void write(std::fstream & File);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~End of Class Definition
};

// Typedef Definitions for CompC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef AX_CompC * AX_CompCP;
typedef std::list<AX_CompC> AX_CompLC;
typedef std::list<AX_CompC>::iterator AX_CompLCIter;
typedef std::list<AX_CompC>::const_iterator AX_CompLCCnstIter;
typedef std::list<AX_CompC *> AX_CompPLC;
typedef std::list<AX_CompC *>::iterator AX_CompPLCIter;
typedef std::list<AX_CompC *>::const_iterator AX_CompPLCCnstIter;

#endif /* AX_COMP_H_INCLUDED */
