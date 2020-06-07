// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// compport.h
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef AX_COMPPORT_H_INCLUDED
#define AX_COMPPORT_H_INCLUDED 1

#include <list>
#include <fstream>
#include "type.h"
#include "enum.h"

#ifndef AX_COMP_H_INCLUDED
#include "comp.h"
#endif /* AX_COMP_H_INCLUDED */

#ifndef AX_BUS_H_INCLUDED
#include "bus.h"
#endif /* AX_BUS_H_INCLUDED */

#ifndef AX_INTERFACE_H_INCLUDED
#include "interface.h"
#endif /* AX_INTERFACE_H_INCLUDED */

// Forward Declarations ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class AX_CompPortC;
typedef AX_CompPortC * AX_CompPortCP;

// Class Definition for CompPortC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class AX_CompPortC
{
public:
    int m_id;

private:
    std::string m_name;
    int m_db_idx;
    PortTypeE m_type;

    // Of the following two members, only one must be valid.
    AX_CompCP m_compP; // the comp to which this port belongs
    AX_ArchGraphCP m_agP; // the archgraph to which this port belongs
 
    AX_InterfacePLC m_intrfcPL; // list of iterfaces it can connect to
    AX_BusPLC m_busPL; // list of buses connected to this port
 
    int m_sample_size; // number of bits per sample
    AX_IntLC m_sample_rateL; // list of possible number of samples 
    int m_assign_sample_rate; // assigned number of samples per execution
 
    AX_CompPortCP m_sw_portP; // a sw port mapping to the hw port
    int m_refCnt;  // no. of sw ports reference to this port

    int m_temp;

public:
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructor
     AX_CompPortC();
     AX_CompPortC(const AX_CompPortC & other);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Destructor
     ~AX_CompPortC();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Assignator & Comparators
     AX_CompPortC & operator= (const AX_CompPortC & other);
     void copy(const AX_CompPortC & other);
     bool operator== (const AX_CompPortC & other);
     bool operator!= (const AX_CompPortC & other) 
     {
        return(!(*this == other));
     }

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Sets & Gets
     void set_name(std::string & name){m_name = name;}
     std::string get_name(){return m_name;}

     void set_id(int id){m_id = id;}
     const int get_id(){return m_id;}

     void set_db_idx(int db_idx){m_db_idx = db_idx;}
     int get_db_idx(){return m_db_idx;}

     void set_comp(AX_CompCP compP){m_compP = compP;}
     AX_CompCP get_comp(){return m_compP;}

     void set_ag(AX_ArchGraphCP agP){m_agP = agP;}
     AX_ArchGraphCP get_ag(){return m_agP;}

     void set_sample_size(int i){m_sample_size = i;}
     int get_sample_size(){return m_sample_size;}

     void set_assign_sample_rate(int i){m_assign_sample_rate = i;}
     int get_assign_sample_rate(){return m_assign_sample_rate;}

     void set_type(PortTypeE type){m_type = type;}
     PortTypeE get_type(){return m_type;}

     void set_sw_port(AX_CompPortCP port){m_sw_portP = port;}
     AX_CompPortCP get_sw_port(){return m_sw_portP;}

     void set_refCnt(int refCnt){m_refCnt = refCnt;}
     int get_refCnt(){return m_refCnt;}
     int inc_refCnt()
     {
        m_refCnt++;
        return m_refCnt;
     }
     int dec_refCnt()
     {
        m_refCnt--;
        return m_refCnt;
     }

     void set_temp(int temp){m_temp = temp;}
     int get_temp(){return m_temp;}

  // ~~~~~~~~~~~~~~~~~~~~~ Interface Pointer List Manipulations
     AX_InterfacePLCIter add_new_intrfc(AX_InterfaceCP intrfcP);

     AX_InterfaceCP get_intrfc(int index);
     AX_InterfaceCP get_intrfc(std::string name);
     void set2_first_intrfc(AX_InterfacePLCIter & iIter)
     {
         iIter = m_intrfcPL.begin();
     }
     AX_InterfacePLCIter get2_first_intrfc(){return m_intrfcPL.begin();}
     void set2_last_intrfc(AX_InterfacePLCIter & iIter)
     {
         iIter = --m_intrfcPL.end();
     }
     AX_InterfacePLCIter get2_last_intrfc(){return --m_intrfcPL.end();}
     AX_InterfacePLCIter intrfc_list_end(){return m_intrfcPL.end();}

     int get_no_intrfcs(){return m_intrfcPL.size();}

     void remove_intrfc(AX_InterfacePLCIter & iter){m_intrfcPL.erase(iter);}
     void remove_intrfc(AX_InterfaceCP intrfcP);
     AX_InterfacePLCIter delete_intrfc(AX_InterfacePLCIter & iter);
     void remove_all_intrfcs(){m_intrfcPL.clear();}

     bool copy_intrfcs(AX_InterfaceLC * intrfcL, AX_CompPortCP funcP);
     AX_InterfaceCP ran_pick_intrfc();
     bool find_common_intrfc(AX_InterfacePLC & intrfcPL);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~ Bus Pointer List Manipulations
     AX_BusPLCIter add_new_bus(AX_BusCP busP);
  
     AX_BusCP get_bus(int index);
     void set2_first_bus(AX_BusPLCIter & iter){iter = m_busPL.begin();}
     AX_BusPLCIter get2_first_bus(){return m_busPL.begin();}
     void set2_last_bus(AX_BusPLCIter & iter){iter = --m_busPL.end();}
     AX_BusPLCIter get2_last_bus(){return --m_busPL.end();}
     AX_BusPLCIter bus_list_end(){return m_busPL.end();}

     int get_no_buses(){return m_busPL.size();}

     void remove_bus(AX_BusPLCIter & iter){m_busPL.erase(iter);}
     void remove_bus(AX_BusCP busP);
     AX_BusPLCIter delete_bus(AX_BusPLCIter & iter);
     AX_BusPLCIter delete_bus(AX_BusCP busP);
     void scan_delete_bus(AX_BusCP busP);
     void remove_all_buses(){m_busPL.clear();}

     AX_BusCP func_on_port(AX_FuncBlkCP funcP);

  // ~~~~~~~~~~~~~~~~~~ Sample Rate Instance List Manipulations
     AX_IntLCIter add_new_sample_rate(int rate);

     AX_IntCP get_sample_rate(int index);
     void set2_first_sample_rate(AX_IntLCIter & iter){iter = m_sample_rateL.begin();}
     AX_IntLCIter get2_first_sample_rate(){return m_sample_rateL.begin();}
     void set2_last_sample_rate(AX_IntLCIter & iter){iter = --m_sample_rateL.end();}
     AX_IntLCIter get2_last_sample_rate(){return --m_sample_rateL.end();}
     AX_IntLCIter sample_rate_list_end(){return m_sample_rateL.end();}

     int get_no_sample_rates(){return m_sample_rateL.size();}

     void remove_sample_rate(AX_IntLCIter & iter){m_sample_rateL.erase(iter);}
     void remove_sample_rate(int);
     AX_IntLCIter delete_sample_rate(AX_IntLCIter & iter);
     void remove_all_sample_rates();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Structure List Manipulations

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Special Functions

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Miscellaneous Functions
     void clean_up();
     void dump();
     void dump_simple();
     void write(std::fstream & File);
     void write_simple(std::fstream & File);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~End of Class Definition
};

// Typedef Definitions for CompPortC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef std::list<AX_CompPortC> AX_CompPortLC;
typedef std::list<AX_CompPortC>::iterator AX_CompPortLCIter;
typedef std::list<AX_CompPortC>::const_iterator AX_CompPortLCCnstIter;
typedef std::list<AX_CompPortC *> AX_CompPortPLC;
typedef std::list<AX_CompPortC *>::iterator AX_CompPortPLCIter;
typedef std::list<AX_CompPortC *>::const_iterator AX_CompPortPLCCnstIter;

#endif /* AX_COMPPORT_H_INCLUDED */
