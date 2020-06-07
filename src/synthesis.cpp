#define NO_EVOLVE 0

#include <iostream>
#include "synthesis.h"
#include "evolve.h"
using namespace std;

// -----------------------------------------------------------
// -----------------------------------------------------------
AX_SynthesisC::AX_SynthesisC()
{
}

// -----------------------------------------------------------
// -----------------------------------------------------------
AX_SynthesisC::~AX_SynthesisC()
{
}

// -----------------------------------------------------------
// -----------------------------------------------------------
int AX_SynthesisC::dsn_synthesis(string confName, string specsName,
                                 string compsName, string tgName,
                                 AX_DsnC & dsn)
{
   // Read in component list, task graph, design specs,
   // and fitness function parameters.

   if (!dsn.read_dsnspec(specsName))
      return(2);
   cerr << "Done reading specs\n";

   if (!dsn.read_comps(compsName))
      return(3);
   cerr << "Done reading library\n";
   if (!dsn.read_tg(tgName))
      return(4);
   cerr << "Done reading task graph\n";
#if DEBUG >= 5
   dsn.dump(); 
#endif

#if NO_EVOLVE
   exit(0);
#endif

   float alpha = 0.0, beta = 0.0, gama = 0.0, margin = 0.0;
   if (!read_config(confName, alpha, beta, gama, margin))
      return(4);
   cerr << "Done reading config file\n";
#if DEBUG >= 4
   cerr << "Fitness function parameters - alpha:"
        << alpha << ", beta:" << beta
        << ", gama:" << gama << endl;
#endif

#if DEBUG >= 5
   cerr << "<<<<<<======== Original graph\n";
   dsn.get_tg()->dump();

   AX_TaskGraphC nTg;
   {
      AX_TaskGraphC nTg1;
      nTg1.copy(*dsn.get_tg());
      nTg = nTg1;
   }
   cerr << "<<<<<<======== Original graph\n";
   dsn.get_tg()->dump();
   cerr << "\n\n<<<<<<======== New graph\n";
   nTg.dump();
#endif

   // set up a seed
   srand(RAN_SEED);

   // start the GA
   AX_EvolveC GA;
   GA.populate1(dsn);
#if DEBUG >= 1
   dsn.write_agL_files();
#endif

   if (ACTIVE_NORM)
      GA.get_normalizing_factors(dsn);

   AX_ArchGraphLC tempL;
   AX_ArchGraphLCIter tempI;
   for (tempI = dsn.get2_first_ag();
        tempI != dsn.ag_list_end(); tempI++)
   {
      AX_ArchGraphC nAg;
      AX_ArchGraphLCIter nAgI;
      tempL.push_back(nAg);
      nAgI = --tempL.end();
      (*nAgI).copy(*tempI);
   }
#if DEBUG >= 4
   dsn.write_agL_files();
#endif

   cerr << "Evolving..." << endl;
   float evVal, PevVal = 0.0;
   int convCnt = 0;
   AX_ArchGraphCP * ranking;

   for (int i = 0; i < NO_GENERATION; i++)
   {
      // perform mutation, rank the result against the
      // gene pool in the design, then prune. The result
      // of pruning is placed back to the design
      if (!GA.mutate(dsn, i+1))
         return(1);
#if DEBUG >= 0
      cerr << "Gen " << i << "\tmutate    - ";
#else
      cerr << "Step " << i << "\tExploring - ";
#endif
      AX_ArchGraphLCIter agI;
      GA.merge_pools(dsn, tempL);

      tempL.clear();

      ranking = new AX_ArchGraphCP[dsn.get_no_ags()];
      GA.rank_fitness(dsn, ranking, alpha, beta, gama);

      GA.prune(dsn, ranking);
      delete [] ranking;
      for (tempI = dsn.get2_first_ag();
           tempI != dsn.ag_list_end(); tempI++)
      {
         AX_ArchGraphC nAg;
         AX_ArchGraphLCIter nAgI;
         tempL.push_back(nAg);
         nAgI = --tempL.end();
         (*nAgI).copy(*tempI);
      }

      // perform crossover, rank the result against the
      // gene pool in the design, then prune. The result
      // of pruning is placed back to the design
      if (!GA.crossover(dsn, i+1))
         return(2);
#if DEBUG >= 0
      cerr << "  \tcrossover - ";
#else
      cerr << "       \tExploring - ";
#endif
      GA.merge_pools(dsn, tempL);
      tempL.clear();
      ranking = new AX_ArchGraphCP[dsn.get_no_ags()];
      GA.rank_fitness(dsn, ranking, alpha, beta, gama);
      evVal = GA.prune(dsn, ranking);
      delete [] ranking;
      for (tempI = dsn.get2_first_ag();
           tempI != dsn.ag_list_end(); tempI++)
      {
         AX_ArchGraphC nAg;
         AX_ArchGraphLCIter nAgI;
         tempL.push_back(nAg);
         nAgI = --tempL.end();
         (*nAgI).copy(*tempI);
      }
#if DEBUG >= 3
      dsn.write_agL_files();
#endif

      // perform sharing, rank the result against the
      // gene pool in the design, then prune. The result
      // of pruning is placed back to the design
      if (!GA.share_mutate(dsn, i+1))
         return(3);
#if DEBUG >= 0
      cerr << "    \tsharing   - ";
#else
      cerr << "       \tExploring - ";
#endif
      GA.merge_pools(dsn, tempL);
      tempL.clear();
      ranking = new AX_ArchGraphCP[dsn.get_no_ags()];
      GA.rank_fitness(dsn, ranking, alpha, beta, gama);
#if DEBUG >= 4
      for (int j = 0; j < dsn.get_no_ags(); j++)
      {
          cerr << ranking[j]->get_name() << endl
               << "    latency = " << ranking[j]->get_latency() << endl
               << "    area = " << ranking[j]->get_area() << endl
               << "    power = " << ranking[j]->get_power() << endl
               << "    cost = " << ranking[j]->get_cost() << endl
               << "    fitness = " << ranking[j]->get_fitness() << endl;
      }
#endif
      evVal = GA.prune(dsn, ranking);
      delete [] ranking;

      // testing the convergence
      if (my_fabs(evVal - PevVal) < margin)
         convCnt++;
      else
         convCnt = 0;
      if (convCnt > 5)
         break;
      PevVal = evVal;

      // tempL = dsn.get_agL();
      for (tempI = dsn.get2_first_ag();
           tempI != dsn.ag_list_end(); tempI++)
      {
         AX_ArchGraphC nAg;
         AX_ArchGraphLCIter nAgI;
         tempL.push_back(nAg);
         nAgI = --tempL.end();
         (*nAgI).copy(*tempI);
      }
   }
   cerr << "Writing results" << endl;

   GA.display_agL_fitness(dsn.get_agL());
   dsn.write_agL_files();
   dsn.write_agL_files_SystemC();
   dsn.write_bkui_dom_file();
   dsn.write_bkui_data_file();
   dsn.check_ags();

   return(0);
}

