// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// gen_sc.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <iostream>
#include "gen_sc.h"
using namespace std;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructor
// ------------------------------------------------------------
   AX_Gen_SystemCC::AX_Gen_SystemCC( string _blkName, string _path )
   {
       m_platfmP = NULL;
       m_blkName = _blkName;
       m_path    = _path;
   }
// ------------------------------------------------------------
   AX_Gen_SystemCC::AX_Gen_SystemCC(const AX_Gen_SystemCC & other)
   {
      copy(other);
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Destructor
// ------------------------------------------------------------
   AX_Gen_SystemCC::~AX_Gen_SystemCC()
   {
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Assignator & Comparators
// ------------------------------------------------------------
   AX_Gen_SystemCC & AX_Gen_SystemCC::operator= (const AX_Gen_SystemCC & other)
   {
      if (&other == this)
         return(*this);

      copy(other);

      return(*this);
   }
// ------------------------------------------------------------
   void AX_Gen_SystemCC::copy(const AX_Gen_SystemCC & other)
   {
      m_platfmP = other.m_platfmP;
      m_blkName = other.m_blkName;
      m_path    = other.m_path;
   }
// ------------------------------------------------------------
   bool AX_Gen_SystemCC::operator== (const AX_Gen_SystemCC & other)
   {
      return true;
   }
// ------------------------------------------------------------
   bool AX_Gen_SystemCC::operator== (const string name)
   {
      return true;
   }
// ------------------------------------------------------------
   bool AX_Gen_SystemCC::operator!= (const string name)
   {
       return (!(*this == name));
   }
// ------------------------------------------------------------

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Instance List Manipulations
// ------------------------------------------------------------
   void AX_Gen_SystemCC::write()
   {
       string fileNameH = m_path + m_blkName + ".h";
       string fileNameC = m_path + m_blkName + ".cpp";

       fstream scFileH, scFileC;

       scFileH.open( fileNameH.c_str(), fstream::out );
       scFileH << "#ifndef " << m_blkName << "_H" << endl;
       scFileH << "#define " << m_blkName << "_H" << endl;
       scFileH << "SC_MODULE(" << m_blkName << ") { " << endl;
       scFileH << "    // Port declarations" << endl << endl << endl;
       scFileH << "    sc_in<bool> Clk;" << endl << endl << endl;
       scFileH << "    // Channel / Submodule instances" << endl << endl << endl;
       scFileH << "    void worker();" << endl << endl;
       scFileH << "    SC_CTOR(" << m_blkName << ") { " << endl;
       scFileH << "        SC_THREAD(worker);" << endl;
       scFileH << "            sensitive << Clk.pos();" << endl;
       scFileH << "    }" << endl << endl;
       scFileH << "};" << endl;
       scFileH << "#endif" << endl << endl;
       scFileH.close();

       scFileC.open( fileNameC.c_str(), fstream::out );
       scFileC << "#define SC_INCLUDE_DYNAMIC_PROCESSES" << endl << endl;
       scFileC << "#include <systemc.h>" << endl;
       scFileC << "#include <tlm.h>" << endl;
       scFileC << "#include <tlm_utils/simple_initiator_socket.h>" << endl;
       scFileC << "#include <tlm_utils/simple_target_socket.h>" << endl;
       scFileC << "using namespace sc_core;" << endl;
       scFileC << "using namespace sc_dt;" << endl;
       scFileC << "using namespace std;" << endl;
       scFileC << "// declare above SystemC and TLM headers and defines prior include our own header" << endl;
       scFileC << "#include \""<< m_blkName << ".h" << "\"" << endl << endl << endl;
       scFileC << "void " << m_blkName << "::worker()" << endl;
       scFileC << "{" << endl;
       scFileC << "    while(true)" << endl;
       scFileC << "    {" << endl;
       scFileC << "        wait();    // wait for clock trigger" << endl;
       scFileC << "    }" << endl;
       scFileC << "}" << endl << endl;
       scFileC.close();

   }

   void AX_Gen_SystemCC::copy_resource( )
   {
   }
// ------------------------------------------------------------
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~End of Function Definitions
