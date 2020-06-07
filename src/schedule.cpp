#include <iostream>
#include <math.h>
#include "schedule.h"
using namespace std;

// ------------------------------------------------------------------
// Assumptions:
// A task can belong to at most
//   - one iteration block, and
//   - one cycle
// Cycles and iteration blocks cannot cross each other.
// ------------------------------------------------------------------
// scheduler(dsn)
// {
//    while (unprocessed iteration_blk)
//    {
//       - identify tasks and conns in the iteration block
//         (if task/conn is in the iteration block, task/conn.temp = 1
//         else = 0)
//       - initialize start and finish times of these tasks and connections
//         (task/conn.start = task/conn.finish = -1) 
//       - identify root and sink tasks with temp = 1. set root.start = 0
//       - identify cycles within this iteration block
//         (use dfs among tasks/conn with temp = 1. cycle head is known so 
//         start dfs from there)
//       - determine back succs and back preds in the iteration block
//         (this is done during the dfs)
//       - compute min ii due to cycles in the iteration block
//         (collect number of delays and latency of the cycle during dfs)
//       - schedule iteration block assuming it is a separate task graph
//         - success = false
//         - for (ii = min_ii; ii < ???; ii++) {
//            - schedule task graph for ii
//            - if scheduling not successful
//               - return false
//              else
//               - break from (for ii) loop
//         }
//    }
//    - process top level graph (assume iteration blocks have been replaced
//      by a node. this also implies that cycles within an iteration block
//      are ignored from any processing)
//    - identify tasks and conns not in any iteration block
//      (if task/conn is in an iteration block, temp = 0, else = 1)
//    - initialize start and finish times of tasks and connections with
//      temp = 1.
//    - identify root and sink tasks with temp = 1. set root.start = 0
//    - identify cycles. cycle can only be formed if there is a cycle head
//      with temp = 1.
//      (use dfs among all task/conn irrespective of temp. cycle head is known 
//      so start dfs from cycle heads with temp = 1)
//    - determine back succs and back preds in the iteration block
//      (this is done during the dfs)
//    - compute min ii due to cycles
//      (collect number of delays and latency of the cycle during dfs)
//    - schedule top level. during scheduling adjust the start and finish times
//      of tasks within iteration blocks.
// }

// ----------------------------------------------------------
// ----------------------------------------------------------
AX_ScheduleC::AX_ScheduleC()
{
}

// ----------------------------------------------------------
// ----------------------------------------------------------
AX_ScheduleC::~AX_ScheduleC()
{
}

// ------------------------------------------------------------
// ------------------------------------------------------------
void AX_ScheduleC::determine_back_succ_pred(AX_TaskGraphCP tGraphP)
{
  AX_TaskLCIter taskIter;
  for (taskIter = tGraphP->get2_first_task(); 
       taskIter != tGraphP->task_list_end(); taskIter++)
    {
      if ((*taskIter).get_type() != DELAY)
	continue;
      AX_TaskPLCIter succ;
      for (succ = (*taskIter).get2_first_succ();
	   succ != (*taskIter).succ_list_end(); succ++)
	{
	  AX_TaskPLCIter pred;
	  for (pred = (*taskIter).get2_first_pred();
	       pred != (*taskIter).pred_list_end(); pred++)
	    {
	      (*succ)->set_backpred(*pred);
	      (*pred)->set_backsucc(*succ);
	      (*succ)->set_lcd((*taskIter).get_delay());
	    } // end for pred
	} // end for succ
    } // end for taskIter
}

// ----------------------------------------------------------
// ----------------------------------------------------------
void AX_ScheduleC::set_task_temp(AX_TaskGraphCP tGraphP)
{
  // Initialize all tasks temp to 0;
  AX_TaskLCIter taskIter;
  for (taskIter = tGraphP->get2_first_task(); 
       taskIter != tGraphP->task_list_end(); taskIter++)
    {
      (*taskIter).set_temp(0);
    }

  int itrBlkIdx;
  itrBlkIdx = 1;
  // For each iteration block in the task graph
  AX_IterBlkLSIter itrBlkIter;
  for (itrBlkIter = tGraphP->get2_first_iterblk(); 
       itrBlkIter != tGraphP->iterblk_list_end(); itrBlkIter++)
    {
      AX_TaskPLCIter taskPIter;
      for (taskPIter = ((*itrBlkIter).m_taskPL).begin();
	   taskPIter != ((*itrBlkIter).m_taskPL).end();
	   taskPIter++)
	{
	  (*taskPIter)->set_temp(itrBlkIdx);
	} // end for taskPIter
      (*itrBlkIter).m_basedelay = -1;
      itrBlkIdx++;
    }
}

// ----------------------------------------------------------
// ----------------------------------------------------------
void AX_ScheduleC::identify_root(AX_TaskGraphCP tGraphP,
				  AX_TaskPLC & root_taskPL,
				  AX_TaskPLC & ready_taskPL)
{
  AX_TaskLCIter taskIter;
  for (taskIter = tGraphP->get2_first_task(); 
       taskIter != tGraphP->task_list_end(); taskIter++)
    {
      // identify root task
      bool isroot;
      isroot = true;
      AX_TaskPLCIter pred;
      for (pred = (*taskIter).get2_first_pred();
	   pred != (*taskIter).pred_list_end(); pred++)
	{
	  if ((*pred)->get_type() == DELAY)
	    continue;
	  isroot = false;
	  break; // break from for pred loop
	} // end for pred
      if (isroot == true)
	{
	  root_taskPL.push_back(&(*taskIter));
	  ready_taskPL.push_back(&(*taskIter));
	}
    } // end for taskIter
}

// ------------------------------------------------------------
// ------------------------------------------------------------ 
void AX_ScheduleC::schedule_task_inports(AX_TaskGraphCP tGraphP,
					  AX_ArchGraphC & aGraph)
{
  AX_TaskPortLCIter tport;
  for (tport = tGraphP->get2_first_tg_inport();
       tport != tGraphP->tg_inport_list_end(); tport++)
    {
      AX_ConnPLCIter conn;
      conn = (*tport).get2_first_conn();
      AX_BusCP bus;
      bus = aGraph.get_bus_by_conn((*conn));
      if (bus == NULL)
	continue;
      assign_bus(&(*tport), aGraph, 0);
    } // end for port
}

