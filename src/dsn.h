// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// dsn.h
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef AX_DSN_H_INCLUDED
#define AX_DSN_H_INCLUDED 1

#include <list>
#include <fstream>
#include <string.h>
#include "type.h"
#include "enum.h"
#include "util.h"

#ifndef AX_DSNSPEC_H_INCLUDED
#include "dsnspec.h"
#endif /* AX_DSNSPEC_H_INCLUDED */

#ifndef AX_FUNCBLK_H_INCLUDED
#include "funcblk.h"
#endif /* AX_FUNCBLK_H_INCLUDED */

#ifndef AX_INTERFACE_H_INCLUDED
#include "interface.h"
#endif /* AX_INTERFACE_H_INCLUDED */

#ifndef AX_PLATFORM_H_INCLUDED
#include "platform.h"
#endif /* AX_PLATFORM_H_INCLUDED */

#ifndef AX_TASKGRAPH_H_INCLUDED
#include "taskgraph.h"
#endif /* AX_TASKGRAPH_H_INCLUDED */

#ifndef AX_ARCHGRAPH_H_INCLUDED
#include "archgraph.h"
#endif /* AX_ARCHGRAPH_H_INCLUDED */

#ifndef AX_UTIL_H_INCLUDED
#include "util.h"
#endif /* AX_UTIL_H_INCLUDED */

// Forward declaration

