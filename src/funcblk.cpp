// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// funcblk.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <iostream>
#include "funcblk.h"

using namespace std;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructor
// ------------------------------------------------------------
   AX_FuncBlkC::AX_FuncBlkC()
   {
      this->set_category(FUNCBLK);
      m_function.clear();
      m_type = NULLTYPE;
      m_functype = FUNC_TYPE_UNKNOWN;
      m_platfmP = NULL;
      m_max_clk_rate = 0;
      m_no_stages = 0;
      m_leak_current = 0;
      m_run_pwr = 0;
      m_util = 0.0;
      m_temp = -1;
   }
// ------------------------------------------------------------
   AX_FuncBlkC::AX_FuncBlkC(const AX_FuncBlkC & other):AX_CompC(other)
   {
      copy(other);
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Destructor
// ------------------------------------------------------------
   AX_FuncBlkC::~AX_FuncBlkC()
   {
      this->AX_CompC::~AX_CompC();
      remove_all_taskrefs();
      remove_all_inports();
      remove_all_outports();
      remove_all_ioports();
      remove_all_fncblks();
      remove_all_deps();
      remove_all_refs();
      remove_all_uattrs();
      clean_up(); 
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Assignator & Comparators
// ------------------------------------------------------------
   AX_FuncBlkC & AX_FuncBlkC::operator= (const AX_FuncBlkC & other)
   {
      if (&other == this)
         return(*this);

      copy(other);

      return(*this);
   }
// ------------------------------------------------------------
   void AX_FuncBlkC::copy(const AX_FuncBlkC & other)
   {
      AX_CompC::copy(other);

      m_function = other.m_function;
      m_type = other.m_type;
      m_functype = other.m_functype;
      m_platfmP = other.m_platfmP;

      AX_TaskRefLSCnstIter taskrefI;
      remove_all_taskrefs();
      for (taskrefI = other.m_taskrefL.begin();
           taskrefI != other.m_taskrefL.end(); taskrefI++)
      {
          AX_TaskRefS nTaskR;
          AX_TaskRefLSIter nTaskI;

          m_taskrefL.push_back(nTaskR);
          nTaskI = --m_taskrefL.end();

          (*nTaskI).m_taskP = (*taskrefI).m_taskP;
          (*nTaskI).m_start = (*taskrefI).m_start;
          (*nTaskI).m_finish = (*taskrefI).m_finish;
      }

      AX_CompPortPLCCnstIter pIter;

      m_inportPL.clear();
      for (pIter = other.m_inportPL.begin();
           pIter != other.m_inportPL.end(); pIter++)
          m_inportPL.push_back(*pIter);

      m_outportPL.clear();
      for (pIter = other.m_outportPL.begin();
           pIter != other.m_outportPL.end(); pIter++)
          m_outportPL.push_back(*pIter);

      m_ioportPL.clear();
      for (pIter = other.m_ioportPL.begin();
           pIter != other.m_ioportPL.end(); pIter++)
          m_ioportPL.push_back(*pIter);

      AX_FuncBlkPLCCnstIter fIter;
      m_fncblkPL.clear();
      for (fIter = other.m_fncblkPL.begin();
           fIter != other.m_fncblkPL.end(); fIter++)
          m_fncblkPL.push_back(*fIter);

      m_depPL.clear();
      for (fIter = other.m_depPL.begin();
           fIter != other.m_depPL.end(); fIter++)
         m_depPL.push_back(*fIter);

      m_refPL.clear();
      for (fIter = other.m_refPL.begin();
           fIter != other.m_refPL.end(); fIter++)
      {
          AX_FuncBlkCP fTmpP = NULL;
          AX_FuncBlkPLCIter fTmpI;
          m_refPL.push_back(fTmpP);
          fTmpI = --m_refPL.end();
          (*fTmpI) = (*fIter);
      }

      m_max_clk_rate = other.m_max_clk_rate;
      m_no_stages = other.m_no_stages;
      m_leak_current = other.m_leak_current;
      m_run_pwr = other.m_run_pwr;
      m_util = other.m_util;
      m_temp = other.m_temp;

      AX_UAttrLCCnstIter uIter;
      AX_UAttrLCIter nuIter;

      m_uattrL.clear();
      for (uIter = other.m_uattrL.begin();
           uIter != other.m_uattrL.end(); uIter++)
      {
         AX_UAttrC nTemp;
         m_uattrL.push_back(nTemp);
         nuIter = --m_uattrL.end();
         (*nuIter).copy(*uIter);
      } 
   }
// ------------------------------------------------------------
   void AX_FuncBlkC::copyBasic(const AX_FuncBlkC & other)
   {
      AX_CompC::copy(other);

      m_function = other.m_function;
      m_type = other.m_type;
      m_functype = other.m_functype;
      m_platfmP = other.m_platfmP;
      m_max_clk_rate = other.m_max_clk_rate;
      m_no_stages = other.m_no_stages;
      m_leak_current = other.m_leak_current;
      m_run_pwr = other.m_run_pwr;
      m_util = other.m_util;
      m_temp = other.m_temp;

      AX_TaskRefLSCnstIter taskrefI;
      remove_all_taskrefs();
      for (taskrefI = other.m_taskrefL.begin();
           taskrefI != other.m_taskrefL.end(); taskrefI++)
      {
          AX_TaskRefS nTaskR;
          AX_TaskRefLSIter nTaskI;

          m_taskrefL.push_back(nTaskR);
          nTaskI = --m_taskrefL.end();

          (*nTaskI).m_taskP = (*taskrefI).m_taskP;
          (*nTaskI).m_start = (*taskrefI).m_start;
          (*nTaskI).m_finish = (*taskrefI).m_finish;
      }

      m_inportPL.clear();
      m_outportPL.clear();
      m_ioportPL.clear();
      m_refPL.clear();
      m_depPL.clear();

      AX_FuncBlkPLCCnstIter fpIter;
      m_fncblkPL.clear();
      for (fpIter = other.m_fncblkPL.begin();
           fpIter != other.m_fncblkPL.end(); fpIter++)
          m_fncblkPL.push_back(*fpIter);

      AX_UAttrLCCnstIter uIter;
      AX_UAttrLCIter nuIter;

      m_uattrL.clear();
      for (uIter = other.m_uattrL.begin();
           uIter != other.m_uattrL.end(); uIter++)
      {
         AX_UAttrC nTemp;
         m_uattrL.push_back(nTemp);
         nuIter = --m_uattrL.end();
         (*nuIter).copy(*uIter);
      } 
   }
// ------------------------------------------------------------
   bool AX_FuncBlkC::operator== (const AX_FuncBlkC & other)
   {
      return (AX_CompC::operator== (other));
   }
// ------------------------------------------------------------
   bool AX_FuncBlkC::operator== (const string name) 
   {
      return (AX_CompC::operator== (name));
   }
// ------------------------------------------------------------
   bool AX_FuncBlkC::operator!= (const string name) 
   {
       return (!(*this == name));
   }
// ------------------------------------------------------------

// ------------------------------------------------------------
   void AX_FuncBlkC::clean_sw_ports() 
   {
      AX_CompPortPLCIter portPI;
      for (portPI = get2_first_inport();
           portPI != inport_list_end(); portPI++)
         (*portPI)->set_sw_port(NULL);
      for (portPI = get2_first_outport();
           portPI != outport_list_end(); portPI++)
         (*portPI)->set_sw_port(NULL);
      for (portPI = get2_first_ioport();
           portPI != ioport_list_end(); portPI++)
         (*portPI)->set_sw_port(NULL);
   }
// ------------------------------------------------------------
   void AX_FuncBlkC::clean_sw_ports(AX_FuncBlkCP refFuncP) 
   {
      AX_CompPortPLCIter refPortPI;
      for (refPortPI = refFuncP->get2_first_inport();
           refPortPI != refFuncP->inport_list_end(); refPortPI++)
      {
         AX_CompPortPLCIter portPI;
         for (portPI = m_inportPL.begin();
              portPI != m_inportPL.end(); portPI++)
            if ((*portPI)->get_sw_port() == (*refPortPI))
            {
               (*refPortPI)->dec_refCnt();
               (*portPI)->set_sw_port(NULL);
            }
         for (portPI = m_outportPL.begin();
              portPI != m_outportPL.end(); portPI++)
            if ((*portPI)->get_sw_port() == (*refPortPI))
            {
               (*refPortPI)->dec_refCnt();
               (*portPI)->set_sw_port(NULL);
            }
         for (portPI = m_ioportPL.begin();
              portPI != m_ioportPL.end(); portPI++)
            if ((*portPI)->get_sw_port() == (*refPortPI))
            {
               (*refPortPI)->dec_refCnt();
               (*portPI)->set_sw_port(NULL);
            }
      }
      for (refPortPI = refFuncP->get2_first_outport();
           refPortPI != refFuncP->outport_list_end(); refPortPI++)
      {
         AX_CompPortPLCIter portPI;
         for (portPI = m_inportPL.begin();
              portPI != m_inportPL.end(); portPI++)
            if ((*portPI)->get_sw_port() == (*refPortPI))
            {
               (*refPortPI)->dec_refCnt();
               (*portPI)->set_sw_port(NULL);
            }
         for (portPI = m_outportPL.begin();
              portPI != m_outportPL.end(); portPI++)
            if ((*portPI)->get_sw_port() == (*refPortPI))
            {
               (*refPortPI)->dec_refCnt();
               (*portPI)->set_sw_port(NULL);
            }
         for (portPI = m_ioportPL.begin();
              portPI != m_ioportPL.end(); portPI++)
            if ((*portPI)->get_sw_port() == (*refPortPI))
            {
               (*refPortPI)->dec_refCnt();
               (*portPI)->set_sw_port(NULL);
            }
      }
      for (refPortPI = refFuncP->get2_first_ioport();
           refPortPI != refFuncP->ioport_list_end(); refPortPI++)
      {
         AX_CompPortPLCIter portPI;
         for (portPI = m_inportPL.begin();
              portPI != m_inportPL.end(); portPI++)
            if ((*portPI)->get_sw_port() == (*refPortPI))
            {
               (*refPortPI)->dec_refCnt();
               (*portPI)->set_sw_port(NULL);
            }
         for (portPI = m_outportPL.begin();
              portPI != m_outportPL.end(); portPI++)
            if ((*portPI)->get_sw_port() == (*refPortPI))
            {
               (*refPortPI)->dec_refCnt();
               (*portPI)->set_sw_port(NULL);
            }
         for (portPI = m_ioportPL.begin();
              portPI != m_ioportPL.end(); portPI++)
            if ((*portPI)->get_sw_port() == (*refPortPI))
            {
               (*refPortPI)->dec_refCnt();
               (*portPI)->set_sw_port(NULL);
            }
      }
   }

// ------------------------------------------------------------
   AX_TaskRefLSIter AX_FuncBlkC::add_new_taskref(AX_TaskRefS taskref) 
   {
      AX_TaskRefLSIter nTaskI;

      for (nTaskI = m_taskrefL.begin();
           nTaskI != m_taskrefL.end(); nTaskI++)
          if (taskref.m_taskP == (*nTaskI).m_taskP)
             return nTaskI;

      AX_TaskRefS nTaskR;

      m_taskrefL.push_back(nTaskR);
      nTaskI = --m_taskrefL.end();
      (*nTaskI).m_taskP = taskref.m_taskP;
      (*nTaskI).m_start = taskref.m_start;
      (*nTaskI).m_finish = taskref.m_finish;

    return nTaskI;
   }
// ------------------------------------------------------------
   AX_TaskRefLSIter AX_FuncBlkC::add_new_taskref(AX_TaskCP taskP) 
   {
      AX_TaskRefLSIter nTaskI;

      for (nTaskI = m_taskrefL.begin();
           nTaskI != m_taskrefL.end(); nTaskI++)
          if (taskP == (*nTaskI).m_taskP)
             return nTaskI;

      AX_TaskRefS nTaskR;

      m_taskrefL.push_back(nTaskR);
      nTaskI = --m_taskrefL.end();
      (*nTaskI).m_taskP = taskP;
      (*nTaskI).m_start = -1;
      (*nTaskI).m_finish = -1;

    return nTaskI;
   }
// ------------------------------------------------------------
   AX_TaskRefSP AX_FuncBlkC::get_taskref(int index) 
   {
      AX_TaskRefSP taskrefP;

      if ((index < 0)||((unsigned int)index > m_taskrefL.size()-1))
         taskrefP = NULL;
      else
      {
         AX_TaskRefLSIter fIter;
         int i;

         fIter = m_taskrefL.begin();
         for(i = 0; i < index; i++) fIter++;

         taskrefP = &(*fIter);
      }

      return taskrefP;
   }
// ------------------------------------------------------------
   void AX_FuncBlkC::remove_taskref(AX_TaskRefSP taskrefP) 
   {
      AX_TaskRefLSIter fIter;

      for (fIter = m_taskrefL.begin();
           fIter != m_taskrefL.end(); fIter++)
          if (&(*fIter) == taskrefP)
          {
             m_taskrefL.erase(fIter);
             return;
          }
      return;
   }
// ------------------------------------------------------------
   AX_TaskRefLSIter AX_FuncBlkC::delete_taskref(AX_TaskRefLSIter & iter) 
   {
      AX_TaskRefLSIter retIter;

      if (m_taskrefL.end() == iter)
         return iter;
      else if ((--m_taskrefL.end()) == iter)
      {
         (*iter).m_taskP = NULL;
         (*iter).m_start = 0;
         (*iter).m_finish = 0;
         m_taskrefL.erase(iter);
         retIter = m_taskrefL.end();
      }
      else
      {
         retIter = ++iter;
         iter--;
         (*iter).m_taskP = NULL;
         (*iter).m_start = 0;
         (*iter).m_finish = 0;
         m_taskrefL.erase(iter);
      }

      return retIter;
   }
// ------------------------------------------------------------
   AX_TaskRefLSIter AX_FuncBlkC::delete_taskref(AX_TaskRefSP taskrefP) 
   {
      AX_TaskRefLSIter iter, retIter;

      if (taskrefP == NULL)
         return m_taskrefL.end();

      if (&(*(--m_taskrefL.end())) == taskrefP)
      {
         iter = --m_taskrefL.end();
         (*iter).m_taskP = NULL;
         (*iter).m_start = 0;
         (*iter).m_finish = 0;
         m_taskrefL.erase(iter);
         return m_taskrefL.end();
      }
      else
      {
         for (iter = m_taskrefL.begin();
              iter != m_taskrefL.end(); iter++)
         {
            if (&(*iter) == taskrefP)
            {
               retIter = ++iter;
               iter--;
               (*iter).m_taskP = NULL;
               (*iter).m_start = 0;
               (*iter).m_finish = 0;
               m_taskrefL.erase(iter);
               return retIter;
            }
         }
      }

      return retIter;
   }
// ------------------------------------------------------------
   AX_TaskRefLSIter AX_FuncBlkC::delete_taskref(AX_TaskCP taskP) 
   {
      AX_TaskRefLSIter iter, retIter;

      if (taskP == NULL)
         return m_taskrefL.end();

      if ((*(--m_taskrefL.end())).m_taskP == taskP)
      {
         iter = --m_taskrefL.end();
         (*iter).m_taskP = NULL;
         (*iter).m_start = 0;
         (*iter).m_finish = 0;
         m_taskrefL.erase(iter);
         return m_taskrefL.end();
      }
      else
      {
         for (iter = m_taskrefL.begin();
              iter != m_taskrefL.end(); iter++)
         {
            if ((*iter).m_taskP == taskP)
            {
               retIter = ++iter;
               iter--;
               (*iter).m_taskP = NULL;
               (*iter).m_start = 0;
               (*iter).m_finish = 0;
               m_taskrefL.erase(iter);
               return retIter;
            }
         }
      }

      return retIter;
   }
// ------------------------------------------------------------
   void AX_FuncBlkC::remove_all_taskrefs()
   {
      AX_TaskRefLSIter iter;
      while ((iter = m_taskrefL.begin()) != m_taskrefL.end())
      {
         (*iter).m_taskP = NULL;
         (*iter).m_start = 0;
         (*iter).m_finish = 0;
         m_taskrefL.erase(iter);
      }
   } 
// ------------------------------------------------------------
   bool AX_FuncBlkC::is_mapped_task(AX_TaskCP taskP)
   {
      AX_TaskRefLSIter iter;
      for (iter = m_taskrefL.begin();
           iter != m_taskrefL.end(); iter++)
      {
         if ((*iter).m_taskP == taskP)
            return true;
      }

      return false;
   }
// ------------------------------------------------------------
   bool compare_start(AX_TaskRefS const & r1,
                      AX_TaskRefS const & r2)
   {
      return (r1.m_start < r2.m_start);
   }
// ------------------------------------------------------------
   void AX_FuncBlkC::sort_taskref_by_start()
   {
      if (m_taskrefL.size() <= 0)
         return;

      m_taskrefL.sort(compare_start);

      return;
   }

// ------------------------------------------------------------
   void AX_FuncBlkC::takeout_oth_ports(AX_ArchGraphC & ag,
                                        AX_CompPortCP rd_portP,
                                        AX_TaskCP taskP)
   {
      AX_CompPortCP portP;
      if (m_type == HARDWARE)
         portP = rd_portP;
      else if (rd_portP->get_sw_port() != NULL)
         portP = rd_portP->get_sw_port();
      else
         return;

      AX_BusPLCIter busPI;
      bool stepback = false;
      for (busPI = portP->get2_first_bus();
           busPI != portP->bus_list_end(); busPI++)
      {
         if (portP->get_no_buses() <= 0)
            break;

         AX_ConnPntLC connPntL;
         AX_ConnPLC connL;
         AX_TaskPortPLCIter tPortPI;

         if (stepback)
         {
            busPI--;
            stepback = false;
         }

         if ((*busPI) == NULL)
         {
            busPI = portP->delete_bus(busPI);
            if (busPI != portP->get2_first_bus())
               busPI--;
            else
               stepback = true;
            continue;
         }

         connPntL.clear();
         connL.clear();
         for (tPortPI = taskP->get2_first_inport();
              tPortPI != taskP->inport_list_end(); tPortPI++)
            if ((*tPortPI)->get_name() == rd_portP->get_name())
            {
               ag.collect_oth_ports((*tPortPI), connPntL, connL, true);
            }

         for (tPortPI = taskP->get2_first_outport();
              tPortPI != taskP->outport_list_end(); tPortPI++)
            if ((*tPortPI)->get_name() == rd_portP->get_name())
            {
               ag.collect_oth_ports((*tPortPI), connPntL, connL, false);
            }

         AX_ConnPLCIter cnIter1;
         AX_ConnRefLSIter crIter1;

         bool delBusFmPort = false;
         if (m_type == HARDWARE ||
            (m_type == SOFTWARE && portP->get_refCnt() == 1))
         {
            (*busPI)->delete_connpnt(portP->get_ag(),
                                     portP->get_comp(), portP);
            delBusFmPort = true;
         }

         // when the other side of rd_portP is a a software port,
         // collect_oth_ports() will not include it into the connPntL,
         // for it may remove a processor/memory port from the bus
         // which also connects to other ports. When this happens,
         // there maybe a chance that the other process/memory port
         // becomes the only connpnt on the bus. So when this happens
         // we also remove that bus.
         if ((*busPI)->get_no_connpnts() == 1)
         {
            AX_BusCP delBusP = (*busPI);
            AX_ConnPntLCIter conPntI;

            conPntI = (*busPI)->get2_first_connpnt();
            (*conPntI).get_port()->delete_bus(*busPI);
            (*busPI)->remove_all_connpnts();
            (*busPI)->remove_all_connrefs();
            busPI = portP->delete_bus(*busPI);
            if (busPI != portP->get2_first_bus())
               busPI--;
            else
               stepback = true;

            ag.delete_bus_fm_ports(delBusP);
            ag.delete_bus(delBusP);
            delBusFmPort = false;
         }
         else if ((*busPI)->get_no_connpnts() == 0)
         {
            AX_BusCP delBusP = (*busPI);

            (*busPI)->remove_all_connrefs();
            if (portP->get_no_buses() > 0)
            {
               busPI = portP->delete_bus(*busPI);
               if ((busPI != portP->get2_first_bus()) &&
                   (portP->get_no_buses() != 0))
                  busPI--;
               else
                  stepback = true;
            }

            ag.delete_bus(delBusP);
            delBusFmPort = false;
         }
         else if (m_type == HARDWARE)
         {
            if (portP->get_no_buses() > 0)
            {
               busPI = portP->delete_bus(*busPI);
               if ((busPI != portP->get2_first_bus()) &&
                   (portP->get_no_buses() != 0))
                  busPI--;
               else
                  stepback = true;
            }
            delBusFmPort = false;
         }

         if (delBusFmPort)
         {
            busPI = portP->delete_bus(*busPI);
            if ((busPI != portP->get2_first_bus()) &&
                (portP->get_no_buses() != 0))
               busPI--;
            else
               stepback = true;
         }

         AX_ConnPntLCIter connPntI, cpyI;
         AX_ConnPLCIter iCntIter = connL.begin();
         for (cpyI = connPntL.begin(); cpyI != connPntL.end(); cpyI++)
         {
            bool found = false;
            AX_BusPLCIter srchBusPI;
            bool walkback = false;
            AX_BusCP delBusP = NULL;

            for (srchBusPI = (*cpyI).get_port()->get2_first_bus();
                 srchBusPI != (*cpyI).get_port()->bus_list_end();
                 srchBusPI++)
            {
               if (walkback)
               {
                  srchBusPI--;
                  walkback = true;
               }
               for (connPntI = (*srchBusPI)->get2_first_connpnt();
                    connPntI != (*srchBusPI)->connpnt_list_end();
                    connPntI++)
               {
                  if ((*cpyI).get_ag() == (*connPntI).get_ag() &&
                      (*cpyI).get_comp() == (*connPntI).get_comp() &&
                      (*cpyI).get_port() == (*connPntI).get_port())
                  {
                     delBusP = (*srchBusPI);
                     (*srchBusPI)->delete_connpnt(connPntI);
                     (*srchBusPI)->delete_connref(*iCntIter);
                     srchBusPI = (*cpyI).get_port()->delete_bus(*srchBusPI);
                     if ((*cpyI).get_port()->get_no_buses() == 0)
                        srchBusPI = (*cpyI).get_port()->bus_list_end();
                     else if (srchBusPI != (*cpyI).get_port()->get2_first_bus())
                        srchBusPI--;
                     else
                        walkback = true;
                     found = true;
                     iCntIter++;
                     break;
                  }
               }
               if (found)
               {
                  if (delBusP->get_no_connpnts() == 1)
                  {
                     AX_ConnPntLCIter conPntI;
                     conPntI = delBusP->get2_first_connpnt();
                     (*conPntI).get_port()->delete_bus(delBusP);
                     delBusP->remove_all_connpnts();
                     delBusP->remove_all_connrefs();
                     ag.delete_bus_fm_ports(delBusP);
                     ag.delete_bus(delBusP);
                  }
                  else if (delBusP->get_no_connpnts() == 0)
                  {
                     delBusP->remove_all_connrefs();
                     ag.delete_bus(delBusP);
                  }

                  break;
               }
            }
#if DEBUG >= 6
            if (!found)
            {
               cerr << "takeout_oth_ports-12::Cannot find other port ";
               if ((*cpyI).get_ag() != NULL)
                  cerr << (*cpyI).get_ag()->get_name() << ".";
               else
                 cerr << (*cpyI).get_comp()->get_name() << ".";
               cerr << (*cpyI).get_port()->get_name() << endl;
               // dump();
               //return;
            }
#endif
         } // end for connpnt loop
      } // end for bus loop

      return;
   }

// ------------------------------------------------------------
   void AX_FuncBlkC::erase_connrefs(AX_ArchGraphC & ag)
   {
      AX_TaskRefLSIter taskrefI;
      for (taskrefI = m_taskrefL.begin();
           taskrefI != m_taskrefL.end(); taskrefI++)
         erase_connrefs(ag, (*taskrefI).m_taskP);
   }
// ------------------------------------------------------------
   void AX_FuncBlkC::erase_connrefs(AX_ArchGraphC & ag, AX_TaskCP taskP)
   {
      AX_CompPortPLCIter portPI;
      AX_BusPLCIter busPI;
      AX_ConnRefLSIter conRefI;

      // take care of input ports
      for (portPI = m_inportPL.begin();
           portPI != m_inportPL.end(); portPI++)
      {
         takeout_oth_ports(ag, *portPI, taskP);
         // get its own connrefs first
         for (busPI = (*portPI)->get2_first_bus();
              busPI != (*portPI)->bus_list_end(); busPI++)
         {
            bool stepback = false;
            for (conRefI = (*busPI)->get2_first_connref();
                 conRefI != (*busPI)->connref_list_end(); conRefI++)
            {
               if (stepback)
               {
                  conRefI--;
                  stepback = false;
               }
               if ((*conRefI).m_connP->get_src_task() == taskP ||
                   (*conRefI).m_connP->get_sink_task() == taskP)
               {
                  conRefI = (*busPI)->delete_connref(conRefI);
                  if (conRefI != (*busPI)->get2_first_connref())
                     conRefI--;
                  else
                     stepback = true;
               }
            } // end inport connref loop
         } // end inport bus loop

         // then take care of referenced processor/memory port, if any
         if ((*portPI)->get_sw_port() != NULL)
         {
            AX_CompPortCP swPortP = (*portPI)->get_sw_port();
            for (busPI = swPortP->get2_first_bus();
                 busPI != swPortP->bus_list_end(); busPI++)
            {
               bool stepback = false;
               for (conRefI = (*busPI)->get2_first_connref();
                    conRefI != (*busPI)->connref_list_end(); conRefI++)
               {
                  if (stepback)
                  {
                     conRefI--;
                     stepback = false;
                  }
                  if ((*conRefI).m_connP->get_src_task() == taskP ||
                      (*conRefI).m_connP->get_sink_task() == taskP)
                  {
                     conRefI = (*busPI)->delete_connref(conRefI);
                     if (conRefI != (*busPI)->get2_first_connref())
                        conRefI--;
                     else
                        stepback = true;
                  }
               } // end inport's sw_port connref loop
            } // end inport's sw_port bus loop
            AX_FuncBlkCP mapFuncP;
            mapFuncP = (AX_FuncBlkC *)(*portPI)->get_comp();
            if (mapFuncP->get_no_taskrefs() <= 0)
               (*portPI)->set_sw_port(NULL);
            ag.adjust_refCnt(swPortP);
         } // end inport's get_sw_port if
         (*portPI)->remove_all_buses();
      } // end inport list

      // take care of output ports
      for (portPI = m_outportPL.begin();
           portPI != m_outportPL.end(); portPI++)
      {
         takeout_oth_ports(ag, *portPI, taskP);
         // get its own connrefs first
         for (busPI = (*portPI)->get2_first_bus();
              busPI != (*portPI)->bus_list_end(); busPI++)
         {
            bool stepback = false;
            for (conRefI = (*busPI)->get2_first_connref();
                 conRefI != (*busPI)->connref_list_end(); conRefI++)
            {
               if (stepback)
               {
                  conRefI--;
                  stepback = false;
               }
               if ((*conRefI).m_connP->get_src_task() == taskP ||
                   (*conRefI).m_connP->get_sink_task() == taskP)
               {
                  conRefI = (*busPI)->delete_connref(conRefI);
                  if (conRefI != (*busPI)->get2_first_connref())
                     conRefI--;
                  else
                     stepback = true;
               }
            } // end outport connref loop
         } // end outport bus loop

         // then take care of referenced processor/memory port, if any
         if ((*portPI)->get_sw_port() != NULL)
         {
            AX_CompPortCP swPortP = (*portPI)->get_sw_port();
            for (busPI = swPortP->get2_first_bus();
                 busPI != swPortP->bus_list_end(); busPI++)
            {
               bool stepback = false;
               for (conRefI = (*busPI)->get2_first_connref();
                    conRefI != (*busPI)->connref_list_end(); conRefI++)
               {
                  if (stepback)
                  {
                     conRefI--;
                     stepback = false;
                  }
                  if ((*conRefI).m_connP->get_src_task() == taskP ||
                      (*conRefI).m_connP->get_sink_task() == taskP)
                  {
                     conRefI = (*busPI)->delete_connref(conRefI);
                     if (conRefI != (*busPI)->get2_first_connref())
                        conRefI--;
                     else
                        stepback = true;
                  }
               } // end outport sw_port's connref loop
            } // end outport sw_port's bus loop
            AX_FuncBlkCP mapFuncP;
            mapFuncP = (AX_FuncBlkC *)(*portPI)->get_comp();
            if (mapFuncP->get_no_taskrefs() <= 0)
               (*portPI)->set_sw_port(NULL);
            ag.adjust_refCnt(swPortP);
         } // end outport get_sw_port if
         (*portPI)->remove_all_buses();
      } // end outport list

      // take care of io ports
      for (portPI = m_ioportPL.begin();
           portPI != m_ioportPL.end(); portPI++)
      {
         takeout_oth_ports(ag, *portPI, taskP);
         // get its own connrefs first
         for (busPI = (*portPI)->get2_first_bus();
              busPI != (*portPI)->bus_list_end(); busPI++)
         {
            bool stepback = false;
            for (conRefI = (*busPI)->get2_first_connref();
                 conRefI != (*busPI)->connref_list_end(); conRefI++)
            {
               if (stepback)
               {
                  conRefI--;
                  stepback = false;
               }
               if ((*conRefI).m_connP->get_src_task() == taskP ||
                   (*conRefI).m_connP->get_sink_task() == taskP)
               {
                  conRefI = (*busPI)->delete_connref(conRefI);
                  if (conRefI != (*busPI)->get2_first_connref())
                     conRefI--;
                  else
                     stepback = true;
               }
            } // end ioport connref loop
         } // end ioport bus loop

         // then take care of referenced processor/memory port, if any
         if ((*portPI)->get_sw_port() != NULL)
         {
            AX_CompPortCP swPortP = (*portPI)->get_sw_port();
            for (busPI = swPortP->get2_first_bus();
                 busPI != swPortP->bus_list_end(); busPI++)
            {
               bool stepback = false;
               for (conRefI = (*busPI)->get2_first_connref();
                    conRefI != (*busPI)->connref_list_end(); conRefI++)
               {
                  if (stepback)
                  {
                     conRefI--;
                     stepback = false;
                  }
                  if ((*conRefI).m_connP->get_src_task() == taskP ||
                      (*conRefI).m_connP->get_sink_task() == taskP)
                  {
                     conRefI = (*busPI)->delete_connref(conRefI);
                     if (conRefI != (*busPI)->get2_first_connref())
                        conRefI--;
                     else
                        stepback = true;
                  }
               } // end ioport's sw_port connref loop
            } // end ioport's sw_port bus loop
            AX_FuncBlkCP mapFuncP;
            mapFuncP = (AX_FuncBlkC *)(*portPI)->get_comp();
            if (mapFuncP->get_no_taskrefs() <= 0)
               (*portPI)->set_sw_port(NULL);
            ag.adjust_refCnt(swPortP);
         } // end ioport get_sw_port if
         (*portPI)->remove_all_buses();
      } // end ioport list

      return;
   }

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Pointer List Manipulations
// ------------------------------------------------------------
   AX_CompPortPLCIter AX_FuncBlkC::add_new_inport(AX_CompPortCP portP) 
   {
      AX_CompPortPLCIter pIter;

      for (pIter = m_inportPL.begin();
           pIter != m_inportPL.end(); pIter++)
          if (portP == (*pIter))
             return pIter;

      m_inportPL.push_back(portP);
      pIter = --m_inportPL.end();

      return pIter;
   }
// ------------------------------------------------------------
   AX_CompPortCP AX_FuncBlkC::get_inport(int index) 
   {
      AX_CompPortCP portP;

      if ((index < 0)||((unsigned int)index > m_inportPL.size()-1)) 
         portP = NULL;
      else
      {
         AX_CompPortPLCIter pIter;
         int i;
      
         pIter = m_inportPL.begin();
         for(i = 0; i < index; i++) pIter++;
       
         portP = (*pIter);   
      }

      return portP;      
   }
// ------------------------------------------------------------
   AX_CompPortCP AX_FuncBlkC::get_inport(string name) 
   {
      if (name.empty())
         return NULL;
      else
      {
         AX_CompPortPLCIter pIter;
      
         for (pIter = m_inportPL.begin();
              pIter != m_inportPL.end(); pIter++)
            if ((*pIter)->get_name() == name)
               return (*pIter);
      }

      return NULL;      
   }
// ------------------------------------------------------------
   int AX_FuncBlkC::get_inportIdx(string name) 
   {
      if (name.empty())
         return -1;
      else
      {
         AX_CompPortPLCIter pIter;
         int idx = 0;
      
         for (pIter = m_inportPL.begin();
              pIter != m_inportPL.end(); pIter++)
         {
            if ((*pIter)->get_name() == name)
               return (idx);
            idx++;
         }
      }

      return -1;      
   }
// ------------------------------------------------------------
   void AX_FuncBlkC::remove_inport(AX_CompPortCP portP) 
   {
      AX_CompPortPLCIter pIter;

      for (pIter = m_inportPL.begin();
           pIter != m_inportPL.end(); pIter++)
          if ((*pIter) == portP)
          {
             m_inportPL.erase(pIter);
             return;
          }
      return;
   }
// ------------------------------------------------------------
   AX_CompPortCP AX_FuncBlkC::any_connected_inport() 
   {
      AX_CompPortPLCIter pIter;

      for (pIter = m_inportPL.begin();
           pIter != m_inportPL.end(); pIter++)
          if ((*pIter)->get_no_buses() > 0)
             return (*pIter);

      return NULL;
   }
// ------------------------------------------------------------
   AX_CompPortPLCIter AX_FuncBlkC::delete_inport(AX_CompPortPLCIter & iter) 
   {
      AX_CompPortPLCIter retIter;

      if (m_inportPL.end() == iter)
         return iter;
      else if ((--m_inportPL.end()) == iter)
      {
         m_inportPL.erase(iter);
         retIter = m_inportPL.end();
      }
      else
      {
         retIter = ++iter;
         iter--;
         m_inportPL.erase(iter);
      }

      return retIter;
   }
// ------------------------------------------------------------
   AX_CompPortPLCIter AX_FuncBlkC::delete_inport(AX_CompPortCP portP) 
   {
      AX_CompPortPLCIter retIter, iter;

      if (portP == NULL)
         return m_inportPL.end();

      iter = --m_inportPL.end();
      if ((*iter) == portP)
      {
         m_inportPL.erase(iter);
         return m_inportPL.end();
      }
      retIter = m_inportPL.end();
      for (iter = m_inportPL.begin();
           iter != m_inportPL.end(); iter++)
         if ((*iter) == portP)
         {
            retIter = ++iter;
            iter--;
            m_inportPL.erase(iter);
         }

      return retIter;
   }
// ------------------------------------------------------------
   AX_CompPortPLCIter AX_FuncBlkC::add_new_outport(AX_CompPortCP portP) 
   {
      AX_CompPortPLCIter pIter;

      for (pIter = m_outportPL.begin();
           pIter != m_outportPL.end(); pIter++)
          if (portP == (*pIter))
             return pIter;

      m_outportPL.push_back(portP);
      pIter = --m_outportPL.end();

      return pIter;
   }
// ------------------------------------------------------------
   AX_CompPortCP AX_FuncBlkC::get_outport(int index) 
   {
      AX_CompPortCP portP;

      if ((index < 0)||((unsigned int)index > m_outportPL.size()-1)) 
         portP = NULL;
      else
      {
         AX_CompPortPLCIter pIter;
         int i;
         
         pIter = m_outportPL.begin();
         for(i = 0; i < index; i++) pIter++;
       
         portP = (*pIter);
      }

      return portP;
   }
// ------------------------------------------------------------
   AX_CompPortCP AX_FuncBlkC::get_outport(string name) 
   {
      if (name.empty())
         return NULL;
      else
      {
         AX_CompPortPLCIter pIter;
      
         for (pIter = m_outportPL.begin();
              pIter != m_outportPL.end(); pIter++)
            if ((*pIter)->get_name() == name)
               return (*pIter);
      }

      return NULL;      
   }
// ------------------------------------------------------------
   int AX_FuncBlkC::get_outportIdx(string name) 
   {
      if (name.empty())
         return -1;
      else
      {
         AX_CompPortPLCIter pIter;
         int idx = 0;
      
         for (pIter = m_outportPL.begin();
              pIter != m_outportPL.end(); pIter++)
         {
            if ((*pIter)->get_name() == name)
               return (idx);
            idx++;
         }
      }

      return -1;      
   }
// ------------------------------------------------------------
   void AX_FuncBlkC::remove_outport(AX_CompPortCP portP) 
   {
      AX_CompPortPLCIter pIter;

      for (pIter = m_outportPL.begin();
           pIter != m_outportPL.end(); pIter++)
          if ((*pIter) == portP)
          {
             m_outportPL.erase(pIter);
             return;
          }
      return;
   }
// ------------------------------------------------------------
   AX_CompPortCP AX_FuncBlkC::any_connected_outport() 
   {
      AX_CompPortPLCIter pIter;

      for (pIter = m_outportPL.begin();
           pIter != m_outportPL.end(); pIter++)
          if ((*pIter)->get_no_buses() > 0)
             return (*pIter);

      return NULL;
   }
// ------------------------------------------------------------
   AX_CompPortPLCIter AX_FuncBlkC::delete_outport(AX_CompPortPLCIter & iter)
   {
      AX_CompPortPLCIter retIter;

      if (m_outportPL.end() == iter)
         return iter;
      else if ((--m_outportPL.end()) == iter)
      {
         m_outportPL.erase(iter);
         retIter = m_outportPL.end();
      }
      else
      {
         retIter = ++iter;
         iter--;
         m_outportPL.erase(iter);
      }

      return retIter;
   }
// ------------------------------------------------------------
   AX_CompPortPLCIter AX_FuncBlkC::delete_outport(AX_CompPortCP portP) 
   {
      AX_CompPortPLCIter retIter, iter;

      if (portP == NULL)
         return m_outportPL.end();

      iter = --m_outportPL.end();
      if ((*iter) == portP)
      {
         m_outportPL.erase(iter);
         return m_outportPL.end();
      }
      retIter = m_outportPL.end();
      for (iter = m_outportPL.begin();
           iter != m_outportPL.end(); iter++)
         if ((*iter) == portP)
         {
            retIter = ++iter;
            iter--;
            m_outportPL.erase(iter);
         }

      return retIter;
   }
// ------------------------------------------------------------
   AX_CompPortPLCIter AX_FuncBlkC::add_new_ioport(AX_CompPortCP portP) 
   {
      AX_CompPortPLCIter pIter;

      for (pIter = m_ioportPL.begin();
           pIter != m_ioportPL.end(); pIter++)
          if (portP == (*pIter))
             return pIter;

      m_ioportPL.push_back(portP);
      pIter = --m_ioportPL.end();

      return pIter;
   }
// ------------------------------------------------------------
   AX_CompPortCP AX_FuncBlkC::get_ioport(int index) 
   {
      AX_CompPortCP portP;

      if ((index < 0)||((unsigned int)index > m_ioportPL.size()-1))
         portP = NULL;
      else
      {
         AX_CompPortPLCIter pIter;
         int i;

         pIter = m_ioportPL.begin();
         for(i = 0; i < index; i++) pIter++;

         portP = (*pIter);
      }

      return portP;
   }
// ------------------------------------------------------------
   int AX_FuncBlkC::get_ioportIdx(string name) 
   {
      if (name.empty())
         return -1;
      else
      {
         AX_CompPortPLCIter pIter;
         int idx = 0;
      
         for (pIter = m_ioportPL.begin();
              pIter != m_ioportPL.end(); pIter++)
         {
            if ((*pIter)->get_name() == name)
               return (idx);
            idx++;
         }
      }

      return -1;      
   }
// ------------------------------------------------------------
   AX_CompPortCP AX_FuncBlkC::get_ioport(string name) 
   {
      if (name.empty())
         return NULL;
      else
      {
         AX_CompPortPLCIter pIter;
      
         for (pIter = m_ioportPL.begin();
              pIter != m_ioportPL.end(); pIter++)
            if ((*pIter)->get_name() == name)
               return (*pIter);
      }

      return NULL;      
   }
// ------------------------------------------------------------
   void AX_FuncBlkC::remove_ioport(AX_CompPortCP portP) 
   {
      AX_CompPortPLCIter pIter;

      for (pIter = m_ioportPL.begin();
           pIter != m_ioportPL.end(); pIter++)
          if ((*pIter) == portP)
          {
             m_ioportPL.erase(pIter);
             return;
          }
      return;
   }
// ------------------------------------------------------------
   AX_CompPortPLCIter AX_FuncBlkC::delete_ioport(AX_CompPortPLCIter & iter) 
   {
      AX_CompPortPLCIter retIter;

      if (m_ioportPL.end() == iter)
         return iter;
      else if ((--m_ioportPL.end()) == iter)
      {
         m_ioportPL.erase(iter);
         retIter = m_ioportPL.end();
      }
      else
      {
         retIter = ++iter;
         iter--;
         m_ioportPL.erase(iter);
      }

      return retIter;
   } 
// ------------------------------------------------------------
   AX_CompPortPLCIter AX_FuncBlkC::delete_ioport(AX_CompPortCP portP) 
   {
      AX_CompPortPLCIter retIter, iter;

      if (portP == NULL)
         return m_ioportPL.end();

      iter = --m_ioportPL.end();
      if ((*iter) == portP)
      {
         m_ioportPL.erase(iter);
         return m_ioportPL.end();
      }
      retIter = m_ioportPL.end();
      for (iter = m_ioportPL.begin();
           iter != m_ioportPL.end(); iter++)
         if ((*iter) == portP)
         {
            retIter = ++iter;
            iter--;
            m_ioportPL.erase(iter);
         }

      return retIter;
   }
// ------------------------------------------------------------
// this module tries to locate the first empty data/IO port for
// multi-port connection. But if all the data ports are occupied,
// then find the one that is connected with the busP passed in and
// return that port. But if busP is null, then randomly select
// a port to return with.
// This module is primarary designed for memory/memory core.
// ------------------------------------------------------------
   AX_CompPortCP AX_FuncBlkC::get_an_empty_ioport(AX_BusCP busP) 
   {
      AX_CompPortPLCIter iter;

      if (m_ioportPL.size() == 0)
         return NULL;

      // first try to locate an empty port
      for (iter = m_ioportPL.begin();
           iter != m_ioportPL.end(); iter++)
      {
         if ((*iter)->get_no_buses() == 0)
            return (*iter);
      }

      // oops, no empty port. See if busP is null
      if (busP == NULL)
      {
         int pNum = my_rand(m_ioportPL.size());
         int iCnt = 0;
         for (iter = m_ioportPL.begin();
              iter != m_ioportPL.end(); iter++)
         {
             if (iCnt == pNum)
                return *iter;
             else
                iCnt++;
         }
      }
      
      // Ok. Find the port that connects to busP
      for (iter = m_ioportPL.begin();
           iter != m_ioportPL.end(); iter++)
      {
         AX_BusPLCIter busPI;
         for (busPI = (*iter)->get2_first_bus();
              busPI != (*iter)->bus_list_end(); busPI++)
            if ((*busPI) == busP)
               return (*iter);
      }
      return NULL;
   }
// ------------------------------------------------------------
   AX_FuncBlkPLCIter AX_FuncBlkC::add_new_fncblk(AX_FuncBlkCP fncblkP) 
   {
      AX_FuncBlkPLCIter fIter;

      for (fIter = m_fncblkPL.begin();
           fIter != m_fncblkPL.end(); fIter++)
          if (fncblkP == (*fIter))
             return fIter;

      m_fncblkPL.push_back(fncblkP);
      fIter = --m_fncblkPL.end();

    return fIter;
   }
// ------------------------------------------------------------
   AX_FuncBlkCP AX_FuncBlkC::get_fncblk(int index) 
   {
      AX_FuncBlkCP fncblkP;

      if ((index < 0)||((unsigned int)index > m_fncblkPL.size()-1))
         fncblkP = NULL;
      else
      {
         AX_FuncBlkPLCIter fIter;
         int i;

         fIter = m_fncblkPL.begin();
         for(i = 0; i < index; i++) fIter++;

         fncblkP = (*fIter);
      }

      return fncblkP;
   }
// ------------------------------------------------------------
   void AX_FuncBlkC::remove_fncblk(AX_FuncBlkCP fncblkP) 
   {
      AX_FuncBlkPLCIter fIter;

      for (fIter = m_fncblkPL.begin();
           fIter != m_fncblkPL.end(); fIter++)
          if ((*fIter) == fncblkP)
          {
             m_fncblkPL.erase(fIter);
             return;
          }
      return;
   }
// ------------------------------------------------------------
   AX_FuncBlkPLCIter AX_FuncBlkC::delete_fncblk(AX_FuncBlkPLCIter & iter) 
   {
      AX_FuncBlkPLCIter retIter;

      if (m_fncblkPL.end() == iter)
         return iter;
      else if ((--m_fncblkPL.end()) == iter)
      {
         m_fncblkPL.erase(iter);
         retIter = m_fncblkPL.end();
      }
      else
      {
         retIter = ++iter;
         iter--;
         m_fncblkPL.erase(iter);
      }

      return retIter;
   }
// ------------------------------------------------------------

// ------------------------------------------------------------
   AX_FuncBlkPLCIter AX_FuncBlkC::add_new_dep(AX_FuncBlkCP depP) 
   {
      AX_FuncBlkPLCIter fIter;

      for (fIter = m_depPL.begin();
           fIter != m_depPL.end(); fIter++)
          if (depP == (*fIter))
             return fIter;

      m_depPL.push_back(depP);
      fIter = --m_depPL.end();

    return fIter;
   }
// ------------------------------------------------------------
   AX_FuncBlkCP AX_FuncBlkC::get_dep(int index) 
   {
      AX_FuncBlkCP depP;

      if ((index < 0)||((unsigned int)index > m_depPL.size()-1))
         depP = NULL;
      else
      {
         AX_FuncBlkPLCIter fIter;
         int i;

         fIter = m_depPL.begin();
         for(i = 0; i < index; i++) fIter++;

         depP = (*fIter);
      }

      return depP;
   }
// ------------------------------------------------------------
   void AX_FuncBlkC::remove_dep(AX_FuncBlkCP depP) 
   {
      AX_FuncBlkPLCIter fIter;

      for (fIter = m_depPL.begin();
           fIter != m_depPL.end(); fIter++)
          if ((*fIter) == depP)
          {
             m_depPL.erase(fIter);
             return;
          }
      return;
   }
// ------------------------------------------------------------
   AX_FuncBlkPLCIter AX_FuncBlkC::delete_dep(AX_FuncBlkPLCIter & iter) 
   {
      AX_FuncBlkPLCIter retIter;

      if (m_depPL.end() == iter)
         return iter;
      else if ((--m_depPL.end()) == iter)
      {
         m_depPL.erase(iter);
         retIter = m_depPL.end();
      }
      else
      {
         retIter = ++iter;
         iter--;
         m_depPL.erase(iter);
      }

      return retIter;
   }
// ------------------------------------------------------------
   AX_FuncBlkPLCIter AX_FuncBlkC::delete_dep(AX_FuncBlkCP funcP) 
   {
      AX_FuncBlkPLCIter retIter, iter;

      if (funcP == NULL)
         return m_depPL.end();

      iter = --m_depPL.end();
      if ((*iter) == funcP)
      {
         m_depPL.erase(iter);
         return m_depPL.end();
      }

      retIter = m_depPL.end();
      for (iter = m_depPL.begin();
           iter != m_depPL.end(); iter++)
         if ((*iter) == funcP)
         {
            retIter = ++iter;
            iter--;
            m_depPL.erase(iter);
            break;
         }

      return retIter;
   }
// ------------------------------------------------------------
   AX_FuncBlkCP AX_FuncBlkC::find_proc_in_dep() 
   {
      AX_FuncBlkPLCIter fpIter;

      for (fpIter = m_depPL.begin();
           fpIter != m_depPL.end(); fpIter++)
         if ((*fpIter)->get_functype() == PROCESSOR)
            return (*fpIter);

      return NULL;
   }
// ------------------------------------------------------------

// ------------------------------------------------------------
   AX_FuncBlkPLCIter AX_FuncBlkC::add_new_ref(AX_FuncBlkCP refP) 
   {
      AX_FuncBlkPLCIter fIter;

      for (fIter = m_refPL.begin();
           fIter != m_refPL.end(); fIter++)
          if (refP == (*fIter))
             return fIter;

      AX_FuncBlkCP fTmpP = NULL;
      m_refPL.push_back(fTmpP);
      fIter = --m_refPL.end();
      (*fIter) = refP;

    return fIter;
   }
// ------------------------------------------------------------
   AX_FuncBlkCP AX_FuncBlkC::get_ref(int index) 
   {
      AX_FuncBlkCP refP;

      if ((index < 0)||((unsigned int)index > m_refPL.size()-1))
         refP = NULL;
      else
      {
         AX_FuncBlkPLCIter fIter;
         int i;

         fIter = m_refPL.begin();
         for(i = 0; i < index; i++) fIter++;

         refP = (*fIter);
      }

      return refP;
   }
// ------------------------------------------------------------
   void AX_FuncBlkC::remove_ref(AX_FuncBlkCP refP) 
   {
      AX_FuncBlkPLCIter fIter;

      for (fIter = m_refPL.begin();
           fIter != m_refPL.end(); fIter++)
          if ((*fIter) == refP)
          {
             m_refPL.erase(fIter);
             return;
          }
      return;
   }
// ------------------------------------------------------------
   AX_FuncBlkPLCIter AX_FuncBlkC::delete_ref(AX_FuncBlkPLCIter & iter) 
   {
      AX_FuncBlkPLCIter retIter;

      if (m_refPL.end() == iter)
         return iter;
      else if ((m_refPL.size() != 0) &&
               ((--m_refPL.end()) == iter))
      {
         m_refPL.erase(iter);
         retIter = m_refPL.end();
      }
      else
      {
         retIter = ++iter;
         iter--;
         m_refPL.erase(iter);
      }

      return retIter;
   }
// ------------------------------------------------------------
   AX_FuncBlkPLCIter AX_FuncBlkC::delete_ref(AX_FuncBlkCP funcP)
   {
      AX_FuncBlkPLCIter iter, retIter = m_refPL.end();

      if (funcP == NULL)
         return m_refPL.end();
      else if ((m_refPL.size() != 0) &&
               ((*(--m_refPL.end())) == funcP))
      {
         iter = --m_refPL.end();
         m_refPL.erase(iter);
         return m_refPL.end();
      }
      else
      {
         for (iter = m_refPL.begin(); iter != m_refPL.end(); iter++)
            if ((*iter) == funcP)
            {
               retIter = ++iter;
               iter--;
               m_refPL.erase(iter);
               return retIter;
            }
      }

      return retIter;
   }


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Instance List Manipulations
// ------------------------------------------------------------
   AX_UAttrLCIter AX_FuncBlkC::add_new_uattr(AX_UAttrC & uattr) 
   {
      AX_UAttrLCIter uIter;

      for (uIter = m_uattrL.begin();
           uIter != m_uattrL.end(); uIter++)
          if (uattr == (*uIter))
             return uIter;

      m_uattrL.push_back(uattr);

      return (--m_uattrL.end());
   }
// ------------------------------------------------------------
   AX_UAttrCP AX_FuncBlkC::get_uattr(int index) 
   {
      AX_UAttrCP uattrP;

      if ((index < 0)||((unsigned int)index > m_uattrL.size()-1))
         uattrP = NULL;
      else
      {
         AX_UAttrLCIter uIter;
         int i;

         uIter = m_uattrL.begin();
         for(i = 0; i < index; i++) uIter++;

         uattrP = &(*uIter);
      }

      return uattrP;
   }
// ------------------------------------------------------------
   AX_UAttrCP AX_FuncBlkC::get_uattr(string uName) 
   {
      if (uName.empty())
         return NULL;
      else
      {
         AX_UAttrLCIter uIter;
         for (uIter = get2_first_uattr();
              uIter != uattr_list_end(); uIter++)
         {
            if ((*uIter).get_name() == uName)
               return &(*uIter);
         }
      }

      return NULL;
   }
// ------------------------------------------------------------
   void AX_FuncBlkC::remove_uattr(AX_UAttrCP uattrP)
   {
       AX_UAttrLCIter uIter;

       for (uIter = m_uattrL.begin();
            uIter != m_uattrL.end(); uIter++)
           if (&(*uIter) == uattrP)
           {
               m_uattrL.erase(uIter);
               return;
           }
       return;
   }
// ------------------------------------------------------------
   AX_UAttrLCIter AX_FuncBlkC::delete_uattr(AX_UAttrLCIter & iter)
   {
      AX_UAttrLCIter uIter;

      if (m_uattrL.end() == iter)
         return iter;
      else if ((--m_uattrL.end()) == iter)
      {
         AX_UAttrC aTmp;
         (*iter) = aTmp;
         m_uattrL.erase(iter);
         uIter = m_uattrL.end();
      }
      else
      {
         uIter = ++iter;
         iter--;
         AX_UAttrC aTmp;
         (*iter) = aTmp;
         m_uattrL.erase(iter);
      }

      return uIter;
   }
// ------------------------------------------------------------
   AX_UAttrLCIter AX_FuncBlkC::delete_uattr(AX_UAttrCP uattrP)
   {
      AX_UAttrLCIter uIter, retIter;

      if (uattrP == NULL)
         return m_uattrL.end();
      else if (&(*(--m_uattrL.end())) == uattrP)
      {
         uIter = --m_uattrL.end();
         AX_UAttrC aTmp;
         (*uIter) = aTmp;
         m_uattrL.erase(uIter);
         return m_uattrL.end();
      }
      else
      {
         for (uIter = m_uattrL.begin();
              uIter != m_uattrL.end(); uIter++)
            if (&(*uIter) == uattrP)
            {
               retIter = ++uIter;
               uIter--;
               AX_UAttrC aTmp;
               (*uIter) = aTmp;
               m_uattrL.erase(uIter);
               return retIter;
            }
      }

      return uIter;
   }
// ------------------------------------------------------------
   void AX_FuncBlkC::remove_all_uattrs()
   {
      AX_UAttrLCIter uIter;
 
      AX_UAttrC aTmp;
      while ((uIter = m_uattrL.begin()) != m_uattrL.end())
      {
         (*uIter) = aTmp;
         m_uattrL.erase(uIter);
      }

      m_uattrL.clear();
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Structure List Manipulations


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Special Functions
// ------------------------------------------------------------
   bool AX_FuncBlkC::copy_intrfcs(AX_InterfaceLC * intrfcL,
                                      AX_FuncBlkCP oFuncP)
   {
      AX_CompPortPLCIter portPI;
      for (portPI = oFuncP->get2_first_inport();
           portPI != oFuncP->inport_list_end(); portPI++)
      {
         AX_CompPortPLCIter thisPI;
         AX_CompPortCP portP = NULL;
         for (thisPI = get2_first_inport();
              thisPI != inport_list_end(); thisPI++)
            if ((*portPI)->get_name() == (*thisPI)->get_name())
            {
               portP = (*thisPI);
               break;
            }
         if (portP == NULL)
         {
#if DEBUG >= 1
            cerr << "Cannot find port " << (*portPI)->get_name()
                 << " in " << this->get_name() << endl;
#endif
            return false;
         }
         (*thisPI)->copy_intrfcs(intrfcL, (*portPI));
      }

      for (portPI = oFuncP->get2_first_outport();
           portPI != oFuncP->outport_list_end(); portPI++)
      {
         AX_CompPortPLCIter thisPI;
         AX_CompPortCP portP = NULL;
         for (thisPI = get2_first_outport();
              thisPI != outport_list_end(); thisPI++)
            if ((*portPI)->get_name() == (*thisPI)->get_name())
            {
               portP = (*thisPI);
               break;
            }
         if (portP == NULL)
         {
#if DEBUG >= 1
            cerr << "Cannot find port " << (*portPI)->get_name()
                 << " in " << this->get_name() << endl;
#endif
            return false;
         }
         (*thisPI)->copy_intrfcs(intrfcL, (*portPI));
      }

      for (portPI = oFuncP->get2_first_ioport();
           portPI != oFuncP->ioport_list_end(); portPI++)
      {
         AX_CompPortPLCIter thisPI;
         AX_CompPortCP portP = NULL;
         for (thisPI = get2_first_ioport();
              thisPI != ioport_list_end(); thisPI++)
            if ((*portPI)->get_name() == (*thisPI)->get_name())
            {
               portP = (*thisPI);
               break;
            }
         if (portP == NULL)
         {
#if DEBUG >= 1
            cerr << "Cannot find port " << (*portPI)->get_name()
                 << " in " << this->get_name() << endl;
#endif
            return false;
         }
         (*thisPI)->copy_intrfcs(intrfcL, (*portPI));
      }
      return true;
   }

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Miscellaneous Functions
// ------------------------------------------------------------
   void AX_FuncBlkC::clean_up()
   {
      m_function.clear();
      m_type = NULLTYPE;
      m_functype = FUNC_TYPE_UNKNOWN;
      m_platfmP = NULL;
      m_max_clk_rate = 0;
      m_no_stages = 0;
      m_leak_current = 0;
      m_run_pwr = 0;
      m_util = 0.0;
      m_temp = 0;
   }
// ------------------------------------------------------------
   void AX_FuncBlkC::dump()
   {
       AX_CompC::dump();
       AX_InterfacePLCIter iIter;
       AX_UAttrLCIter uIter;

       // dumping basic info
       cerr << "    Function " << m_function
            << "  type " << m_type
            << "  functype " << m_functype << endl;
       cerr.precision(4);
       cerr << "    Utilization " << m_util << endl;

       // dumping platform
       if (m_platfmP != NULL)
       {
           cerr << "    Platform:" << endl;
           m_platfmP->dump_simple();
       }

       // dumping tasks
       AX_TaskRefLSIter taskrefI;
       if (m_taskrefL.size() > 0)
       {
           cerr << "    Tasks: ";
           for (taskrefI = m_taskrefL.begin();
                taskrefI != m_taskrefL.end(); taskrefI++)
           {
               cerr << (*taskrefI).m_taskP->get_name() << ":";
               cerr << "scheduled [" << (*taskrefI).m_start << ","
                    << (*taskrefI).m_finish << "]";
               if (taskrefI != (--m_taskrefL.end()))
                  cerr << "  ";
           }
           cerr << endl;
       }

       // dumping input ports
       AX_CompPortPLCIter cpIter;
       if (m_inportPL.size() > 0)
       {
           cerr << "    Input port:" << endl;
           for (cpIter = m_inportPL.begin();
                cpIter != m_inportPL.end(); cpIter++)
               (*cpIter)->dump_simple();
       }

       // dumping output ports
       if (m_outportPL.size() > 0)
       {
           cerr << "    Output port:" << endl;
           for (cpIter = m_outportPL.begin();
                cpIter != m_outportPL.end(); cpIter++)
               (*cpIter)->dump_simple();
       }

       // dumping io ports
       if (m_ioportPL.size() > 0)
       {
           cerr << "    IO port:" << endl;
           for (cpIter = m_ioportPL.begin();
                cpIter != m_ioportPL.end(); cpIter++)
               (*cpIter)->dump_simple();
       }

       // dumping local variables
       cerr << "    clk_rate: " << m_max_clk_rate
            << "  no_stages: " << m_no_stages << endl;
       cerr << "    leak_current: " << m_leak_current
            << "  run_pwr: " << m_run_pwr 
            << "  temp: " << m_temp << endl;

       // dumping dependency list
       AX_FuncBlkPLCIter fpIter;
       if (m_fncblkPL.size() > 0)
       {
           cerr << "    fncblkPL:" << endl;
           cerr << "      ";
           for (fpIter = m_fncblkPL.begin();
                fpIter != m_fncblkPL.end(); fpIter++)
           {
              cerr << (*fpIter)->get_name();
              if (fpIter != (--m_fncblkPL.end()))
                 cerr << ", ";
              else
                 cerr << endl;
           }
       }
       else
           cerr << "    fncblkPL is null" << endl;

       if (m_depPL.size() > 0)
       {
           cerr << "    depPL:" << endl;
           for (fpIter = m_depPL.begin();
                fpIter != m_depPL.end(); fpIter++)
           {
               cerr << "  ";
               (*fpIter)->dump_simple();
           }
       }

       // dumping references
       if (m_refPL.size() > 0)
       {
           cerr << "    refPL:" << endl;
           for (fpIter = m_refPL.begin();
                fpIter != m_refPL.end(); fpIter++)
           {
               cerr << "  ";
               (*fpIter)->dump_simple();
               if ((*fpIter)->get_no_taskrefs() > 0)
                  cerr << "      ";
               AX_TaskRefLSIter trefI;
               for (trefI = (*fpIter)->get2_first_taskref();
                    trefI != (*fpIter)->taskref_list_end(); trefI++)
               {
                  cerr << "[" << (*trefI).m_start << ","
                       << (*trefI).m_finish << "] ";
               }
               if ((*fpIter)->get_no_taskrefs() > 0)
                  cerr << endl;
           }
       }

       // dumping user attributes
       if (m_uattrL.size() > 0)
       {
           cerr << "    User defined attrs:" << endl;
           for (uIter = m_uattrL.begin();
                uIter != m_uattrL.end(); uIter++)
               (*uIter).dump();
       }
       cerr << endl;
   }
// ------------------------------------------------------------
   void AX_FuncBlkC::dump_simple()
   {
      AX_CompC::dump();
   }
// ------------------------------------------------------------
   void AX_FuncBlkC::write_SystemC( AX_Gen_SystemCC* _gen_sc )
   {
       // put tasks into Gen_SystemC object
       _gen_sc->copy_resource();
   }
// ------------------------------------------------------------
   void AX_FuncBlkC::write(fstream & File)
   {
       AX_CompC::write(File);
       AX_InterfacePLCIter iIter;
       AX_UAttrLCIter uIter;

       // writing basic info
       File << "    Function " << m_function
            << "  type " << m_type
            << "  functype " << m_functype << endl;
       File.precision(4);
       File << "    Utilization " << m_util << endl;

       // writing platform
       if (m_platfmP != NULL)
       {
           File << "    Platform:" << endl;
           m_platfmP->write_simple(File);
       }

       // writing tasks
       AX_TaskRefLSIter taskrefI;
       if (m_taskrefL.size() > 0)
       {
           File << "    Tasks: ";
           for (taskrefI = m_taskrefL.begin();
                taskrefI != m_taskrefL.end(); taskrefI++)
           {
               File << (*taskrefI).m_taskP->get_name() << ":";
               File << "[" << (*taskrefI).m_start << ","
                    << (*taskrefI).m_finish << "]";
               if (taskrefI != (--m_taskrefL.end()))
                  File << "  ";
           }
           File << endl;
       }

       // writing input ports
       AX_CompPortPLCIter cpIter;
       if (m_inportPL.size() > 0)
       {
           File << "    Input port:" << endl;
           for (cpIter = m_inportPL.begin();
                cpIter != m_inportPL.end(); cpIter++)
               (*cpIter)->write_simple(File);
       }

       // writing output ports
       if (m_outportPL.size() > 0)
       {
           File << "    Output port:" << endl;
           for (cpIter = m_outportPL.begin();
                cpIter != m_outportPL.end(); cpIter++)
               (*cpIter)->write_simple(File);
       }

       // writing io ports
       if (m_ioportPL.size() > 0)
       {
           File << "    IO port:" << endl;
           for (cpIter = m_ioportPL.begin();
                cpIter != m_ioportPL.end(); cpIter++)
               (*cpIter)->write_simple(File);
       }

       // writing local variables
       File << "    clk_rate: " << m_max_clk_rate
            << "  no_stages: " << m_no_stages << endl;
       File << "    leak_current: " << m_leak_current
            << "  run_pwr: " << m_run_pwr 
            << "  temp: " << m_temp << endl;

       // writing dependency list
       AX_FuncBlkPLCIter fpIter;
       if (m_fncblkPL.size() > 0)
       {
           File << "    fncblkPL:" << endl;
           File << "      ";
           for (fpIter = m_fncblkPL.begin();
                fpIter != m_fncblkPL.end(); fpIter++)
           {
              File << (*fpIter)->get_name();
              if (fpIter != (--m_fncblkPL.end()))
                 File << ", ";
              else
                 File << endl;
           }
       }

       if (m_depPL.size() > 0)
       {
           File << "    depPL:" << endl;
           for (fpIter = m_depPL.begin();
                fpIter != m_depPL.end(); fpIter++)
           {
               File << "  ";
               (*fpIter)->write_simple(File);
           }
       }

       // writing references
       if (m_refPL.size() > 0)
       {
           File << "    refPL:" << endl;
           for (fpIter = m_refPL.begin();
                fpIter != m_refPL.end(); fpIter++)
           {
               File << "  ";
               (*fpIter)->write_simple(File);
               if ((*fpIter)->get_no_taskrefs() > 0)
                  File << "      ";
               AX_TaskRefLSIter trefI;
               for (trefI = (*fpIter)->get2_first_taskref();
                    trefI != (*fpIter)->taskref_list_end(); trefI++)
               {
                  File << "[" << (*trefI).m_start << ","
                       << (*trefI).m_finish << "] ";
               }
               if ((*fpIter)->get_no_taskrefs() > 0)
                  File << endl;
           }
       }

       // writing user attributes
       if (m_uattrL.size() > 0)
       {
           File << "    User defined attrs:" << endl;
           for (uIter = m_uattrL.begin();
                uIter != m_uattrL.end(); uIter++)
               (*uIter).write(File);
       }
       File << endl;
   }
// ------------------------------------------------------------
   void AX_FuncBlkC::write_simple(fstream & File)
   {
      AX_CompC::write(File);
   }
// ------------------------------------------------------------

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~End of Function Definitions
