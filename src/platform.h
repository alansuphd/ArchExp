// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// platform.h
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef AX_PLATFORM_H_INCLUDED
#define AX_PLATFORM_H_INCLUDED 1

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

#ifndef AX_INTERFACE_H_INCLUDED
#include "interface.h"
#endif /* AX_INTERFACE_H_INCLUDED */

// Class Definition for PlatformC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class AX_PlatformC : public AX_CompC
{
private:
    PlatformE m_level;                 // level of platform
    AX_PlatformPLC m_subpltfmPL;      // list of possible sub-platforms
    AX_PlatformPLC m_impl_subpltfmPL; // list of implemented sub-platforms
    AX_InterfacePLC m_intrfcPL;       // list of possible interfaces
    AX_InterfacePLC m_impl_intrfcPL;  // list of implemented interfaces
    AX_UAttrLC m_uattrL;             // list of user defined attributes

public:
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructor
     AX_PlatformC();
     AX_PlatformC(const AX_PlatformC & other);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Destructor
     ~AX_PlatformC();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Assignator & Comparators
     AX_PlatformC & operator= (const AX_PlatformC & other);
     void copy(const AX_PlatformC & other);
     void copyBasic(const AX_PlatformC & other);
     bool operator== (const AX_PlatformC & other);
     bool operator!= (const AX_PlatformC & other) 
     {
        return(!(*this == other));
     }
     bool operator== (const std::string name);
     bool operator!= (const std::string name);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Sets & Gets
     void set_level(PlatformE level){m_level = level;};
     PlatformE get_level(){return m_level;};

  // ~~~~~~~~~~ Possible Subplatform Pointer List Manipulations
     AX_PlatformPLCIter add_new_subpltfm(AX_PlatformCP subpltfmP);

     AX_PlatformCP get_subpltfm(int index);
     void set2_first_subpltfm(AX_PlatformPLCIter & pIter){pIter = m_subpltfmPL.begin();}
     AX_PlatformPLCIter get2_first_subpltfm(){return m_subpltfmPL.begin();}
     void set2_last_subpltfm(AX_PlatformPLCIter & pIter){pIter = --m_subpltfmPL.end();}
     AX_PlatformPLCIter get2_last_subpltfm(){return --m_subpltfmPL.end();}
     AX_PlatformPLCIter subpltfm_list_end(){return m_subpltfmPL.end();}

     int get_no_subpltfms(){return m_subpltfmPL.size();}

     void remove_subpltfm(AX_PlatformPLCIter & iter){m_subpltfmPL.erase(iter);}
     void remove_subpltfm(AX_PlatformCP subpltfmP);
     AX_PlatformPLCIter delete_subpltfm(AX_PlatformPLCIter & iter);
     void remove_all_subpltfms(){m_subpltfmPL.clear();}

  // ~~~~~~~~ Implemented Subplatform Pointer List Manipulations
     AX_PlatformPLCIter add_new_impl_subpltfm(AX_PlatformCP subpltfmP);

     AX_PlatformCP get_impl_subpltfm(int index);
     void set2_first_impl_subpltfm(AX_PlatformPLCIter & pIter){pIter = m_impl_subpltfmPL.begin();}
     AX_PlatformPLCIter get2_first_impl_subpltfm(){return m_impl_subpltfmPL.begin();}
     void set2_last_impl_subpltfm(AX_PlatformPLCIter & pIter){pIter = --m_impl_subpltfmPL.end();}
     AX_PlatformPLCIter get2_last_impl_subpltfm(){return --m_impl_subpltfmPL.end();}
     AX_PlatformPLCIter impl_subpltfm_list_end(){return m_impl_subpltfmPL.end();}

     int get_no_impl_subpltfms(){return m_impl_subpltfmPL.size();}

     void remove_impl_subpltfm(AX_PlatformPLCIter & iter){m_impl_subpltfmPL.erase(iter);}
     void remove_impl_subpltfm(AX_PlatformCP subpltfmP);
     AX_PlatformPLCIter delete_impl_subpltfm(AX_PlatformPLCIter & iter);
     void remove_all_impl_subpltfms(){m_impl_subpltfmPL.clear();}

  // ~~~~~~~~~~~~ Possible Interface Pointer List Manipulations
     AX_InterfacePLCIter add_new_intrfc(AX_InterfaceCP intrfcP);

     AX_InterfaceCP get_intrfc(int index);
     void set2_first_intrfc(AX_InterfacePLCIter & iIter){iIter = m_intrfcPL.begin();}
     AX_InterfacePLCIter get2_first_intrfc(){return m_intrfcPL.begin();}
     void set2_last_intrfc(AX_InterfacePLCIter & iIter){iIter = --m_intrfcPL.end();}
     AX_InterfacePLCIter get2_last_intrfc(){return --m_intrfcPL.end();}
     AX_InterfacePLCIter intrfc_list_end(){return m_intrfcPL.end();}

     int get_no_intrfcs(){return m_intrfcPL.size();}

     void remove_intrfc(AX_InterfacePLCIter & iter){m_intrfcPL.erase(iter);}
     void remove_intrfc(AX_InterfaceCP intrfcP);
     AX_InterfacePLCIter delete_intrfc(AX_InterfacePLCIter & iter);
     void remove_all_intrfcs(){m_intrfcPL.clear();}

  // ~~~~~~~~~ Implemented Interface Pointer List Manipulations
     AX_InterfacePLCIter add_new_impl_intrfc(AX_InterfaceCP intrfcP);

     AX_InterfaceCP get_impl_intrfc(int index);
     void set2_first_impl_intrfc(AX_InterfacePLCIter & iIter){iIter = m_impl_intrfcPL.begin();}
     AX_InterfacePLCIter get2_first_impl_intrfc(){return m_impl_intrfcPL.begin();}
     void set2_last_impl_intrfc(AX_InterfacePLCIter & iIter){iIter = --m_impl_intrfcPL.end();}
     AX_InterfacePLCIter get2_last_impl_intrfc(){return --m_impl_intrfcPL.end();}
     AX_InterfacePLCIter impl_intrfc_list_end(){return m_impl_intrfcPL.end();}

     int get_no_impl_intrfcs(){return m_impl_intrfcPL.size();}

     void remove_impl_intrfc(AX_InterfacePLCIter & iter){m_impl_intrfcPL.erase(iter);}
     void remove_impl_intrfc(AX_InterfaceCP intrfcP);
     AX_InterfacePLCIter delete_impl_intrfc(AX_InterfacePLCIter & iter);
     void remove_all_impl_intrfcs(){m_impl_intrfcPL.clear();}

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
     void set_category(CategoryE category){AX_CompC::set_category(category);}
     CategoryE get_category(){return AX_CompC::get_category();}

     void set_name(std::string & name){AX_CompC::set_name(name);}
     std::string get_name(){return AX_CompC::get_name();}

     void set_db_idx(int db_idx){AX_CompC::set_db_idx(db_idx);}
     int get_db_idx(){return AX_CompC::get_db_idx();}

     void set_tot_size(float size){AX_CompC::set_tot_size(size);}
     float get_tot_size(){return AX_CompC::get_tot_size();}

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Miscellaneous Functions
     void clean_up();
     void dump();
     void write(std::fstream & File);
     void dump_simple();
     void write_simple(std::fstream & File);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~End of Class Definition
};

// Typedef Definitions for PlatformC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef AX_PlatformC * AX_PlatformCP;
typedef std::list<AX_PlatformC> AX_PlatformLC;
typedef std::list<AX_PlatformC>::iterator AX_PlatformLCIter;
typedef std::list<AX_PlatformC>::const_iterator AX_PlatformLCCnstIter;
typedef std::list<AX_PlatformC *> AX_PlatformPLC;
typedef std::list<AX_PlatformC *>::iterator AX_PlatformPLCIter;
typedef std::list<AX_PlatformC *>::const_iterator AX_PlatformPLCCnstIter;

#endif /* AX_PLATFORM_H_INCLUDED */