// ------------------------------------------------------------
// ------------------------------------------------------------ 
int AX_ScheduleC::all_pred_scheduled_with_io(AX_TaskC * task, 
					      AX_ArchGraphC & aGraph)
{
  int finish;
  finish = 0;

  AX_TaskPortPLCIter port;
  for (port = task->get2_first_inport();
       port != task->inport_list_end(); port++)
    {
      AX_ConnPLCIter conn;
      conn = (*port)->get2_first_conn();
      AX_TaskCP src;
      src = (*conn)->get_src_task();
      AX_BusCP bus;
      bus = aGraph.get_bus_by_conn((*conn));
      if (bus == NULL)
	{
	  AX_SchdTimeLSIter schedtimeIter;
	  schedtimeIter = aGraph.get_schdtime_by_task(src);
	  if (schedtimeIter == aGraph.schdtime_list_end())
	    return(-1);
	  int x;
	  x = (*schedtimeIter).m_finish;
	  if (finish < x)
	    finish = x;
	  continue;
	}
      AX_ConnRefLSIter connref;
      connref = bus->add_new_connref((*conn));
      int x = (*connref).m_finish;
      if (x < 0)
	{
	  if (src == NULL)
	    continue;
	  if (src->get_type() != ATOMIC)
	    continue;
	  return(-1);
	}
      if (finish < x)
	finish = x;
    } // end for port

  return(finish);
}

// ------------------------------------------------------------
// ------------------------------------------------------------ 
int AX_ScheduleC::all_pred_scheduled(AX_TaskC * task, 
				      AX_ArchGraphC & aGraph)
{
  AX_TaskPLCIter pred;
  int finish;
  
  finish = 0;
  for (pred = task->get2_first_pred();
       pred != task->pred_list_end(); pred++)
    {
      if ((*pred)->get_type() != ATOMIC)
	continue;
      AX_SchdTimeLSIter pred_schedIter;
      pred_schedIter = aGraph.get_schdtime_by_task((*pred));
      if (pred_schedIter == aGraph.schdtime_list_end())
	return(-1);
      if ((*pred_schedIter).m_start < 0)
	return(-1);
      if ((*pred_schedIter).m_finish > finish)
	finish = (*pred_schedIter).m_finish;
    }
  return(finish);
}

// ----------------------------------------------------------
// ----------------------------------------------------------
AX_FuncBlkCP AX_ScheduleC::earliest_func_blk_avail(AX_ArchGraphC & aGraph,
						     AX_TaskC * task, 
						     int starting,
						     int & earliest_start)
{
  earliest_start = starting;
  AX_FuncBlkCP funcblk;
  funcblk = aGraph.find_fncblk_by_task(task);
  if (funcblk->get_type() == HARDWARE)
    return(funcblk);
  AX_UAttrCP uattrP;
  uattrP = funcblk->get_uattr(UATTRDATACOMPDELAY);
  int comp_delay;
  comp_delay = my_atoi(uattrP->get_value());
  if (comp_delay < 1 && task->get_type() == ATOMIC)
    comp_delay = 1;

  AX_FuncBlkPLCIter depblk;
  for (depblk = funcblk->get2_first_dep();
       depblk != funcblk->dep_list_end(); depblk++)
    {
      if ((*depblk)->get_functype() == PROCESSOR)
	break;
    }
#if DEBUG >= 1
  if (depblk == funcblk->dep_list_end())
  {
     cerr << "ERROR -- Cannot find dependent processor for FuncBlk "
          << funcblk->get_name() << endl;
     exit(0);
  }
#endif

  (*depblk)->sort_taskref_by_start();
  AX_TaskRefLSIter taskrefIter;
  taskrefIter = (*depblk)->get2_first_taskref();
  if (taskrefIter == (*depblk)->taskref_list_end())
    return(funcblk);
  if ((*taskrefIter).m_start > earliest_start + comp_delay)
    return(funcblk);
  for (taskrefIter = (*depblk)->get2_first_taskref();
       taskrefIter != (*depblk)->taskref_list_end(); taskrefIter++)
    {
      if ((*taskrefIter).m_start <= earliest_start &&
	  (*taskrefIter).m_finish > earliest_start)
	{
	  earliest_start = (*taskrefIter).m_finish + 1;
	  break;
	}
      if ((*taskrefIter).m_start > earliest_start)
	break;
    } // end for taskrefIter
  if (taskrefIter == (*depblk)->taskref_list_end())
    return(funcblk);
  if ((*taskrefIter).m_start > earliest_start + comp_delay)
    return(funcblk);
  while (taskrefIter != (*depblk)->taskref_list_end())
    {
      if ((*taskrefIter).m_finish < earliest_start)
	{
	  AX_TaskRefLSIter taskrefIter1;
	  taskrefIter1 = taskrefIter;
	  taskrefIter1++;
	  if (taskrefIter1 == (*depblk)->taskref_list_end())
	    return(funcblk);
	  if ((*taskrefIter1).m_start > earliest_start + comp_delay)
	    return(funcblk);
	  earliest_start = (*taskrefIter1).m_finish + 1;
	}
      else
	earliest_start = (*taskrefIter).m_finish + 1;
      taskrefIter++;
    } // end while
  return(funcblk);
}

// ------------------------------------------------------------
// ------------------------------------------------------------ 
void AX_ScheduleC::assign_bus_conn(AX_ConnC * conn,
				    AX_BusCP bus,
				    int start,
				    int finish)
{
  AX_ConnRefLSIter connref;
  connref = bus->add_new_connref(conn);
  if (connref == bus->connref_list_end())
    return;
  (*connref).m_start = start;
  (*connref).m_finish = finish;
  my_sort_bus_connref(bus);
}

// ------------------------------------------------------------
// ------------------------------------------------------------ 
void AX_ScheduleC::my_sort_bus_connref(AX_BusC * bus)
{
  AX_ConnRefS tref;
  AX_ConnRefLSIter trefI;

  tref.m_connP = AX_ConnCP(1);
  tref.m_start = LARGEINTEGER;
  tref.m_finish = LARGEINTEGER;
  bus->add_new_connref(tref);

  tref.m_connP = AX_ConnCP(2);
  tref.m_start = LARGEINTEGER;
  tref.m_finish = LARGEINTEGER;
  bus->add_new_connref(tref);

  bus->sort_connref_by_start();

  bool stepback = false;
  for (trefI = bus->get2_first_connref();
       trefI != bus->connref_list_end(); trefI++)
  {
     if (stepback)
     {
	trefI--;
	stepback = true;
     }
     if ((*trefI).m_connP == AX_ConnCP(1) ||
	 (*trefI).m_connP == AX_ConnCP(2))
     {
        trefI = bus->delete_connref(trefI);
        if (trefI != bus->get2_first_connref())
	   trefI--;
        else
	   stepback = true;
     }
  }
} 

