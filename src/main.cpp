#define NO_EVOLVE 0

#include <iostream>
#include "synthesis.h"
#include "config.h"
using namespace std;

int main(int argc, char* argv[])
{
   // Read in component list, task graph, design specs,
   // and fitness function parameters.

   AX_SynthesisC synth;
   AX_DsnC dsn;

   string  specsPath, libPath, specsName, confName, compsName, tgName;
   int     synthret;

   if (argc < 5)
   {
      cerr << "Need to start the program like:" << endl;
      cerr << "   archexp spec_file component_file"
           << " taskgraph_file control_file" << endl;
      return(1);
   }

   synth.get_projPaths("", specsPath, libPath);

   specsName = specsPath + argv[1];
   compsName = libPath   + argv[2];
   tgName    = specsPath + argv[3];
   confName  = specsPath + argv[4];

   synthret = synth.dsn_synthesis(confName, specsName, compsName, tgName, dsn);

   exit(synthret);
}