// ----------------------------------------------------------
// -----------------------------------------------------------
bool AX_SynthesisC::read_config(string FileName,
                                float & alpha, float & beta,
                                float & gama, float & margin)
{
    string rdStr;
    char rdBuf[256];

    fstream CfgFile;
    CfgFile.open(FileName.c_str(), fstream::in);

    if (!CfgFile)
    {
       cerr << "Cannot open " << FileName << endl;
       return false;
    }

    CfgFile >> rdBuf;
    rdStr = rdBuf;

    while ((strncmp(rdStr.c_str(), "//", 2) == 0) &&
           !CfgFile.eof())
    {
       // A comment line. Consume the line
       char getc = '0';
       while (getc != '\0' && getc != '\n')
          CfgFile.get(getc);

       // Get next item
       CfgFile >> rdBuf;
       rdStr = rdBuf;
       continue;
    }

    // get fitness function setting
    FIT_FUNC = atoi(rdStr.c_str());
    CfgFile >> rdBuf;
    rdStr = rdBuf;

    while ((strncmp(rdStr.c_str(), "//", 2) == 0) &&
           !CfgFile.eof())
    {
       // A comment line. Consume the line
       char getc = '0';
       while (getc != '\0' && getc != '\n')
          CfgFile.get(getc);

       // Get next item
       CfgFile >> rdBuf;
       rdStr = rdBuf;
       continue;
    }

    // get normalizing flag setting
    ACTIVE_NORM = (bool)atoi(rdStr.c_str());
    CfgFile >> rdBuf;
    rdStr = rdBuf;

    while ((strncmp(rdStr.c_str(), "//", 2) == 0) &&
           !CfgFile.eof())
    {
       // A comment line. Consume the line
       char getc = '0';
       while (getc != '\0' && getc != '\n')
          CfgFile.get(getc);

       // Get next item
       CfgFile >> rdBuf;
       rdStr = rdBuf;
       continue;
    }

    // get alpha value
    alpha = atof(rdStr.c_str());
    CfgFile >> rdBuf;
    rdStr = rdBuf;

    while ((strncmp(rdStr.c_str(), "//", 2) == 0) &&
           !CfgFile.eof())
    {
       // A comment line. Consume the line
       char getc = '0';
       while (getc != '\0' && getc != '\n')
          CfgFile.get(getc);

       // Get next item
       CfgFile >> rdBuf;
       rdStr = rdBuf;
       continue;
    }

    // get beta value
    beta = atof(rdStr.c_str());
    CfgFile >> rdBuf;
    rdStr = rdBuf;

    while ((strncmp(rdStr.c_str(), "//", 2) == 0) &&
           !CfgFile.eof())
    {
       // A comment line. Consume the line
       char getc = '0';
       while (getc != '\0' && getc != '\n')
          CfgFile.get(getc);

       // Get next item
       CfgFile >> rdBuf;
       rdStr = rdBuf;
       continue;
    }

    // get gama value
    gama = atof(rdStr.c_str());
    CfgFile >> rdBuf;
    rdStr = rdBuf;

    while ((strncmp(rdStr.c_str(), "//", 2) == 0) &&
           !CfgFile.eof())
    {
       // A comment line. Consume the line
       char getc = '0';
       while (getc != '\0' && getc != '\n')
          CfgFile.get(getc);

       // Get next item
       CfgFile >> rdBuf;
       rdStr = rdBuf;
       continue;
    }

    // get margin value
    margin = atof(rdStr.c_str());
    CfgFile >> rdBuf;
    rdStr = rdBuf;

    while ((strncmp(rdStr.c_str(), "//", 2) == 0) &&
           !CfgFile.eof())
    {
       // A comment line. Consume the line
       char getc = '0';
       while (getc != '\0' && getc != '\n')
          CfgFile.get(getc);

       // Get next item
       CfgFile >> rdBuf;
       rdStr = rdBuf;
       continue;
    }

    // get POP_SIZE value
    POP_SIZE = atoi(rdStr.c_str());
    CfgFile >> rdBuf;
    rdStr = rdBuf;

    while ((strncmp(rdStr.c_str(), "//", 2) == 0) &&
           !CfgFile.eof())
    {
       // A comment line. Consume the line
       char getc = '0';
       while (getc != '\0' && getc != '\n')
          CfgFile.get(getc);

       // Get next item
       CfgFile >> rdBuf;
       rdStr = rdBuf;
       continue;
    }

    // get NO_GENERATION value
    NO_GENERATION = atoi(rdStr.c_str());

    CfgFile >> rdBuf;
    rdStr = rdBuf;

    while ((strncmp(rdStr.c_str(), "//", 2) == 0) &&
           !CfgFile.eof())
    {
       // A comment line. Consume the line
       char getc = '0';
       while (getc != '\0' && getc != '\n')
          CfgFile.get(getc);

       // Get next item
       CfgFile >> rdBuf;
       rdStr = rdBuf;
       continue;
    }

    // get ASIC_COMP_METHOD
    ASIC_COMP_METHOD = atoi(rdStr.c_str());

    CfgFile >> rdBuf;
    rdStr = rdBuf;

    while ((strncmp(rdStr.c_str(), "//", 2) == 0) &&
           !CfgFile.eof())
    {
       // A comment line. Consume the line
       char getc = '0';
       while (getc != '\0' && getc != '\n')
          CfgFile.get(getc);

       // Get next item
       CfgFile >> rdBuf;
       rdStr = rdBuf;
       continue;
    }

    // get MAXGATECOUNT value
    MAXGATECOUNT = atoi(rdStr.c_str());

    CfgFile >> rdBuf;
    rdStr = rdBuf;

    while ((strncmp(rdStr.c_str(), "//", 2) == 0) &&
           !CfgFile.eof())
    {
       // A comment line. Consume the line
       char getc = '0';
       while (getc != '\0' && getc != '\n')
          CfgFile.get(getc);

       // Get next item
       CfgFile >> rdBuf;
       rdStr = rdBuf;
       continue;
    }

    // get ASIC_AREA value
    ASIC_AREA = atoi(rdStr.c_str());

    CfgFile >> rdBuf;
    rdStr = rdBuf;

    while ((strncmp(rdStr.c_str(), "//", 2) == 0) &&
           !CfgFile.eof())
    {
       // A comment line. Consume the line
       char getc = '0';
       while (getc != '\0' && getc != '\n')
          CfgFile.get(getc);

       // Get next item
       CfgFile >> rdBuf;
       rdStr = rdBuf;
       continue;
    }

    // get MEMCORE_AREA value
    MEMCORE_AREA = atoi(rdStr.c_str());

    CfgFile >> rdBuf;
    rdStr = rdBuf;

    while ((strncmp(rdStr.c_str(), "//", 2) == 0) &&
           !CfgFile.eof())
    {
       // A comment line. Consume the line
       char getc = '0';
       while (getc != '\0' && getc != '\n')
          CfgFile.get(getc);

       // Get next item
       CfgFile >> rdBuf;
       rdStr = rdBuf;
       continue;
    }

    // get BUS_CLK_RATE value
    BUS_CLK_RATE = atof(rdStr.c_str());

    CfgFile >> rdBuf;
    rdStr = rdBuf;

    while ((strncmp(rdStr.c_str(), "//", 2) == 0) &&
           !CfgFile.eof())
    {
       // A comment line. Consume the line
       char getc = '0';
       while (getc != '\0' && getc != '\n')
          CfgFile.get(getc);

       // Get next item
       CfgFile >> rdBuf;
       rdStr = rdBuf;
       continue;
    }

    // get BUS_WIDTH value
    BUS_WIDTH = atoi(rdStr.c_str());

    CfgFile >> rdBuf;
    rdStr = rdBuf;

    while ((strncmp(rdStr.c_str(), "//", 2) == 0) &&
           !CfgFile.eof())
    {
       // A comment line. Consume the line
       char getc = '0';
       while (getc != '\0' && getc != '\n')
          CfgFile.get(getc);

       // Get next item
       CfgFile >> rdBuf;
       rdStr = rdBuf;
       continue;
    }

    // get DSP_UTIL_MARGIN value
    DSP_UTIL_MARGIN = atof(rdStr.c_str());
    if (DSP_UTIL_MARGIN <= 0)
       DSP_UTIL_MARGIN = 0.0001;
    else if (DSP_UTIL_MARGIN >= 1)
       DSP_UTIL_MARGIN = 0.9999;

    CfgFile >> rdBuf;
    rdStr = rdBuf;

    while ((strncmp(rdStr.c_str(), "//", 2) == 0) &&
           !CfgFile.eof())
    {
       // A comment line. Consume the line
       char getc = '0';
       while (getc != '\0' && getc != '\n')
          CfgFile.get(getc);

       // Get next item
       CfgFile >> rdBuf;
       rdStr = rdBuf;
       continue;
    }

    // get RAN_SEED value
    RAN_SEED = atoi(rdStr.c_str());

    return true;
}

// -----------------------------------------------------------
// -----------------------------------------------------------
void AX_SynthesisC::get_projPaths(string home, string &specsPath,
                                  string &libPath)
{
    if (home == "") {
        specsPath = "./specs/";
        libPath = "./lib/";
    } else {
        specsPath = home + "/specs/";
        libPath = home + "/lib/";
    }
    return;
}

// -----------------------------------------------------------
// -----------------------------------------------------------
void AX_SynthesisC::get_specsPath(string home, string &specsPath)
{
    if (home == "") {
        specsPath = "./specs/";
    } else {
        specsPath = home + "/specs/";
    }
    return;
}

// -----------------------------------------------------------
// -----------------------------------------------------------
void AX_SynthesisC::get_libPath(string home, string &libPath)
{
    if (home == "") {
        libPath = "./lib/";
    } else {
        libPath = home + "/lib/";
    }
    return;
}