// Class Definition for DsnC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class AX_DsnC
{
private:

    std::string     m_name; //Design name
    int             m_id;   //Design id

    AX_PlatformLC   m_platfmL; //List of platforms
    AX_InterfaceLC  m_intrfcL; //List of interfaces
    AX_FuncBlkLC    m_fncblkL; //List of funcional blocks
    AX_CompPortLC   m_compportL; //List of components

    AX_TaskGraphC   m_tg;       //Task graph
    AX_DsnSpecLC    m_dsnspecL; //Design Specifications
    AX_ArchGraphLC  m_agL;      //List of architecture graphs

    int             m_temp;

public:
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructor
     AX_DsnC();
     AX_DsnC(AX_DsnC & other);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Destructor
     ~AX_DsnC();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Assignator & Comparators
     AX_DsnC & operator= (AX_DsnC & other);
     bool operator== (const AX_DsnC & other);
     bool operator!= (const AX_DsnC & other) 
     {
        return(!(*this == other));
     }

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Sets & Gets
     void set_name(std::string & name){m_name = name;}
     std::string get_name(){return m_name;}

     void set_id(int id){m_id = id;}
     int get_id(){return m_id;}
 
     AX_TaskGraphCP get_tg(){return &m_tg;};

     AX_ArchGraphLC & get_agL(){return m_agL;};
 
     void set_temp(int temp){m_temp = temp;}
     int get_temp(){return m_temp;}

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Pointer List Manipulations

  // ~~~~~~~~~~~~~~~ Function Block Instance List Manipulations
     AX_FuncBlkLCIter create_new_fncblk();
     AX_FuncBlkLCIter create_new_fncblk(std::string & name);
     AX_FuncBlkLCIter get_fncblk(std::string & name);
     AX_FuncBlkLCIter add_new_fncblk(AX_FuncBlkC & fncblk);

     bool is_new_fncblk(std::string & name);
     AX_FuncBlkCP get_fncblk(int index);
     int get_fncblk_id(std::string name);
     void set2_first_fncblk(AX_FuncBlkLCIter & fIter)
     {
         fIter = m_fncblkL.begin();
     }
     AX_FuncBlkLCIter get2_first_fncblk(){return m_fncblkL.begin();}
     void set2_last_fncblk(AX_FuncBlkLCIter & fIter)
     {
         fIter = --m_fncblkL.end();
     }
     AX_FuncBlkLCIter get2_last_fncblk(){return --m_fncblkL.end();}
     AX_FuncBlkLCIter fncblk_list_end(){return m_fncblkL.end();}
     AX_FuncBlkLCIter get_first_fncblk_by_func(std::string func);
     AX_FuncBlkLCIter get_next_fncblk_by_func(AX_FuncBlkLCIter iter,
                                               std::string func);
     bool set_proc_family(std::string & family, AX_FuncBlkCP funcp);

     int get_no_fncblks(){return m_fncblkL.size();}

     void remove_fncblk(AX_FuncBlkLCIter & iter){m_fncblkL.erase(iter);}
     void remove_fncblk(AX_FuncBlkCP fncblkP);
     AX_FuncBlkLCIter delete_fncblk(AX_FuncBlkLCIter & iter);
     void remove_all_fncblks();

     AX_FuncBlkCP ran_pick_mem(int & cap, int min, int archwidth);
     bool fncblk_exists(AX_FuncBlkCP funcP);

  // ~~~~~~~~~~~~~~~~~~~~ Interface Instance List Manipulations
     AX_InterfaceLCIter create_new_intrfc();
     AX_InterfaceLCIter create_new_intrfc(std::string & name);
     AX_InterfaceLCIter get_intrfc(std::string & name);
     AX_InterfaceLCIter add_new_intrfc(AX_InterfaceC & intrfc);

     bool is_new_intrfc(std::string & name);
     AX_InterfaceCP get_intrfc(int index);
     int get_intrfc_id(std::string name);
     AX_InterfaceLC & get_intrfcL(){return m_intrfcL;};
     void set2_first_intrfc(AX_InterfaceLCIter & iIter)
     {
         iIter = m_intrfcL.begin();
     }
     AX_InterfaceLCIter get2_first_intrfc(){return m_intrfcL.begin();}
     void set2_last_intrfc(AX_InterfaceLCIter & iIter)
     {
         iIter = --m_intrfcL.end();
     }
     AX_InterfaceLCIter get2_last_intrfc(){return --m_intrfcL.end();}
     AX_InterfaceLCIter intrfc_list_end(){return m_intrfcL.end();}

     int get_no_intrfcs(){return m_intrfcL.size();}

     void remove_intrfc(AX_InterfaceLCIter & iter){m_intrfcL.erase(iter);}
     void remove_intrfc(AX_InterfaceCP intrfcP);
     AX_InterfaceLCIter delete_intrfc(AX_InterfaceLCIter & iter);
     void remove_all_intrfcs();

  // ~~~~~~~~~~~~~~~~~~~~~ Platform Instance List Manipulations
     AX_PlatformLCIter create_new_platfm();
     AX_PlatformLCIter create_new_platfm(std::string & name);
     AX_PlatformLCIter get_platfm(std::string & name);
     AX_PlatformLCIter add_new_platfm(AX_PlatformC & platfm);

     bool is_new_platfm(std::string & name);
     AX_PlatformCP get_platfm(int index);
     int get_platfm_id(std::string name);
     void set2_first_platfm(AX_PlatformLCIter & pIter)
     {
         pIter = m_platfmL.begin();
     }
     AX_PlatformLCIter get2_first_platfm(){return m_platfmL.begin();}
     void set2_last_platfm(AX_PlatformLCIter & pIter)
     {
         pIter = --m_platfmL.end();
     }
     AX_PlatformLCIter get2_last_platfm(){return --m_platfmL.end();}
     AX_PlatformLCIter platfm_list_end(){return m_platfmL.end();}

     int get_no_platfms(){return m_platfmL.size();}

     void remove_platfm(AX_PlatformLCIter & iter){m_platfmL.erase(iter);}
     void remove_platfm(AX_PlatformCP platfmP);
     AX_PlatformLCIter delete_platfm(AX_PlatformLCIter & iter);
     void remove_all_platfms();

  // ~~~~~~~~~~~~~~~~~~~~~ CompPort Instance List Manipulations
     AX_CompPortLCIter create_new_compport();
     AX_CompPortLCIter create_new_compport(std::string & name);
     AX_CompPortLCIter get_compport(std::string & name);
     AX_CompPortLCIter add_new_compport(AX_CompPortC & compport);

     bool is_new_compport(std::string & name);
     AX_CompPortCP get_compport(int index);
     AX_CompPortLCIter get_compport(AX_FuncBlkCP funcP, std::string name);
     void set2_first_compport(AX_CompPortLCIter & cIter)
     {
         cIter = m_compportL.begin();
     }
     AX_CompPortLCIter get2_first_compport(){return m_compportL.begin();}
     void set2_last_compport(AX_CompPortLCIter & cIter)
     {
         cIter = --m_compportL.end();
     }
     AX_CompPortLCIter get2_last_compport(){return --m_compportL.end();}
     AX_CompPortLCIter compport_list_end(){return m_compportL.end();}

     int get_no_compports(){return m_compportL.size();}

     void remove_compport(AX_CompPortLCIter & iter){m_compportL.erase(iter);}
     void remove_compport(AX_CompPortCP compportP);
     AX_CompPortLCIter delete_compport(AX_CompPortLCIter & iter);
     void remove_all_compports();

  // ~~~~~~~~~~~ Design Information Instance List Manipulations
     AX_DsnSpecLCIter create_new_dsnspec();
     AX_DsnSpecLCIter create_new_dsnspec(std::string & name);
     AX_DsnSpecLCIter get_dsnspec(std::string & name);
     AX_DsnSpecLCIter add_new_dsnspec(AX_DsnSpecC & dsnspec);

     bool is_new_dsnspec(std::string & name);
     AX_DsnSpecCP get_dsnspec(int index);
     AX_DsnSpecLCIter get_dsnspec(std::string name);
     void set2_first_dsnspec(AX_DsnSpecLCIter & dIter)
     {
        dIter = m_dsnspecL.begin();
     }
     AX_DsnSpecLCIter get2_first_dsnspec(){return m_dsnspecL.begin();}
     void set2_last_dsnspec(AX_DsnSpecLCIter & dIter)
     {
        dIter = --m_dsnspecL.end();
     }
     AX_DsnSpecLCIter get2_last_dsnspec(){return --m_dsnspecL.end();}
     AX_DsnSpecLCIter dsnspec_list_end(){return m_dsnspecL.end();}

     int get_no_dsnspecs(){return m_dsnspecL.size();}

     void remove_dsnspec(AX_DsnSpecLCIter & iter){m_dsnspecL.erase(iter);}
     void remove_dsnspec(AX_DsnSpecCP dsnspecP);
     AX_DsnSpecLCIter delete_dsnspec(AX_DsnSpecLCIter & iter);
     void remove_all_dsnspecs();

  // ~~~~~~~~~~~ Architecture Graph List Manipulation
     AX_ArchGraphLCIter create_new_ag();
     AX_ArchGraphLCIter create_new_ag(std::string & name);
     AX_ArchGraphLCIter get_ag(std::string & name);
     AX_ArchGraphLCIter add_new_ag(AX_ArchGraphC & ag);
     AX_ArchGraphLCIter add_ag(AX_ArchGraphC & ag);

     bool is_new_ag(std::string & name);
     AX_ArchGraphCP get_ag(int index);
     void set2_first_ag(AX_ArchGraphLCIter & agIter)
     {
        agIter = m_agL.begin();
     }
     AX_ArchGraphLCIter get2_first_ag(){return m_agL.begin();}
     void set2_last_ag(AX_ArchGraphLCIter & agIter)
     {
        agIter = --m_agL.end();
     }
     AX_ArchGraphLCIter get2_last_ag(){return --m_agL.end();}
     AX_ArchGraphLCIter ag_list_end(){return m_agL.end();}

     int get_no_ags(){return m_agL.size();}

     void remove_ag(AX_ArchGraphLCIter & iter){m_agL.erase(iter);}
     void remove_ag(AX_ArchGraphCP agP);
     void remove_ag(std::string name);
     AX_ArchGraphLCIter delete_ag(AX_ArchGraphLCIter & iter);
     AX_ArchGraphLCIter delete_ag(AX_ArchGraphCP agP);
     AX_ArchGraphLCIter delete_ag(std::string name);
     void remove_all_ags();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Structure List Manipulations
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Structure List Manipulations

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~DSN Read Functions
     int  dsn_synthesis(std::string specsPath, std::string compsPath,
                        std::string tgPath, AX_DsnC dsn);
     bool read_config(std::string FileName,
                      float & alpha, float & beta,
                      float & gama, float & margin);
     void get_projPaths(std::string home,
                        std::string &specsPath,
                        std::string &compsPath,
                        std::string &tgPath);
     void get_specsPath(std::string home, std::string &specsPath);
     void get_compsPath(std::string home, std::string &compsPath);
     void get_tgPath(std::string home, std::string &tgPath);
     bool read_dsnspec(std::string FileName);
     bool read_comps(std::string FileName);
     bool read_tg(std::string FileName);

     void write_uattr(AX_UAttrC & uattr, std::fstream & file);
     void write_port(int & idx, AX_CompPortCP portP, std::fstream & file);
     void write_tport(int & idx, AX_TaskPortCP portP, std::fstream & file);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Special Functions
     bool parse_iterblk(std::string & ibStr, AX_TaskGraphC & tg,
                        AX_IterBlkSP iterblkP);
     bool parse_cyclehead(std::string & cyStr, AX_TaskGraphC & tg);
     bool check_ags();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Miscellaneous Functions
     void clean_up();
     void dump();
     void write(std::fstream & File);
     void write_agL_files_SystemC();
     void write_agL_files();
     void write_bkui_dom_file();
     void write_bkui_data_file();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~End of Class Definition
};

// Typedef Definitions for DsnC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef AX_DsnC * AX_DsnCP;
typedef std::list<AX_DsnC> AX_DsnLC;
typedef std::list<AX_DsnC>::iterator AX_DsnLCIter;
typedef std::list<AX_DsnC>::const_iterator AX_DsnLCCnstIter;
typedef std::list<AX_DsnC *> AX_DsnPLC;
typedef std::list<AX_DsnC *>::iterator AX_DsnPLCIter;
typedef std::list<AX_DsnC *>::const_iterator AX_DsnPLCCnstIter;

#endif /* AX_DSN_H_INCLUDED */
