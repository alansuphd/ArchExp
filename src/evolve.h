// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// evolve.h
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef AX_EVOLVE_H_INCLUDED
#define AX_EVOLVE_H_INCLUDED 1

#include <list>
#include "type.h"
#include "enum.h"
#include "util.h"

#ifndef AX_DSN_H_INCLUDED
#include "dsn.h"
#endif /* AX_DSN_H_INCLUDED */

#ifndef AX_SHARE_H_INCLUDED
#include "share.h"
#endif /* AX_SHARE_H_INCLUDED */

#ifndef AX_SCHEDULE_H_INCLUDED
#include "schedule.h"
#endif /* AX_SCHEDULE_H_INCLUDED */

// Forward Declarations ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Class Definition for EvolveC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class AX_EvolveC
{
public:
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructor
  AX_EvolveC();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Destructor
  ~AX_EvolveC();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Operations
     bool populate1(AX_DsnC & dsn);
     bool replace_func(AX_DsnC & dsn, AX_ArchGraphC & ag,
                          AX_TaskCP taskP, AX_FuncBlkCP origFuncP,
                          AX_FuncBlkCP platFuncP,
                          AX_FuncBlkCP & newFuncP, int & lNum3);
     bool mutate(AX_DsnC & dsn, int gen);
     void backup_intrfcPLs(AX_ArchGraphCP agP, AX_FuncBlkCP funcP,
                           AX_CompPortLC & inPortL,
                           AX_CompPortLC & outPortL,
                           AX_CompPortLC & ioPortL);
     void restore_intrfcPLs(AX_FuncBlkCP funcP,
                            AX_CompPortLC inPortL,
                            AX_CompPortLC outPortL,
                            AX_CompPortLC ioPortL);
     bool crossover(AX_DsnC & dsn, int gen);
     bool share_mutate(AX_DsnC & dsn, int gen);
     float prune(AX_DsnC & dsn, AX_ArchGraphCP * ranking);
     bool connect_comps(AX_DsnC & dsn, AX_ArchGraphC & ag, int & bNum);
     bool connect_comp(AX_ArchGraphC & ag, AX_TaskCP taskP, int & bNum);
     bool connect_comp(AX_ArchGraphC & ag, AX_FuncBlkCP funcP,
                          int & bNum);
     bool fix_connect(AX_ArchGraphC & ag, AX_FuncBlkCP funcP,
                         int & bNum);
     bool connect(AX_ArchGraphC & ag, AX_TaskGraphCP srcTgP,
                     AX_TaskCP srcTaskP, AX_TaskPortCP srcPortP,
                     AX_TaskGraphCP sinkTgP, AX_TaskCP sinkTaskP,
                     AX_TaskPortCP sinkPortP, int & bNum, AX_ConnCP connP);
     bool mend_connect(AX_DsnC & dsn, AX_ArchGraphC & ag);
     bool mend_bus(AX_ArchGraphC & ag);
     bool merge_buses(AX_BusCP aBusP, AX_BusCP bBusP,
                         AX_ArchGraphC & ag);
     AX_InterfaceCP select_intrfc_fm_buses(AX_BusCP aBusP, AX_BusCP bBusP,
                                            AX_ArchGraphC & ag);

     void fix(AX_DsnC & dsn);
     void copy_funcblk_platfm(AX_ArchGraphC & ag, AX_FuncBlkC & nFuncC,
                              AX_FuncBlkC & oFuncC);
     void merge_pools(AX_DsnC & dsn, AX_ArchGraphLC & mergL);
     void get_normalizing_factors(AX_DsnC & dsn);
     void rank_fitness(AX_DsnC & dsn, AX_ArchGraphCP * ranking,
                       float alpha, float beta, float gama);
     void get_ag_data_0(AX_ArchGraphC & ag);
     void get_ag_data_1(AX_ArchGraphC & ag);
     void fitness(AX_DsnC & dsn, float alpha, float beta, float gama);
     float fitness(AX_ArchGraphC & ag, float alpha, float beta, float gama);
     void sort_ranking(AX_ArchGraphCP * ranking, int rSize);
     bool reassign_sw_port(AX_DsnC & dsn, AX_ArchGraphC & ag,
                              AX_FuncBlkCP funcP);
     bool add_dependent_comps(AX_DsnC & dsn, AX_ArchGraphC & ag,
                                 int & fNum, int & pNum, int & bNum);
     bool add_dependent_comps(AX_DsnC & dsn, AX_ArchGraphC & ag,
                                 AX_FuncBlkC & funcC,
                                 int & fNum, int & pNum, int & bNum);
     bool expand_proc_refPL(bool Cpy, AX_DsnC & dsn, AX_ArchGraphC & ag,
                               AX_FuncBlkCP masProcP, AX_FuncBlkCP cpyFuncP,
                               int & fNum, int & pNum, int & bNum);
     void display_agL_fitness(AX_ArchGraphLC & agL);


  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~End of Class Definition
};

#endif /* AX_EVOLVE_H_INCLUDED */
