// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// synthesis.h
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef AX_SYNTHESIS_H_INCLUDED
#define AX_SYNTHESIS_H_INCLUDED 1

#include <list>
#include <fstream>
#include "type.h"
#include "enum.h"
#include "util.h"
#include "extern.h"

#ifndef AX_DSN_H_INCLUDED
#include "dsn.h"
#endif /* AX_DSN_H_INCLUDED */

// Forward Declarations ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Class Definition for ScheduleC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class AX_SynthesisC
{
public:
    int  dsn_synthesis(std::string confName, std::string specsName,
                       std::string compsName, std::string tgName,
                       AX_DsnC & dsn);
    bool read_config(std::string FileName,
                     float & alpha, float & beta,
                     float & gama, float & margin);
    void get_projPaths(std::string home,
                       std::string &specsPath,
                       std::string &libPath);
    void get_specsPath(std::string home, std::string &specsPath);
    void get_libPath(std::string home, std::string &libPath);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructor
    AX_SynthesisC();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Destructor
    ~AX_SynthesisC();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~End of Class Definition
};

#endif /* AX_SYNTHESISEDULE_H_INCLUDED */