// ------------------------------------------------------------
// ------------------------------------------------------------ 
void AX_ScheduleC::assign_bus_conn_1(AX_ArchGraphC & aGraph,
				      AX_TaskPortC * port,
				      AX_ConnC * conn,
				      int start,
				      int & earliest_start,
				      int & finish)
{
  AX_BusCP bus;
  bus = aGraph.get_bus_by_conn(conn);
  float x;
  x = port->get_sample_size() * 1000 / (BUS_CLK_RATE * BUS_WIDTH);
  int delay;
  delay = (int) (ceil(x));
  earliest_start = start;
  finish = earliest_start + delay - 1;

  my_sort_bus_connref(bus);

  AX_ConnRefLSIter connrefIter;
  connrefIter = bus->get2_first_connref();
  if (connrefIter == bus->connref_list_end())
    {
      assign_bus_conn(conn, bus, earliest_start, finish);
      return;
    }
  if ((*connrefIter).m_start > finish)
    {
      assign_bus_conn(conn, bus, earliest_start, finish);
      return;
    }
  for (connrefIter = bus->get2_first_connref();
       connrefIter != bus->connref_list_end(); connrefIter++)
    {
      if ((*connrefIter).m_start <= earliest_start &&
	  (*connrefIter).m_finish > earliest_start)
	{
	  earliest_start = (*connrefIter).m_finish + 1;
	  finish =  earliest_start + delay - 1;
	  break;
	}
      if ((*connrefIter).m_start > earliest_start)
	break;
    } // end for taskrefIter
  if (connrefIter == bus->connref_list_end())
    {
      assign_bus_conn(conn, bus, earliest_start, finish);
      return;
    }

  if ((*connrefIter).m_start > finish)
    {
      assign_bus_conn(conn, bus, earliest_start, finish);
      return;
    }
      
  while (connrefIter != bus->connref_list_end())
    {
      if ((*connrefIter).m_finish < earliest_start)
	{
	  AX_ConnRefLSIter connrefIter1;
	  connrefIter1 = connrefIter;
	  connrefIter1++;
	  if (connrefIter1 == bus->connref_list_end())
	    {
	      assign_bus_conn(conn, bus, earliest_start, finish);
	      return;
	    }
	  if ((*connrefIter1).m_start > finish)
	    {
	      assign_bus_conn(conn, bus, earliest_start, finish);
	      return;
	    }
	  earliest_start = (*connrefIter1).m_finish + 1;
	  finish =  earliest_start + delay - 1;
	}
      else
	{
	  earliest_start = (*connrefIter).m_finish + 1;
	  finish =  earliest_start + delay - 1;
	}
      connrefIter++;
    } // end while
  assign_bus_conn(conn, bus, earliest_start, finish);
}

// ------------------------------------------------------------
// ------------------------------------------------------------ 
void AX_ScheduleC::assign_bus(AX_TaskPortC * port,
			       AX_ArchGraphC & aGraph,
			       int start)
{
  AX_Sched_BusLC busses;
  AX_Sched_BusLCIter bus_iter;
  AX_ConnPLCIter conn;
  for (conn = port->get2_first_conn(); 
       conn != port->conn_list_end(); conn++)
    {
      AX_BusCP bus;
      bus = aGraph.get_bus_by_conn((*conn));
      if (bus == NULL)
	continue;
      bool skip;
      skip = false;
      for (bus_iter = busses.begin(); 
	   bus_iter != busses.end(); bus_iter++)
	{
	  if ((*bus_iter).m_bus == bus)
	    {
	      assign_bus_conn((*conn), bus, (*bus_iter).m_start,
			      (*bus_iter).m_finish);
	      skip = true;
	      break;
	    }
	}
      if (skip == true)
	continue;
      int earliest_start;
      int finish;
      assign_bus_conn_1(aGraph, port, (*conn), start, 
			earliest_start, finish);
      AX_Sched_BusC newbus;
      newbus.m_bus = bus;
      newbus.m_start = earliest_start;
      newbus.m_finish = finish;
      busses.push_back(newbus);
    } // end for conn
  busses.clear();
}

// ------------------------------------------------------------
// ------------------------------------------------------------ 
int AX_ScheduleC::assign_task_2_comp(AX_ArchGraphC & aGraph, 
				      AX_TaskC * task, 
				      int pred_finish)
{
  AX_FuncBlkCP funcblk;
  int earliest_start;
  funcblk = earliest_func_blk_avail(aGraph, task, pred_finish + 1, 
				    earliest_start);

  // assign task to function block
  AX_UAttrCP uattrP;
  uattrP = funcblk->get_uattr(UATTRDATACOMPDELAY);
  int delay;
  delay = my_atoi(uattrP->get_value());
  if (delay < 1)
    delay = 1;
  AX_SchdTimeLSIter schedtimeIter;
  schedtimeIter = aGraph.create_new_schdtime();
  (*schedtimeIter).m_taskP = task;
  (*schedtimeIter).m_start = earliest_start;
  (*schedtimeIter).m_finish = delay + earliest_start - 1;
  (*schedtimeIter).m_fncblkP = funcblk;
  AX_TaskRefLSIter tref;
  tref = funcblk->add_new_taskref(task);
  (*tref).m_start = earliest_start;
  (*tref).m_finish = delay + earliest_start - 1;
  funcblk->sort_taskref_by_start();
  if (funcblk->get_type() == SOFTWARE)
    {
      AX_FuncBlkPLCIter depblk;
      for (depblk = funcblk->get2_first_dep();
	   depblk != funcblk->dep_list_end(); depblk++)
	{
	  if ((*depblk)->get_functype() == PROCESSOR)
	    break;
	}
      tref = (*depblk)->add_new_taskref(task);
      (*tref).m_start = earliest_start;
      (*tref).m_finish = delay + earliest_start - 1;
      (*depblk)->sort_taskref_by_start();
      funcblk = (*depblk); // this is useful for connection scheduling
    } // end if funcblk

  // Now schedule the output connections connected to this task
  AX_TaskPortPLCIter port;
  for (port = task->get2_first_outport();
       port != task->outport_list_end(); port++)
    {
      assign_bus((*port), aGraph, delay + earliest_start);
    } // end for port

  return(earliest_start);
}

