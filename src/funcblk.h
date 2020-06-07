// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// funcblk.h
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef AX_FUNCBLK_H_INCLUDED
#define AX_FUNCBLK_H_INCLUDED 1

#include <list>
#include <fstream>
#include "enum.h"
#include "type.h"
#include "gen_sc.h"

#ifndef AX_UATTR_H_INCLUDED
#include "uattr.h"
#endif /* AX_UATTR_H_INCLUDED */

#ifndef AX_TASK_H_INCLUDED
#include "task.h"
#endif /* AX_TASK_H_INCLUDED */

#ifndef AX_COMP_H_INCLUDED
#include "comp.h"
#endif /* AX_COMP_H_INCLUDED */

#ifndef AX_INTERFACE_H_INCLUDED
#include "interface.h"
#endif /* AX_INTERFACE_H_INCLUDED */

#ifndef AX_PLATFORM_H_INCLUDED
#include "platform.h"
#endif /* AX_PLATFORM_H_INCLUDED */

#ifndef AX_COMPPORT_H_INCLUDED
#include "compport.h"
#endif /* AX_COMPPORT_H_INCLUDED */

class AX_Gen_SystemCC;

struct AX_TaskRefS
{
    AX_TaskCP m_taskP;
    int m_start;
    int m_finish;
};

typedef AX_TaskRefS * AX_TaskRefSP;
typedef std::list<AX_TaskRefS> AX_TaskRefLS;
typedef std::list<AX_TaskRefS>::iterator AX_TaskRefLSIter;
typedef std::list<AX_TaskRefS>::const_iterator AX_TaskRefLSCnstIter;
typedef std::list<AX_TaskRefS *> AX_TaskRefPLS;
typedef std::list<AX_TaskRefS *>::iterator AX_TaskRefPLSIter;
typedef std::list<AX_TaskRefS *>::const_iterator AX_TaskRefPLSCnstIter;

