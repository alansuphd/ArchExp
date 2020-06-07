
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// evolve.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <iostream>
#include <math.h>
#include "evolve.h"
#include "extern.h"
using namespace std;

// ------------------------------------------------------------
void find_special_err(AX_TaskGraphC & tg,
                             AX_ArchGraphC & ag, string posStr)
{
   AX_CompPortLCIter portI;
   for (portI = ag.get2_first_compport();
        portI != ag.compport_list_end(); portI++)
   {
      if ((*portI).get_comp() != NULL)
      {
         AX_FuncBlkCP procP;
         procP = AX_FuncBlkCP((*portI).get_comp());
         if (procP->get_functype() != PROCESSOR &&
             procP->get_functype() != MEMORY &&
             procP->get_functype() != MEMORY_CORE)
            continue;
         AX_CompPortPLCIter portPI;
         for (portPI = procP->get2_first_ioport();
              portPI != procP->ioport_list_end(); portPI++)
         {
            int refCnt = 0;
            AX_CompPortLCIter swPortI;
            for (swPortI = ag.get2_first_compport();
                 swPortI != ag.compport_list_end(); swPortI++)
            {
               if ((*swPortI).get_sw_port() == (*portPI))
                  refCnt++;
            }
            if (refCnt != (*portPI)->get_refCnt())
            {
               cerr << "Goes wrong after " << posStr << endl;
               cerr << ag.get_name() << ": "
                    << procP->get_name() << "."
                    << (*portPI)->get_name()
                    << "'s refCnt " << (*portPI)->get_refCnt()
                    << " does not match real refCnt " << refCnt << endl;
            }
         }
      }
   }
}

