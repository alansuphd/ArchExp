// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// uattr.h
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef AX_UATTR_H_INCLUDED
#define AX_UATTR_H_INCLUDED 1

#include <list>
#include <fstream>
#include "type.h"
#include "enum.h"

// Class Definition for UAttrC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class AX_UAttrC
{
private:

    std::string m_name; // attribute name
    UAttrTypeE m_type; // attribute type

    std::string m_value;

public:
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructor
     AX_UAttrC();
     AX_UAttrC(const AX_UAttrC & other);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Destructor
     ~AX_UAttrC();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Assignator & Comparators
     AX_UAttrC & operator= (const AX_UAttrC & other);
     void copy(const AX_UAttrC & other);
     bool operator== (const AX_UAttrC & other);
     bool operator!= (const AX_UAttrC & other) 
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
    void set_name(std::string & name){m_name = name;}
    std::string get_name(){return m_name;}

    void set_type(UAttrTypeE type){m_type = type;}
    UAttrTypeE get_type(){return m_type;}

    void set_value(std::string value){m_value = value;}
    void set_value(char * value){m_value = value;}
    std::string get_value(){return m_value;}

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Pointer List Manipulations

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Instance List Manipulations

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Structure List Manipulations

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Special Functions
     void parse_attr(std::string attrStr);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Miscellaneous Functions
     void clean_up();
     void dump();
     void write(std::fstream & File);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~End of Class Definition
};

// Typedef Definitions for UAttrC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef AX_UAttrC * AX_UAttrCP;
typedef std::list<AX_UAttrC> AX_UAttrLC;
typedef std::list<AX_UAttrC>::iterator AX_UAttrLCIter;
typedef std::list<AX_UAttrC>::const_iterator AX_UAttrLCCnstIter;
typedef std::list<AX_UAttrC *> AX_UAttrPLC;
typedef std::list<AX_UAttrC *>::iterator AX_UAttrPLCIter;
typedef std::list<AX_UAttrC *>::const_iterator AX_UAttrPLCCnstIter;

#endif /* AX_UATTR_H_INCLUDED */