// ------------------------------------------------------------
// ------------------------------------------------------------ 
void AX_ScheduleC::update_ready_task(AX_TaskC * task, 
				      AX_TaskPLC & ready_taskPL,
				      AX_ArchGraphC & aGraph)
{
  AX_TaskPLCIter succ;
  for (succ = task->get2_first_succ();
       succ != task->succ_list_end(); succ++)
    {
      if (all_pred_scheduled((*succ), aGraph) != -1)
	ready_taskPL.push_back((*succ));
    }
  AX_TaskPLCIter taskitr;
  taskitr = ready_taskPL.begin();
  ready_taskPL.erase(taskitr);
}

// ------------------------------------------------------------
// ------------------------------------------------------------ 
bool AX_ScheduleC::is_iterblk_ready(int itrBlkIdx, 
					AX_TaskGraphCP tGraphP,
					AX_ArchGraphC & aGraph)
{
  AX_TaskLCIter taskIter;
  for (taskIter = tGraphP->get2_first_task(); 
       taskIter != tGraphP->task_list_end(); taskIter++)
    {
      if ((*taskIter).get_temp() != itrBlkIdx)
	continue;
      if ((*taskIter).get_type() == DELAY)
	continue;
      AX_TaskPLCIter pred;
      for (pred = (*taskIter).get2_first_pred();
	   pred != (*taskIter).pred_list_end(); pred++)
	{
	  if ((*pred)->get_temp() == itrBlkIdx)
	    continue;
	  if ((*pred)->get_type() == DELAY)
	    continue;
	  AX_SchdTimeLSIter pred_schedIter;
	  pred_schedIter = aGraph.get_schdtime_by_task((*pred));
	  if (pred_schedIter == aGraph.schdtime_list_end())
	    return(false);
	  if ((*pred_schedIter).m_start < 0)
	    return(false);
	}
    }
  return(true);
}

// ----------------------------------------------------------
// ----------------------------------------------------------
void AX_ScheduleC::identify_root_sink_itrblk(AX_TaskGraphCP tGraphP,
					      AX_TaskPLC & root_taskPL,
					      AX_TaskPLC & sink_taskPL,
					      int itrBlkIdx)
{
  AX_TaskLCIter taskIter;
  for (taskIter = tGraphP->get2_first_task(); 
       taskIter != tGraphP->task_list_end(); taskIter++)
    {
      if ((*taskIter).get_temp() != itrBlkIdx)
	continue;

      // identify root task
      bool isroot;
      isroot = true;
      AX_TaskPLCIter pred;
      for (pred = (*taskIter).get2_first_pred();
	   pred != (*taskIter).pred_list_end(); pred++)
	{
	  if ((*pred)->get_type() != DELAY &&
	      (*pred)->get_temp() == itrBlkIdx)
	    {
	      isroot = false;
	      break; // break from for pred loop
	    }
	} // end for pred
      if (isroot == true)
	{
	  root_taskPL.push_back(&(*taskIter));
	}

      // identify sink task
      bool issink;
      issink = true;
      AX_TaskPLCIter succ;
      for (succ = (*taskIter).get2_first_succ();
	   succ != (*taskIter).succ_list_end(); succ++)
	{
	  if ((*succ)->get_temp() == itrBlkIdx &&
	      (*succ)->get_type() != DELAY)
	    {
	      issink = false;
	      break; // break from for succ loop
	    }
	} // end for succ
      if (issink == true)
	  sink_taskPL.push_back(&(*taskIter));
    } // end for taskIter
}

// ----------------------------------------------------------
// ----------------------------------------------------------
int AX_ScheduleC::get_task_latency(AX_TaskC * task, 
				    AX_ArchGraphC & aGraph)
{
  AX_FuncBlkCP funcblk;
  funcblk = aGraph.find_fncblk_by_task(task);
  AX_UAttrCP uattrP;
  uattrP = funcblk->get_uattr(UATTRDATACOMPDELAY);
  int latency;
  latency = my_atoi(uattrP->get_value());
  if (latency < 1 && task->get_type() == ATOMIC)
    latency = 1;
  return(latency);
}

// ------------------------------------------------------------
// ------------------------------------------------------------ 
int AX_ScheduleC::determine_all_pred_done(AX_TaskC * task, 
					   AX_ArchGraphC & aGraph,
					   int itrBlkIdx)
{
  AX_TaskPLCIter pred;
  int pred_asap;
  int asap;
  int comp_delay;
  
  asap = 0;
  for (pred = task->get2_first_pred();
       pred != task->pred_list_end(); pred++)
    {
      if ((*pred)->get_temp() != itrBlkIdx)
	continue;
      if ((pred_asap = (*pred)->get_asap()) < 0)
	return(-1);
      comp_delay = get_task_latency((*pred), aGraph);
      if (comp_delay < 1 && (*pred)->get_type() == ATOMIC)
	comp_delay = 1;
      if (pred_asap + comp_delay > asap)
	asap = pred_asap + comp_delay;
    }
  return(asap);
}

// ------------------------------------------------------------
// ------------------------------------------------------------ 
int AX_ScheduleC::determine_all_succ_done(AX_TaskC * task, 
					   AX_ArchGraphC & aGraph,
					   int itrBlkIdx)
{
  AX_TaskPLCIter succ;
  int succ_alap;
  int alap;
  int comp_delay;
  
  comp_delay = get_task_latency(task, aGraph);
  if (comp_delay < 1 && task->get_type() == ATOMIC)
    comp_delay = 1;
  alap = LARGEINTEGER;
  for (succ = task->get2_first_succ();
       succ != task->succ_list_end(); succ++)
    {
      if ((*succ)->get_temp() != itrBlkIdx)
	continue;
      if ((succ_alap = (*succ)->get_alap()) < 0)
	return(-1);
      if (succ_alap - comp_delay < alap)
	alap = succ_alap - comp_delay;
    }
  return(alap);
}