void find_err(AX_TaskGraphC & tg, AX_ArchGraphC & ag, string posStr)
{
   // check tasks
   AX_TaskLCIter taskI;
   AX_FuncBlkLCIter funcI;
#if DEBUG >= 3
   cerr << "Checking " << ag.get_name() << endl;
#endif
   for (taskI = tg.get2_first_task();
        taskI != tg.task_list_end(); taskI++)
   {
      AX_FuncBlkCP firstFuncP = NULL;;
      bool found = false, first = false;
      for (funcI = ag.get2_first_fncblk();
           funcI != ag.fncblk_list_end(); funcI++)
      {
	 if ((*funcI).get_functype() == PROCESSOR)
	    continue;
         if ((*funcI).is_mapped_task(&(*taskI)))
         {
            if (found && first)
            {
               cerr << "Goes wrong after " << posStr << endl;
               cerr << ag.get_name() << ": "
                    << (*taskI).get_name()
                    << " maps to :"
                    << firstFuncP->get_name()
                    << "," << (*funcI).get_name();
               first = false;
               continue;
            }
            if (found && !first)
            {
               cerr << (*funcI).get_name();
               continue;
            }
            found = first = true;
            firstFuncP = &(*funcI);
         } // end if is_mapped_task
      } // end fncblk for
      if (found && !first)
         cerr << endl;
      if (!found)
      {
          cerr << ag.get_name() << ": "
               << "Cannot find mapping for "
               << (*taskI).get_name() << endl;
      }
   } // end task for

   // check conns
   AX_ConnLCIter connI;
   for (connI = tg.get2_first_conn();
        connI != tg.conn_list_end(); connI++)
   {
      AX_BusLCIter busI;
      AX_BusCP firstBusP = NULL;;
      bool found = false, first = false;
      for (busI = ag.get2_first_bus();
           busI != ag.bus_list_end(); busI++)
      {
         //AX_ConnPntCP connPntP;
         if ((*busI).is_mapped_conn(&(*connI)))
         {
            if (found && first)
            {
               cerr << "Goes wrong after " << posStr << endl;
               cerr << ag.get_name() << ": ";
               if ((*connI).get_src_tg() != NULL)
                  cerr << (*connI).get_src_tg()->get_name();
               else
                  cerr << (*connI).get_src_task()->get_name();
               cerr << "."
                    << (*connI).get_src_port()->get_name();
               cerr << "->";
               if ((*connI).get_sink_tg() != NULL)
                  cerr << (*connI).get_sink_tg()->get_name();
               else
                  cerr << (*connI).get_sink_task()->get_name();
               cerr << "."
                    << (*connI).get_sink_port()->get_name()
                    << " maps to :" << endl;
               firstBusP->dump_simple();
               (*busI).dump_simple();
               first = false;
               continue;
            }
            if (found && !first)
            {
               (*busI).dump_simple();
               continue;
            }
            found = first = true;
            firstBusP = &(*busI);
         } // end if is_mapped_conn
      } // end bus for
      if (!found)
      {
         if ((*connI).get_src_task() != NULL &&
             (*connI).get_sink_task() != NULL)
         {
            // source and sink tasks run on the same component,
            // so there is no connection mapping, Not an error
            AX_FuncBlkCP srcFuncP, sinkFuncP;
            AX_TaskCP srcTaskP, sinkTaskP;
            srcTaskP = (*connI).get_src_task();
            sinkTaskP = (*connI).get_sink_task();
            srcFuncP = ag.get_fncblk_by_task(srcTaskP);
            sinkFuncP = ag.get_fncblk_by_task(sinkTaskP);
            if (srcFuncP == sinkFuncP)
               continue;
            AX_FuncBlkPLCIter srcPI, sinkPI;
            for (srcPI = srcFuncP->get2_first_dep();
                 srcPI != srcFuncP->dep_list_end(); srcPI++)
               if ((*srcPI)->get_functype() == PROCESSOR)
                  break;
            for (sinkPI = sinkFuncP->get2_first_dep();
                 sinkPI != sinkFuncP->dep_list_end(); sinkPI++)
               if ((*sinkPI)->get_functype() == PROCESSOR)
                  break;
            if (srcPI != srcFuncP->dep_list_end() &&
                sinkPI != sinkFuncP->dep_list_end() &&
                (*srcPI) == (*sinkPI))
               continue;
         }
         cerr << "Goes wrong after " << posStr << endl;
         cerr << ag.get_name() << ": "
              << "Cannot find mapping for ";
         if ((*connI).get_src_tg() != NULL)
            cerr << (*connI).get_src_tg()->get_name();
         else
            cerr << (*connI).get_src_task()->get_name();
         cerr << "."
              << (*connI).get_src_port()->get_name();
         cerr << "->";
         if ((*connI).get_sink_tg() != NULL)
            cerr << (*connI).get_sink_tg()->get_name();
         else
            cerr << (*connI).get_sink_task()->get_name();
         cerr << "."
              << (*connI).get_sink_port()->get_name()
              << endl;
         continue;
      }
      AX_BusCP fBusP = ag.get_bus_by_conn(&(*connI));
      if ((*connI).get_src_task() != NULL)
      {
         AX_FuncBlkCP fFuncP;
         AX_CompPortCP fPortP, sPortP;
         string pName = (*connI).get_src_port()->get_name();
         fFuncP = ag.get_fncblk_by_task((*connI).get_src_task());
         fPortP = fFuncP->get_outport(pName);
         if (fPortP == NULL)
         {
            cerr << "Goes wrong after " << posStr << endl;
            cerr << ag.get_name() << ": "
                 << " cannot find src port " << pName
                 << " in " << fFuncP->get_name();
         }
         if ((sPortP = fPortP->get_sw_port()) != NULL)
         {
            fPortP = sPortP;
            fFuncP = (AX_FuncBlkC *)sPortP->get_comp();
         }
         bool found = false;
         AX_ConnPntLCIter conpntI;
         for (conpntI = fBusP->get2_first_connpnt();
              conpntI != fBusP->connpnt_list_end(); conpntI++)
         {
            if ((*conpntI).get_comp() == fFuncP &&
                (*conpntI).get_port() == fPortP)
            {
               found = true;
               break;
            }
         }
         if (!found)
         {
            cerr << "Goes wrong after " << posStr << endl;
            cerr << ag.get_name() << ":"
                 << " cannot find matching connpnt "
                 << fFuncP->get_name() << "."
                 << fPortP->get_name() << " on bus "
                 << fBusP->get_id() << " for "
                 << (*connI).get_src_task()->get_name()
                 << "." << pName << endl;
         }
      }
      if ((*connI).get_sink_task() != NULL)
      {
         AX_FuncBlkCP fFuncP;
         AX_CompPortCP fPortP, sPortP;
         string pName = (*connI).get_sink_port()->get_name();
         fFuncP = ag.get_fncblk_by_task((*connI).get_sink_task());
         fPortP = fFuncP->get_inport(pName);
         if (fPortP == NULL)
         {
            cerr << "Goes wrong after " << posStr << endl;
            cerr << ag.get_name() << ": "
                 << " cannot find sink port " << pName
                 << " in " << fFuncP->get_name() << endl;
         }
         if ((sPortP = fPortP->get_sw_port()) != NULL)
         {
            fPortP = sPortP;
            fFuncP = (AX_FuncBlkC *)sPortP->get_comp();
         }
         bool found = false;
         AX_ConnPntLCIter conpntI;
         for (conpntI = fBusP->get2_first_connpnt();
              conpntI != fBusP->connpnt_list_end(); conpntI++)
         {
            if ((*conpntI).get_comp() == fFuncP &&
                (*conpntI).get_port() == fPortP)
            {
               found = true;
               break;
            }
         }
         if (!found)
         {
            cerr << "Goes wrong after " << posStr << endl;
            cerr << ag.get_name() << ": "
                 << " cannot find matching connpnt "
                 << fFuncP->get_name() << "."
                 << fPortP->get_name() << " on bus "
                 << fBusP->get_id() << " for "
                 << (*connI).get_sink_task()->get_name()
                 << "." << pName << endl;
         }
      }
   } // end conn for

   // check for one or none connection buses
   AX_BusLCIter busI;
   for (busI = ag.get2_first_bus();
        busI != ag.bus_list_end(); busI++)
   {
      if ((*busI).get_no_connpnts() <= 1)
      {
         cerr << "Goes wrong after " << posStr << endl;
         cerr << ag.get_name()
              << ": Useless bus found:\n";
         (*busI).dump();
      }
      if ((*busI).get_intrfc()->get_intrtype() != INTRFC_WIRE &&
          (*busI).get_intrfc()->get_intrtype() != INTRFC_BUS)
      {
         cerr << "Goes wrong after " << posStr << endl;
         cerr << ag.get_name()
              << ": Incorrect interface type found:\n";
         (*busI).dump();
      }
      AX_ConnPntLCIter conpntI;
      for (conpntI = (*busI).get2_first_connpnt();
           conpntI != (*busI).connpnt_list_end(); conpntI++)
      {
         bool found = false;
         AX_BusPLCIter srBusPI;
         AX_CompPortCP srPortP = (*conpntI).get_port();
         for (srBusPI = srPortP->get2_first_bus();
              srBusPI != srPortP->bus_list_end(); srBusPI++)
            if ((*srBusPI) == &(*busI))
            {
               found = true;
               break;
            }
         if (!found)
         {
            cerr << "Goes wrong after " << posStr << endl;
            cerr << ag.get_name()
                 << ": bus " << (*busI).get_id()
                 << " has a connpnt ";
            if ((*conpntI).get_ag() != NULL)
               cerr << (*conpntI).get_ag()->get_name();
            else
               cerr << (*conpntI).get_comp()->get_name();
            cerr << "." << (*conpntI).get_port()->get_name();
            cerr << " that the port does not contain the bus"
                 << endl;
         }
      }
      for (conpntI = (*busI).get2_first_connpnt();
           conpntI != (*busI).get2_last_connpnt(); conpntI++)
      {
         AX_ConnPntLCIter nxtPntI, conpntI1;
         conpntI1 = conpntI;
         conpntI1++;
         for (nxtPntI = conpntI1;
              nxtPntI != (*busI).connpnt_list_end(); nxtPntI++)
            if ((*nxtPntI).get_ag() == (*conpntI).get_ag() &&
                (*nxtPntI).get_comp() == (*conpntI).get_comp() &&
                (*nxtPntI).get_port() == (*conpntI).get_port())
            {
               cerr << "Goes wrong after " << posStr << endl;
               cerr << ag.get_name()
                    << ": bus " << (*busI).get_id()
                    << " has duplicate connpnts ";
               if ((*conpntI).get_ag() != NULL)
                  cerr << (*conpntI).get_ag()->get_name();
               else
                  cerr << (*conpntI).get_comp()->get_name();
               cerr << "." << (*conpntI).get_port()->get_name()
                    << endl;
            }
      }
   } // end checking useless buses

   // checking compport's bus list
   AX_CompPortLCIter portI;
   for (portI = ag.get2_first_compport();
        portI != ag.compport_list_end(); portI++)
   {
      AX_BusPLCIter busPI;
      bool errPrted = false;
      for (busPI = (*portI).get2_first_bus();
           busPI != (*portI).bus_list_end(); busPI++)
      {
         if ((*busPI)->get_intrfc()->get_intrtype() != INTRFC_WIRE &&
             (*busPI)->get_intrfc()->get_intrtype() != INTRFC_BUS)
         {
            cerr << "Goes wrong after " << posStr << endl;
            cerr << ag.get_name()
                 << ": Incorrect intrtype found in port ";
            if ((*portI).get_ag() != NULL)
               cerr << (*portI).get_ag()->get_name();
            else
               cerr << (*portI).get_comp()->get_name();
            cerr << "." << (*portI).get_name() << endl;
         }
         if ((*busPI)->get_intrfc()->get_intrtype() == INTRFC_BUS &&
             (*portI).get_no_buses() > 1 && !errPrted)
         {
            cerr << "Goes wrong after " << posStr << endl;
            cerr << ag.get_name()
                 << ": multi-bus: ";
            for (int i = 0; i < (*portI).get_no_buses(); i++)
               cerr << (*portI).get_bus(i)->get_intrfc()->get_name()
                    << " ";
            cerr << endl;
            errPrted = true;
         }

         AX_InterfaceLCIter intrfcI;
         bool found = false;
         for (intrfcI = ag.get2_first_intrfc();
              intrfcI != ag.intrfc_list_end(); intrfcI++)
         {
            if ((*busPI)->get_intrfc() == &(*intrfcI))
            {
               found = true;
               break;
            }
         }
         if (!found)
         {
            AX_FuncBlkCP fncblkP;
            fncblkP = (AX_FuncBlkC *)(*portI).get_comp();
            cerr << "Goes wrong after " << posStr << endl;
            cerr << ag.get_name() << ": ";
            if (fncblkP == NULL)
               cerr << ag.get_name();
            else
               cerr << fncblkP->get_name();
            cerr << "." << (*portI).get_name()
                 << "'s bus " << (*busPI)->get_id()
                 << "'s interface "
                 << (*busPI)->get_intrfc()->get_name()
                 << " does not belong to this graph\n" << endl;
         }

         found = false;
         AX_ConnPntLCIter conpntI;
         for (conpntI = (*busPI)->get2_first_connpnt();
              conpntI != (*busPI)->connpnt_list_end(); conpntI++)
         {
            if ((*conpntI).get_port() == &(*portI))
            {
               found = true;
               break;
            }
         }
         if (!found)
         {
            cerr << "Goes wrong after " << posStr << endl;
            cerr << ag.get_name() << ": ";
            if ((*portI).get_ag() != NULL)
               cerr << (*portI).get_ag()->get_name();
            else
               cerr << (*portI).get_comp()->get_name();
            cerr << "." << (*portI).get_name()
                 << " ref to bus " << (*busPI)->get_id()
                 << " that does not have connpnt to it"
                 << endl;
         }
      }
      if ((*portI).get_comp() == NULL)
         continue;
      AX_FuncBlkCP checkP = (AX_FuncBlkCP)(*portI).get_comp();
      AX_TaskRefLSIter taskrefI;
      for (taskrefI = checkP->get2_first_taskref();
           taskrefI != checkP->taskref_list_end(); taskrefI++)
      {
         if ((*taskrefI).m_taskP == NULL)
         {
            cerr << "Goes wrong after " << posStr << endl;
            cerr << ag.get_name()
                 << ": Incorrect taskref in "
                 << checkP->get_name() << "."
                 << (*portI).get_name() << endl;
         }
      }
      if (AX_FuncBlkCP((*portI).get_comp())->get_type() == SOFTWARE)
         continue;
      if ((*portI).get_no_intrfcs() == 0)
      {
         cerr << "Goes wrong after " << posStr << endl;
         cerr << ag.get_name()
              << ": No interface on "
              << (*portI).get_comp()->get_name()
              << "." << (*portI).get_name() << endl;
      }

      // checking the refCnt
      AX_FuncBlkCP procP;
      procP = AX_FuncBlkCP((*portI).get_comp());
      if (procP->get_functype() != PROCESSOR &&
          procP->get_functype() != MEMORY &&
          procP->get_functype() != MEMORY_CORE)
         continue;
      AX_CompPortPLCIter portPI;
      for (portPI = procP->get2_first_ioport();
           portPI != procP->ioport_list_end(); portPI++)
      {
         int refCnt = 0;
         AX_CompPortLCIter swPortI;
         for (swPortI = ag.get2_first_compport();
              swPortI != ag.compport_list_end(); swPortI++)
         {
            if ((*swPortI).get_sw_port() == (*portPI))
               refCnt++;
         }
         if (refCnt != (*portPI)->get_refCnt())
         {
            cerr << "Goes wrong after " << posStr << endl;
            cerr << ag.get_name() << ": "
                 << procP->get_name() << "."
                 << (*portPI)->get_name()
                 << "'s refCnt " << (*portPI)->get_refCnt()
                 << " does not match real refCnt " << refCnt << endl;
         }
      }
   }

   // check the fncblkPL depPL and refPL links
   for (funcI = ag.get2_first_fncblk();
        funcI != ag.fncblk_list_end(); funcI++)
   {
      AX_FuncBlkPLCIter funcPI;
      if ((*funcI).get_functype() != PROCESSOR)
         continue;
      if ((*funcI).get_no_deps() > 1)
      {
         cerr << "Goes wrong after " << posStr << endl;
         cerr << ag.get_name() << ": ";
         cerr << (*funcI).get_name()
              << " has more than one entry in depPL\n";
      }
      if ((*funcI).get_no_refs() <= 0)
      {
         cerr << "Goes wrong after " << posStr << endl;
         cerr << ag.get_name() << ": ";
         cerr << (*funcI).get_name()
              << " has no fncblk in refPL\n";
      }
      AX_FuncBlkPLCIter refFunPI;
      AX_FuncBlkLCIter tgtFunI;
      for (refFunPI = (*funcI).get2_first_ref();
           refFunPI != (*funcI).ref_list_end(); refFunPI++)
      {
         for (tgtFunI = ag.get2_first_fncblk();
              tgtFunI != ag.fncblk_list_end(); tgtFunI++)
         {
            if ((*refFunPI) == &(*tgtFunI))
               break;
         }
         if (tgtFunI == ag.fncblk_list_end())
         {
            cerr << "Goes wrong after " << posStr << endl;
            cerr << ag.get_name() << ": ";
            cerr << (*funcI).get_name()
                 << " has wrong fncblk in refPL\n";
         }
      }
      for (funcPI = (*funcI).get2_first_dep();
           funcPI != (*funcI).dep_list_end(); funcPI++)
      {
         if ((*funcPI)->get_functype() != MEMORY &&
             (*funcPI)->get_functype() != MEMORY_CORE)
         {
            cerr << "Goes wrong after " << posStr << endl;
            cerr << ag.get_name() << ": ";
            cerr << (*funcI).get_name()
                 << " does not have memory in depPL\n";
         }
         if ((*funcPI)->get_functype() == MEMORY ||
             (*funcPI)->get_functype() == MEMORY_CORE) {
            AX_FuncBlkPLCIter depPI;
            bool depFlag = false;
            for (depPI = (*funcPI)->get2_first_dep();
                 depPI != (*funcPI)->dep_list_end(); depPI++) {
               if ((*depPI) == &(*funcI)) {
                  depFlag = true;
               }
            }
            if (!depFlag) {
               cerr << "Goes wrong after " << posStr << endl;
               cerr << ag.get_name() << ": ";
               cerr << (*funcI).get_name()
                    << " has "
                    << (*depPI)->get_name()
                    << " in depPL but not vise versa\n";
            }
         }
      }
   }
   for (funcI = ag.get2_first_fncblk();
        funcI != ag.fncblk_list_end(); funcI++)
   {
      AX_FuncBlkPLCIter funcPI;
      if ((*funcI).get_functype() != MEMORY &&
          (*funcI).get_functype() != MEMORY_CORE)
         continue;
      if ((*funcI).get_no_deps() > 1)
      {
         cerr << "Goes wrong after " << posStr << endl;
         cerr << ag.get_name() << ": ";
         cerr << (*funcI).get_name()
              << " has more than one entry in depPL\n";
      }
      if ((*funcI).get_no_refs() <= 0)
      {
         cerr << "Goes wrong after " << posStr << endl;
         cerr << ag.get_name() << ": ";
         cerr << (*funcI).get_name()
              << " has no fncblk in refPL\n";
      }
      for (funcPI = (*funcI).get2_first_dep();
           funcPI != (*funcI).dep_list_end(); funcPI++)
      {
         if ((*funcPI)->get_functype() != PROCESSOR)
         {
            cerr << "Goes wrong after " << posStr << endl;
            cerr << ag.get_name() << ": ";
            cerr << (*funcI).get_name()
                 << " does not have processor in depPL\n";
         }
         if ((*funcPI)->get_functype() == PROCESSOR)
         {
            AX_FuncBlkPLCIter depPI;
            for (depPI = (*funcPI)->get2_first_dep();
                 depPI != (*funcPI)->dep_list_end(); depPI++)
               if ((*depPI) != &(*funcI))
               {
                  cerr << "Goes wrong after " << posStr << endl;
                  cerr << ag.get_name() << ": ";
                  cerr << (*funcI).get_name()
                       << " has "
                       << (*depPI)->get_name()
                       << " in depPL but not vise versa\n";
               }
         }
      }
   }
   for (funcI = ag.get2_first_fncblk();
        funcI != ag.fncblk_list_end(); funcI++)
   {
      AX_FuncBlkPLCIter funcPI;
      if ((*funcI).get_functype() != FUNCTION)
         continue;
      if ((*funcI).get_no_taskrefs() <= 0)
      {
         cerr << "Goes wrong after " << posStr << endl;
         cerr << ag.get_name() << ": ";
         cerr << (*funcI).get_name()
              << " has no task ref to it\n";
      }
      if ((*funcI).get_type() == SOFTWARE)
         if ((*funcI).get_no_deps() <= 0)
         {
            cerr << "Goes wrong after " << posStr << endl;
            cerr << ag.get_name() << ": ";
            cerr << (*funcI).get_name()
                 << " has no proc/mem in depPL\n";
         }
      for (funcPI = (*funcI).get2_first_dep();
           funcPI != (*funcI).dep_list_end(); funcPI++)
      {
         bool found = false;
         AX_FuncBlkPLCIter refPI;
         for (refPI = (*funcPI)->get2_first_ref();
              refPI != (*funcPI)->ref_list_end(); refPI++)
         {
            if ((*refPI) == &(*funcI))
            {
               found = true;
               break;
            }
         }
         if (!found)
         {
            cerr << "Goes wrong after " << posStr << endl;
            cerr << ag.get_name() << ": ";
            cerr << (*funcI).get_name()
                 << " has " << (*funcPI)->get_name()
                 << " in depPL but not vise versa in refPL\n";
         }
      }
   }

   // check port for bus links
   for (funcI = ag.get2_first_fncblk();
        funcI != ag.fncblk_list_end(); funcI++)
   {
      AX_CompPortPLCIter portPI;
      for (portPI = (*funcI).get2_first_inport();
           portPI != (*funcI).inport_list_end(); portPI++)
      {
         AX_BusPLCIter busPI;
         for (busPI = (*portPI)->get2_first_bus();
              busPI != (*portPI)->bus_list_end(); busPI++)
          {
             //AX_InterfaceCP intrfcP = (*busPI)->get_intrfc();
             AX_ConnPntLCIter cpntI;
             bool found = false;
             for (cpntI = (*busPI)->get2_first_connpnt();
                  cpntI != (*busPI)->connpnt_list_end(); cpntI++)
             {
                if ((*cpntI).get_comp() == &(*funcI) &&
                    (*cpntI).get_port() == (*portPI))
                {
                   found = true;
                   break;
                }
             }
             if (!found)
             {
                cerr << posStr << " - ";
                cerr << "Bus ID " << (*busPI)->get_id();
                cerr << " does not have connpnt "
                     << (*funcI).get_name() << "."
                     << (*portPI)->get_name() << endl;
             }
         } // end busPI for
      } // end inport for

      for (portPI = (*funcI).get2_first_outport();
           portPI != (*funcI).outport_list_end(); portPI++)
      {
         AX_BusPLCIter busPI;
         for (busPI = (*portPI)->get2_first_bus();
              busPI != (*portPI)->bus_list_end(); busPI++)
         {
             //AX_InterfaceCP intrfcP = (*busPI)->get_intrfc();
             AX_ConnPntLCIter cpntI;
             bool found = false;
             for (cpntI = (*busPI)->get2_first_connpnt();
                  cpntI != (*busPI)->connpnt_list_end(); cpntI++)
             {
                if ((*cpntI).get_comp() == &(*funcI) &&
                    (*cpntI).get_port() == (*portPI))
                {
                   found = true;
                   break;
                }
             }
             if (!found)
             {
                cerr << posStr << " - ";
                cerr << "Bus ID " << (*busPI)->get_id();
                cerr << " does not have connpnt "
                     << (*funcI).get_name() << "."
                     << (*portPI)->get_name() << endl;
             }
         } // end busPI for
      } // end inport for

      for (portPI = (*funcI).get2_first_ioport();
           portPI != (*funcI).ioport_list_end(); portPI++)
      {
         AX_BusPLCIter busPI;
         for (busPI = (*portPI)->get2_first_bus();
              busPI != (*portPI)->bus_list_end(); busPI++)
         {
             //AX_InterfaceCP intrfcP = (*busPI)->get_intrfc();
             AX_ConnPntLCIter cpntI;
             bool found = false;
             for (cpntI = (*busPI)->get2_first_connpnt();
                  cpntI != (*busPI)->connpnt_list_end(); cpntI++)
             {
                if ((*cpntI).get_comp() == &(*funcI) &&
                    (*cpntI).get_port() == (*portPI))
                {
                   found = true;
                   break;
                }
             }
             if (!found)
             {
                cerr << posStr << " - ";
             cerr << "Bus ID " << (*busPI)->get_id();
             cerr << " does not have connpnt "
                  << (*funcI).get_name() << "."
                  << (*portPI)->get_name() << endl;
             }
         } // end busPI for
      } // end ioport for
   } // end funcI loop
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructor
// ------------------------------------------------------------
  AX_EvolveC::AX_EvolveC()
  { 
  }

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Destructor
// ------------------------------------------------------------
  AX_EvolveC::~AX_EvolveC()
  {
  }
// ------------------------------------------------------------

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Operations
// ------------------------------------------------------------
   bool AX_EvolveC::populate1(AX_DsnC & dsn)
   {
       char buf1[50];
       for (int i = 0; i < POP_SIZE; i++)
       {
          AX_TaskLCIter tIter;
          AX_ArchGraphC nArch;
          AX_ArchGraphLCIter aIter;
          AX_PlatformLCIter pfIter;
          AX_InterfaceLCIter ifIter;

          aIter = dsn.add_ag(nArch);
          my_itoa(i, buf1);
          string aName = dsn.get_name();
          aName += "_Gen1_";
          aName += buf1;
          (*aIter).set_name(aName);
          (*aIter).set_id(i);
          int pfNum = 0, ifNum = 0, tNum = 0, pNum = 0, bNum = 0;

          // 1st pass: get platform basics
          for (pfIter = dsn.get2_first_platfm();
               pfIter != dsn.platfm_list_end(); pfIter++)
          {
              AX_PlatformC nPlatfm;
              AX_PlatformLCIter nPlatfmI;

              nPlatfmI = (*aIter).add_new_platfm(nPlatfm);
              (*nPlatfmI).copyBasic(*pfIter);
              (*nPlatfmI).set_id(pfNum++);
          }
          // 1st pass: get interface basics
          for (ifIter = dsn.get2_first_intrfc();
               ifIter != dsn.intrfc_list_end(); ifIter++)
          {
              AX_InterfaceC nIntrfc;
              AX_InterfaceLCIter nIntrfcI;

              nIntrfcI = (*aIter).add_new_intrfc(nIntrfc);
              (*nIntrfcI).copy(*ifIter);
              (*nIntrfcI).set_platfm(NULL);
              (*nIntrfcI).set_id(ifNum++);
          }

          // 2nd pass: get platform references
          for (pfIter = dsn.get2_first_platfm();
               pfIter != dsn.platfm_list_end(); pfIter++)
          {
             // reconstructing the sub-platform and
             // implement sub-platform
             AX_PlatformPLCIter pfpIter;
             AX_PlatformCP PlatfmP;
             AX_PlatformCP subPlatfmP;

             // get the relative platform for reconstructing
             PlatfmP = (*aIter).get_platfm((*pfIter).get_name());
             // reconstruct the sub-platform
             PlatfmP->remove_all_subpltfms();
             for (pfpIter = (*pfIter).get2_first_subpltfm();
                  pfpIter != (*pfIter).subpltfm_list_end(); pfpIter++)
             {
                subPlatfmP = (*aIter).get_platfm((*pfpIter)->get_name());
                PlatfmP->add_new_subpltfm(subPlatfmP);
             }
             PlatfmP->remove_all_impl_subpltfms();

             // reconstructing the interface and implement interface
             AX_InterfacePLCIter ifpIter;
             AX_InterfaceCP subIntrfcP;

             // reconstruct the interface
             PlatfmP->remove_all_intrfcs();
             for (ifpIter = (*pfIter).get2_first_intrfc();
                  ifpIter != (*pfIter).intrfc_list_end(); ifpIter++)
             {
                subIntrfcP = (*aIter).get_intrfc((*ifpIter)->get_name());
#if DEBUG >= 1
                if (subIntrfcP == NULL)
                   cerr << "\n<<=== (a) Cannot find intrfc "
                        << (*ifpIter)->get_name() << " in "
                        << (*aIter).get_name() << endl;
#endif
                PlatfmP->add_new_intrfc(subIntrfcP);
             }
             PlatfmP->remove_all_impl_intrfcs();
          }

          // 2nd pass: get interface references
          for (ifIter = dsn.get2_first_intrfc();
               ifIter != dsn.intrfc_list_end(); ifIter++)
          {
             AX_InterfaceCP rIntrfcP;
             AX_PlatformCP nPlatfmP, oPlatfmP;
             rIntrfcP = (*aIter).get_intrfc((*ifIter).get_name());
             oPlatfmP = (*ifIter).get_platfm();
#if DEBUG >= 1
             if (oPlatfmP == NULL)
                cerr << "<<======= Interface "
                     << (*ifIter).get_name()
                     << " does not have a platform\n";
#endif
             nPlatfmP = (*aIter).get_platfm(oPlatfmP->get_name());
             rIntrfcP->set_platfm(nPlatfmP);
          }

          // selecte a funcblk for each task
          for (tIter = dsn.get_tg()->get2_first_task();
               tIter != dsn.get_tg()->task_list_end(); tIter++)
          {
              if ((*tIter).get_no_fncblks() <= 0)
              {
                 cerr << "Task " << (*tIter).get_name()
                      << " has no component list." << endl;
                 return false;
              }
              AX_FuncBlkCP funcP = (*tIter).ran_pick_fncblk();
              AX_FuncBlkC nFunc;
              AX_FuncBlkLCIter fIter;
              fIter = (*aIter).add_fncblk(nFunc);
              (*fIter).copyBasic(*funcP);

              string extID;
              my_itoa(tNum, buf1);
              extID = (*fIter).get_name() + "_";
              extID = extID + buf1;
              (*fIter).set_name(extID);

              (*fIter).remove_all_taskrefs();
              (*fIter).add_new_taskref(&(*tIter));
              (*fIter).set_id(tNum++);

              // constructing platform pointer
              AX_PlatformCP rPlatfmP = NULL, oPlatfmP;
              oPlatfmP = funcP->get_platfm();
              if (oPlatfmP != NULL)
                 rPlatfmP = (*aIter).get_platfm(oPlatfmP->get_name());
              (*fIter).set_platfm(rPlatfmP);

              if ((*tIter).get_type() != DISTRIBUTOR &&
                  (*tIter).get_type() != COLLECTOR)
                 (*aIter).add_compports_by_fncblk(funcP, &(*fIter), pNum);
              else
                 (*aIter).add_compports_by_task(&(*tIter), funcP,
                                                &(*fIter), pNum);
          }
          add_dependent_comps(dsn, (*aIter), tNum, pNum, bNum);
          if (!connect_comps(dsn, (*aIter), bNum))
             return false;

          AX_ScheduleC scheduler;
          scheduler.sched_main(dsn, (*aIter));
          (*aIter).get_arch_latency(*dsn.get_tg());
          (*aIter).compute_utilization(*dsn.get_tg());
#if DEBUG >= 1
          find_err(*dsn.get_tg(), (*aIter), "populate1");
#endif
#if DEBUG >= 4
          cerr << (*aIter).get_name() << ": ";
          AX_FuncBlkLCIter rdIter;
          for (rdIter = (*aIter).get2_first_fncblk();
               rdIter != (*aIter).fncblk_list_end(); rdIter++)
              cerr << (*rdIter).get_name() << " ";
          cerr << endl;
#endif
       }
       return true;
   }
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Operations
// ------------------------------------------------------------
   bool AX_EvolveC::replace_func(AX_DsnC & dsn, AX_ArchGraphC & ag,
                                     AX_TaskCP taskP,
                                     AX_FuncBlkCP origFuncP,
                                     AX_FuncBlkCP platFuncP,
                                     AX_FuncBlkCP & newFuncP,
                                     int & lNum3)
   {
       int lNum = (*ag.get2_last_fncblk()).get_id() + 1;
       AX_FuncBlkCP funcP;
       char buf1[20];

       if (platFuncP == NULL)
          funcP = taskP->ran_pick_fncblk();
       else
          funcP = platFuncP;
          
       AX_FuncBlkC nFunc;
       AX_FuncBlkLCIter fIter;
       fIter = ag.add_fncblk(nFunc);
       (*fIter).copyBasic(*funcP);

       string extID;
       my_itoa(lNum, buf1);
       if (platFuncP == NULL)
          extID = (*fIter).get_name() + "_";
       else
          extID = trim_id((*fIter).get_name()) + "_";
       extID = extID + buf1;
       (*fIter).set_name(extID);

       (*fIter).remove_all_taskrefs();
       (*fIter).add_new_taskref(taskP);
       (*fIter).set_id(lNum);
#if DEBUG >= 4
       cerr << "AX_EvolveC::replace_func-0 replace funcblk" << endl;
       funcP->dump();
       cerr << "AX_EvolveC::replace_func-0 with funcblk" << endl;
       (*fIter).dump();
#endif

       // reconstructing platform and interface references
       copy_funcblk_platfm(ag, (*fIter), *funcP);
       newFuncP = &(*fIter);

       // remove the task referencing then remove the connections
       // referenced. If no more task reference to this func anymore,
       // erase the depPL and the affected dependent components
       origFuncP->delete_taskref(taskP);
       origFuncP->erase_connrefs(ag, taskP);
#if DEBUG >= 4
       cerr << "AX_EvolveC::replace_func-1 after erase_connrefs" << endl;
#endif
       if (origFuncP->get_no_taskrefs() == 0)
          ag.erase_dep_fm_fncblk(origFuncP, NULL);
#if DEBUG >= 4
       cerr << "AX_EvolveC::replace_func-2 after erase_dep_fm_fncblk" << endl;
#endif

       // change ownership of compports
       if (origFuncP->get_no_taskrefs() == 0)
       {
          ag.change_compports_owner(origFuncP, newFuncP, funcP, taskP);
       }
       else
       {
          int pNum;
          pNum = (*ag.get2_last_compport()).get_id();
          if (taskP->get_type() != DISTRIBUTOR &&
              taskP->get_type() != COLLECTOR)
          {
             ag.add_compports_by_fncblk(funcP, newFuncP, pNum);
             newFuncP->clean_sw_ports();
          }
          else
          {
             ag.add_compports_by_task(taskP, funcP, newFuncP, pNum);
          }
       }
#if DEBUG >= 4
       cerr << "AX_EvolveC::replace_func-3 after change compport ownership" << endl;
#endif

       // remove replaced funcblk and compports
       // add_dependent components
       int lNum1 = (*ag.get2_last_fncblk()).get_id() + 1;
       int lNum2 = (*ag.get2_last_compport()).get_id() + 1;
       lNum3 = (*ag.get2_last_bus()).get_id() + 1;
       add_dependent_comps(dsn, ag, *newFuncP, lNum1, lNum2, lNum3);
#if DEBUG >= 4
       cerr << "AX_EvolveC::replace_func-4 after add_dependent_comps" << endl;
#endif

       return true;
   }
// ------------------------------------------------------------
   bool AX_EvolveC::mutate(AX_DsnC & dsn, int gen)
   {
       char buf1[50];
       my_itoa(gen, buf1);
       string GenNo = buf1;

       //bool first = true;
       for (int i = 0; i < POP_SIZE; i++)
       {
          AX_TaskLCIter tIter;
          my_itoa(i, buf1);
          string popStr = buf1;
          int tNum = 0;
          for (tIter = dsn.get_tg()->get2_first_task();
               tIter != dsn.get_tg()->task_list_end(); tIter++)
          {
             if ((*tIter).get_no_fncblks() == 1)
                continue;

             // 10% chance of performing mutattion on this task
             int iNum = my_rand(100);
             if (0 <= iNum && iNum <= 9)
             {
                AX_ArchGraphLCIter aIter;
                int aNum = 0;
                for (aIter = dsn.get2_first_ag();
                     aIter != dsn.ag_list_end(); aIter++)
                {
                   // 50% chance to mutate on this architecture
                   if (my_rand(100) < 50)
                   {
                      my_itoa(i, buf1);
                      string aName = dsn.get_name();
                      aName += "_Gen";
                      aName += GenNo;
                      aName += "_Mu";
                      aName += popStr;
                      aName += ".";
                      my_itoa(tNum, buf1);
                      aName += buf1;
                      aName += ".";
                      my_itoa(aNum, buf1);
                      aName += buf1;
#if DEBUG >= 4
                      cerr << (*aIter).get_name() << "->"
                           << aName << "." << endl;
#endif
                      (*aIter).set_name(aName);
                      (*aIter).reset_compport_temp();
                      AX_FuncBlkCP mutFuncP;
                      mutFuncP = (*aIter).find_fncblk_by_task(&(*tIter));
                      if (mutFuncP == NULL)
                      {
#if DEBUG >= 1
                         cerr << aName
                              << ": Cannot find funcblk maps to task "
                              << (*tIter).get_name() << endl;
                         (*aIter).dump();
#endif
                         return false;
                      }
                      AX_FuncBlkCP nFuncP;
                      int lNum3;
                      if (!replace_func(dsn, (*aIter), &(*tIter),
                                        mutFuncP, NULL, nFuncP, lNum3))
                         return false;
#if DEBUG >= 4
                      cerr << (*tIter).get_name()
                           << ": mutate " << mutFuncP->get_name();
                      cerr << " with " << nFuncP->get_name() << endl;
#endif

                      // finally delete the mutated fncblk
                      bool funcAlive = true;
                      if (mutFuncP->get_no_taskrefs() == 0)
                      {
                         (*aIter).delete_fncblk(mutFuncP);
                         funcAlive = false;
                      }
                      if (!connect_comp((*aIter), &(*tIter), lNum3))
                         return false;
                      if (funcAlive &&
                          !fix_connect((*aIter), mutFuncP, lNum3))
                         return false;

                      // sort fncblk, compport and bus lists
                      (*aIter).sort_fncblk_by_id();
                      (*aIter).sort_compport_by_id();
                      (*aIter).sort_bus_by_id();

                      // re-numbering fncblk, compport and bus lists
                      (*aIter).renum_fncblk_id();
                      (*aIter).renum_compport_id();
                      (*aIter).renum_bus_id();

                      AX_ScheduleC scheduler;
                      scheduler.sched_main(dsn, (*aIter));
                      (*aIter).get_arch_latency(*dsn.get_tg());
#if DEBUG >= 1
                      find_err(*dsn.get_tg(), (*aIter), "mutate");
#endif
                      (*aIter).compute_utilization(*dsn.get_tg());
                   }
                   aNum++;
                } // end archgraph loop
             } // end mutation if
             tNum++;
          } // end task loop
       } // end population loop
       return true;
   }

// ------------------------------------------------------------
   void AX_EvolveC::backup_intrfcPLs(AX_ArchGraphCP agP,
                                      AX_FuncBlkCP funcP,
                                      AX_CompPortLC & inPortL,
                                      AX_CompPortLC & outPortL,
                                      AX_CompPortLC & ioPortL)
   {
       AX_CompPortPLCIter portPI;
       inPortL.clear();
       outPortL.clear();
       ioPortL.clear();
       for (portPI = funcP->get2_first_inport();
            portPI != funcP->inport_list_end(); portPI++)
       {
          AX_CompPortC newPort;
          AX_CompPortLCIter newPortI;
          string nTmpS = (*portPI)->get_name();
          newPort.set_name(nTmpS);
          inPortL.push_back(newPort);
          newPortI = --inPortL.end();
          AX_InterfacePLCIter intrPI;
          for (intrPI = (*portPI)->get2_first_intrfc();
               intrPI != (*portPI)->intrfc_list_end(); intrPI++)
          {
             AX_InterfaceCP newIntrP;
             newIntrP = agP->get_intrfc((*intrPI)->get_name());
             (*newPortI).add_new_intrfc(newIntrP);
          }
       }
       for (portPI = funcP->get2_first_outport();
            portPI != funcP->outport_list_end(); portPI++)
       {
          AX_CompPortC newPort;
          AX_CompPortLCIter newPortI;
          string nTmpS = (*portPI)->get_name();
          newPort.set_name(nTmpS);
          outPortL.push_back(newPort);
          newPortI = --outPortL.end();
          AX_InterfacePLCIter intrPI;
          for (intrPI = (*portPI)->get2_first_intrfc();
               intrPI != (*portPI)->intrfc_list_end(); intrPI++)
          {
             AX_InterfaceCP newIntrP;
             newIntrP = agP->get_intrfc((*intrPI)->get_name());
             (*newPortI).add_new_intrfc(newIntrP);
          }
       }
       for (portPI = funcP->get2_first_ioport();
            portPI != funcP->ioport_list_end(); portPI++)
       {
          AX_CompPortC newPort;
          AX_CompPortLCIter newPortI;
          string nTmpS = (*portPI)->get_name();
          newPort.set_name(nTmpS);
          ioPortL.push_back(newPort);
          newPortI = --ioPortL.end();
          AX_InterfacePLCIter intrPI;
          for (intrPI = (*portPI)->get2_first_intrfc();
               intrPI != (*portPI)->intrfc_list_end(); intrPI++)
          {
             AX_InterfaceCP newIntrP;
             newIntrP = agP->get_intrfc((*intrPI)->get_name());
             (*newPortI).add_new_intrfc(newIntrP);
          }
       }
   }
// ------------------------------------------------------------
   void AX_EvolveC::restore_intrfcPLs(AX_FuncBlkCP funcP,
                                       AX_CompPortLC inPortL,
                                       AX_CompPortLC outPortL,
                                       AX_CompPortLC ioPortL)
   {
       AX_CompPortLCIter bPortI;
       for (bPortI = inPortL.begin();
            bPortI != inPortL.end(); bPortI++)
       {
          AX_CompPortPLCIter portPI;
          for (portPI = funcP->get2_first_inport();
               portPI != funcP->inport_list_end(); portPI++)
          {
             if ((*bPortI).get_name() == (*portPI)->get_name())
             {
                AX_InterfacePLCIter intrPI;
                for (intrPI = (*bPortI).get2_first_intrfc();
                     intrPI != (*bPortI).intrfc_list_end(); intrPI++)
                   (*portPI)->add_new_intrfc(*intrPI);
                break;
             }
          } // end inportPL loop
       } // end inPortL loop
       for (bPortI = outPortL.begin();
            bPortI != outPortL.end(); bPortI++)
       {
          AX_CompPortPLCIter portPI;
          for (portPI = funcP->get2_first_outport();
               portPI != funcP->outport_list_end(); portPI++)
          {
             if ((*bPortI).get_name() == (*portPI)->get_name())
             {
                AX_InterfacePLCIter intrPI;
                for (intrPI = (*bPortI).get2_first_intrfc();
                     intrPI != (*bPortI).intrfc_list_end(); intrPI++)
                   (*portPI)->add_new_intrfc(*intrPI);
                break;
             }
          } // end outportPL loop
       } // end outPortL loop
       for (bPortI = ioPortL.begin();
            bPortI != ioPortL.end(); bPortI++)
       {
          AX_CompPortPLCIter portPI;
          for (portPI = funcP->get2_first_ioport();
               portPI != funcP->ioport_list_end(); portPI++)
          {
             if ((*bPortI).get_name() == (*portPI)->get_name())
             {
                AX_InterfacePLCIter intrPI;
                for (intrPI = (*bPortI).get2_first_intrfc();
                     intrPI != (*bPortI).intrfc_list_end(); intrPI++)
                   (*portPI)->add_new_intrfc(*intrPI);
                break;
             }
          } // end ioportPL loop
       } // end ioPortL loop
   }
// ------------------------------------------------------------
   bool AX_EvolveC::crossover(AX_DsnC & dsn, int gen)
   {
       char buf1[50];
       my_itoa(gen, buf1);
       string GenNo = buf1;

       for (int i = 0; i < POP_SIZE; i++)
       {
          AX_TaskLCIter tIter;
          my_itoa(i, buf1);
          string popStr = buf1;
          int tNum = 0;
          for (tIter = dsn.get_tg()->get2_first_task();
               tIter != dsn.get_tg()->task_list_end(); tIter++)
          {
             if ((*tIter).get_no_fncblks() == 1)
                continue;

             // 10% chance of performing crossover on this task
             int iNum = my_rand(100);
             if (50 <= iNum && iNum <= 59)
             {
                // frst pick 15% of the population for the first
                // half of the crossover pair, then select another
                // 15% as the second half. Then do the cross over.

                // prepare the integer arrays
                int * pool = new int[POP_SIZE];
                for (int j = 0; j < POP_SIZE; j++)
                   pool[j] = j;
                int sSize = (int)((float)POP_SIZE * 0.15);
                int * firstHalf = new int[sSize];
                int * secondHalf = new int[sSize];

                // get first half
                pick_numbers(pool,POP_SIZE,firstHalf,sSize);
                // get second half
                pick_numbers(pool,POP_SIZE,secondHalf,sSize);
                delete [] pool;

                // perform crossover
                for (int j = 0; j < sSize; j++)
                {
                   //int lNum;
                   AX_ArchGraphCP firstAG, secondAG;
                   firstAG = dsn.get_ag(firstHalf[j]);
                   secondAG = dsn.get_ag(secondHalf[j]);

                   my_itoa(j, buf1);
                   string aName = dsn.get_name();
                   aName += "_Gen";
                   aName += GenNo;
                   aName += "_Cr";
                   aName += popStr;
                   aName += ".";
                   my_itoa(tNum, buf1);
                   aName += buf1;
                   aName += ".";
                   my_itoa(firstHalf[j], buf1);
                   aName += buf1;
                   aName += ".";
                   my_itoa(secondHalf[j], buf1);
                   aName += buf1;
                   string fName = aName;
                   fName += "_F";
                   string sName = aName;
                   sName += "_S";

#if DEBUG >= 4
                   cerr << firstAG->get_name() << "->"
                        << fName << "." << endl;
                   cerr << secondAG->get_name() << "->"
                        << sName << "." << endl;

                   string ofName = firstAG->get_name();
                   string osName = secondAG->get_name();
#endif

                   firstAG->set_name(fName);
                   firstAG->reset_compport_temp();
                   secondAG->set_name(sName);
                   secondAG->reset_compport_temp();

                   AX_FuncBlkCP fFuncP, sFuncP;
                   fFuncP = firstAG->find_fncblk_by_task(&(*tIter));
                   sFuncP = secondAG->find_fncblk_by_task(&(*tIter));
                   if (fFuncP == NULL)
                   {
#if DEBUG >= 1
                      cerr << fName
                           << ": Cannot find funcblk maps to task "
                           << (*tIter).get_name() << endl;
                      firstAG->dump();
#endif
                      return false;
                   }
                   if (sFuncP == NULL)
                   {
#if DEBUG >= 1
                      cerr << sName
                           << ": Cannot find funcblk maps to task "
                           << (*tIter).get_name() << endl;
                      secondAG->dump();
#endif
                      return false;
                   }
                   AX_FuncBlkCP nfFuncP, nsFuncP;
                   int lNum3_1, lNum3_2;

                   AX_CompPortLC inPortL, outPortL, ioPortL;
                   backup_intrfcPLs(secondAG, fFuncP,
                                    inPortL, outPortL, ioPortL);
                   if (!replace_func(dsn, *firstAG, &(*tIter), fFuncP,
                                     sFuncP, nfFuncP, lNum3_1))
                      return false;

                   if (!replace_func(dsn, *secondAG, &(*tIter), sFuncP,
                                     fFuncP, nsFuncP, lNum3_2))
                      return false;
                   restore_intrfcPLs(nsFuncP, inPortL, outPortL, ioPortL);

#if DEBUG >= 4
                   cerr << "  " << (*tIter).get_name();
                   cerr << ": crossover " << nfFuncP->get_name();
                   cerr << " with " << nsFuncP->get_name() << endl;
#endif

                   // finally delete the mutated fncblk
                   bool fFuncAlive = true;
                   if (fFuncP->get_no_taskrefs() == 0)
                   {
                      firstAG->delete_fncblk(fFuncP);
                      fFuncAlive = false;
                   }
                   if (!connect_comp(*firstAG, &(*tIter), lNum3_1))
                      return false;
                   if (fFuncAlive &&
                       !fix_connect(*firstAG, fFuncP, lNum3_1))
                      return false;

                   // sort fncblk, compport and bus lists
                   firstAG->sort_fncblk_by_id();
                   firstAG->sort_compport_by_id();
                   firstAG->sort_bus_by_id();

                   // re-numbering fncblk, compport and bus lists
                   firstAG->renum_fncblk_id();
                   firstAG->renum_compport_id();
                   firstAG->renum_bus_id();

                   // finally delete the mutated fncblk
                   bool sFuncAlive = true;
                   if (sFuncP->get_no_taskrefs() == 0)
                   {
                      secondAG->delete_fncblk(sFuncP);
                      sFuncAlive = false;
                   }
                   if (!connect_comp(*secondAG, &(*tIter), lNum3_2))
                      return false;
                   if (sFuncAlive &&
                       !fix_connect(*secondAG, sFuncP, lNum3_2))
                      return false;

                   // sort fncblk, compport and bus lists
                   secondAG->sort_fncblk_by_id();
                   secondAG->sort_compport_by_id();
                   secondAG->sort_bus_by_id();

                   // re-numbering fncblk, compport and bus lists
                   secondAG->renum_fncblk_id();
                   secondAG->renum_compport_id();
                   secondAG->renum_bus_id();

                   AX_ScheduleC scheduler;
                   scheduler.sched_main(dsn, *firstAG);
                   scheduler.sched_main(dsn, *secondAG);
                   firstAG->get_arch_latency(*dsn.get_tg());
                   secondAG->get_arch_latency(*dsn.get_tg());
                   firstAG->compute_utilization(*dsn.get_tg());
                   secondAG->compute_utilization(*dsn.get_tg());
#if DEBUG >= 1
                   find_err(*dsn.get_tg(), *firstAG, "crossover");
                   find_err(*dsn.get_tg(), *secondAG, "crossover");
#endif
                }
             }
             tNum++;
          }
       }
       return true;
   }
// ------------------------------------------------------------
   bool AX_EvolveC::share_mutate(AX_DsnC & dsn, int gen)
   {
       char buf1[50];
       my_itoa(gen, buf1);
       string GenNo = buf1;

       AX_ArchGraphLCIter agI;
       int popNum = 0;
       AX_ShareLC shareL;
       //int noTask = dsn.get_tg()->get_no_tasks();
  
       for (agI = dsn.get2_first_ag();
            agI != dsn.ag_list_end(); agI++)
       {
          my_itoa(popNum++, buf1);
          string popStr = buf1;
          bool first = true;
          int fNum = (*(*agI).get2_last_fncblk()).get_id();
          int pNum = (*(*agI).get2_last_compport()).get_id();
          int bNum = (*(*agI).get2_last_bus()).get_id();
          AX_ShareLCIter shareI;
#if DEBUG >= 3
          int listSize;
          listSize = (*agI).get_sharable_lists(*dsn.get_tg(), shareL);
#else
          (*agI).get_sharable_lists(*dsn.get_tg(), shareL);
#endif

#if DEBUG >= 3
          cerr << "\n\n===>>> " << (*agI).get_name()
               << ": " << listSize << " lists  <<<===\n\n";
          for (shareI = shareL.begin();
               shareI != shareL.end(); shareI++)
             (*shareI).dump();
#endif
          for (shareI = shareL.begin();
               shareI != shareL.end(); shareI++)
          {
             // toss the coin to determine how many tasks in
             // here wants to share
             int noShare = my_rand((*shareI).get_no_tasks() + 1);
             if (noShare <= 1)
                continue;

             if (first)
             {
                first = false;
                string aName = dsn.get_name();
                aName += "_Gen";
                aName += GenNo;
                aName += "_Sh";
                aName += popStr;
                (*agI).set_name(aName);
                (*agI).reset_compport_temp();
             }

             // randomly determine the starting point of task
             // to share. For example, there are 5 tasks that
             // can share this component, and it is randomly
             // decided 3 tasks to share (noShare). Then we
             // randomly select a starting point for the sharing
             // task, say 4 (strPnt). Then task number 4, 0 and 1
             // share the same component. Notice the index to
             // the task is wrapped around.

             //AX_TaskCP masTaskP = (*shareI).get_task(strPnt);
             int strPnt = my_rand((*shareI).get_no_tasks());
             AX_FuncBlkCP masFuncP = (*shareI).get_sw_fncblk(strPnt);
             AX_FuncBlkCP masProcP = (*shareI).get_fncblk(strPnt);

             int shareSize = 0;
             for (int i = 0; i < (*shareI).get_no_tasks(); i++)
                 shareSize += (*shareI).get_sw_fncblk(i)->get_no_taskrefs();
             AX_FuncBlkCP * copyFuncPA;
             copyFuncPA = new AX_FuncBlkCP[shareSize];
             for (int i = 0; i < shareSize; i++)
                copyFuncPA[i] = NULL;

             int noCopy = 0;
             copyFuncPA[noCopy++] = masFuncP;
             for (int cpi = strPnt + 1; cpi < strPnt + noShare; cpi++)
             {
                //AX_TaskCP cpyTaskP = (*shareI).get_task(idx);
                //AX_FuncBlkCP cpyProcP = (*shareI).get_fncblk(idx);
                int idx = cpi % (*shareI).get_no_tasks();
                AX_FuncBlkCP cpyFuncP = (*shareI).get_sw_fncblk(idx);
                AX_FuncBlkCP existFuncP = NULL;
                bool cpyFound = false;
                for (int i = 0; i < noCopy; i++)
                {
                   if (trim_id(cpyFuncP->get_name()) ==
                       trim_id(copyFuncPA[i]->get_name()))
                   {
                      existFuncP = copyFuncPA[i];
                      cpyFound = true;
                   }
                }
                if (!cpyFound)
                   copyFuncPA[noCopy++] = cpyFuncP;

                AX_TaskRefLSIter taskrefI;
                if (cpyFound)
                {
                   // already a copy of cpyFuncP, existFuncP, resides in
                   // masProcP, so we move everything from cpyFuncP to
                   // existFuncP then delete cpyFuncP
                   while ((taskrefI = cpyFuncP->get2_first_taskref()) !=
                          cpyFuncP->taskref_list_end())
                   {
                      AX_TaskRefS nTaskRef;
                      nTaskRef.m_taskP = (*taskrefI).m_taskP;
                      nTaskRef.m_start = (*taskrefI).m_start;
                      nTaskRef.m_finish = (*taskrefI).m_finish;

                      cpyFuncP->delete_taskref(taskrefI);
                      cpyFuncP->erase_connrefs((*agI), nTaskRef.m_taskP);

                      AX_TaskRefLSIter nTaskRefI;
                      nTaskRefI = existFuncP->add_new_taskref(nTaskRef);
                      (*agI).erase_dep_fm_fncblk(cpyFuncP, NULL);
                      expand_proc_refPL(false, dsn, (*agI), masProcP,
                                        existFuncP, fNum, pNum, bNum);
                      connect_comp((*agI), (*nTaskRefI).m_taskP, bNum);
                   }
                   (*agI).delete_compports_by_fncblk(cpyFuncP);
                   (*agI).delete_fncblk(cpyFuncP);
                }
                else
                {
                   // masFuncP and cpyFuncP are different components
                   // that just share the same processor. So we keep
                   // cpyFuncP and make it maps to the processor
                   cpyFuncP->erase_connrefs(*agI);
                   (*agI).erase_dep_fm_fncblk(cpyFuncP, NULL);
                   expand_proc_refPL(true, dsn, (*agI), masProcP,
                                     cpyFuncP, fNum, pNum, bNum);
                   connect_comp((*agI), cpyFuncP, bNum);
                   cpyFuncP->remove_all_deps();
                   cpyFuncP->add_new_dep(masProcP);
#if DEBUG >= 4
                   cerr << "masFuncP is" << endl;
                   masFuncP->dump();
                   cerr << "cpyFuncP is" << endl;
                   cpyFuncP->dump();
                   cerr << "cpi=" << cpi << " masProcP is" << endl;
                   masProcP->dump();
                   cerr << "masProcP m_depPL size is "
                        << masProcP->get_no_deps() << endl;
#endif
                   if (masProcP->get_no_deps() > 0)
                   {
#if DEBUG >= 4
                      cerr << "masProcP has dependents" << endl;
                      masProcP->dump();
#endif
                      AX_FuncBlkPLCIter depI;
                      for (depI = masProcP->get2_first_dep();
                           depI != masProcP->dep_list_end(); depI++)
                      {
                         cpyFuncP->add_new_dep(*depI);
                         (*depI)->add_new_ref(cpyFuncP);
                      }
                   }
                }
             } // end cpi loop
             delete [] copyFuncPA;
          } // end shareL loop

          // sort fncblk, compport and bus lists
          (*agI).sort_fncblk_by_id();
          (*agI).sort_compport_by_id();
          (*agI).sort_bus_by_id();
          // re-numbering fncblk, compport and bus lists
          (*agI).renum_fncblk_id();
          (*agI).renum_compport_id();
          (*agI).renum_bus_id();
          shareL.clear();

          AX_ScheduleC scheduler;
          scheduler.sched_main(dsn, (*agI));
          (*agI).get_arch_latency(*dsn.get_tg());
          (*agI).compute_utilization(*dsn.get_tg());
#if DEBUG >= 1
          find_err(*dsn.get_tg(), (*agI), "share");
#endif
       } // end ag loop

       shareL.clear();

       return true;
   }

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Operations
// ------------------------------------------------------------
   bool AX_EvolveC::connect_comps(AX_DsnC & dsn, AX_ArchGraphC & ag,
                                      int & bNum)
   {
       AX_ConnLCIter cIter;
       //bool first = true, first1 = true, first2 = true;

       AX_TaskGraphCP tgP = dsn.get_tg();
       for (cIter = tgP->get2_first_conn();
            cIter != tgP->conn_list_end(); cIter++)
       {
          AX_TaskCP srcTaskP, sinkTaskP;
          AX_TaskPortCP srcPortP, sinkPortP;
          AX_TaskGraphCP srcTgP, sinkTgP;

          srcTgP = (*cIter).get_src_tg();
          sinkTgP = (*cIter).get_sink_tg();
          srcTaskP = (*cIter).get_src_task();
          sinkTaskP = (*cIter).get_sink_task();
          srcPortP = (*cIter).get_src_port();
          sinkPortP = (*cIter).get_sink_port();

          // connecting the components by taskgraph connections
          if (!connect(ag, srcTgP, srcTaskP, srcPortP,
                       sinkTgP, sinkTaskP, sinkPortP, bNum, &(*cIter)))
             return false;
       } // end tg connection loop

       return true;
   }

// ------------------------------------------------------------
   bool AX_EvolveC::fix_connect(AX_ArchGraphC & ag,
                                    AX_FuncBlkCP funcP, int & bNum)
   {
       AX_TaskRefLSIter taskrefI;
       for (taskrefI = funcP->get2_first_taskref();
            taskrefI != funcP->taskref_list_end(); taskrefI++)
       {
          AX_TaskPortPLCIter tportPI;
          AX_TaskCP taskP = (*taskrefI).m_taskP;
          for (tportPI = taskP->get2_first_inport();
               tportPI != taskP->inport_list_end(); tportPI++)
          {
             AX_ConnPLCIter connPI;
             for (connPI = (*tportPI)->get2_first_conn();
                  connPI != (*tportPI)->conn_list_end(); connPI++)
             {
                 AX_BusCP busP;
                 busP = ag.get_bus_by_conn(*connPI);
                 if (busP == NULL && !connect_comp(ag, taskP, bNum))
                    return false;
             } // end conn loop
          } // end inport loop
          for (tportPI = taskP->get2_first_outport();
               tportPI != taskP->outport_list_end(); tportPI++)
          {
             AX_ConnPLCIter connPI;
             for (connPI = (*tportPI)->get2_first_conn();
                  connPI != (*tportPI)->conn_list_end(); connPI++)
             {
                 AX_BusCP busP;
                 busP = ag.get_bus_by_conn(*connPI);
                 if (busP == NULL && !connect_comp(ag, taskP, bNum))
                    return false;
             } // end conn loop
          } // end inport loop
       }
       return true;
   }
// ------------------------------------------------------------
   bool AX_EvolveC::connect_comp(AX_ArchGraphC & ag,
                                     AX_FuncBlkCP funcP, int & bNum)
   {
       AX_TaskRefLSIter taskrefI;
       for (taskrefI = funcP->get2_first_taskref();
            taskrefI != funcP->taskref_list_end(); taskrefI++)
          if (!connect_comp(ag, (*taskrefI).m_taskP, bNum))
             return false;

       return true;
   }
// ------------------------------------------------------------
   bool AX_EvolveC::connect_comp(AX_ArchGraphC & ag,
                                     AX_TaskCP taskP, int & bNum)
   {
       AX_TaskPortPLCIter tportPI;
       AX_TaskCP srcTaskP, sinkTaskP;
       AX_TaskPortCP srcPortP, sinkPortP;
       AX_TaskGraphCP srcTgP, sinkTgP;

       for (tportPI = taskP->get2_first_inport();
            tportPI != taskP->inport_list_end(); tportPI++)
       {
          AX_ConnPLCIter connPI;
          for (connPI = (*tportPI)->get2_first_conn();
               connPI != (*tportPI)->conn_list_end(); connPI++)
          {
             srcTgP = (*connPI)->get_src_tg();
             sinkTgP = (*connPI)->get_sink_tg();
             srcTaskP = (*connPI)->get_src_task();
             sinkTaskP = (*connPI)->get_sink_task();
             srcPortP = (*connPI)->get_src_port();
             sinkPortP = (*connPI)->get_sink_port();

             // connecting the components by taskgraph connections
             if (!connect(ag, srcTgP, srcTaskP, srcPortP,
                          sinkTgP, sinkTaskP, sinkPortP, bNum, (*connPI)))
                return false;
          }
       }
       for (tportPI = taskP->get2_first_outport();
            tportPI != taskP->outport_list_end(); tportPI++)
       {
          AX_ConnPLCIter connPI;
          for (connPI = (*tportPI)->get2_first_conn();
               connPI != (*tportPI)->conn_list_end(); connPI++)
          {
             srcTgP = (*connPI)->get_src_tg();
             sinkTgP = (*connPI)->get_sink_tg();
             srcTaskP = (*connPI)->get_src_task();
             sinkTaskP = (*connPI)->get_sink_task();
             srcPortP = (*connPI)->get_src_port();
             sinkPortP = (*connPI)->get_sink_port();

             // connecting the components by taskgraph connections
             if (!connect(ag, srcTgP, srcTaskP, srcPortP,
                          sinkTgP, sinkTaskP, sinkPortP, bNum, (*connPI)))
                return false;
          }
       }

       return true;
   }
// ------------------------------------------------------------
   bool AX_EvolveC::connect(AX_ArchGraphC & ag, AX_TaskGraphCP srcTgP,
                                AX_TaskCP srcTaskP, AX_TaskPortCP srcTPortP,
                                AX_TaskGraphCP sinkTgP, AX_TaskCP sinkTaskP,
                                AX_TaskPortCP sinkTPortP, int & bNum,
                                AX_ConnCP connP)
   {
       AX_CompPortCP srcCmPortP, sinkCmPortP;
       AX_CompPortLCIter gPortI;

       // pass 1: handle the source port
       AX_ConnPntC srcCnPnt;
       int pNum = 0;
       bool useMemPort = false; // a flag to indicate the source port
                                   // already uses the memory port, so
                                   // the sink port need not to try to
                                   // do memory port connection
       AX_CompPortCP swSinkPortP = NULL;
       AX_CompPortCP swSrcPortP = NULL;
       bool srcIncRefCnt = true;
       bool sinkIncRefCnt = true;
       
#if DEBUG >= 4
       cerr << "Connecting ";
       if (srcTgP != NULL)
          cerr << srcTgP->get_name();
       else
          cerr << srcTaskP->get_name();
       cerr << "." << srcTPortP->get_name() << " -> ";
       if (sinkTgP != NULL)
          cerr << sinkTgP->get_name();
       else
          cerr << sinkTaskP->get_name();
       cerr << "." << sinkTPortP->get_name() << endl;
#endif

       if (srcTaskP == NULL)
       {
          // this is a graph port. So add ag compport
          srcCnPnt.set_comp(NULL);
          srcCnPnt.set_ag(&ag);

          gPortI = ag.add_new_ag_inport(srcTPortP->get_name());
          (*gPortI).set_id(pNum++);
          srcCnPnt.set_port(&(*gPortI));
          srcCmPortP = &(*gPortI);
       } // if src is a graph port
       else
       {
          // this is a taskport. So add compport
          srcCmPortP = ag.get_compport(srcTaskP, srcTPortP->get_name());
          if (srcCmPortP == NULL)
          {
#if DEBUG >= 1
             cerr << "Cannot find compport referenced to taskport "
                  << srcTaskP->get_name() << ":"
                  << srcTPortP->get_name() << endl;
             fstream ag_dump;
             ag_dump.open("AGDump.txt", fstream::out|fstream::app);
             ag.write(ag_dump);
#endif
             return false;
          }
          if (AX_FuncBlkCP(srcCmPortP->get_comp())->get_type() == SOFTWARE &&
              srcCmPortP->get_sw_port() == NULL)
          {
             AX_FuncBlkCP procP;
             AX_FuncBlkCP funcP = AX_FuncBlkCP(srcCmPortP->get_comp());
             swSrcPortP = srcCmPortP;
             procP = funcP->find_proc_in_dep();
             if (procP == NULL)
             {
#if DEBUG >= 1
                cerr << "Cannot find processor in depPL of "
                     << srcCnPnt.get_comp()->get_name() << endl;
#endif
                return false;
             }
             if (procP->get_no_ioports() <= 0)
             {
                cerr << "Lib error: no data ports in "
                     << procP->get_name() << endl;
                exit(1);
             }
             srcCmPortP = procP->get_an_empty_ioport(NULL);

             // consider the case where this processor uses
             // a multi-port memory, then we like to make use
             // of the empty data port for data communication
             AX_FuncBlkPLCIter srchMemPI;
             AX_FuncBlkCP srchMemP;
             bool DoMultMem = false;
             AX_BusCP connBusP;
             for (srchMemPI = procP->get2_first_dep();
                  srchMemPI != procP->dep_list_end(); srchMemPI++)
             {
                if ((*srchMemPI)->get_functype() == MEMORY ||
                    (*srchMemPI)->get_functype() == MEMORY_CORE)
                   if ((*srchMemPI)->get_no_ioports() > 1)
                   {
                      connBusP = srcCmPortP->func_on_port(*srchMemPI);
                      // connBusP = null means the referenced memory
                      // is not connected to this port
                      if (connBusP == NULL)
                         continue;
                      srchMemP = (*srchMemPI);
                      DoMultMem = true;
                      break;
                   }
             } // end dep loop to look for memory used
             if (DoMultMem)
             {
                AX_CompPortCP repPortP;
                repPortP = srchMemP->get_an_empty_ioport(NULL);
                if (repPortP == NULL)
                {
                   cerr << "Lib error: " << srchMemP->get_name()
                        << " does not have data ports\n";
                   exit(1);
                }
                if (repPortP != srcCmPortP)
                {
                   srcCmPortP = repPortP;
                   useMemPort = true;
                }
             }

             // srcCmPortP = (*procP->get2_first_ioport());
             srcCmPortP->inc_refCnt();
             swSrcPortP->set_sw_port(srcCmPortP);
             ag.adjust_refCnt(srcCmPortP);
          }
          if (AX_FuncBlkCP(srcCmPortP->get_comp())->get_type() == SOFTWARE &&
             srcCmPortP->get_sw_port() != NULL)
          {
             swSrcPortP = srcCmPortP;
             srcCmPortP = srcCmPortP->get_sw_port();
             if (swSrcPortP->get_temp() == 1)
             {
                srcCmPortP->inc_refCnt();
                swSrcPortP->set_temp(-1);
             }
             else
                srcIncRefCnt = false;
             ag.adjust_refCnt(srcCmPortP);
          }
          srcCnPnt.set_comp(srcCmPortP->get_comp());
          srcCnPnt.set_ag(NULL);
          srcCnPnt.set_port(srcCmPortP);
       } // if src is a compport

       // pass 1: handle the sink port
       AX_ConnPntC sinkCnPnt;
       pNum = 0;
       if (sinkTaskP == NULL)
       {
          // this is a graph port. So add ag compport
          sinkCnPnt.set_comp(NULL);
          sinkCnPnt.set_ag(&ag);

          gPortI = ag.add_new_ag_outport(sinkTPortP->get_name());
          (*gPortI).set_id(pNum++);
          sinkCnPnt.set_port(&(*gPortI));
          sinkCmPortP = &(*gPortI);
       } // if sink is a graph port
       else
       {
          // this is a taskport. So add compport
          sinkCmPortP = ag.get_compport(sinkTaskP, sinkTPortP->get_name());
          if (sinkCmPortP == NULL)
          {
#if DEBUG >= 1
             cerr << "Cannot find compport referenced to taskport "
                  << sinkTaskP->get_name() << ":"
                  << sinkTPortP->get_name() << endl;
#endif
             return false;
          }
          if (AX_FuncBlkCP(sinkCmPortP->get_comp())->get_type() == SOFTWARE &&
              sinkCmPortP->get_sw_port() == NULL)
          {
             AX_FuncBlkCP procP;
             AX_FuncBlkCP funcP = AX_FuncBlkCP(sinkCmPortP->get_comp());
             swSinkPortP = sinkCmPortP;
             procP = funcP->find_proc_in_dep();
             if (procP == NULL)
             {
#if DEBUG >= 1
                cerr << "Cannot find processor in depPL of "
                     << sinkCmPortP->get_comp()->get_name() << endl;
#endif
                return false;
             }
             if (procP->get_no_ioports() <= 0)
             {
                cerr << "Lib error: no data ports in "
                     << procP->get_name() << endl;
                exit(1);
             }
             sinkCmPortP = procP->get_an_empty_ioport(NULL);

             if (!useMemPort)
             {
                // consider the case where this processor uses
                // a multi-port memory, then we like to make use
                // of the empty data port for data communication
                AX_FuncBlkPLCIter srchMemPI;
                AX_FuncBlkCP srchMemP = NULL;
                bool DoMultMem = false;
                AX_BusCP connBusP;
                for (srchMemPI = procP->get2_first_dep();
                     srchMemPI != procP->dep_list_end(); srchMemPI++)
                {
                   if ((*srchMemPI)->get_functype() == MEMORY ||
                       (*srchMemPI)->get_functype() == MEMORY_CORE)
                      if ((*srchMemPI)->get_no_ioports() > 1)
                      {
                         connBusP = srcCmPortP->func_on_port(*srchMemPI);
                         // connBusP = null means the referenced memory
                         // is not connected to this port
                         if (connBusP == NULL)
                            continue;
                         srchMemP = (*srchMemPI);
                         DoMultMem = true;
                      }
                } // end dep loop to loop for memory used
                if (DoMultMem)
                {
                   AX_CompPortCP repPortP;
                   repPortP = srchMemP->get_an_empty_ioport(NULL);
                   if (repPortP == NULL)
                   {
                      cerr << "Lib error: " << srchMemP->get_name()
                           << " does not have data ports\n";
                      exit(1);
                   }
                   if (repPortP != sinkCmPortP)
                   {
                      sinkCmPortP = repPortP;
                      useMemPort = true;
                   }
                }
             } // end useMemPort

             // sinkCmPortP = (*procP->get2_first_ioport());
             sinkCmPortP->inc_refCnt();
             swSinkPortP->set_sw_port(sinkCmPortP);
             ag.adjust_refCnt(sinkCmPortP);
          }
          if (AX_FuncBlkCP(sinkCmPortP->get_comp())->get_type()==SOFTWARE &&
              sinkCmPortP->get_sw_port() != NULL)
          {
             swSinkPortP = sinkCmPortP;
             sinkCmPortP = sinkCmPortP->get_sw_port();
             if (swSinkPortP->get_temp() == 1)
             {
                sinkCmPortP->inc_refCnt();
                swSinkPortP->set_temp(-1);
             }
             else
                sinkIncRefCnt = false;
             ag.adjust_refCnt(sinkCmPortP);
          }
          sinkCnPnt.set_comp(sinkCmPortP->get_comp());
          sinkCnPnt.set_ag(NULL);
          sinkCnPnt.set_port(sinkCmPortP);
       } // if sink is a compport

#if DEBUG >= 4
       cerr << "After 1st pass: ";
       if (srcCnPnt.get_ag() != NULL)
          cerr << srcCnPnt.get_ag()->get_name() << ".";
       else
          cerr << srcCnPnt.get_comp()->get_name() << ".";
       cerr << srcCnPnt.get_port()->get_name() << " -> ";
       if (sinkCnPnt.get_ag() != NULL)
          cerr << sinkCnPnt.get_ag()->get_name() << ".";
       else
          cerr << sinkCnPnt.get_comp()->get_name() << ".";
       cerr << sinkCnPnt.get_port()->get_name() << endl;
#endif

       // remove the original connref first
       AX_BusLCIter srchBusI;
       for (srchBusI = ag.get2_first_bus();
            srchBusI != ag.bus_list_end(); srchBusI++)
          if ((*srchBusI).is_mapped_conn(connP))
             {
                (*srchBusI).delete_connref(connP);
             }

       // if src and sink runs on the same component then we don't
       // need to connect them.
       if (srcCnPnt.get_comp() != NULL && sinkCnPnt.get_comp() != NULL &&
           srcCnPnt.get_comp() == sinkCnPnt.get_comp())
       {
          FuncTypeE type = AX_FuncBlkCP(srcCnPnt.get_comp())->get_functype();
          if (type == PROCESSOR || type == MEMORY || type == MEMORY_CORE)
          {
             AX_FuncBlkCP swFuncP = NULL;
             if (srcIncRefCnt)
                srcCnPnt.get_port()->dec_refCnt();
             if (srcTPortP->get_no_conns() > 1)
             {
                AX_ConnPLCIter connPI;
                bool clean = true;
                for (connPI = srcTPortP->get2_first_conn();
                     connPI != srcTPortP->conn_list_end(); connPI++)
                {
                   AX_TaskCP othTaskP;
                   othTaskP = (*connPI)->get_sink_task();
                   if (othTaskP == NULL ||
                     ag.get_fncblk_by_task(othTaskP) != sinkCnPnt.get_comp())
                   {
                      clean = false;
                      break;
                   }
                }
                if (clean)
                {
                   swFuncP = AX_FuncBlkCP(swSrcPortP->get_comp());
                   if (swFuncP->get_no_taskrefs() == 1)
                      swSrcPortP->set_sw_port(NULL);
                }
             }
             else
             {
                swFuncP = AX_FuncBlkCP(swSrcPortP->get_comp());
                if (swFuncP->get_no_taskrefs() == 1)
                   swSrcPortP->set_sw_port(NULL);
             }

             if (sinkIncRefCnt)
                sinkCnPnt.get_port()->dec_refCnt();
             swFuncP = AX_FuncBlkCP(swSinkPortP->get_comp());
             if (swFuncP->get_no_taskrefs() == 1)
                swSinkPortP->set_sw_port(NULL);

             AX_BusCP connBusP;
             connBusP = ag.get_bus_by_conn(connP);
             if (connBusP != NULL)
                connBusP->delete_connref(connP);
          }
          ag.adjust_refCnt(srcCnPnt.get_port());
          ag.adjust_refCnt(sinkCnPnt.get_port());
          return true;
       }

       // pass 2: get the bus and interface
       // first we get the list of possible interfaces
       // can be used for this connection
       int BusNo = 0;
       if ((BusNo = sinkCnPnt.get_port()->get_no_buses()) > 1)
       {
#if DEBUG >= 4
          cerr << "Sink port ";
          if (sinkCnPnt.get_ag() != NULL)
             cerr << sinkCnPnt.get_ag()->get_name() << ":";
          else
             cerr << sinkCnPnt.get_comp()->get_name() << ":";
          cerr << sinkCnPnt.get_port()->get_name()
               << " has more than one input buses:\n";
          AX_BusPLCIter prtBusPI;
          for (prtBusPI = sinkCnPnt.get_port()->get2_first_bus();
               prtBusPI != sinkCnPnt.get_port()->bus_list_end(); prtBusPI++)
             cerr << (*prtBusPI)->get_intrfc()->get_name() << endl;
#endif
          return false;
       } // if sink port has more than one bus wired
       else if (BusNo == 1)
       {
          AX_BusCP sBusP = sinkCnPnt.get_port()->get_bus(0);
          InterfaceE iType = sBusP->get_intrfc()->get_intrtype();
          if (iType == INTRFC_WIRE)
          {
#if DEBUG >= 4
             cerr << "Sink port ";
             if (sinkCnPnt.get_ag() != NULL)
                cerr << sinkCnPnt.get_ag()->get_name() << ":";
             else
                cerr << sinkCnPnt.get_comp()->get_name() << ":";
             cerr << sinkCnPnt.get_port()->get_name()
                  << " has connected with a wire type bus\n";
#endif
             return false;
          } // if sink port is wired with a wire already
          else if (iType == INTRFC_BUS)
          {
             // the sink is connect to a bus, so add soure point
             // to the bus too, if the source point can use
             // this bus
             if (srcCnPnt.get_comp() != NULL)
             {
                AX_InterfaceCP testIP;
                AX_CompPortCP testCP = srcCnPnt.get_port();
                testIP = testCP->get_intrfc(sBusP->get_intrfc()->get_name());
                if (testIP == NULL)
                {
#if DEBUG >= 1
                   if (srcCnPnt.get_ag() != NULL)
                      cerr << srcCnPnt.get_ag()->get_name() << ":";
                   else
                      cerr << srcCnPnt.get_comp()->get_name() << ":";
                   cerr << srcCnPnt.get_port()->get_name()
                        << " cannot connect to intrfc "
                        << sBusP->get_intrfc()->get_name() << endl;
#endif
                   return false;
                }
             }

             // everything OK, put the src point to the bus
             // and process the next connection
             sBusP->add_new_connpnt(srcCnPnt);
             sBusP->add_new_connref(connP);
             if (srcCnPnt.get_port()->get_no_buses() == 0 ||
                 srcCnPnt.get_port()->get_no_buses() > 1)
             {
                // When the sink port is connected but
                // the src port is not, or the src port is
                // connected with wires
                srcCnPnt.get_port()->add_new_bus(sBusP);
             }
             else
             {
                AX_BusCP srcBusP = srcCnPnt.get_port()->get_bus(0);
                InterfaceE srcType = srcBusP->get_intrfc()->get_intrtype();
                if (srcType == INTRFC_WIRE)
                   srcCnPnt.get_port()->add_new_bus(sBusP);
                else if (srcType == INTRFC_BUS)
                {
                   if (!merge_buses(sBusP, srcBusP, ag))
                   {
#if DEBUG >= 1
                      cerr << ag.get_name() << ": ";
                      cerr << "Cannot merge buses due to library limitation."
                           << endl << "Functional blocks on bus 1: ";
                      AX_ConnPntLCIter cnPntI;
                      for (cnPntI = sBusP->get2_first_connpnt();
                           cnPntI != sBusP->connpnt_list_end(); cnPntI++)
                      {
                          if ((*cnPntI).get_comp() != NULL)
                             cerr << (*cnPntI).get_comp()->get_name();
                          else
                             cerr << ag.get_name();
                          cerr << "."
                               << (*cnPntI).get_port()->get_name();
                          if (cnPntI != sBusP->get2_last_connpnt())
                               cerr << ", ";
                          else 
                             cerr << endl;
                      }
                      cerr << "\nFunction blocks on bus 2: ";
                      for (cnPntI = srcBusP->get2_first_connpnt();
                           cnPntI != srcBusP->connpnt_list_end(); cnPntI++)
                      {
                          if ((*cnPntI).get_comp() != NULL)
                             cerr << (*cnPntI).get_comp()->get_name();
                          else
                             cerr << ag.get_name();
                          cerr << "."
                               << (*cnPntI).get_port()->get_name();
                          if (cnPntI != srcBusP->get2_last_connpnt())
                               cerr << ", ";
                          else 
                             cerr << endl;
                      }
#endif
                      return false;
                   }
                   if (sBusP != srcBusP)
                   {
                      srcCnPnt.get_port()->add_new_bus(sBusP);
                      srcBusP = NULL;
                   }
                }
                else
                {
#if DEBUG >= 1
                   cerr << "Interface "
                        << srcBusP->get_intrfc()->get_name()
                        << " intrtype incorrect\n";
#endif
                   return false;
                }
             }
#if DEBUG >= 5
             cerr << "After bus insertion to the src port:" << endl;
             srcCnPnt.get_port()->dump();
#endif
             return true;
          } // if sink port is wired with a bus
          else
          {
#if DEBUG >= 1
             cerr << "Interface "
                  << sBusP->get_intrfc()->get_name()
                  << " intrtype incorrect\n";
#endif
             return false;
          }
       } // if sink port has one bus wired


       if (srcCnPnt.get_port()->get_no_buses() == 0)
       {
          // When both src and sink ports are not connected.
          // This is known because the connected sink port
          // is handled in the previous section.

          AX_InterfaceCP selIntrfcP = NULL;
          AX_BusLCIter nBusI;
          AX_BusPLCIter nBusPI;
          AX_BusCP nBusP;
          AX_CompPortCP conPortP;
          AX_FuncBlkCP srcFuncP, sinkFuncP;

          srcFuncP = (AX_FuncBlkCP)srcCnPnt.get_comp();
          sinkFuncP = (AX_FuncBlkCP)sinkCnPnt.get_comp();
          if (srcFuncP != NULL &&
              srcFuncP->get_functype() == FUNC_DISTRIBUTOR &&
              (conPortP = srcFuncP->any_connected_outport()) == NULL)
          {
             // if the source comp is a distributor and none of it's
             // output ports are connected
             AX_CompPortCP srcPortP, sinkPortP;
             srcPortP = srcCnPnt.get_port();
             sinkPortP = sinkCnPnt.get_port();
             AX_InterfacePLC intrfcPL;
             ag.get_intrfc_candidates(srcPortP, sinkPortP,
                                      intrfcPL, GET_BUS_ONLY);
             if (intrfcPL.size() == 0)
             {
                cerr << "Library limitation: connect()-1 Cannot find "
                     << "interfaces between ";
                if (srcPortP->get_ag() != NULL)
                   cerr << ag.get_name();
                else
                   cerr << srcPortP->get_comp()->get_name();
                cerr << "." << srcPortP->get_name() << " -> ";
                if (sinkPortP->get_ag() != NULL)
                   cerr << ag.get_name();
                else
                   cerr << sinkPortP->get_comp()->get_name();
                cerr << "." << sinkPortP->get_name() << endl;
                AX_InterfacePLCIter canI;
                for (canI = intrfcPL.begin();
                     canI != intrfcPL.end(); canI++)
                    (*canI)->dump();
                return false;
             }
             int rNum = my_rand(intrfcPL.size());
             AX_InterfacePLCIter rIter = intrfcPL.begin();
             for (unsigned int k = 0; k < intrfcPL.size(); k++)
             {
                 if (rNum == (int)k)
                 {
                    selIntrfcP = *rIter;
                    break;
                 }
                 rIter++;
             }
             AX_BusC nBus;
             nBusI = ag.add_new_bus(nBus);
             nBusP = &(*nBusI);
          }
          else if (srcFuncP != NULL &&
                   srcFuncP->get_functype() == FUNC_DISTRIBUTOR &&
                   (conPortP = srcFuncP->any_connected_outport()) != NULL)
          {
             // if the source comp is a distributor and any of it's
             // output ports are connected, get the connecting bus
             nBusPI = conPortP->get2_first_bus();
             nBusP = (*nBusPI);
             selIntrfcP = nBusP->get_intrfc();
          }
          else if (sinkFuncP != NULL &&
                   sinkFuncP->get_functype() == FUNC_COLLECTOR &&
                   (conPortP = sinkFuncP->any_connected_inport()) != NULL)
          {
             // if the sink comp is a collector and any of it's
             // output ports are connected, get the connecting bus
             nBusPI = conPortP->get2_first_bus();
             nBusP = (*nBusPI);
             selIntrfcP = nBusP->get_intrfc();
          }
          else
          {
             // nothing to do with distributor and collector, we are
             // homefree for randomly create a bus
             AX_CompPortCP srcPortP, sinkPortP;
             srcPortP = srcCnPnt.get_port();
             sinkPortP = sinkCnPnt.get_port();
             AX_InterfacePLC intrfcPL;
             ag.get_intrfc_candidates(srcPortP, sinkPortP,
                                      intrfcPL, GET_ANY_TYPE);
             if (intrfcPL.size() == 0)
             {
                cerr << "Library limitation: connect()-2 Cannot find "
                     << "interfaces between ";
                if (srcPortP->get_ag() != NULL)
                   cerr << ag.get_name();
                else
                   cerr << srcPortP->get_comp()->get_name();
                cerr << "." << srcPortP->get_name() << " -> ";
                if (sinkPortP->get_ag() != NULL)
                   cerr << ag.get_name();
                else
                   cerr << sinkPortP->get_comp()->get_name();
                cerr << "." << sinkPortP->get_name() << endl;
                AX_InterfacePLCIter canI;
                for (canI = intrfcPL.begin();
                     canI != intrfcPL.end(); canI++)
                    (*canI)->dump();
                return false;
             }
             int rNum = my_rand(intrfcPL.size());
             AX_InterfacePLCIter rIter = intrfcPL.begin();
             for (unsigned int k = 0; k < intrfcPL.size(); k++)
             {
                 if (rNum == (int)k)
                 {
                    selIntrfcP = *rIter;
                    break;
                 }
                 rIter++;
             }
             intrfcPL.clear();
             AX_BusC nBus;
             nBusI = ag.add_new_bus(nBus);
             nBusP = &(*nBusI);
          }

          nBusP->set_id(bNum++);
          nBusP->set_intrfc(selIntrfcP);
          nBusP->add_new_connref(connP);
          nBusP->add_new_connpnt(srcCnPnt);
          nBusP->add_new_connpnt(sinkCnPnt);
#if DEBUG >= 4
          if (sinkFuncP != NULL)
             if (sinkFuncP->get_functype() == PROCESSOR)
                cerr << sinkFuncP->get_name() << "."
                     << sinkCnPnt.get_port()->get_name()
                     << " connect to "
                     << nBusP->get_intrfc()->get_name() << endl;
#endif
          if (srcTaskP == NULL)
          {
             (*gPortI).add_new_bus(nBusP);
             sinkCnPnt.get_port()->add_new_bus(nBusP);
          }
          else if (sinkTaskP == NULL)
          {
             (*gPortI).add_new_bus(nBusP);
             srcCnPnt.get_port()->add_new_bus(nBusP);
          }
          else
          {
             srcCnPnt.get_port()->add_new_bus(nBusP);
             sinkCnPnt.get_port()->add_new_bus(nBusP);
          }
        }
       else // end if both src and sink ports are not connected
       {
          // When src port is connected but sink port is not.

          AX_BusCP busP;
          int sbNum = my_rand(srcCnPnt.get_port()->get_no_buses());
          busP = srcCnPnt.get_port()->get_bus(sbNum);

          if (busP->get_intrfc()->get_intrtype() == INTRFC_BUS)
          {
             busP->add_new_connpnt(sinkCnPnt);
             busP->add_new_connref(connP);
             sinkCnPnt.get_port()->add_new_bus(busP);
          } // end INTRFC_BUS
          else if (busP->get_intrfc()->get_intrtype() == INTRFC_WIRE)
          {
             AX_InterfacePLC intrfcPL;
             CandidateTypeE selTypeE;

             // if the sink comp is processor or memory then
             // we can only select a bus type interface
             AX_FuncBlkCP tFP;
             tFP = (AX_FuncBlkCP)sinkCnPnt.get_comp();
             if (sinkCnPnt.get_comp() != NULL &&
                 (tFP->get_functype() == PROCESSOR ||
                  tFP->get_functype() == MEMORY ||
                  tFP->get_functype() == MEMORY_CORE))
                selTypeE = GET_BUS_ONLY;
             else
                selTypeE = GET_ANY_TYPE;
             AX_CompPortCP srcPortP, sinkPortP;
             srcPortP = srcCnPnt.get_port();
             sinkPortP = sinkCnPnt.get_port();
             ag.get_intrfc_candidates(srcPortP, sinkPortP,
                                      intrfcPL, selTypeE);
             if (intrfcPL.size() == 0)
             {
                cerr << "Library limitation: connect()-3 Cannot find "
                     << "interfaces between ";
                if (srcPortP->get_ag() != NULL)
                   cerr << ag.get_name();
                else
                   cerr << srcPortP->get_comp()->get_name();
                cerr << "." << srcPortP->get_name()
                     << " -> ";
                if (sinkPortP->get_ag() != NULL)
                   cerr << ag.get_name();
                else
                   cerr << sinkPortP->get_comp()->get_name();
                cerr << "." << sinkPortP->get_name() << endl;
                AX_InterfacePLCIter canI;
                for (canI = intrfcPL.begin();
                     canI != intrfcPL.end(); canI++)
                    (*canI)->dump();
                return false;
             }
             int rNum = my_rand(intrfcPL.size());
             AX_InterfaceCP selIntrfcP = NULL;
             AX_InterfacePLCIter rIter = intrfcPL.begin();
             for (unsigned int k = 0; k < intrfcPL.size(); k++)
             {
                 if (rNum == (int)k)
                 {
                    selIntrfcP = *rIter;
                    break;
                 }
                 rIter++;
             }
             intrfcPL.clear();
   
             AX_BusC nBus;
             AX_BusLCIter nBusI;
             nBusI = ag.add_new_bus(nBus);
             (*nBusI).set_id(bNum++);
             (*nBusI).set_intrfc(selIntrfcP);
             (*nBusI).add_new_connref(connP);
             (*nBusI).add_new_connpnt(srcCnPnt);
             (*nBusI).add_new_connpnt(sinkCnPnt);
             if (srcTaskP == NULL)
             {
                (*gPortI).add_new_bus(&(*nBusI));
                sinkCnPnt.get_port()->add_new_bus(&(*nBusI));
             }
             else if (sinkTaskP == NULL)
             {
                (*gPortI).add_new_bus(&(*nBusI));
                srcCnPnt.get_port()->add_new_bus(&(*nBusI));
             }
             else
             {
                srcCnPnt.get_port()->add_new_bus(&(*nBusI));
                sinkCnPnt.get_port()->add_new_bus(&(*nBusI));
             }
          } // end INTRFC_WIRE
          else
          {
#if DEBUG >= 1
             cerr << "Interface "
                  << busP->get_intrfc()->get_name()
                  << " intrtype incorrect\n";
#endif
             return false;
          } // end INTRFC if
       } // end if src port is connected but sink port is not

       return true;
   }
// ------------------------------------------------------------
   bool AX_EvolveC::mend_connect(AX_DsnC & dsn, AX_ArchGraphC & ag)
   {
      // check conns
      AX_ConnLCIter connI;
      int bNum = (*ag.get2_last_bus()).get_id();

      for (connI = dsn.get_tg()->get2_first_conn();
           connI != dsn.get_tg()->conn_list_end(); connI++)
      {
         AX_BusLCIter busI;
         bool found = false;
         for (busI = ag.get2_first_bus();
              busI != ag.bus_list_end(); busI++)
         {
            //AX_ConnPntCP connPntP;
            if ((*busI).is_mapped_conn(&(*connI)))
            {
               found = true;
               break;
            } // end if is_mapped_conn
         } // end bus for
         if (!found)
         {
            if ((*connI).get_src_task() != NULL &&
                (*connI).get_sink_task() != NULL)
            {
               // source and sink tasks run on the same component,
               // so there is no connection mapping, Not an error
               AX_FuncBlkCP srcFuncP, sinkFuncP;
               AX_TaskCP srcTaskP, sinkTaskP;
               srcTaskP = (*connI).get_src_task();
               sinkTaskP = (*connI).get_sink_task();
               srcFuncP = ag.get_fncblk_by_task(srcTaskP);
               sinkFuncP = ag.get_fncblk_by_task(sinkTaskP);
               if (srcFuncP == sinkFuncP)
                  continue;
               AX_FuncBlkPLCIter srcPI, sinkPI;
               for (srcPI = srcFuncP->get2_first_dep();
                    srcPI != srcFuncP->dep_list_end(); srcPI++)
                  if ((*srcPI)->get_functype() == PROCESSOR)
                     break;
               for (sinkPI = sinkFuncP->get2_first_dep();
                   sinkPI != sinkFuncP->dep_list_end(); sinkPI++)
                  if ((*sinkPI)->get_functype() == PROCESSOR)
                     break;
               if (srcPI != srcFuncP->dep_list_end() &&
                   sinkPI != sinkFuncP->dep_list_end() &&
                   (*srcPI) == (*sinkPI))
                  continue;
            }

            // Unmapped connection found, let's map it
            AX_TaskCP srcTaskP, sinkTaskP;
            AX_TaskPortCP srcPortP, sinkPortP;
            AX_TaskGraphCP srcTgP, sinkTgP;

            srcTgP = (*connI).get_src_tg();
            sinkTgP = (*connI).get_sink_tg();
            srcTaskP = (*connI).get_src_task();
            sinkTaskP = (*connI).get_sink_task();
            srcPortP = (*connI).get_src_port();
            sinkPortP = (*connI).get_sink_port();

            // connecting the components by taskgraph connections
            if (!connect(ag, srcTgP, srcTaskP, srcPortP,
                         sinkTgP, sinkTaskP, sinkPortP, bNum, &(*connI)))
               return false;
         } // end !found if
      } // end conn for

      return true;
   } 
// ------------------------------------------------------------
   bool AX_EvolveC::mend_bus(AX_ArchGraphC & ag)
   {
      AX_BusLCIter busI;
      bool stepback = false;

      for (busI = ag.get2_first_bus();
           busI != ag.bus_list_end(); busI++)
      {
         if (stepback)
         {
            busI = ag.get2_first_bus();
            stepback = false;
         }
         if ((*busI).get_no_connpnts() <= 1)
         {
            if ((*busI).get_no_connpnts() == 1)
            {
               AX_ConnPntLCIter conpntI;
               conpntI = (*busI).get2_first_connpnt();
               (*conpntI).get_port()->delete_bus(&(*busI));
            }
            (*busI).remove_all_connpnts();
            busI = ag.delete_bus(busI);
            if (busI != ag.get2_first_bus())
               busI--;
            else
               stepback = true;
         }
      } // end checking useless buses

      return true;
   }
// ------------------------------------------------------------
   bool AX_EvolveC::merge_buses(AX_BusCP aBusP,
                                    AX_BusCP bBusP,
                                    AX_ArchGraphC & ag)
   {
       if (aBusP == bBusP)
          return true;

       if (!(aBusP->get_intrfc() == bBusP->get_intrfc() &&
             aBusP->get_intrfc()->get_intrtype() == INTRFC_BUS))
       {
          // Other than the condition that aBusP and bBusP
          // are of the same type of interface and is a bus.
          // See if we can find a common interfaces

          AX_InterfaceCP newIntrfc;
          newIntrfc = select_intrfc_fm_buses(aBusP, bBusP, ag);
          if (newIntrfc == NULL)
             return false;

          // change the interface
          aBusP->set_intrfc(newIntrfc);
       }

       // merge connection points on bBusP to aBusP
       AX_ConnPntLCIter cnPntI;
       for (cnPntI = bBusP->get2_first_connpnt();
            cnPntI != bBusP->connpnt_list_end(); cnPntI++)
       {
          (*cnPntI).get_port()->remove_bus(bBusP);
          (*cnPntI).get_port()->add_new_bus(aBusP);
          aBusP->add_new_connpnt(*cnPntI);
       }

       // merge connection reference on bBusP to aBusP
       AX_ConnRefLSIter conRefI;
       for (conRefI = bBusP->get2_first_connref();
            conRefI != bBusP->connref_list_end(); conRefI++)
       {
          AX_ConnRefS newRef;
          newRef.m_connP = NULL;
          AX_ConnRefLSIter newRefI;
          newRefI = aBusP->add_new_connref(newRef);
          (*newRefI).m_connP = (*conRefI).m_connP;
          (*newRefI).m_start = (*conRefI).m_start;
          (*newRefI).m_finish = (*conRefI).m_finish;
          (*conRefI).m_start = 0;
          (*conRefI).m_finish = 0;
       }
       bBusP->remove_all_connpnts();
       bBusP->remove_all_connrefs();
       ag.delete_bus(bBusP);

       return true;
   }
// ------------------------------------------------------------
   AX_InterfaceCP AX_EvolveC::select_intrfc_fm_buses(AX_BusCP aBusP,
                                                       AX_BusCP bBusP,
                                                       AX_ArchGraphC & ag)
   {
       AX_InterfacePLC intrfcPL;
       ag.collect_all_bus_intrfcs(intrfcPL);

       AX_ConnPntLCIter cnPntI;
       for (cnPntI = aBusP->get2_first_connpnt();
            cnPntI != aBusP->connpnt_list_end(); cnPntI++)
       {
          if ((*cnPntI).get_port() != NULL)
          {
             AX_CompPortCP portP = (*cnPntI).get_port();
             portP->find_common_intrfc(intrfcPL);
          }
       }
       for (cnPntI = bBusP->get2_first_connpnt();
            cnPntI != bBusP->connpnt_list_end(); cnPntI++)
       {
          if ((*cnPntI).get_port() != NULL)
          {
             AX_CompPortCP portP = (*cnPntI).get_port();
             portP->find_common_intrfc(intrfcPL);
          }
       }
       // cannot find any bus interface in common
       if (intrfcPL.size() == 0)
          return NULL;

       // yes we got some interfaces in common. Randomly
       // select one as the new interface
       int inNum = my_rand(intrfcPL.size());
       AX_InterfaceCP selIntrfcP = NULL;
       AX_InterfacePLCIter rIter = intrfcPL.begin();
       for (unsigned int k = 0; k < intrfcPL.size(); k++)
       {
           if (inNum == (int)k)
           {
              selIntrfcP = *rIter;
              break;
           }
           rIter++;
       }

       return selIntrfcP;
   }
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Operations
// ------------------------------------------------------------
   void AX_EvolveC::fix(AX_DsnC & dsn)
   {
   }

// ------------------------------------------------------------
   void AX_EvolveC::copy_funcblk_platfm(AX_ArchGraphC & ag,
                                         AX_FuncBlkC & nFuncC,
                                         AX_FuncBlkC & oFuncC)
   {
       // constructing platform pointer
       AX_PlatformCP nPlatfmP = NULL, oPlatfmP;
       oPlatfmP = oFuncC.get_platfm();
       if (oPlatfmP != NULL)
          nPlatfmP = ag.get_platfm_by_id(oPlatfmP->get_id());
       nFuncC.set_platfm(nPlatfmP);
   }

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Operations
   void AX_EvolveC::get_ag_data_0(AX_ArchGraphC & ag)
   {
       AX_FuncBlkLCIter fIter;
       AX_InterfaceLCIter iIter;
       int totPwr = 0, totArea = 0;
       float totCost = 0.0;
       AX_UAttrCP uattrP;
       int idCnt = 0;
       //bool hwFunc = false;

       for (fIter = ag.get2_first_fncblk();
            fIter != ag.fncblk_list_end(); fIter++)
       {
          if ((*fIter).get_type() == HARDWARE)
          {
             totPwr += (*fIter).get_run_pwr();
             if ((*fIter).get_functype() == MEMORY_CORE)
                totArea += MEMCORE_AREA;  // memory core area
             else if ((*fIter).get_functype() == FUNCTION)
                totArea += ASIC_AREA;     // asic area
             else
             {
                uattrP = (*fIter).get_uattr(UATTRSIZE);
                totArea += my_atoi(uattrP->get_value());
             }
          }

          uattrP = (*fIter).get_uattr(UATTRBASECOST);
          totCost += (float)atof(uattrP->get_value().c_str());

          // clean up the id
          (*fIter).set_id(idCnt++);
       }

       // add ASIC area
       // if (hwFunc)
       //    totArea += ASIC_AREA;

       ag.set_area(totArea);
       ag.set_power(totPwr);
       ag.set_cost(totCost);

       return;
   }
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Operations
   void AX_EvolveC::get_ag_data_1(AX_ArchGraphC & ag)
   {
       AX_FuncBlkLCIter fIter;
       AX_InterfaceLCIter iIter;
       int totPwr = 0, totArea = 0, totGate = 0;
       float totCost = 0.0;
       AX_UAttrCP uattrP;
       int idCnt = 0;
       //bool hwFunc = false;

       for (fIter = ag.get2_first_fncblk();
            fIter != ag.fncblk_list_end(); fIter++)
       {
          if ((*fIter).get_type() == HARDWARE)
          {
             totPwr += (*fIter).get_run_pwr();
             if ((*fIter).get_functype() == MEMORY_CORE)
                totArea += MEMCORE_AREA;  // memory core area
             else if ((*fIter).get_functype() == FUNCTION)
             {
                uattrP = (*fIter).get_uattr(UATTRSIZE);
                totGate += my_atoi(uattrP->get_value());
             }
             else
             {
                uattrP = (*fIter).get_uattr(UATTRSIZE);
                totArea += my_atoi(uattrP->get_value());
             }
          }

          uattrP = (*fIter).get_uattr(UATTRBASECOST);
          totCost += (float)atof(uattrP->get_value().c_str());

          // clean up the id
          (*fIter).set_id(idCnt++);
       }
       totArea += (int)ceil((float)totGate/(float)MAXGATECOUNT) * ASIC_AREA;

       // add ASIC area
       // if (hwFunc)
       //    totArea += ASIC_AREA;

       ag.set_area(totArea);
       ag.set_power(totPwr);
       ag.set_cost(totCost);

       return;
   }

// ------------------------------------------------------------
// ------------------------------------------------------------
// fitness functions:
//    f0 = (alpha * 1/area + beta * 1/cost + gama * 1/power)  
//    f1 = (alpha * area + beta * cost + gama * power)  
//    f2 = 1 / (alpha * area + beta * cost + gama * power)  
// ------------------------------------------------------------
   void AX_EvolveC::fitness(AX_DsnC & dsn, float alpha,
                             float beta, float gama)
   {
       AX_ArchGraphLCIter agIter;
       float fitVal;

       for (agIter = dsn.get2_first_ag();
            agIter != dsn.ag_list_end(); agIter++)
       {
          if (ASIC_COMP_METHOD == 0)
             get_ag_data_0(*agIter);
          else if (ASIC_COMP_METHOD == 1)
             get_ag_data_1(*agIter);
          fitVal = fitness((*agIter), alpha, beta, gama);
          AX_DsnSpecCP specP = dsn.get_dsnspec(0);
          if (specP != NULL)
          {
             if ((*agIter).get_latency() > specP->get_max_latency() ||
                 (*agIter).get_area() > specP->get_max_area() ||
                 (*agIter).get_power() > specP->get_max_power())
                fitVal = 0.0;
          }
          AX_FuncBlkLCIter funcI;
          for (funcI = (*agIter).get2_first_fncblk();
               funcI != (*agIter).fncblk_list_end(); funcI++)
          {
             if ((*funcI).get_functype() == PROCESSOR)
                if ((*funcI).get_util() > DSP_UTIL_MARGIN)
                   fitVal = 0.0;
          }
          (*agIter).set_fitness(fitVal);
#if DEBUG >= 4
          cerr << (*agIter).get_name() << ": " << fitVal << endl;
#endif
       }
   }

// ------------------------------------------------------------
// fitness functions:
//    f0 = (alpha * 1/area + beta * 1/cost + gama * 1/power)  
//    f1 = (alpha * area + beta * cost + gama * power)  
//    f2 = 1 / (alpha * area + beta * cost + gama * power)  
// ------------------------------------------------------------
   float AX_EvolveC::fitness(AX_ArchGraphC & ag, float alpha,
                              float beta, float gama)
   {
       float totArea = 0.0, totCost = 0.0, totPwr = 0.0;

       totArea = (float)ag.get_area();
       totCost = ag.get_cost();
       totPwr = (float)ag.get_power();

       totArea *= ALPHA_NORM;
       totCost *= BETA_NORM;
       totPwr *= GAMA_NORM;

       float dominant;

       // default function is f0
       if (FIT_FUNC == 1)
       {
          dominant = (alpha * totArea) +
                     (beta * totCost) +
                     (gama * totPwr);
       }
       else if (FIT_FUNC == 2)
       {
          dominant = (alpha * totArea) +
                     (beta * totCost) +
                     (gama * totPwr);
          if (dominant != 0.0)
             dominant = 1 / dominant;
          else
             dominant = LARGEINTEGER;
       }
       else
       {
          if (totArea == 0 && totCost != 0.0 && totPwr != 0)
             dominant = beta * (1/totCost) + gama * (1/totPwr);
          else if (totArea != 0 && totCost == 0.0 && totPwr != 0)
             dominant = alpha * (1/totArea) + gama * (1/totPwr);
          else if (totArea != 0 && totCost != 0.0 && totPwr == 0)
             dominant = alpha * (1/totArea) + beta * (1/totCost);
          else if (totArea != 0 && totCost == 0.0 && totPwr == 0)
             dominant = alpha * (1/totArea);
          else if (totArea == 0 && totCost != 0.0 && totPwr == 0)
             dominant = beta * (1/totCost);
          else if (totArea == 0 && totCost == 0.0 && totPwr != 0)
             dominant = gama * (1/totPwr);
          else if (totArea == 0 && totCost == 0.0 && totPwr == 0)
             dominant = (float)LARGEINTEGER;
          else
             dominant = alpha * (1/totArea) +
                        beta * (1/totCost) +
                        gama * (1/totPwr);
       }

       return dominant;
   }

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Operations
// ------------------------------------------------------------
   void AX_EvolveC::merge_pools(AX_DsnC & dsn, AX_ArchGraphLC & agL)
   {
       AX_ArchGraphLCIter aIter;
       for (aIter = agL.begin(); aIter != agL.end(); aIter++)
       {
           AX_ArchGraphC nArch;
           AX_ArchGraphLCIter nArchI;
           nArchI = dsn.add_new_ag(nArch);
           (*nArchI).copy(*aIter);
       }
   }
// ------------------------------------------------------------
   void AX_EvolveC::get_normalizing_factors(AX_DsnC & dsn)
   {
       float totArea = 0.0, totPwr = 0.0, totCost = 0.0;
       int Count = 0;
       AX_ArchGraphLCIter aIter;

       for (aIter = dsn.get2_first_ag();
            aIter != dsn.ag_list_end(); aIter++)
       {
          if (ASIC_COMP_METHOD == 0)
             get_ag_data_0(*aIter);
          else if (ASIC_COMP_METHOD == 1)
             get_ag_data_1(*aIter);
          totArea += (float)(*aIter).get_area();
          totCost += (float)(*aIter).get_cost();
          totPwr += (float)(*aIter).get_power();
          Count++;
       }

       if (Count > 0)
       {
          totArea /= (float)Count;
          totCost /= (float)Count;
          totPwr /= (float)Count;

          // getting area normalizing factor
          int i = 0;
          while (1)
          {
            if (totArea < 10.0 && totArea >= 0.0)
               break;
            totArea /= 10.0;
            i++;
          }
          ALPHA_NORM = 1.0 / (float)pow((double)10, (double)i);

          // getting cost normalizing factor
          i = 0;
          if (totCost > 1.0)
          {
             while (1)
             {
                if (totCost < 10.0 && totCost >= 1.0)
                   break;
                totCost /= 10.0;
                i++;
             }
          }
          else
          {
             while (1)
             {
                if (totCost < 10.0 && totCost >= 1.0)
                   break;
                totCost *= 10.0;
                i--;
             }
          }
          BETA_NORM = 1.0 / (float)pow((double)10, (double)i);

          // getting power normalizing factor
          i = 0;
          while (1)
          {
            if (totPwr < 10.0 && totPwr >= 0.0)
               break;
            totPwr /= 10.0;
            i++;
          }
          GAMA_NORM = 1.0 / (float)pow((double)10, (double)i);
       }
       else
       {
          ALPHA_NORM = 1.0;
          BETA_NORM = 1.0;
          GAMA_NORM = 1.0;
       }
   }
// ------------------------------------------------------------
   void AX_EvolveC::rank_fitness(AX_DsnC & dsn,
                                  AX_ArchGraphCP * ranking,
                                  float alpha, float beta, float gama)
   {
       fitness(dsn, alpha, beta, gama);
       AX_ArchGraphLCIter agIter;
       AX_ArchGraphCP agP;
       int tNum = dsn.get_no_ags();
       int idx = 0;

       for (agIter = dsn.get2_first_ag();
            agIter != dsn.ag_list_end(); agIter++)
       {
          agP = &(*agIter);
          ranking[idx] = agP;
          idx++;
       }
       sort_ranking(ranking, tNum);
#if DEBUG >= 4
       for (int i = 0; i < tNum; i++)
           cerr << i << ". " << ranking[i]->get_name()
                << " = " << ranking[i]->get_fitness() << endl;
#endif
   }

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Operations
// ------------------------------------------------------------
   void AX_EvolveC::sort_ranking(AX_ArchGraphCP * ranking, int rSize)
   {
       AX_ArchGraphCP temp;

       for (int i = 0; i < rSize; i++)
          for (int j = 0; j < rSize; j++)
             if (ranking[i]->get_fitness() > ranking[j]->get_fitness())
             {
                temp = ranking[i];
                ranking[i] = ranking[j];
                ranking[j] = temp;
             }
   }
// ------------------------------------------------------------
   bool AX_EvolveC::reassign_sw_port(AX_DsnC & dsn,
                                         AX_ArchGraphC & ag,
                                         AX_FuncBlkCP funcP)
   {
       AX_CompPortPLCIter portPI;
       for (portPI = funcP->get2_first_inport();
            portPI != funcP->inport_list_end(); portPI++)
       {
          if ((*portPI)->get_sw_port() != NULL)
          {
             AX_FuncBlkCP swBlkP;
             swBlkP = AX_FuncBlkCP((*portPI)->get_sw_port()->get_comp());

             AX_FuncBlkPLCIter prBlkPI;
             for (prBlkPI = funcP->get2_first_dep();
                  prBlkPI != funcP->dep_list_end(); prBlkPI++)
             {
                if ((*prBlkPI)->get_functype() == PROCESSOR)
                   break;
             }
             if (prBlkPI == funcP->dep_list_end())
             {
                cerr << ag.get_name() << ": Cannot find a "
                     << swBlkP->get_name()
                     << " in " << funcP->get_name()
                     << "'s depPL\n";
                return false;
             }
             AX_CompPortPLCIter pPortPI;
             for (pPortPI = (*prBlkPI)->get2_first_ioport();
                  pPortPI != (*prBlkPI)->ioport_list_end(); pPortPI++)
             {
                if ((*pPortPI)->get_name() ==
                    (*portPI)->get_sw_port()->get_name())
                {
                   // consider the case where this processor uses
                   // a multi-port memory, then we like to make use
                   // of the empty data port for data communication
                   AX_CompPortCP pPortP = (*pPortPI);
                   AX_FuncBlkPLCIter srchMemPI;
                   AX_FuncBlkCP srchMemP;
                   bool DoMultMem = false;
                   AX_BusCP connBusP;
                   for (srchMemPI = (*prBlkPI)->get2_first_dep();
                        srchMemPI != (*prBlkPI)->dep_list_end(); srchMemPI++)
                   {
                      if ((*srchMemPI)->get_functype() == MEMORY ||
                          (*srchMemPI)->get_functype() == MEMORY_CORE)
                         if ((*srchMemPI)->get_no_ioports() > 1)
                         {
                            connBusP = pPortP->func_on_port(*srchMemPI);
                            // connBusP = null means the referenced memory
                            // is not connected to this port
                            if (connBusP == NULL)
                               break;
                            srchMemP = (*srchMemPI);
                            DoMultMem = true;
                            break;
                         }
                   } // end dep loop to look for memory used
                   if (DoMultMem)
                   {
                      AX_CompPortCP repPortP;
                      repPortP = srchMemP->get_an_empty_ioport(NULL);
                      if (repPortP == NULL)
                      {
                         cerr << "Lib error: " << srchMemP->get_name()
                              << " does not have data ports\n";
                         exit(1);
                      }
                      if (repPortP != pPortP)
                         pPortP = repPortP;
                   }
                   // removed because the refCnt will be treated in
                   // connect()
                   // pPortP->inc_refCnt();

                   AX_CompPortCP oProcPortP = (*portPI)->get_sw_port();
                   (*portPI)->set_sw_port(pPortP);
                   (*portPI)->set_temp(1);
                   ag.adjust_refCnt(oProcPortP);
                   ag.adjust_refCnt(pPortP);
                   break;
                }
             }
             if (pPortPI == (*prBlkPI)->ioport_list_end())
             {
                cerr << ag.get_name() << ": Cannot find port "
                     << (*portPI)->get_sw_port()->get_name()
                     << " in " << (*prBlkPI)->get_name() << endl;
                return false;
             }
          } // end if sw_port != NULL
       } // end inport loop

       for (portPI = funcP->get2_first_outport();
            portPI != funcP->outport_list_end(); portPI++)
       {
          if ((*portPI)->get_sw_port() != NULL)
          {
             AX_FuncBlkCP swBlkP;
             swBlkP = AX_FuncBlkCP((*portPI)->get_sw_port()->get_comp());

             AX_FuncBlkPLCIter prBlkPI;
             for (prBlkPI = funcP->get2_first_dep();
                  prBlkPI != funcP->dep_list_end(); prBlkPI++)
             {
                if ((*prBlkPI)->get_functype() == PROCESSOR)
                   break;
             }
             if (prBlkPI == funcP->dep_list_end())
             {
                cerr << ag.get_name() << ": Cannot find a "
                     << swBlkP->get_name()
                     << " in " << funcP->get_name()
                     << "'s depPL\n";
                return false;
             }
             AX_CompPortPLCIter pPortPI;
             for (pPortPI = (*prBlkPI)->get2_first_ioport();
                  pPortPI != (*prBlkPI)->ioport_list_end(); pPortPI++)
             {
                if ((*pPortPI)->get_name() ==
                    (*portPI)->get_sw_port()->get_name())
                {
                   // consider the case where this processor uses
                   // a multi-port memory, then we like to make use
                   // of the empty data port for data communication
                   AX_CompPortCP pPortP = (*pPortPI);
                   AX_FuncBlkPLCIter srchMemPI;
                   AX_FuncBlkCP srchMemP;
                   bool DoMultMem = false;
                   AX_BusCP connBusP;
                   for (srchMemPI = (*prBlkPI)->get2_first_dep();
                        srchMemPI != (*prBlkPI)->dep_list_end(); srchMemPI++)
                   {
                      if ((*srchMemPI)->get_functype() == MEMORY ||
                          (*srchMemPI)->get_functype() == MEMORY_CORE)
                         if ((*srchMemPI)->get_no_ioports() > 1)
                         {
                            connBusP = pPortP->func_on_port(*srchMemPI);
                            // connBusP = null means the referenced memory
                            // is not connected to this port
                            if (connBusP == NULL)
                               break;
                            srchMemP = (*srchMemPI);
                            DoMultMem = true;
                            break;
                         }
                   } // end dep loop to look for memory used
                   if (DoMultMem)
                   {
                      AX_CompPortCP repPortP;
                      repPortP = srchMemP->get_an_empty_ioport(NULL);
                      if (repPortP == NULL)
                      {
                         cerr << "Lib error: " << srchMemP->get_name()
                              << " does not have data ports\n";
                         exit(1);
                      }
                      if (repPortP != pPortP)
                         pPortP = repPortP;
                   }
                   // removed because the refCnt will be treated in
                   // connect()
                   // pPortP->inc_refCnt();

                   AX_CompPortCP oProcPortP = (*portPI)->get_sw_port();
                   (*portPI)->set_sw_port(pPortP);
                   (*portPI)->set_temp(1);
                   ag.adjust_refCnt(oProcPortP);
                   ag.adjust_refCnt(pPortP);
                   break;
                }
             }
             if (pPortPI == (*prBlkPI)->ioport_list_end())
             {
                cerr << ag.get_name() << ": Cannot find port "
                     << (*portPI)->get_sw_port()->get_name()
                     << " in " << (*prBlkPI)->get_name() << endl;
                return false;
             }
          } // end if sw_port != NULL
       } // end outport loop

       return true;
   }
// ------------------------------------------------------------
// Currently the dependency list is only used by software.
// Also the components in the dependency list, which should
// be CPU, memory, and maybe plus DMA, are connected using a
// single bus.
// ------------------------------------------------------------
   bool AX_EvolveC::add_dependent_comps(AX_DsnC & dsn,
                                            AX_ArchGraphC & ag,
                                            int & fNum, int & pNum,
                                            int & bNum)
   {
       AX_FuncBlkLCIter fIter;

       // get the memory first, for we need to get address
       // bus for the memory then we can hook up CPU's
       for (fIter = ag.get2_first_fncblk();
            fIter != ag.fncblk_list_end(); fIter++)
          add_dependent_comps(dsn, ag, (*fIter), fNum, pNum, bNum);

       return true;
   }
// ------------------------------------------------------------
   bool AX_EvolveC::add_dependent_comps(AX_DsnC & dsn,
                                            AX_ArchGraphC & ag,
                                            AX_FuncBlkC & funcC,
                                            int & fNum, int & pNum,
                                            int & bNum)
   {
       if (funcC.get_no_fncblks() <= 0)
          return true;

       AX_FuncBlkPLCIter fpIter;
       AX_FuncBlkLCIter fIter;
       int refSize = 0;
       int refCodeSize = 0;
       int refDataSize = 0;
       int codeCapRem = 0;
       int dataCapRem = 0;
       int genCapRem = 0;
       AX_FuncBlkCP refProcP = NULL;
       AX_UAttrCP uattrP;
       char buf1[128];

       // First find out how may processors are there in this
       // family, then randomly pick one, and just only one
       int nProc = 0, selProc = 0;
       for (fpIter = funcC.get2_first_fncblk();
            fpIter != funcC.fncblk_list_end(); fpIter++)
           if ((*fpIter)->get_functype() == PROCESSOR)
              nProc++;
       if (nProc <= 0)
       {
           cerr << "Cannot find dependent processors in "
                << funcC.get_name();
           return false;
       }
       selProc = my_rand(nProc);
       nProc = 0;

#if DEBUG >= 4
       cerr << "AX_EvolveC::add_dependent_comps-0 in "
            << ag.get_name() << endl;
#endif
       for (fpIter = funcC.get2_first_fncblk();
            fpIter != funcC.fncblk_list_end(); fpIter++)
       {
           // special treatment for memory requirement
           if ((*fpIter)->get_functype() == MEMREQ)
           {
#if DEBUG >= 4
              cerr << "AX_EvolveC::add_dependent_comps-1 found MEMREQ for "
                   << funcC.get_name() << ", " << (*fpIter)->get_name() <<  endl;
#endif
              bool codeMem = true;
              int oneSize = mem_parse((*fpIter)->get_name(),
                                        codeMem);
              refSize += oneSize;
              if (codeMem)
                 refCodeSize += oneSize;
              else
                 refDataSize += oneSize;
           }
           else if ((*fpIter)->get_functype() == PROCESSOR)
           {
              if (nProc != selProc)
              {
                 nProc++;
                 continue;
              }
              else
                 nProc++;

#if DEBUG >= 4
              cerr << "AX_EvolveC::add_dependent_comps-2 found proc for "
                   << funcC.get_name() << ", " << (*fpIter)->get_name() <<  endl;
#endif
              refProcP = (*fpIter);

              // get processor local memory sizes
              float temSize;
              uattrP = (*fpIter)->get_uattr(UATTRONCHIPMEM);
              if (uattrP != NULL)
              {
                 temSize = my_atoi(uattrP->get_value());
                 genCapRem = int(temSize * 1024);
              }
              uattrP = (*fpIter)->get_uattr(UATTRONCHIPCODEMEM);
              if (uattrP != NULL)
              {
                 temSize = my_atoi(uattrP->get_value());
                 codeCapRem = int(temSize * 1024);
              }
              uattrP = (*fpIter)->get_uattr(UATTRONCHIPDATAMEM);
              if (uattrP != NULL)
              {
                 temSize = my_atoi(uattrP->get_value());
                 dataCapRem = int(temSize * 1024);
              }
           }
       }

       if (refSize <= 0)
       {
#if DEBUG >= 1
          cerr << funcC.get_name()
               << " does not have MEM_DATA and MEM_CODE specified "
               << "in the library\n";
#endif
       }

       // adding necessary memories
       bool localMem = false;
       int capRem = 0;
       AX_FuncBlkCP refMemP = NULL;
       AX_BusCP refBusP = NULL;

       uattrP = refProcP->get_uattr(UATTRARCHWIDTH);
       int archwidth = my_atoi(uattrP->get_value());
       // check local memory first
       if (refCodeSize <= codeCapRem &&
           refDataSize <= dataCapRem)
       {
          codeCapRem -= refCodeSize;
          dataCapRem -= refDataSize;
          localMem = true;
#if DEBUG >= 5
          cerr << "AX_EvolveC::add_dependent_comps-3 proc has enough memory." << endl;
#endif
       }
       if (!localMem)
          if (refCodeSize <= genCapRem &&
              refDataSize <= dataCapRem)
          {
             genCapRem -= refCodeSize;
             dataCapRem -= refDataSize;
             localMem = true;
#if DEBUG >= 5
          cerr << "AX_EvolveC::add_dependent_comps-4 proc has enough memory." << endl;
#endif
          }
       if (!localMem)
          if ((refCodeSize + refDataSize) <= genCapRem)
          {
             genCapRem -= refCodeSize;
             genCapRem -= refDataSize;
             localMem = true;
#if DEBUG >= 5
          cerr << "AX_EvolveC::add_dependent_comps-5 proc has enough memory." << endl;
#endif
          }

       // local memory not big enough, get off-chip memory
       if (!localMem)
       {
#if DEBUG >= 3
          cerr << "AX_EvolveC::add_dependent_comps-6 need external memory in "
               << ag.get_name() << "." << funcC.get_name() << endl;
#endif
          AX_FuncBlkC nFunc;
          AX_FuncBlkLCIter nfIter;

          // first we pick a memory in the architecture with
          // largest capacity remains
          refMemP = ag.pick_largest_mem(capRem);

          // if no memory exists or the capacity does not meet the
          // requirement we create a new memory for it
          if (refMemP == NULL ||
              (refMemP != NULL && capRem < refSize))
          {
             refMemP = dsn.ran_pick_mem(capRem, refSize, archwidth);
             if (refMemP == NULL)
             {
                cerr << "Cannot find a memory in the library "
                     << "with capacity more than "
                     << refSize << "B\n";
                return false;
             }

             uattrP = refMemP->get_uattr(UATTRARCHWIDTH);

             // allocate memory size for a memory core
             if (refMemP->get_functype() == MEMORY_CORE)
             {
                // for a memory core, the size is in bits
                if (capRem == 0)
                {
                   cerr << "Lib erryr: memory "
                        << refMemP->get_name()
                        << " has a zero OnChipMem value: "
                        << capRem << endl;
                   exit(1);
                }
                int multi;
                if (capRem != 0)
                   multi = int((refSize * 8)/capRem) + 1;
                else
                   multi = refSize * 8;
                int bitSize = (capRem * multi) + 8;
                capRem = (int)(bitSize / 8);
             }

             nfIter = ag.add_fncblk(nFunc);
             (*nfIter).copyBasic(*refMemP);

             string extID;
             my_itoa(fNum, buf1);
             extID = (*nfIter).get_name() + "_";
             extID = extID + buf1;
             (*nfIter).set_name(extID);

             (*nfIter).remove_all_taskrefs();
             (*nfIter).set_id(fNum++);
             (*nfIter).add_new_ref(&funcC);
             ag.add_compports_by_fncblk(refMemP, &(*nfIter), pNum);
             funcC.add_new_dep(&(*nfIter));
             refMemP = &(*nfIter);
             refMemP->set_tot_size(capRem);
#if DEBUG >= 3
              cerr << "AX_EvolveC::add_dependent_comps-7 memory added " << endl;
              (*nfIter).dump();
#endif
          }

          // get an empty port. Randomly get a port if no emtpy ports
          AX_CompPortCP portP;
          portP = refMemP->get_an_empty_ioport(NULL);

          if (portP->get_no_buses() == 0)
          {
             // get a bus randomly
             AX_InterfaceCP intrP = portP->ran_pick_intrfc();
             refBusP = ag.ran_pick_bus(intrP, bNum);
             refBusP->set_intrfc(intrP);
#if DEBUG >= 4
             if (refBusP->get_intrfc()->get_intrtype() == INTRFC_WIRE)
                cerr << "(b)" << refMemP->get_name()
                     << " is connected with "
                     << refBusP->get_intrfc()->get_name()
                     << endl;
#endif

             AX_ConnPntLCIter cPntI;
             {
                AX_ConnPntC cPnt;
                cPnt.set_ag(NULL);
                cPnt.set_comp((AX_CompCP)&(*nfIter));
                cPnt.set_port(portP);
                cPntI = refBusP->add_new_connpnt(cPnt);
                portP->add_new_bus(refBusP);
             }
          }
          else
             refBusP = portP->get_bus(0);

          capRem -= refSize;

          AX_UAttrC nUattr;
          string nTmpS = UATTRGENCAPREM;
          nUattr.set_name(nTmpS);
          nUattr.set_type(T_INT);
          char uVal[20];
          my_itoa(capRem, uVal);
          nUattr.set_value(uVal);

          uattrP = refMemP->get_uattr(UATTRGENCAPREM);
          if (uattrP != NULL)
             refMemP->delete_uattr(uattrP);
          refMemP->add_new_uattr(nUattr);
       }

       // handle the processors
       nProc = 0;
       for (fpIter = funcC.get2_first_fncblk();
            fpIter != funcC.fncblk_list_end(); fpIter++)
       {
           // treatment for general funcblk
           if ((*fpIter)->get_functype() != MEMREQ)
           {
              if (nProc != selProc)
              {
                 nProc++;
                 continue;
              }
              else
                 nProc++;

              AX_FuncBlkC nFunc;
              AX_FuncBlkLCIter nfIter;

              nfIter = ag.add_fncblk(nFunc);
              (*nfIter).copyBasic(*(*fpIter));

              string extID;
              my_itoa(fNum, buf1);
              extID = (*nfIter).get_name() + "_";
              extID = extID + buf1;
              (*nfIter).set_name(extID);

              (*nfIter).remove_all_taskrefs();
              (*nfIter).set_id(fNum++);
              (*nfIter).add_new_ref(&funcC);
              ag.add_compports_by_fncblk((*fpIter), &(*nfIter), pNum);
              funcC.add_new_dep(&(*nfIter));

              if (!localMem && refBusP == NULL)
              {
#if DEBUG >= 1
                 cerr << "(b)Cannot find a bus for dependent CPU:\n";
                 cerr << "   Arch: " << ag.get_name() << endl;
                 cerr << "   CPU: " << (*nfIter).get_name() << endl;
                 cerr << "   RefFunc: " << funcC.get_name() << endl;
#endif
                 return false;
              }
              if (!localMem && refMemP == NULL)
              {
#if DEBUG >= 1
                 cerr << "(a)Cannot find a memory for dependent CPU:\n";
                 cerr << "   Arch: " << ag.get_name() << endl;
                 cerr << "   CPU: " << (*nfIter).get_name() << endl;
                 cerr << "   RefFunc: " << (*fIter).get_name()
                      << endl;
#endif
                 return false;
              }
#if DEBUG >= 4
              cerr << "AX_EvolveC::add_dependent_comps-8 proc added " << endl;
              (*nfIter).dump();
#endif

              if (!localMem)
              {
                 (*nfIter).add_new_dep(refMemP);
                 refMemP->add_new_dep(&(*nfIter));
                       
                 AX_ConnPntLCIter cPntI;
                 AX_CompPortCP portP = NULL;
                 AX_CompPortPLC portCanL;
                 AX_CompPortPLCIter portPI;
                 portCanL.clear();

                 uattrP = (*nfIter).get_uattr(UATTRARCHWIDTH);
                 int procWid = my_atoi(uattrP->get_value());
                 for (portPI = (*nfIter).get2_first_ioport();
                      portPI != (*nfIter).ioport_list_end(); portPI++)
                 {
                    if ((*portPI)->get_sample_size() == procWid)
                       portCanL.push_back(*portPI);
                 }
                 if (portCanL.size() == 0)
                 {
                    cerr << "Lib error: " << (*nfIter).get_name()
                         << " does not have data port(s) match with its "
                         << "architecture\n";
                    exit(1);
                 }
                 int rPortID = my_rand(portCanL.size());
                 AX_CompPortPLCIter rIter = portCanL.begin();
                 for (unsigned int k = 0; k < portCanL.size(); k++)
                 {
                     if (rPortID == (int)k)
                     {
                        portP = *rIter;
                        break;
                     }
                     rIter++;
                 }
                 // create a local scope
                 {
                    AX_ConnPntC cPnt;
                    cPnt.set_ag(NULL);
                    cPnt.set_comp((AX_CompCP)&(*nfIter));
                    cPnt.set_port(portP);
                    cPntI = refBusP->add_new_connpnt(cPnt);
                    portP->add_new_bus(refBusP);
                 }
                 portCanL.clear();
                 reassign_sw_port(dsn, ag, &funcC);
              }
              else
              {  // if localMem == true
                 reassign_sw_port(dsn, ag, &funcC);
                 { // storing generic memory remaind
                    AX_UAttrC nUattr;
                    string nTmpS = UATTRGENCAPREM;
                    nUattr.set_name(nTmpS);
                    nUattr.set_type(T_INT);
                    char uVal[20];
                    my_itoa(genCapRem, uVal);
                    nUattr.set_value(uVal);

                    uattrP = (*nfIter).get_uattr(UATTRGENCAPREM);
                    if (uattrP != NULL)
                       (*nfIter).delete_uattr(uattrP);
                    (*nfIter).add_new_uattr(nUattr);
                 }
                 { // storing code memory remaind
                    AX_UAttrC nUattr;
                    string nTmpS = UATTRCODECAPREM;
                    nUattr.set_name(nTmpS);
                    nUattr.set_type(T_INT);
                    char uVal[20];
                    my_itoa(codeCapRem, uVal);
                    nUattr.set_value(uVal);

                    uattrP = (*nfIter).get_uattr(UATTRCODECAPREM);
                    if (uattrP != NULL)
                       (*nfIter).delete_uattr(uattrP);
                    (*nfIter).add_new_uattr(nUattr);
                 }
                 { // storing data memory remaind
                    AX_UAttrC nUattr;
                    string nTmpS = UATTRDATACAPREM;
                    nUattr.set_name(nTmpS);
                    nUattr.set_type(T_INT);
                    char uVal[20];
                    my_itoa(dataCapRem, uVal);
                    nUattr.set_value(uVal);

                    uattrP = (*nfIter).get_uattr(UATTRDATACAPREM);
                    if (uattrP != NULL)
                       (*nfIter).delete_uattr(uattrP);
                    (*nfIter).add_new_uattr(nUattr);
                 }
              } // end if localMem == true
           } // end processor
       } // end funcC dep loop

       return true;
   }
// ------------------------------------------------------------
   bool AX_EvolveC::expand_proc_refPL(bool Cpy,
                                          AX_DsnC & dsn,
                                          AX_ArchGraphC & ag,
                                          AX_FuncBlkCP masProcP,
                                          AX_FuncBlkCP cpyFuncP,
                                          int & fNum, int & pNum, int & bNum)
   {
       // !Cpy means the new component add on the processor already
       // resides in the processor already, so it is done, no need
       // to allocate memory to it.
       if (!Cpy)
          return true;

       cpyFuncP->add_new_dep(masProcP);
       masProcP->add_new_ref(cpyFuncP);

       AX_FuncBlkPLCIter fpIter;
       int refSize = 0;
       int refCodeSize = 0;
       int refDataSize = 0;
       int codeCapRem = 0;
       int dataCapRem = 0;
       int genCapRem = 0;
       AX_UAttrCP uattrP;
       char buf1[128];

       for (fpIter = cpyFuncP->get2_first_fncblk();
            fpIter != cpyFuncP->fncblk_list_end(); fpIter++)
       {
          if ((*fpIter)->get_functype() == MEMREQ)
          {
             bool codeMem = true;
             int oneSize = mem_parse((*fpIter)->get_name(),
                                     codeMem);
             refSize += oneSize;
             if (codeMem)
                refCodeSize += oneSize;
             else
                refDataSize += oneSize;
          }
       } // end for


       if (refSize <= 0)
       {
#if DEBUG >= 3
          cerr << cpyFuncP->get_name()
               << " does not have MEM_DATA and MEM_CODE specified "
               << "in the library\n";
#endif
          return false;
       }

       // get remaining DSP local memory
       float temSize;
       uattrP = masProcP->get_uattr(UATTRGENCAPREM);
       if (uattrP != NULL)
       {
          temSize = my_atoi(uattrP->get_value());
          genCapRem = (int)temSize;
       }
       uattrP = masProcP->get_uattr(UATTRCODECAPREM);
       if (uattrP != NULL)
       {
          temSize = my_atoi(uattrP->get_value());
          codeCapRem = (int)temSize;
       }
       uattrP = masProcP->get_uattr(UATTRDATACAPREM);
       if (uattrP != NULL)
       {
          temSize = my_atoi(uattrP->get_value());
          dataCapRem = (int)temSize;
       }

       // adding necessary memories
       AX_FuncBlkCP memP = NULL;
       AX_BusCP busP = NULL;
       bool localMem = false;

       int capRem = 0;

       uattrP = masProcP->get_uattr(UATTRARCHWIDTH);
       int archwidth = my_atoi(uattrP->get_value());

       // check local memory first
       if (refCodeSize <= codeCapRem &&
           refDataSize <= dataCapRem)
       {
          codeCapRem -= refCodeSize;
          dataCapRem -= refDataSize;
          localMem = true;
       }
       if (!localMem)
          if (refCodeSize <= genCapRem &&
              refDataSize <= dataCapRem)
          {
             genCapRem -= refCodeSize;
             dataCapRem -= refDataSize;
             localMem = true;
          }
       if (!localMem)
          if ((refCodeSize + refDataSize) <= genCapRem)
          {
             genCapRem -= refCodeSize;
             genCapRem -= refDataSize;
             localMem = true;
          }

       if (!localMem)
       {
          // local memory not big enough, get off-chip memory
          AX_FuncBlkPLCIter memPI;
          for (memPI = masProcP->get2_first_dep();
               memPI != masProcP->dep_list_end(); memPI++)
          {
             if ((*memPI)->get_functype() == MEMORY ||
                 (*memPI)->get_functype() == MEMORY_CORE)
             {
                memP = (*memPI);
                break;
             }
          }
          if (memP == NULL)
          {
             // no off-chip memory allocated yet, so allocate
             AX_FuncBlkC nFunc;
             AX_FuncBlkLCIter nfIter;

             memP = dsn.ran_pick_mem(capRem, refSize, archwidth);
             if (memP == NULL)
             {
                cerr << "Cannot find a memory in the library "
                     << "with capacity more than "
                     << refSize << "KB\n";
                return false;
             }

             uattrP = memP->get_uattr(UATTRARCHWIDTH);

             // allocate memory size for a memory core
             if (memP->get_functype() == MEMORY_CORE)
             {
                // for a memory core, the size is in bits
                if (capRem == 0)
                {
                   cerr << "Lib erryr: memory "
                        << memP->get_name()
                        << " has a zero OnChipMem value: "
                        << capRem << endl;
                   exit(1);
                }
                int multi;
                if (capRem != 0)
                   multi = ((refSize * 8) / capRem) + 1;
                else
                   multi = refSize * 8;
                int bitSize = (capRem * multi) + 8;
                capRem = int(bitSize / 8);
             }

             nfIter = ag.add_fncblk(nFunc);
             (*nfIter).copyBasic(*memP);

             string extID;
             my_itoa(fNum, buf1);
             extID = (*nfIter).get_name() + "_";
             extID = extID + buf1;
             (*nfIter).set_name(extID);

             (*nfIter).remove_all_taskrefs();
             (*nfIter).set_id(fNum++);
             (*nfIter).add_new_ref(cpyFuncP);
             ag.add_compports_by_fncblk(memP, &(*nfIter), pNum);
             memP = &(*nfIter);
             memP->set_tot_size(capRem);
             memP->add_new_ref(cpyFuncP);
             memP->add_new_dep(masProcP);
             masProcP->add_new_dep(memP);
             cpyFuncP->add_new_dep(memP);

             // hook up with the processor
             AX_CompPortPLCIter portPI;
             AX_CompPortPLC portCanL;
             AX_CompPortCP procPortP = NULL;
             AX_ConnPntLCIter cPntI;
             portCanL.clear();
             for (portPI = masProcP->get2_first_ioport();
                  portPI != masProcP->ioport_list_end(); portPI++)
             {
                if ((*portPI)->get_sample_size() == archwidth)
                   portCanL.push_back(*portPI);
             }
             if (portCanL.size() == 0)
             {
                cerr << "Lib error: " << masProcP->get_name()
                     << " does not have data port(s) match with its "
                     << "architecture\n";
                exit(1);
             }
             int rPortID = my_rand(portCanL.size());
             AX_CompPortPLCIter rIter = portCanL.begin();
             for (unsigned int k = 0; k < portCanL.size(); k++)
             {
                 if (rPortID == (int)k)
                 {
                    procPortP = *rIter;
                    break;
                 }
                 rIter++;
             }
             if ((busP = procPortP->get_bus(0)) == NULL)
             {
                // get a bus randomly
                AX_InterfaceCP intrP = procPortP->ran_pick_intrfc();
                busP = ag.ran_pick_bus(intrP, bNum);
                busP->set_intrfc(intrP);
                procPortP->add_new_bus(busP);
             }

             {
                AX_ConnPntC cPnt;
                cPnt.set_ag(NULL);
                cPnt.set_comp((AX_CompCP)masProcP);
                cPnt.set_port(procPortP);
                cPntI = busP->add_new_connpnt(cPnt);
             }
             portCanL.clear();

             AX_CompPortCP portP;
             rPortID = my_rand(memP->get_no_ioports());
             portP = memP->get_ioport(rPortID);

             {
                AX_ConnPntC cPnt;
                cPnt.set_ag(NULL);
                cPnt.set_comp((AX_CompCP)&(*nfIter));
                cPnt.set_port(portP);
                cPntI = busP->add_new_connpnt(cPnt);
                portP->add_new_bus(busP);
             }

             capRem -= refSize;
             AX_UAttrC nUattr;
             string nTmpS = UATTRGENCAPREM;
             nUattr.set_name(nTmpS);
             nUattr.set_type(T_INT);
             char uVal[20];
             my_itoa(capRem, uVal);
             nUattr.set_value(uVal);

             uattrP = memP->get_uattr(UATTRGENCAPREM);
             if (uattrP != NULL)
                memP->delete_uattr(uattrP);
             memP->add_new_uattr(nUattr);
          } // end no off-chip memory (if memP == NULL)
          else
          {
             // there is already an off-chip memory allocated
             if (memP->get_functype() == MEMORY_CORE)
             {
                // the off-chip memory is a core. Expand the
                // capacity and we are done.
                int totSize = (int)memP->get_tot_size();
                int multi;
                multi = refSize * 8;
                int bitSize = (capRem * multi) + 8;
                capRem = totSize + int(bitSize / 8);

                // set new capacity remains
                AX_UAttrC nUattr;
                string nTmpS = UATTRGENCAPREM;
                nUattr.set_name(nTmpS);
                nUattr.set_type(T_INT);
                char uVal[20];
                my_itoa(0, uVal);
                nUattr.set_value(uVal);

                uattrP = memP->get_uattr(UATTRGENCAPREM);
                if (uattrP != NULL)
                   memP->delete_uattr(uattrP);

                memP->add_new_uattr(nUattr);
                memP->add_new_ref(cpyFuncP);
                cpyFuncP->add_new_dep(memP);
             }
             else
             {
                // the off-chip memory is a memory chip.
                // Try to make use of it.
                uattrP = memP->get_uattr(UATTRGENCAPREM);
                capRem = (int)atof(uattrP->get_value().c_str());
                capRem -= refSize;
                
                AX_UAttrC nUattr;
                string nTmpS = UATTRGENCAPREM;
                nUattr.set_name(nTmpS);
                nUattr.set_type(T_INT);
                char uVal[20];
                my_itoa(capRem, uVal);
                nUattr.set_value(uVal);

                memP->delete_uattr(uattrP);
                memP->add_new_uattr(nUattr);

                memP->add_new_ref(cpyFuncP);
                cpyFuncP->add_new_dep(memP);
             }
          } // end there is already an off-chip memory allocated
          reassign_sw_port(dsn, ag, cpyFuncP);
       }
       else
       {
          // DSP local memory is used
          reassign_sw_port(dsn, ag, cpyFuncP);
          { // storing generic memory remaind
             AX_UAttrC nUattr;
             string nTmpS = UATTRGENCAPREM;
             nUattr.set_name(nTmpS);
             nUattr.set_type(T_INT);
             char uVal[20];
             my_itoa((int)genCapRem, uVal);
             nUattr.set_value(uVal);

             uattrP = masProcP->get_uattr(UATTRGENCAPREM);
             if (uattrP != NULL)
                masProcP->delete_uattr(uattrP);
             masProcP->add_new_uattr(nUattr);
          }
          { // storing code memory remaind
             AX_UAttrC nUattr;
             string nTmpS = UATTRCODECAPREM;
             nUattr.set_name(nTmpS);
             nUattr.set_type(T_INT);
             char uVal[20];
             my_itoa((int)codeCapRem, uVal);
             nUattr.set_value(uVal);

             uattrP = masProcP->get_uattr(UATTRCODECAPREM);
             if (uattrP != NULL)
                masProcP->delete_uattr(uattrP);
             masProcP->add_new_uattr(nUattr);
          }
          { // storing data memory remaind
             AX_UAttrC nUattr;
             string nTmpS = UATTRDATACAPREM;
             nUattr.set_name(nTmpS);
             nUattr.set_type(T_INT);
             char uVal[20];
             my_itoa((int)dataCapRem, uVal);
             nUattr.set_value(uVal);

             uattrP = masProcP->get_uattr(UATTRDATACAPREM);
             if (uattrP != NULL)
                masProcP->delete_uattr(uattrP);
             masProcP->add_new_uattr(nUattr);
          }
       }
       return true;
   }

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Operations
// ------------------------------------------------------------
   float AX_EvolveC::prune(AX_DsnC & dsn, AX_ArchGraphCP * ranking)
   {
       AX_ArchGraphLC newAgL;
       float fitVal, totVal = 0.0;
       float highScore = 0.0, lowScore = 999999.99, avgScore;
       int i;
       int tNum = dsn.get_no_ags();
 
       // get statistic data      
#if DEBUG >= 4
       cerr << "\n\n=========== ArchGraphs remain ============\n\n";
#endif
       for (i = 0; i < POP_SIZE; i++)
       {
#if DEBUG >= 4
          cerr.setf(ios::fixed,ios::floatfield);
          cerr.precision(8);
          cerr << i << ". " << (*ranking[i]).get_name()
               << ":" << ranking[i]->get_fitness() << endl;
#endif
          ranking[i]->set_id(i);
          fitVal = ranking[i]->get_fitness();
          totVal += fitVal;
          if (fitVal > highScore)
             highScore = fitVal;
          if (fitVal < lowScore)
             lowScore = fitVal;
       }

       StringAC arNameA;
       arNameA.clear();
       for (i = POP_SIZE; i < tNum; i++)
          arNameA.push_back(ranking[i]->get_name());

       // prune out low score archgraphs
#if DEBUG >= 4
       cerr << "\n\n=========== ArchGraphs delted ============\n\n";
#endif
       for (i = POP_SIZE; i < tNum; i++)
       {
          AX_ArchGraphLCIter agI;
#if DEBUG >= 4
          cerr.setf(ios::fixed,ios::floatfield);
          cerr.precision(8);
          cerr << i << ". " << ranking[i]->get_name()
               << ":" << ranking[i]->get_fitness() << endl;
#endif
          dsn.delete_ag(ranking[i]);
       }

       cerr.setf(ios::fixed,ios::floatfield);
       cerr.precision(8);
       avgScore = totVal/(float)POP_SIZE;
       cerr << "Avg: " << avgScore
            << "  High: " << highScore
            << "  Low: " << lowScore << endl;

       return avgScore;
   }

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Operations
// ------------------------------------------------------------
   void AX_EvolveC::display_agL_fitness(AX_ArchGraphLC & agL)
   {
       AX_ArchGraphLCIter agIter;
       std::string preName = "";
       int Cnt = 0;
       for (agIter = agL.begin(); agIter != agL.end(); agIter++)
       {
          if (preName != (*agIter).get_name()) {
              cerr << (*agIter).get_name() << ": "
                   << (*agIter).get_fitness() << endl;
#if DEBUG >= 4
              if (!(Cnt % 25))
                 (*agIter).dump();
#endif
              preName = (*agIter).get_name();
              Cnt++;
          }
       }
   }

// ------------------------------------------------------------
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~End of Function Definitions
