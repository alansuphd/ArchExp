// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// connpnt.h
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef AX_CONNPNT_H_INCLUDED
#define AX_CONNPNT_H_INCLUDED 1

#include <list>
#include <fstream>
#include "type.h"
#include "enum.h"

#ifndef AX_COMP_H_INCLUDED
#include "comp.h"
#endif /* AX_COMP_H_INCLUDED */

#ifndef AX_ARCHGRAPH_H_INCLUDED
#include "archgraph.h"
#endif /* AX_ARCHGRAPH_H_INCLUDED */

#ifndef AX_COMPPORT_H_INCLUDED
#include "compport.h"
#endif /* AX_COMPPORT_H_INCLUDED */

// Class Definition for ConnPntC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class AX_ConnPntC
{
private:

  AX_CompCP m_compP;
  AX_ArchGraphCP m_agP;
  AX_CompPortCP m_portP;

public:
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructor
     AX_ConnPntC();
     AX_ConnPntC(const AX_ConnPntC & other);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Destructor
     ~AX_ConnPntC();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Assignator & Comparators
     AX_ConnPntC & operator= (const AX_ConnPntC & other);
     bool operator== (const AX_ConnPntC & other);
     bool operator!= (const AX_ConnPntC & other) 
     {
        return(!(*this == other));
     }

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Sets & Gets

     void set_comp(AX_CompCP compP){m_compP = compP;}
     AX_CompCP get_comp(){return m_compP;}

     void set_ag(AX_ArchGraphCP agP){m_agP = agP;}
     AX_ArchGraphCP get_ag(){return m_agP;}

     void set_port(AX_CompPortCP portP){m_portP = portP;}
     AX_CompPortCP get_port(){return m_portP;}

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

// Typedef Definitions for ConnPntC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef AX_ConnPntC * AX_ConnPntCP;
typedef std::list<AX_ConnPntC> AX_ConnPntLC;
typedef std::list<AX_ConnPntC>::iterator AX_ConnPntLCIter;
typedef std::list<AX_ConnPntC>::const_iterator AX_ConnPntLCCnstIter;
typedef std::list<AX_ConnPntC *> AX_ConnPntPLC;
typedef std::list<AX_ConnPntC *>::iterator AX_ConnPntPLCIter;
typedef std::list<AX_ConnPntC *>::const_iterator AX_ConnPntPLCCnstIter;

#endif /* AX_CONNPNT_H_INCLUDED */