// Class Definition for FuncBlkC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class AX_FuncBlkC : public AX_CompC
{
private:

    std::string m_function;       // function name. Not unique
    CompTypeE m_type;            // component type
    FuncTypeE m_functype;        // function type
    AX_PlatformCP m_platfmP;     // designate platform
    AX_TaskRefLS m_taskrefL;     // mapping tasks

    AX_CompPortPLC m_inportPL;      // list of input ports
    AX_CompPortPLC m_outportPL;     // list of output ports
    AX_CompPortPLC m_ioportPL;      // list of bi-directional I/O ports
    AX_FuncBlkPLC m_fncblkPL;  // list of dependent funcblks to this funcblk
    AX_FuncBlkPLC m_depPL;     // list of actual dependent list to the ag
    AX_FuncBlkPLC m_refPL;     // list of funcblks use this funcblk

    int m_max_clk_rate;          // max clock rate this comp can run
    int m_no_stages;             // number of pipeline stages
    int m_leak_current;          // idel leakage current
    int m_run_pwr;               // runtime power
    float m_util;               // utilization rate
    AX_UAttrLC m_uattrL;        // list of user defined attributes
    int m_temp;                  // a flexible variable

public:
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructor
     AX_FuncBlkC();
     AX_FuncBlkC(const AX_FuncBlkC & other);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Destructor
     ~AX_FuncBlkC();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Assignator & Comparators
     AX_FuncBlkC & operator= (const AX_FuncBlkC & other);
     void copy(const AX_FuncBlkC & other);
     void copyBasic(const AX_FuncBlkC & other);
     bool operator== (const AX_FuncBlkC & other);
     bool operator!= (const AX_FuncBlkC & other) 
     {
        return(!(*this == other));
     }
     bool operator== (const std::string name);
     bool operator!= (const std::string name);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Sets & Gets
     void set_function(std::string function){m_function = function;}
     std::string get_function(){return m_function;}

     void set_type(CompTypeE type){m_type = type;}
     CompTypeE get_type(){return m_type;}

     void set_functype(FuncTypeE functype){m_functype = functype;}
     FuncTypeE get_functype(){return m_functype;}

     void set_platfm(AX_PlatformCP platfmP){m_platfmP = platfmP;}
     AX_PlatformCP get_platfm(){return m_platfmP;}

     void set_max_clk_rate(int clk_rate){m_max_clk_rate = clk_rate;}
     int get_max_clk_rate(){return m_max_clk_rate;}

     void set_no_stages(int no_stages){m_no_stages = no_stages;}
     int get_no_stages(){return m_no_stages;}

     void set_leak_current(int current){m_leak_current = current;}
     int get_leak_current(){return m_leak_current;}

     void set_run_pwr(int pwr){m_run_pwr = pwr;}
     int get_run_pwr(){return m_run_pwr;}

     void set_util(float util){m_util = util;}
     float get_util(){return m_util;}

     void set_temp(int temp){m_temp = temp;}
     int get_temp(){return m_temp;}

     void clean_sw_ports();
     void clean_sw_ports(AX_FuncBlkCP funcP);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~ Mapping Task List Manipulations
     AX_TaskRefLSIter add_new_taskref(AX_TaskRefS taskref);
     AX_TaskRefLSIter add_new_taskref(AX_TaskCP taskP);

     AX_TaskRefSP get_taskref(int index);
     AX_TaskRefSP get_taskref(AX_TaskCP taskP);
     void set2_first_taskref(AX_TaskRefLSIter & fIter)
     {
         fIter = m_taskrefL.begin();
     }
     AX_TaskRefLSIter get2_first_taskref(){return m_taskrefL.begin();}
     void set2_last_taskref(AX_TaskRefLSIter & fIter)
     {
         fIter = --m_taskrefL.end();
     }
     AX_TaskRefLSIter get2_last_taskref(){return --m_taskrefL.end();}
     AX_TaskRefLSIter taskref_list_end(){return m_taskrefL.end();}

     int get_no_taskrefs(){return m_taskrefL.size();}

     void remove_taskref(AX_TaskRefLSIter & iter){m_taskrefL.erase(iter);}
     void remove_taskref(AX_TaskRefSP taskrefP);
     AX_TaskRefLSIter delete_taskref(AX_TaskRefLSIter & iter);
     AX_TaskRefLSIter delete_taskref(AX_TaskRefSP taskrefP);
     AX_TaskRefLSIter delete_taskref(AX_TaskCP taskP);
     void remove_all_taskrefs();

     bool is_mapped_task(AX_TaskCP taskP);
     void sort_taskref_by_start();
     void takeout_oth_ports(AX_ArchGraphC & ag, AX_CompPortCP portP,
                            AX_TaskCP taskP);
     void erase_connrefs(AX_ArchGraphC & ag);
     void erase_connrefs(AX_ArchGraphC & ag, AX_TaskCP taskP);

  // ~~~~~~~~~~~~~~~~~~~~ Input Port Pointer List Manipulations
     AX_CompPortPLCIter add_new_inport(AX_CompPortCP portP);

     AX_CompPortCP get_inport(int index);
     AX_CompPortCP get_inport(std::string name);
     int get_inportIdx(std::string name);
     void set2_first_inport(AX_CompPortPLCIter & pIter){pIter = m_inportPL.begin();}
     AX_CompPortPLCIter get2_first_inport(){return m_inportPL.begin();}
     void set2_last_inport(AX_CompPortPLCIter & pIter){pIter = --m_inportPL.end();}
     AX_CompPortPLCIter get2_last_inport(){return --m_inportPL.end();}
     AX_CompPortPLCIter inport_list_end(){return m_inportPL.end();}

     int get_no_inports(){return m_inportPL.size();} 

     void remove_inport(AX_CompPortPLCIter & iter){m_inportPL.erase(iter);}
     void remove_inport(AX_CompPortCP portP);
     AX_CompPortPLCIter delete_inport(AX_CompPortPLCIter & iter);
     AX_CompPortPLCIter delete_inport(AX_CompPortCP portP);
     void remove_all_inports(){m_inportPL.clear();}

     AX_CompPortCP any_connected_inport();
 
  // ~~~~~~~~~~~~~~~~~~~ Output Port Pointer List Manipulations
     AX_CompPortPLCIter add_new_outport(AX_CompPortCP portP);

     AX_CompPortCP get_outport(int index);
     AX_CompPortCP get_outport(std::string name);
     int get_outportIdx(std::string name);
     void set2_first_outport(AX_CompPortPLCIter & pIter){pIter = m_outportPL.begin();}
     AX_CompPortPLCIter get2_first_outport(){return m_outportPL.begin();}
     void set2_last_outport(AX_CompPortPLCIter & pIter){pIter = --m_outportPL.end();}
     AX_CompPortPLCIter get2_last_outport(){return --m_outportPL.end();}
     AX_CompPortPLCIter outport_list_end(){return m_outportPL.end();}

     int get_no_outports(){return m_outportPL.size();}

     void remove_outport(AX_CompPortPLCIter & iter){m_outportPL.erase(iter);}
     void remove_outport(AX_CompPortCP portP);
     AX_CompPortPLCIter delete_outport(AX_CompPortPLCIter & iter);
     AX_CompPortPLCIter delete_outport(AX_CompPortCP portP);
     void remove_all_outports(){m_outportPL.clear();}

     AX_CompPortCP any_connected_outport();

  // ~~~~~~~~ Bidirectional I/O Port Pointer List Manipulations
     AX_CompPortPLCIter add_new_ioport(AX_CompPortCP portP);

     AX_CompPortCP get_ioport(int index);
     AX_CompPortCP get_ioport(std::string name);
     int get_ioportIdx(std::string name);
     void set2_first_ioport(AX_CompPortPLCIter & pIter){pIter = m_ioportPL.begin();}
     AX_CompPortPLCIter get2_first_ioport(){return m_ioportPL.begin();}
     void set2_last_ioport(AX_CompPortPLCIter & pIter){pIter = --m_ioportPL.end();}
     AX_CompPortPLCIter get2_last_ioport(){return --m_ioportPL.end();}
     AX_CompPortPLCIter ioport_list_end(){return m_ioportPL.end();}

     int get_no_ioports(){return m_ioportPL.size();}

     void remove_ioport(AX_CompPortPLCIter & iter){m_ioportPL.erase(iter);}
     void remove_ioport(AX_CompPortCP portP);
     AX_CompPortPLCIter delete_ioport(AX_CompPortPLCIter & iter);
     AX_CompPortPLCIter delete_ioport(AX_CompPortCP portP);
     void remove_all_ioports(){m_ioportPL.clear();}

     AX_CompPortCP get_an_empty_ioport(AX_BusCP busP);

  // ~~~~~~~~~~~~~~~~~~~ Dependent List (to design) Manipulations
     AX_FuncBlkPLCIter add_new_fncblk(AX_FuncBlkCP fncblkP);

     AX_FuncBlkCP get_fncblk(int index);
     void set2_first_fncblk(AX_FuncBlkPLCIter & fIter)
     {
         fIter = m_fncblkPL.begin();
     }
     AX_FuncBlkPLCIter get2_first_fncblk(){return m_fncblkPL.begin();}
     void set2_last_fncblk(AX_FuncBlkPLCIter & fIter)
     {
         fIter = --m_fncblkPL.end();
     }
     AX_FuncBlkPLCIter get2_last_fncblk(){return --m_fncblkPL.end();}
     AX_FuncBlkPLCIter fncblk_list_end(){return m_fncblkPL.end();}

     int get_no_fncblks(){return m_fncblkPL.size();}

     void remove_fncblk(AX_FuncBlkPLCIter & iter){m_fncblkPL.erase(iter);}
     void remove_fncblk(AX_FuncBlkCP fncblkP);
     AX_FuncBlkPLCIter delete_fncblk(AX_FuncBlkPLCIter & iter);
     void remove_all_fncblks(){m_fncblkPL.clear();}

  // ~~~~~~~~~~~~~~~~ Actual Dependent List (to ag) Manipulations
     AX_FuncBlkPLCIter add_new_dep(AX_FuncBlkCP depP);

     AX_FuncBlkCP get_dep(int index);
     void set2_first_dep(AX_FuncBlkPLCIter & fIter)
     {
         fIter = m_depPL.begin();
     }
     AX_FuncBlkPLCIter get2_first_dep(){return m_depPL.begin();}
     void set2_last_dep(AX_FuncBlkPLCIter & fIter)
     {
         fIter = --m_depPL.end();
     }
     AX_FuncBlkPLCIter get2_last_dep(){return --m_depPL.end();}
     AX_FuncBlkPLCIter dep_list_end(){return m_depPL.end();}

     int get_no_deps(){return m_depPL.size();}

     void remove_dep(AX_FuncBlkPLCIter & iter){m_depPL.erase(iter);}
     void remove_dep(AX_FuncBlkCP depP);
     AX_FuncBlkPLCIter delete_dep(AX_FuncBlkPLCIter & iter);
     AX_FuncBlkPLCIter delete_dep(AX_FuncBlkCP funcP);
     void remove_all_deps(){m_depPL.clear();}

     AX_FuncBlkCP find_proc_in_dep();

  // ~~~~~~~~ Referenced Function Block Point List Manipulations
     AX_FuncBlkPLCIter add_new_ref(AX_FuncBlkCP refP);

     AX_FuncBlkCP get_ref(int index);
     void set2_first_ref(AX_FuncBlkPLCIter & fIter)
     {
         fIter = m_refPL.begin();
     }
     AX_FuncBlkPLCIter get2_first_ref(){return m_refPL.begin();}
     void set2_last_ref(AX_FuncBlkPLCIter & fIter)
     {
         fIter = --m_refPL.end();
     }
     AX_FuncBlkPLCIter get2_last_ref(){return --m_refPL.end();}
     AX_FuncBlkPLCIter ref_list_end(){return m_refPL.end();}

     int get_no_refs(){return m_refPL.size();}

     void remove_ref(AX_FuncBlkPLCIter & iter){m_refPL.erase(iter);}
     void remove_ref(AX_FuncBlkCP refP);
     AX_FuncBlkPLCIter delete_ref(AX_FuncBlkPLCIter & iter);
     AX_FuncBlkPLCIter delete_ref(AX_FuncBlkCP busP);
     void remove_all_refs(){m_refPL.clear();}

  // ~~~~~~~~~~~~~~~ User Attribute Instance List Manipulations
     AX_UAttrLCIter add_new_uattr(AX_UAttrC & uattr);

     AX_UAttrCP get_uattr(int index);
     AX_UAttrCP get_uattr(std::string uName);
     void set2_first_uattr(AX_UAttrLCIter & uIter){uIter = m_uattrL.begin();}
     AX_UAttrLCIter get2_first_uattr(){return m_uattrL.begin();}
     void set2_last_uattr(AX_UAttrLCIter & uIter){uIter = --m_uattrL.end();}
     AX_UAttrLCIter get2_last_uattr(){return --m_uattrL.end();}
     AX_UAttrLCIter uattr_list_end(){return m_uattrL.end();}

     int get_no_uattrs(){return m_uattrL.size();}

     void remove_uattr(AX_UAttrLCIter & iter){m_uattrL.erase(iter);}
     void remove_uattr(AX_UAttrCP uattrP);
     AX_UAttrLCIter delete_uattr(AX_UAttrLCIter & iter);
     AX_UAttrLCIter delete_uattr(AX_UAttrCP uattrP);
     void remove_all_uattrs();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Structure List Manipulations

     bool copy_intrfcs(AX_InterfaceLC * intrfcL, AX_FuncBlkCP oFuncP);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Special Functions
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Derived from CompC
     void set_category(CategoryE cat){AX_CompC::set_category(cat);}
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
     void dump_simple();
     void write(std::fstream & File);
     void write_simple(std::fstream & File);
     void write_SystemC( AX_Gen_SystemCC* _gen_sc );

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~End of Class Definition
};

// Typedef Definitions for FuncBlkC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef AX_FuncBlkC * AX_FuncBlkCP;
typedef std::list<AX_FuncBlkC> AX_FuncBlkLC;
typedef std::list<AX_FuncBlkC>::iterator AX_FuncBlkLCIter;
typedef std::list<AX_FuncBlkC>::const_iterator AX_FuncBlkLCCnstIter;
typedef std::list<AX_FuncBlkC *> AX_FuncBlkPLC;
typedef std::list<AX_FuncBlkC *>::iterator AX_FuncBlkPLCIter;
typedef std::list<AX_FuncBlkC *>::const_iterator AX_FuncBlkPLCCnstIter;

#endif /* AX_FUNCBLK_H_INCLUDED */
