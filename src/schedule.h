// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// schedule.h
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef AX_SCHEDULE_H_INCLUDED
#define AX_SCHEDULE_H_INCLUDED 1

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

// Class Definition used in ScheduleC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class AX_Sched_BusC
{
 public:
  AX_BusC * m_bus;
  int m_start;
  int m_finish;
};

// Typedef Definitions for AX_Sched_BusC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef std::list<AX_Sched_BusC> AX_Sched_BusLC;
typedef std::list<AX_Sched_BusC>::iterator AX_Sched_BusLCIter;
typedef std::list<AX_Sched_BusC>::const_iterator AX_Sched_BusLCCnstIter;
typedef std::list<AX_Sched_BusC *> AX_Sched_BusPLC;
typedef std::list<AX_Sched_BusC *>::iterator AX_Sched_BusPLCIter;
typedef std::list<AX_Sched_BusC *>::const_iterator AX_Sched_BusPLCCnstIter;


// Class Definition for ScheduleC ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class AX_ScheduleC
{
 private:
  void determine_back_succ_pred(AX_TaskGraphCP tGraphP);
  void set_task_temp(AX_TaskGraphCP tGraphP);
  void identify_root(AX_TaskGraphCP tGraphP,
		     AX_TaskPLC & root_taskPL, 
		     AX_TaskPLC & ready_taskPL);
  void schedule_task_inports(AX_TaskGraphCP tGraphP,
			     AX_ArchGraphC & aGraph);
  int all_pred_scheduled_with_io(AX_TaskC * task, 
				 AX_ArchGraphC & aGraph);
  int all_pred_scheduled(AX_TaskC * task, 
			 AX_ArchGraphC & aGraph);
  AX_FuncBlkCP earliest_func_blk_avail(AX_ArchGraphC & aGraph,
					AX_TaskC * task, 
					int starting,
					int & earliest_start);
  void assign_bus_conn(AX_ConnC * conn,
		       AX_BusCP bus,
		       int start,
		       int finish);
  void my_sort_bus_connref(AX_BusC * bus);
  void assign_bus_conn_1(AX_ArchGraphC & aGraph,
			 AX_TaskPortC * port,
			 AX_ConnC * conn,
			 int start,
			 int & earliest_start,
			 int & finish);
  void assign_bus(AX_TaskPortC * port,
		  AX_ArchGraphC & aGraph,
		  int earliest_start);
  int assign_task_2_comp(AX_ArchGraphC & aGraph, 
			 AX_TaskC * task, 
			 int pred_finish);
  void update_ready_task(AX_TaskC * task, 
			 AX_TaskPLC & ready_taskPL,
			 AX_ArchGraphC & aGraph);
  bool is_iterblk_ready(int itrBlkIdx, 
			   AX_TaskGraphCP tGraphP,
			   AX_ArchGraphC & aGraph);
  void identify_root_sink_itrblk(AX_TaskGraphCP tGraphP,
				 AX_TaskPLC & root_taskPL,
				 AX_TaskPLC & sink_taskPL,
				 int itrBlkIdx);
  int get_task_latency(AX_TaskC * task, 
		       AX_ArchGraphC & aGraph);
  int determine_all_pred_done(AX_TaskC * task, 
			      AX_ArchGraphC & aGraph,
			      int itrBlkIdx);
  int determine_all_succ_done(AX_TaskC * task, 
			      AX_ArchGraphC & aGraph,
			      int itrBlkIdx);
  void compute_asap_alap(AX_TaskGraphCP tGraphP, 
			 AX_ArchGraphC & aGraph,
			 AX_TaskPLC & taskPL,
			 AX_TaskPLC & root_taskPL, 
			 AX_TaskPLC & sink_taskPL,
			 int itrBlkIdx);
  void sort_tasks_by_alap(AX_TaskGraphCP tGraphP, 
			  AX_TaskPLC & taskPL);
  bool cycle_dfs_itrblk(AX_TaskGraphCP tGraphP, 
			   AX_ArchGraphC & aGraph,
			   AX_TaskCP headP, 
			   AX_TaskCP curr_taskP, 
			   int & latency, 
			   int & lcd,
			   int itrBlkIdx);
  int start_cycle_dfs_itrblk(AX_TaskGraphCP tGraphP, 
			     AX_ArchGraphC & aGraph,
			     int itrBlkIdx);
  void determine_ii_limits(AX_TaskGraphCP tGraphP, 
			   AX_ArchGraphC & aGraph, 
			   int & max_ii, 
			   int & step_size,
			   int & max_delay,
			   int itrBlkIdx);
  int is_func_free(AX_FuncBlkC * funcblk,
		   int start, 
		   int length);
  int assign_task_2_comp_itrblk(AX_ArchGraphC & aGraph, 
				AX_TaskC * task, 
				int start,
				int finish);
  bool sched_itrblk(AX_TaskGraphCP tGraphP, 
		       AX_ArchGraphC & aGraph, 
		       AX_TaskPLC & taskPL, 
		       AX_TaskPLC & root_taskPL, 
		       AX_TaskPLC & sink_taskPL, 
		       int ii);
  bool schedule_iterblk(AX_TaskPLC & ready_taskPL,
			   AX_TaskGraphCP tGraphP, 
			   AX_ArchGraphC & aGraph,
			   int itrBlkIdx);
  bool schedule_task(AX_TaskPLC & ready_taskPL,
			AX_TaskGraphCP tGraphP,
			AX_ArchGraphC & agraph,
			AX_TaskC * task);
  void check_schedule(AX_ArchGraphC & aGraph);
  void reset_task_temp(AX_TaskGraphCP tGraphP);
 public:
  void sched_main(AX_DsnC & dsn, AX_ArchGraphC & agraph);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructor
  AX_ScheduleC();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Destructor
  ~AX_ScheduleC();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~End of Class Definition
};

#endif /* AX_SCHEDULE_H_INCLUDED */