// ------------------------------------------------------------
// ------------------------------------------------------------ 
void AX_ScheduleC::compute_asap_alap(AX_TaskGraphCP tGraphP, 
				      AX_ArchGraphC & aGraph,
				      AX_TaskPLC & taskPL,
				      AX_TaskPLC & root_taskPL, 
				      AX_TaskPLC & sink_taskPL,
				      int itrBlkIdx)
{
  AX_TaskPLCIter taskPIter;

  //Initialize asap and alap values of all tasks
  for (taskPIter = taskPL.begin();
       taskPIter != taskPL.end(); taskPIter++)
    {
      if ((*taskPIter)->get_type() == DELAY)
	(*taskPIter)->set_asap(0);
      else
	(*taskPIter)->set_asap(-1);
      (*taskPIter)->set_alap(-1);
    }

  // Initialize asap values of all root tasks.
  for (taskPIter = root_taskPL.begin();
       taskPIter != root_taskPL.end(); taskPIter++)
    (*taskPIter)->set_asap(0);

  int max_asap;
  max_asap = 0;

  // Compute asap values of all tasks.
  int repeatflag;
  repeatflag = 1;
  while (repeatflag == 1)
    {
      repeatflag = 0;
      for (taskPIter = taskPL.begin();
	   taskPIter != taskPL.end(); taskPIter++)
	{
	  if ((*taskPIter)->get_asap() > -1)
	    continue;
	  int last;
	  if ((last = determine_all_pred_done((*taskPIter), aGraph,
					      itrBlkIdx)) < 0)
	    {
	      repeatflag = 1;
	      continue;
	    }
	  (*taskPIter)->set_asap(last);
	  int comp_delay;
	  comp_delay = get_task_latency((*taskPIter), aGraph);
	  if (comp_delay < 1 && (*taskPIter)->get_type() == ATOMIC)
	    comp_delay = 1;
	  if (last + comp_delay > max_asap)
	    max_asap = last + comp_delay;
	} // end for taskPIter
    } // end while repeatflag

  // Initialize alap values of all sink tasks.
  for (taskPIter = sink_taskPL.begin();
       taskPIter != sink_taskPL.end(); taskPIter++)
    (*taskPIter)->set_alap(max_asap);
  //Initialize alap values of all DELAY tasks
  for (taskPIter = taskPL.begin();
       taskPIter != taskPL.end(); taskPIter++)
    {
      if ((*taskPIter)->get_type() == DELAY)
	(*taskPIter)->set_alap(max_asap);
    }

  // Compute alap values of all tasks.
  repeatflag = 1;
  while (repeatflag == 1)
    {
      repeatflag = 0;
      for (taskPIter = taskPL.begin();
	   taskPIter != taskPL.end(); taskPIter++)
	{
	  if ((*taskPIter)->get_alap() > -1)
	    continue;
	  int last;
	  if ((last = determine_all_succ_done((*taskPIter), aGraph,
					      itrBlkIdx)) < 0)
	    {
	      repeatflag = 1;
	      continue;
	    }
	  (*taskPIter)->set_alap(last);
	} // end for taskPIter
    } // end while repeatflag
}

// ------------------------------------------------------------
// ------------------------------------------------------------ 
void AX_ScheduleC::sort_tasks_by_alap(AX_TaskGraphCP tGraphP, 
				       AX_TaskPLC & taskPL)
{
  AX_TaskPLCIter piter;
  for (piter = taskPL.begin(); piter != taskPL.end(); piter++)
    {
      AX_TaskPLCIter piter1, piter2;
      AX_TaskCP tTmp;
      piter2 = piter;
      piter2++;
      for (piter1 = piter2; piter1 != taskPL.end(); piter1++)
	{
	  int alap, alap1;
	  alap = (*piter)->get_alap();
	  alap1 = (*piter1)->get_alap();
	  if (alap > alap1)
          {
            tTmp = (*piter);
            (*piter) = (*piter1);
            (*piter) = tTmp;
          }
	}
    }
}

// ----------------------------------------------------------
// ----------------------------------------------------------
bool AX_ScheduleC::cycle_dfs_itrblk(AX_TaskGraphCP tGraphP, 
					AX_ArchGraphC & aGraph,
					AX_TaskCP headP, 
					AX_TaskCP curr_taskP, 
					int & latency, 
					int & lcd,
					int itrBlkIdx)
{
  if (curr_taskP == headP)
  {
    return(true);
  }
  AX_TaskPLCIter succ;
  for (succ = (curr_taskP)->get2_first_succ();
       succ != (curr_taskP)->succ_list_end(); succ++)
    {
      if ((*succ)->get_temp() != itrBlkIdx)
	continue;
      if (cycle_dfs_itrblk(tGraphP, aGraph, headP, (*succ), latency, lcd,
			   itrBlkIdx)
	  == true)
	{
	  latency += get_task_latency(curr_taskP, aGraph);
	  int sample_size;
	  sample_size = curr_taskP->get_max_sample_to_succ((*succ));
	  float x;
	  x = sample_size * 1000 / (BUS_CLK_RATE * BUS_WIDTH);
	  latency = latency + (int) (ceil(x));
	  if (curr_taskP->get_type() == DELAY)
	      lcd += curr_taskP->get_delay();
	  return(true);
	}
    } // end for succ
  return(false);
}

// ----------------------------------------------------------
// ----------------------------------------------------------
int AX_ScheduleC::start_cycle_dfs_itrblk(AX_TaskGraphCP tGraphP, 
					  AX_ArchGraphC & aGraph,
					  int itrBlkIdx)
{
  int min_ii;
  min_ii = 1;
  AX_CycleLSIter cycleIter;
  for (cycleIter = tGraphP->get2_first_cycle();
       cycleIter != tGraphP->cycle_list_end(); cycleIter++)
    {
      if (((*cycleIter).m_headP)->get_temp() != itrBlkIdx)
	continue;
      (*cycleIter).m_cycle_latency = 0;
      (*cycleIter).m_lcd = 0;
      AX_TaskCP headP;
      headP = (*cycleIter).m_headP;
      AX_TaskPLCIter succ;
      for (succ = (headP)->get2_first_succ();
	   succ != (headP)->succ_list_end(); succ++)
	{
	  if ((*succ)->get_temp() != itrBlkIdx)
	    continue;
	  if ((*succ) == headP)
	    continue;
	  int latency;
	  latency = 0;
	  int lcd;
	  lcd = 0;
	  if (cycle_dfs_itrblk(tGraphP, aGraph, headP, (*succ), latency, lcd,
			       itrBlkIdx) == true)
	    {
	      latency += get_task_latency(headP, aGraph);
	      int sample_size;
	      sample_size = headP->get_max_sample_to_succ((*succ));
	      float x;
	      x = sample_size * 1000 / (BUS_CLK_RATE * BUS_WIDTH);
	      latency = latency + (int) (ceil(x));
	      (*cycleIter).m_cycle_latency = latency;
	      (*cycleIter).m_lcd = lcd;
	      break;
	    }
	} // end for succ
      int temp;
      temp = (*cycleIter).m_cycle_latency / (*cycleIter).m_lcd;
      if (temp > min_ii)
	min_ii = temp;
    }
  return(min_ii);
}

