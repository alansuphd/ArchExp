// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// dsnspec.h
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef AX_DSNSPEC_H_INCLUDED
#define AX_DSNSPEC_H_INCLUDED 1

#include <list>
#include <fstream>
#include "type.h"
#include "enum.h"

// Forward Declarations ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class AX_DsnC;

// Class Definition for DsnSpecC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class AX_DsnSpecC
{
private:

    std::string m_name;
    int     m_id;
    AX_DsnC * m_dsnP; //pointer to parent design

    //Design Specifications
    int     m_maxmemory; //maximum memory allocation
    int     m_maxlatency; //maximum latency
    int     m_maxperiodicity; //maximum periodicity
    float   m_maxpower; //maximum power (mW)
    float   m_maxarea; //maximum area (mm^{2})
    float   m_maxcost; //maximum cost (US$)

    //Misc
    int     m_temp;

public:
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructor
     AX_DsnSpecC();
     AX_DsnSpecC(const AX_DsnSpecC & other);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Destructor
     ~AX_DsnSpecC();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Assignator & Comparators
     AX_DsnSpecC & operator= (const AX_DsnSpecC & other);
     bool operator== (const AX_DsnSpecC & other);
     bool operator!= (const AX_DsnSpecC & other) 
     {
        return(!(*this == other));
     }

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Sets & Gets
     void set_name(std::string & name){m_name = name;}
     std::string get_name(){return m_name;}

     void set_id(int id){m_id = id;}
     int get_id(){return m_id;}

     void set_dsn(AX_DsnCP dsnP){m_dsnP = dsnP;}
     AX_DsnCP get_dsn(){return m_dsnP;}

     void set_max_memory(int maxmem){m_maxmemory = maxmem;}
     int get_max_memory(){return m_maxmemory;}

     void set_max_latency(int maxlat){m_maxlatency = maxlat;}
     int get_max_latency(){return m_maxlatency;}

     void set_max_periodicity(int maxperiod){m_maxperiodicity = maxperiod;}
     int get_max_periodicity(){return m_maxperiodicity;}

     void set_max_power(float maxpow){m_maxpower = maxpow;}
     float get_max_power(){return m_maxpower;}

     void set_max_area(float maxarea){m_maxarea = maxarea;}
     float get_max_area(){return m_maxarea;}

     void set_max_cost(float maxcost){m_maxcost = maxcost;}
     float get_max_cost(){return m_maxcost;}

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

// Typedef Definitions for DsnSpecC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef AX_DsnSpecC * AX_DsnSpecCP;
typedef std::list<AX_DsnSpecC> AX_DsnSpecLC;
typedef std::list<AX_DsnSpecC>::iterator AX_DsnSpecLCIter;
typedef std::list<AX_DsnSpecC>::const_iterator AX_DsnSpecLCCnstIter;
typedef std::list<AX_DsnSpecC *> AX_DsnSpecPLC;
typedef std::list<AX_DsnSpecC *>::iterator AX_DsnSpecPLCIter;
typedef std::list<AX_DsnSpecC *>::const_iterator AX_DsnSpecPLCCnstIter;

#endif /* AX_DSNSPEC_H_INCLUDED */
