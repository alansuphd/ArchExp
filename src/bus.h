// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// bus.h
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef AX_BUS_H_INCLUDED
#define AX_BUS_H_INCLUDED

#include <list>
#include <fstream>
#include "type.h"
#include "enum.h"

#ifndef AX_INTERFACE_H_INCLUDED
#include "interface.h"
#endif /* AX_INTERFACE_H_INCLUDED */

#ifndef AX_COMP_H_INCLUDED
#include "comp.h"
#endif /* AX_COMP_H_INCLUDED */

#ifndef AX_ARCHGRAPH_H_INCLUDED
#include "archgraph.h"
#endif /* AX_ARCHGRAPH_H_INCLUDED */

#ifndef AX_CONNPNT_H_INCLUDED
#include "connpnt.h"
#endif /* AX_CONNPNT_H_INCLUDED */

struct AX_ConnRefS
{
    AX_ConnCP m_connP;
    int m_start;
    int m_finish;
};

typedef AX_ConnRefS * AX_ConnRefSP;
typedef std::list<AX_ConnRefS> AX_ConnRefLS;
typedef std::list<AX_ConnRefS>::iterator AX_ConnRefLSIter;
typedef std::list<AX_ConnRefS>::const_iterator AX_ConnRefLSCnstIter;
typedef std::list<AX_ConnRefS *> AX_ConnRefPLS;
typedef std::list<AX_ConnRefS *>::iterator AX_ConnRefPLSIter;
typedef std::list<AX_ConnRefS *>::const_iterator AX_ConnRefPLSCnstIter;

// Class Definition for BusC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class AX_BusC
{
public:
  int m_id;

private:
  int m_db_idx;
  AX_InterfaceCP m_intrfcP;
  AX_ConnPntLC m_connpntL;
  AX_ConnRefLS m_connrefL;
  float m_util;

public:
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructor
     AX_BusC();
     AX_BusC(const AX_BusC & other);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Destructor
     ~AX_BusC();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Assignator & Comparators
     AX_BusC & operator= (const AX_BusC & other);
     bool operator== (const AX_BusC & other);
     bool operator!= (const AX_BusC & other) 
     {
        return(!(*this == other));
     }

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Sets & Gets
     void set_id(int id){m_id = id;}
     int get_id(){return m_id;}

     void set_db_idx(int db_idx){m_db_idx = db_idx;}
     int get_db_idx(){return m_db_idx;}

     void set_intrfc(AX_InterfaceCP intrfc){m_intrfcP = intrfc;}
     AX_InterfaceCP get_intrfc(){return m_intrfcP;}

     void set_util(float util){m_util = util;}
     float get_util(){return m_util;}

  // ~~~~~~~~~~~~~~~~~~~~~~ Connection Point List Manipulations
     AX_ConnPntLCIter add_new_connpnt(AX_ConnPntC & connpnt);
     AX_ConnPntLCIter add_connpnt(AX_ConnPntC & connpnt);

     AX_ConnPntCP get_connpnt(int index);
     void set2_first_connpnt(AX_ConnPntLCIter & fIter)
     {
        fIter = m_connpntL.begin();
     }
     AX_ConnPntLCIter get2_first_connpnt(){return m_connpntL.begin();}
     void set2_last_connpnt(AX_ConnPntLCIter & fIter)
     {
        fIter = --m_connpntL.end();
     }
     AX_ConnPntLCIter get2_last_connpnt(){return --m_connpntL.end();}
     AX_ConnPntLCIter connpnt_list_end(){return m_connpntL.end();}

     int get_no_connpnts(){return m_connpntL.size();}

     void remove_connpnt(AX_ConnPntLCIter & iter){m_connpntL.erase(iter);}
     void remove_connpnt(AX_ConnPntCP connpntP);
     AX_ConnPntLCIter delete_connpnt(AX_ConnPntLCIter & iter);
     AX_ConnPntLCIter delete_connpnt(AX_ConnPntCP connpntP);
     AX_ConnPntLCIter delete_connpnt(AX_ArchGraphCP agP, AX_CompCP compP,
                                      AX_CompPortCP portP);
     void remove_all_connpnts(){m_connpntL.clear();}

     AX_ConnPntCP find_connpnt_by_comp_port(AX_CompCP comp,
                                             AX_CompPortCP port);
     AX_ConnPntCP find_connpnt_by_ag_port(AX_ArchGraphCP ag,
                                           AX_CompPortCP port);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~ Mapping Task List Manipulations
     AX_ConnRefLSIter add_new_connref(AX_ConnRefS connref);
     AX_ConnRefLSIter add_new_connref(AX_ConnCP connP);

     AX_ConnRefSP get_connref(int index);
     void set2_first_connref(AX_ConnRefLSIter & fIter)
     {
         fIter = m_connrefL.begin();
     }
     AX_ConnRefLSIter get2_first_connref(){return m_connrefL.begin();}
     void set2_last_connref(AX_ConnRefLSIter & fIter)
     {
         fIter = --m_connrefL.end();
     }
     AX_ConnRefLSIter get2_last_connref(){return --m_connrefL.end();}
     AX_ConnRefLSIter connref_list_end(){return m_connrefL.end();}

     int get_no_connrefs(){return m_connrefL.size();}

     void remove_connref(AX_ConnRefLSIter & iter){m_connrefL.erase(iter);}
     void remove_connref(AX_ConnRefSP connrefP);
     void remove_connref(AX_ConnCP connP);
     AX_ConnRefLSIter delete_connref(AX_ConnRefLSIter & iter);
     AX_ConnRefLSIter delete_connref(AX_ConnRefSP connrefP);
     AX_ConnRefLSIter delete_connref(AX_ConnCP connP);
     AX_ConnRefLSIter delete_connref(bool source, AX_TaskCP taskP,
                                      std::string pName);
     void remove_all_connrefs();

     bool is_mapped_conn(AX_ConnCP connP);
     void sort_connref_by_start();
     void clean_connrefs();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Instance List Manipulations

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Structure List Manipulations

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Special Functions

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Miscellaneous Functions
     void clean_up();
     void dump();
     void write(std::fstream & File);
     void dump_simple();
     void write_simple(std::fstream & File);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~End of Class Definition
};

// Typedef Definitions for BusC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef AX_BusC * AX_BusCP;
typedef std::list<AX_BusC> AX_BusLC;
typedef std::list<AX_BusC>::iterator AX_BusLCIter;
typedef std::list<AX_BusC>::const_iterator AX_BusLCCnstIter;
typedef std::list<AX_BusC *> AX_BusPLC;
typedef std::list<AX_BusC *>::iterator AX_BusPLCIter;
typedef std::list<AX_BusC *>::const_iterator AX_BusPLCCnstIter;

#endif /* AX_BUS_H_INCLUDED */