// ----------------------------------------------------------
// ----------------------------------------------------------
void AX_ScheduleC::determine_ii_limits(AX_TaskGraphCP tGraphP, 
					AX_ArchGraphC & aGraph, 
					int & max_ii, 
					int & step_size,
					int & max_delay,
					int itrBlkIdx)
{

  // max_ii = sum(delays of all tasks)
  // step size = min(delay of all tasks) / 2;

  AX_TaskLCIter taskIter;
  for (taskIter = tGraphP->get2_first_task(); 
       taskIter != tGraphP->task_list_end(); taskIter++)
    {
      if ((*taskIter).get_temp() != itrBlkIdx)
	continue;
      int del;
      del = get_task_latency(&(*taskIter), aGraph);
      if (max_delay < del)      
	max_delay = del;
      max_ii = max_ii + del;
      if (step_size > del / 2)
	step_size = del / 2;
    }
  if (step_size < 1)
    step_size = 1;
}

// ----------------------------------------------------------
// ------------------------------------------------------------
int AX_ScheduleC::is_func_free(AX_FuncBlkC * funcblk,
				    int start, 
				    int length)
{
  if (funcblk->get_type() == HARDWARE)
    return(-1);
  AX_FuncBlkPLCIter depblk;
  for (depblk = funcblk->get2_first_dep();
       depblk != funcblk->dep_list_end(); depblk++)
    {
      if ((*depblk)->get_functype() == PROCESSOR)
	break;
    }
  (*depblk)->sort_taskref_by_start();
  AX_TaskRefLSIter taskrefIter;
  taskrefIter = (*depblk)->get2_first_taskref();
  if (taskrefIter == (*depblk)->taskref_list_end())
    return(-1);
  if ((*taskrefIter).m_start > start + length)
    return(-1);
  for (taskrefIter = (*depblk)->get2_first_taskref();
       taskrefIter != (*depblk)->taskref_list_end(); taskrefIter++)
    {
      if ((*taskrefIter).m_start <= start &&
	  (*taskrefIter).m_finish > start)
	{
	  return((*taskrefIter).m_finish + 1);
	}
      if ((*taskrefIter).m_start > start)
	break;
    }
  if (taskrefIter == (*depblk)->taskref_list_end())
    return(-1);
  if ((*taskrefIter).m_start > start + length)
    return(-1);
  while (taskrefIter != (*depblk)->taskref_list_end())
    {
      if ((*taskrefIter).m_finish < start)
	{
	  AX_TaskRefLSIter taskrefIter1;
	  taskrefIter1 = taskrefIter;
	  taskrefIter1++;
	  if (taskrefIter1 == (*depblk)->taskref_list_end())
	    return(-1);
	  if ((*taskrefIter1).m_start > start + length)
	    return(-1);
	  return((*taskrefIter1).m_finish + 1);
	}
      else
	return((*taskrefIter).m_finish + 1);
      taskrefIter++;
    } // end while
  return(-1);
}

// ----------------------------------------------------------
// ------------------------------------------------------------
int AX_ScheduleC::assign_task_2_comp_itrblk(AX_ArchGraphC & aGraph, 
					     AX_TaskC * task, 
					     int start,
					     int finish)
{
  AX_FuncBlkCP funcblk;
  funcblk = aGraph.find_fncblk_by_task(task);
  // assign task to function block
  AX_UAttrCP uattrP;
  uattrP = funcblk->get_uattr(UATTRDATACOMPDELAY);
  int delay;
  delay = my_atoi(uattrP->get_value());
  if (delay < 1)
    delay = 1;
  AX_SchdTimeLSIter schedtimeIter;
  schedtimeIter = aGraph.create_new_schdtime();
  (*schedtimeIter).m_taskP = task;
  (*schedtimeIter).m_start = start;
  (*schedtimeIter).m_finish = finish - 1;
  (*schedtimeIter).m_fncblkP = funcblk;
  AX_TaskRefLSIter tref;
  tref = funcblk->add_new_taskref(task);
  (*tref).m_start = start;
  (*tref).m_finish = finish - 1;
  funcblk->sort_taskref_by_start();
  if (funcblk->get_type() == SOFTWARE)
    {
      AX_FuncBlkPLCIter depblk;
      for (depblk = funcblk->get2_first_dep();
	   depblk != funcblk->dep_list_end(); depblk++)
	{
	  if ((*depblk)->get_functype() == PROCESSOR)
	    break;
	}
      tref = (*depblk)->add_new_taskref(task);
      (*tref).m_start = start;
      (*tref).m_finish = finish - 1;
      (*depblk)->sort_taskref_by_start();
    } // end if funcblk

  // Now schedule the output connections connected to this task
  AX_TaskPortPLCIter port;
  for (port = task->get2_first_outport();
       port != task->outport_list_end(); port++)
    {
      assign_bus((*port), aGraph, finish);
    } // end for port

  return(start);
}

