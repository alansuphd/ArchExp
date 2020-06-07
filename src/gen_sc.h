// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// gen_sc.h
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef AX_GEN_SYSTEMC_H_INCLUDED
#define AX_GEN_SYSTEMC_H_INCLUDED 1

// Forward Declarations ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class AX_Gen_SystemCC;

#include <list>
#include <fstream>

// Typedef Definitions for InterfaceC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef AX_Gen_SystemCC * AX_Gen_SystemCCP;
typedef std::list<AX_Gen_SystemCC> AX_Gen_SystemCLC;
typedef std::list<AX_Gen_SystemCC>::iterator AX_Gen_SystemCLCIter;
typedef std::list<AX_Gen_SystemCC>::const_iterator AX_Gen_SystemCLCCnstIter;
typedef std::list<AX_Gen_SystemCC *> AX_Gen_SystemCPLC;
typedef std::list<AX_Gen_SystemCC *>::iterator AX_Gen_SystemCPLCIter;
typedef std::list<AX_Gen_SystemCC *>::const_iterator AX_Gen_SystemCPLCCnstIter;

#include "type.h"
#include "enum.h"
#include "funcblk.h"

#ifndef AX_PLATFORM_H_INCLUDED
#include "platform.h"
#endif /* AX_PLATFORM_H_INCLUDED */

// Class Definition for InterfaceC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class AX_Gen_SystemCC
{
private:
    std::string   m_blkName;
    std::string   m_path;
    AX_PlatformCP m_platfmP;     // which platform this gen_sc is used

public:
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructor
     AX_Gen_SystemCC( std::string _blkName, std::string _path );
     AX_Gen_SystemCC( const AX_Gen_SystemCC & other );

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Destructor
     ~AX_Gen_SystemCC();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Assignator & Comparators
     AX_Gen_SystemCC & operator= (const AX_Gen_SystemCC & other);
     void copy(const AX_Gen_SystemCC & other);
     bool operator== (const AX_Gen_SystemCC & other);
     bool operator!= (const AX_Gen_SystemCC & other) 
     {
        return(!(*this == other));
     }
     bool operator== (const std::string name);
     bool operator!= (const std::string name);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Sets & Gets
     void set_platfm(AX_PlatformCP platfmP){m_platfmP = platfmP;};
     AX_PlatformCP get_platfm(){return m_platfmP;};

     void write();
     void copy_resource();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~End of Class Definition
};

#endif /* AX_GEN_SYSTEMC_H_INCLUDED */
