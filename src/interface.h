// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// interface.h
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef AX_INTERFACE_H_INCLUDED
#define AX_INTERFACE_H_INCLUDED 1

#include <list>
#include <fstream>
#include "type.h"
#include "enum.h"

#ifndef AX_UATTR_H_INCLUDED
#include "uattr.h"
#endif /* AX_UATTR_H_INCLUDED */

#ifndef AX_COMP_H_INCLUDED
#include "comp.h"
#endif /* AX_COMP_H_INCLUDED */

#ifndef AX_PLATFORM_H_INCLUDED
#include "platform.h"
#endif /* AX_PLATFORM_H_INCLUDED */

// Forward Declarations ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class AX_InterfaceC;

// Typedef Definitions for InterfaceC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef AX_InterfaceC * AX_InterfaceCP;
typedef std::list<AX_InterfaceC> AX_InterfaceLC;
typedef std::list<AX_InterfaceC>::iterator AX_InterfaceLCIter;
typedef std::list<AX_InterfaceC>::const_iterator AX_InterfaceLCCnstIter;
typedef std::list<AX_InterfaceC *> AX_InterfacePLC;
typedef std::list<AX_InterfaceC *>::iterator AX_InterfacePLCIter;
typedef std::list<AX_InterfaceC *>::const_iterator AX_InterfacePLCCnstIter;

// Class Definition for InterfaceC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class AX_InterfaceC : public AX_CompC
{
private:

    AX_PlatformCP  m_platfmP;     // which platform this interface is used
    InterfaceE     m_intrtype;    // interface type

    AX_UAttrLC m_uattrL;        // list of user defined attributes

public:
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructor
     AX_InterfaceC();
     AX_InterfaceC(const AX_InterfaceC & other);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Destructor
     ~AX_InterfaceC();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Assignator & Comparators
     AX_InterfaceC & operator= (const AX_InterfaceC & other);
     void copy(const AX_InterfaceC & other);
     bool operator== (const AX_InterfaceC & other);
     bool operator!= (const AX_InterfaceC & other) 
     {
        return(!(*this == other));
     }
     bool operator== (const std::string name);
     bool operator!= (const std::string name);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Sets & Gets
     void set_platfm(AX_PlatformCP platfmP){m_platfmP = platfmP;};
     AX_PlatformCP get_platfm(){return m_platfmP;};

     void set_intrtype(InterfaceE intr){m_intrtype = intr;};
     InterfaceE get_intrtype(){return m_intrtype;};

  // ~~~~~~~~~~~~~~~ User Attribute Instance List Manipulations
     AX_UAttrLCIter add_new_uattr(AX_UAttrC & uattr);

     AX_UAttrCP get_uattr(int index);
     void set2_first_uattr(AX_UAttrLCIter & uIter){uIter = m_uattrL.begin();}
     AX_UAttrLCIter get2_first_uattr(){return m_uattrL.begin();}
     void set2_last_uattr(AX_UAttrLCIter & uIter){uIter = --m_uattrL.end();}
     AX_UAttrLCIter get2_last_uattr(){return --m_uattrL.end();}
     AX_UAttrLCIter uattr_list_end(){return m_uattrL.end();}

     int get_no_uattrs(){return m_uattrL.size();}

     void remove_uattr(AX_UAttrLCIter & iter){m_uattrL.erase(iter);}
     void remove_uattr(AX_UAttrCP uattrP);
     AX_UAttrLCIter delete_uattr(AX_UAttrLCIter & iter);
     void remove_all_uattrs();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Structure List Manipulations

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Special Functions
     void set_category(CategoryE category){AX_CompC::set_category(category);};
     CategoryE get_category(){return AX_CompC::get_category();};

     void set_name(std::string & name){AX_CompC::set_name(name);};
     std::string get_name(){return AX_CompC::get_name();};

     void set_db_idx(int db_idx){AX_CompC::set_db_idx(db_idx);};
     int get_db_idx(){return AX_CompC::get_db_idx();};

     void set_tot_size(float size){AX_CompC::set_tot_size(size);}
     float get_tot_size(){return AX_CompC::get_tot_size();}

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Miscellaneous Functions
     void clean_up();
     void dump();
     void dump_simple();
     void write(std::fstream & File);
     void write_simple(std::fstream & File);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~End of Class Definition
};

#endif /* AX_INTERFACE_H_INCLUDED */