// ------------------------------------------------------------ 
// ----------------------------------------------------------
bool AX_ScheduleC::sched_itrblk(AX_TaskGraphCP tGraphP, 
				    AX_ArchGraphC & aGraph, 
				    AX_TaskPLC & taskPL, 
				    AX_TaskPLC & ready_taskPL, 
				    AX_TaskPLC & sink_taskPL, 
				    int ii)
{
  // First determine the chunk of time needed by the iteration block.
  int earliest_start = 0;
  int cyc_delay = 0;
  AX_TaskPLCIter taskP;
  for (taskP = taskPL.begin(); taskP != taskPL.end(); taskP++)
    {
      int pred_finish;
      pred_finish = all_pred_scheduled_with_io((*taskP), aGraph);
      if (pred_finish > earliest_start)
	earliest_start = pred_finish;
      cyc_delay += get_task_latency((*taskP), aGraph);
    }

  taskP = taskPL.begin();
  AX_IterBlkSP itrblk;
  itrblk = (*taskP)->get_iterblk();

  for (taskP = taskPL.begin(); taskP != taskPL.end(); taskP++)
    {
      AX_TaskPLCIter task1;
      for (task1 = taskPL.begin(); task1 != taskPL.end(); task1++)
	{
	  int sample_size;
	  sample_size = (*taskP)->get_max_sample_to_succ((*task1));
	  if (sample_size < 1)
	    continue;
	  float x;
	  x = sample_size * 1000 / (BUS_CLK_RATE * BUS_WIDTH);
	  cyc_delay = cyc_delay + (int) (ceil(x));
	}
    }

  int iter_delay;
  iter_delay = cyc_delay + (itrblk->m_itercnt - 1) * ii;

  // Now create a list of SW components used by the tasks in the iteration blk.
  AX_FuncBlkPLC funcPL;
  for (taskP = taskPL.begin(); taskP != taskPL.end(); taskP++)
    {
      AX_FuncBlkCP funcblk;
      funcblk = aGraph.find_fncblk_by_task((*taskP));
      if (funcblk->get_type() == SOFTWARE)
	funcPL.push_back(funcblk);
    }

  earliest_start++;
  int repeatflag;
  repeatflag = 1;
  while (repeatflag)
    {
      repeatflag = 0;
      AX_FuncBlkPLCIter func_iter;
      for (func_iter = funcPL.begin(); func_iter != funcPL.end();
	   func_iter++)
	{
	  int i;
	  i = is_func_free((*func_iter), earliest_start, iter_delay);
	  if (i > -1)
	    {
	      earliest_start = i;
	      repeatflag = 1;
	      break;
	    }
	} // end for
    } // end while repeatflag

  // Now assign the tasks to their respective components.
  for (taskP = taskPL.begin(); taskP != taskPL.end(); taskP++)
    {
      assign_task_2_comp_itrblk(aGraph, (*taskP), earliest_start, 
				earliest_start + iter_delay);
    }

  // Add all succs of sink tasks to ready list
  for (taskP = sink_taskPL.begin(); taskP != sink_taskPL.end(); taskP++)
    {
      AX_TaskPLCIter succ;
      for (succ = (*taskP)->get2_first_succ();
	   succ != (*taskP)->succ_list_end(); succ++)
	{
	  if (all_pred_scheduled((*succ), aGraph) != -1)
	    ready_taskPL.push_back((*succ));
	}
    }

  return(true);
}

// ------------------------------------------------------------
// ------------------------------------------------------------ 
bool AX_ScheduleC::schedule_iterblk(AX_TaskPLC & ready_taskPL,
					AX_TaskGraphCP tGraphP, 
					AX_ArchGraphC & aGraph,
					int itrBlkIdx)
{
  if (is_iterblk_ready(itrBlkIdx, tGraphP, aGraph) == false)
    {
      AX_TaskPLCIter taskitr;
      taskitr = ready_taskPL.begin();
      ready_taskPL.erase(taskitr);
      return(true);
    }
  AX_TaskPLC root_taskPL;
  AX_TaskPLC sink_taskPL;
  identify_root_sink_itrblk(tGraphP, root_taskPL, sink_taskPL, itrBlkIdx);

  // Create a list of instructions in the iteration block 
  // which are to be scheduled.
  AX_TaskPLC taskPL;
  AX_TaskLCIter taskIter;
  for (taskIter = tGraphP->get2_first_task(); 
       taskIter != tGraphP->task_list_end(); taskIter++)
    {
      if ((*taskIter).get_temp() != itrBlkIdx)
	continue;
      taskPL.push_back(&(*taskIter));
      (*taskIter).set_asap(-1);
    }

  // Compute asap values of all tasks.
  compute_asap_alap(tGraphP, aGraph, taskPL, root_taskPL, sink_taskPL,
		    itrBlkIdx);

  // Sort tasks by their alap values.
  sort_tasks_by_alap(tGraphP, taskPL);

  // For each cycle in the task graph, compute the min_ii and
  // cycle latency.
  int min_ii;
  min_ii = start_cycle_dfs_itrblk(tGraphP, aGraph, itrBlkIdx);

  // Comput determine limits of max_ii and the step size.
  int max_ii;
  int step_size;
  max_ii = 1;
  step_size = LARGEINTEGER;
  int max_delay; // max delay of any task
  max_delay = 0;
  determine_ii_limits(tGraphP, aGraph, max_ii, step_size, max_delay, 
		      itrBlkIdx);

  if (max_delay > min_ii)
    min_ii = max_delay;
  if (max_ii < min_ii)
    max_ii = min_ii + 1;
  int i;
  for (i = min_ii; i <= max_ii; i = i + step_size)
    {
      if (sched_itrblk(tGraphP, aGraph, taskPL, ready_taskPL, 
			sink_taskPL, i) == true)
	{
//	  update_finish_times(aGraph, taskPL, sink_taskPL, 
//			      i,  (*itrBlkIter).m_itercnt);
	  break;
	}
      aGraph.clean_taskref_in_proc();
    } // end for i

  return(true);
}

// ------------------------------------------------------------
// ------------------------------------------------------------ 
bool AX_ScheduleC::schedule_task(AX_TaskPLC & ready_taskPL,
				     AX_TaskGraphCP tGraphP, 
				     AX_ArchGraphC & aGraph,
				     AX_TaskC * task)

{
  // if sched already exists, then error
  AX_SchdTimeLSIter schedtimeIter;
  schedtimeIter = aGraph.get_schdtime_by_task(task);
  if (schedtimeIter != aGraph.schdtime_list_end())
    {
#if DEBUG >= 5
      cerr << "\n\n\n ERROR.........Sched already exists\n\n";
      cerr << "schedule_task-0 task: " << task->get_name() << endl;
      AX_SchdTimeLSIter scIter;
      aGraph.schdtimes_dump();
#endif
      AX_TaskPLCIter taskitr;
      taskitr = ready_taskPL.begin();
      ready_taskPL.erase(taskitr);
      return(true);
    }

  if (task->get_iterblk() != NULL)
    {
      schedule_iterblk(ready_taskPL, tGraphP, aGraph, task->get_temp());
      return(true);
    }
  int pred_finish;
  pred_finish = all_pred_scheduled_with_io(task, aGraph);
  if (pred_finish == -1)
    return(false);

  int start_time;
  start_time = assign_task_2_comp(aGraph, task, pred_finish);
  if (start_time == -1)
    return(false);

  update_ready_task(task, ready_taskPL, aGraph);
  return(true);
}

// ------------------------------------------------------------
// ------------------------------------------------------------ 
void AX_ScheduleC::check_schedule(AX_ArchGraphC & aGraph)
{
  // check for overlap schedule.
  AX_FuncBlkLCIter func;
  for (func = aGraph.get2_first_fncblk();
       func != aGraph.fncblk_list_end(); func++)
    {
      (*func).sort_taskref_by_start();
      int prev_finish;
      prev_finish = -1;
      int prev_index;
      prev_index = -1;
      AX_TaskRefLSIter taskrefIter;
      for (taskrefIter = (*func).get2_first_taskref();
	   taskrefIter != (*func).taskref_list_end(); taskrefIter++)
	  {
	    AX_TaskC * task;
	    task = (*taskrefIter).m_taskP;
	    if ((*taskrefIter).m_start < prev_finish)
	      {
		if (prev_index == 0 || task->get_temp() != prev_index)
		  {
                    cerr << endl << aGraph.get_name()
		         << ": Error in scheduling\n\n";
		  } // end prev_index
	      } // end if 
	    prev_finish = (*taskrefIter).m_finish;
	    prev_index = task->get_temp();
	  } // end for
    } // end for func

  AX_BusLCIter busI;
  for (busI = aGraph.get2_first_bus();
       busI != aGraph.bus_list_end(); busI++)
    {
       AX_ConnRefLSIter conrefI;
       for (conrefI = (*busI).get2_first_connref();
            conrefI != (*busI).connref_list_end(); conrefI++)
       {
          AX_ConnRefLSIter srchI;
          for (srchI = (*busI).get2_first_connref();
               srchI != (*busI).connref_list_end(); srchI++)
          {
             if ((*conrefI).m_connP == (*srchI).m_connP)
                continue;
             if ((*conrefI).m_start == (*srchI).m_start)
             {
                if ((*conrefI).m_connP->get_src_task() !=
                    (*srchI).m_connP->get_src_task())
                {
                   cerr << endl << aGraph.get_name()
		        << ": Error in bus scheduling\n";
                   if ((*conrefI).m_connP->get_src_tg() != NULL)
                      cerr << "\t" << aGraph.get_name();
                   else
                      cerr << "\t"
                    << (*conrefI).m_connP->get_src_task()->get_name();
                   cerr << "."
                     << (*conrefI).m_connP->get_src_port()->get_name()
                        << " -> ";
                   if ((*conrefI).m_connP->get_sink_tg() != NULL)
                      cerr << "\t" << aGraph.get_name();
                   else
                      cerr << "\t"
                    << (*conrefI).m_connP->get_sink_task()->get_name();
                   cerr << "."
                     << (*conrefI).m_connP->get_sink_port()->get_name()
                        << ": [" << (*conrefI).m_start << ","
                        << (*conrefI).m_finish << "]" << endl;

                   if ((*srchI).m_connP->get_src_tg() != NULL)
                      cerr << "\t" << aGraph.get_name();
                   else
                      cerr << "\t"
                      << (*srchI).m_connP->get_src_task()->get_name();
                   cerr << "."
                        << (*srchI).m_connP->get_src_port()->get_name()
                        << " -> ";
                   if ((*srchI).m_connP->get_sink_tg() != NULL)
                      cerr << "\t" << aGraph.get_name();
                   else
                      cerr << "\t"
                     << (*srchI).m_connP->get_sink_task()->get_name();
                   cerr << "."
                     << (*srchI).m_connP->get_sink_port()->get_name()
                        << ": [" << (*srchI).m_start << ","
                        << (*srchI).m_finish << "]" << endl;
                }
             }
          } // end inner connref loop
       } // end outer connref loop
    } // end bus loop
}

// ----------------------------------------------------------
// ----------------------------------------------------------
void AX_ScheduleC::reset_task_temp(AX_TaskGraphCP tGraphP)
{
  // Initialize all tasks temp to 0;
  AX_TaskLCIter taskIter;
  for (taskIter = tGraphP->get2_first_task(); 
       taskIter != tGraphP->task_list_end(); taskIter++)
    {
      (*taskIter).set_temp(-1);
    }
}

// ------------------------------------------------------------
// ------------------------------------------------------------ 
void AX_ScheduleC::sched_main(AX_DsnC & dsn, 
			       AX_ArchGraphC & aGraph)
{
  AX_TaskGraphCP tGraphP;
  tGraphP = dsn.get_tg();
  
  // initialize arch graph and func blocks
  aGraph.remove_all_schdtimes();
  aGraph.clean_taskref_in_proc();
  aGraph.clean_all_bus_connrefs();

  // Determine back succs and back pred for all tasks in the TG.
  determine_back_succ_pred(tGraphP);

#if DEBUG >= 4
  cerr << "\n\n>>>>>>>>>>>>>>>Before scheduling.\n";
  //tGraphP->dump_simple();
  //aGraph.dump_simple();
#endif

  // Initialize all tasks temp
  set_task_temp(tGraphP);

#if DEBUG >= 4
  cerr << ">>>>>>>>>>>>>>>After set_task_temp().\n";
  //tGraphP->dump_simple();
  //aGraph.dump_simple();
#endif

  // Identify root and ready tasks
  AX_TaskPLC root_taskPL;
  AX_TaskPLC ready_taskPL;
  identify_root(tGraphP, root_taskPL, ready_taskPL);

#if DEBUG >= 4
  cerr << ">>>>>>>>>>>>>>>After identify_root().\n";
  AX_TaskPLCIter tpIter;
  cerr << "  Ready task list:" << endl;
  for (tpIter = ready_taskPL.begin();
       tpIter != ready_taskPL.end(); tpIter++)
     cerr <<  "    Task: " << (*tpIter)->get_name() << endl;
#endif

  // schedule input ports to the task graph
  schedule_task_inports(tGraphP, aGraph);

#if DEBUG >= 4
  cerr << ">>>>>>>>>>>>>>>After schedule_task_inports().\n";
#endif

  while (ready_taskPL.begin() != ready_taskPL.end())
    {
      schedule_task(ready_taskPL, tGraphP, aGraph, 
		    (*ready_taskPL.begin()));
#if DEBUG >= 4
      cerr << ">>>>>>>>>>>>>>>Doing schedule_task().\n";
      AX_TaskPLCIter tpIter;
      cerr << "  Ready task list:" << endl;
      for (tpIter = ready_taskPL.begin();
           tpIter != ready_taskPL.end(); tpIter++)
         cerr <<  "    Task: " << (*tpIter)->get_name() << endl;
#endif
    } // end while ready_taskPL

#if DEBUG >= 4
  cerr << "\n\n>>>>>>>>>>>>>>>After scheduling.\n";
  aGraph.dump_small();
  cerr << "\n\n>>>>>>>>>>>>>>>Done\n";
#endif

  // check_schedule(aGraph);

  // Initialize all tasks temp to -1;
  reset_task_temp(tGraphP);

  aGraph.clean_taskref_in_proc();
}
