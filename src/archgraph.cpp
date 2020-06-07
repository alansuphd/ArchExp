// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// archgraph.c
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <iostream>
#include "archgraph.h"

void find_special_err(AX_ArchGraphC & ag, char * posStr);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructor
// ------------------------------------------------------------
   AX_ArchGraphC::AX_ArchGraphC()
   {
      m_name.clear();
      m_id = -1;
      m_dsnP = NULL;
      m_status = UNASSEMBLED;
      m_latency = -1;
      m_area = -1;
      m_power = -1;
      m_cost = -1.0;
      m_fitness = 0.0;
      m_temp = -1;
   }
// ------------------------------------------------------------
   AX_ArchGraphC::AX_ArchGraphC(const AX_ArchGraphC & other)
   {
      m_name = other.m_name;
      m_id = other.m_id;
      m_dsnP = other.m_dsnP;
      m_status = other.m_status;
   }
// ------------------------------------------------------------
   AX_ArchGraphC::AX_ArchGraphC(AX_ArchGraphC & other)
   {
      copy(other);
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Destructor
// ------------------------------------------------------------
   AX_ArchGraphC::~AX_ArchGraphC()
   {
      remove_all_ag_inports();
      remove_all_ag_outports();
      remove_all_fncblks();
      remove_all_intrfcs();
      remove_all_platfms();
      remove_all_compports();
      remove_all_buses();
      remove_all_schdtimes();
      clean_up();
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Assignator & Comparators
// ------------------------------------------------------------
  AX_ArchGraphC & AX_ArchGraphC::operator= (const AX_ArchGraphC & other)
  {
      if (&other == this)
         return(*this);

      return(*this);
   }
// ------------------------------------------------------------
  AX_ArchGraphC & AX_ArchGraphC::operator= (AX_ArchGraphC & other)
  {
      if (&other == this)
         return(*this);

      copy(other);

      return(*this);
   }
// ------------------------------------------------------------
   void AX_ArchGraphC::copy(AX_ArchGraphC & other)
   {
      m_name = other.m_name;
      m_id = other.m_id;
      m_dsnP = other.m_dsnP;
      m_status = other.m_status;

      AX_CompPortLCIter pIter;
      AX_CompPortPLCIter ppIter;
      AX_CompPortLCIter newPortI;
      AX_BusLCIter newBusI;
      AX_BusPLCIter bpIter;
      AX_BusLCIter newBusPI;
      AX_BusCP busP;
      AX_ConnPntLCIter newConnPntI;

      // reconstructing the platforms. Becuase of the
      // sub-platform and interface, the copy has to
      // be executed in two passes. We do the platform first
      m_platfmL.clear();
      AX_PlatformLCIter pfIter;
      for (pfIter = other.m_platfmL.begin();
           pfIter != other.m_platfmL.end(); pfIter++)
      {
         AX_PlatformC nPlatfm;
         AX_PlatformLCIter nPlatfmI;
         m_platfmL.push_back(nPlatfm);
         nPlatfmI = --m_platfmL.end();
         (*nPlatfmI).copyBasic(*pfIter);
      }

      // reconstructing the interface, 1st pass
      m_intrfcL.clear();
      AX_InterfaceLCIter ifIter;
      for (ifIter = other.m_intrfcL.begin();
           ifIter != other.m_intrfcL.end(); ifIter++)
      {
         AX_InterfaceC nIntrfc;
         AX_InterfaceLCIter nIntrfcI;
         m_intrfcL.push_back(nIntrfc);
         nIntrfcI = --m_intrfcL.end();
         (*nIntrfcI).copy(*ifIter);
         (*nIntrfcI).set_platfm(NULL);
      }

      // the second pass of the platform and interface
      // reconstructing. Again we do the platform first
      for (pfIter = other.m_platfmL.begin();
           pfIter != other.m_platfmL.end(); pfIter++)
      {
          // reconstructing the sub-platform and
          // implement sub-platform
          AX_PlatformPLCIter pfpIter;
          AX_PlatformCP PlatfmP;
          AX_PlatformCP subPlatfmP;

          // get the relative platform for reconstructing
          PlatfmP = get_platfm_by_id((*pfIter).get_id());
          // reconstruct the sub-platform
          PlatfmP->remove_all_subpltfms();
          for (pfpIter = (*pfIter).get2_first_subpltfm();
               pfpIter != (*pfIter).subpltfm_list_end(); pfpIter++)
          {
             subPlatfmP = get_platfm_by_id((*pfpIter)->get_id());
             PlatfmP->add_new_subpltfm(subPlatfmP);
          }
          // reconstruct the implement sub-platform
          PlatfmP->remove_all_impl_subpltfms();
          for (pfpIter = (*pfIter).get2_first_impl_subpltfm();
               pfpIter != (*pfIter).impl_subpltfm_list_end(); pfpIter++)
          {
             subPlatfmP = get_platfm_by_id((*pfpIter)->get_id());
             PlatfmP->add_new_impl_subpltfm(subPlatfmP);
          }

          // reconstructing the interface and implement interface
          AX_InterfacePLCIter ifpIter;
          AX_InterfaceCP subIntrfcP;

          // reconstruct the interface
          PlatfmP->remove_all_intrfcs();
          for (ifpIter = (*pfIter).get2_first_intrfc();
               ifpIter != (*pfIter).intrfc_list_end(); ifpIter++)
          {
             subIntrfcP = get_intrfc_by_id((*ifpIter)->get_id());
             PlatfmP->add_new_intrfc(subIntrfcP);
          }
          // reconstruct the implement interface
          PlatfmP->remove_all_impl_intrfcs();
          for (ifpIter = (*pfIter).get2_first_impl_intrfc();
               ifpIter != (*pfIter).impl_intrfc_list_end(); ifpIter++)
          {
             subIntrfcP = get_intrfc_by_id((*ifpIter)->get_id());
             PlatfmP->add_new_impl_intrfc(subIntrfcP);
          }
      }

      // reconstructing the interface, 2st pass
      for (ifIter = other.m_intrfcL.begin();
           ifIter != other.m_intrfcL.end(); ifIter++)
      {
          AX_InterfaceCP nIntrfcP;
          AX_PlatformCP refPlatfmP;

          nIntrfcP = get_intrfc_by_id((*ifIter).get_id());
          refPlatfmP = get_platfm_by_id((*ifIter).get_platfm()->get_id());
          nIntrfcP->set_platfm(refPlatfmP);
      }

      // re-construct the bus, 1st pass. The second pass will be
      // done in the port re-construction, to fill in connpntL info
      m_busL.clear();
      AX_BusLCIter busI;
      for (busI = other.m_busL.begin();
           busI != other.m_busL.end(); busI++)
      {
         AX_BusLCIter newBusI;
         newBusI = create_new_bus();
         (*newBusI).set_id((*busI).get_id());
         (*newBusI).set_db_idx((*busI).get_db_idx());
         AX_InterfaceCP newIntrfcP;
         newIntrfcP = get_intrfc_by_id((*busI).get_intrfc()->get_id());
#if DEBUG >= 1
         if (newIntrfcP == NULL)
         {
            cerr << m_name << ": Cannot find intrfc "
                 << (*busI).get_intrfc()->get_name()
                 << " by id " << (*busI).get_intrfc()->get_id() << endl;
         }
#endif
         (*newBusI).set_intrfc(newIntrfcP);

         // reconstruct the connrefL info
         AX_ConnRefLSIter conRefI;
         for (conRefI = (*busI).get2_first_connref();
              conRefI != (*busI).connref_list_end(); conRefI++)
         {
            AX_ConnRefS newRef;
            newRef.m_connP = NULL;
            AX_ConnRefLSIter newRefI;
            newRefI = (*newBusI).add_new_connref(newRef);
            (*newRefI).m_connP = (*conRefI).m_connP;
            (*newRefI).m_start = (*conRefI).m_start;
            (*newRefI).m_finish = (*conRefI).m_finish;
         }
      }

      // reconstruct the graph input ports and buses (partially)
      m_ag_inportL.clear();
      for (pIter = other.m_ag_inportL.begin();
           pIter != other.m_ag_inportL.end(); pIter++)
      {
         AX_CompPortC nPort;
         m_ag_inportL.push_back(nPort);
         newPortI = --m_ag_inportL.end();
         (*newPortI).copy(*pIter);
         (*newPortI).remove_all_buses();
         (*newPortI).remove_all_intrfcs();
         (*newPortI).set_comp(NULL);
         (*newPortI).set_ag(this);

         // reconstruct the interface pointers
         AX_InterfacePLCIter ifpIter;
         for (ifpIter = (*pIter).get2_first_intrfc();
              ifpIter != (*pIter).intrfc_list_end(); ifpIter++)
         {
#if DEBUG >= 1
             if ((*ifpIter) == NULL)
                cerr << "\n<<=== " << other.m_name << ":"
                     << (*pIter).get_name()
                     << " has null pointer in the intrfcL\n";
#endif
             AX_InterfaceCP nIntrfcP;
             nIntrfcP = get_intrfc_by_id((*ifpIter)->get_id());
             (*newPortI).add_new_intrfc(nIntrfcP);
         }

         // adding input buses with partial information
         for (bpIter = (*pIter).get2_first_bus();
              bpIter != (*pIter).bus_list_end(); bpIter++)
         {
            busP = get_bus_by_id((*bpIter)->get_id());
            if (busP == NULL)
            {
#if DEBUG >= 1
               cerr << "Cannot find bus by id\n";
#endif
               continue;
            } // cannot find bus by id
            else
            {
               AX_ConnPntC nConnPnt;
               nConnPnt.set_ag(this);
               nConnPnt.set_comp(NULL);
               nConnPnt.set_port(&(*newPortI));
               newConnPntI = busP->add_new_connpnt(nConnPnt);
               (*newPortI).add_new_bus(busP);
            } // bus already constructed
         }
      }

      // reconstruct the graph output ports and buses (partially)
      m_ag_outportL.clear();
      for (pIter = other.m_ag_outportL.begin();
           pIter != other.m_ag_outportL.end(); pIter++)
      {
         AX_CompPortC nPort;
         m_ag_outportL.push_back(nPort);
         newPortI = --m_ag_outportL.end();
         (*newPortI).copy(*pIter);
         (*newPortI).remove_all_buses();
         (*newPortI).remove_all_intrfcs();
         (*newPortI).set_comp(NULL);
         (*newPortI).set_ag(this);

         // reconstruct the interface pointers
         AX_InterfacePLCIter ifpIter;
         for (ifpIter = (*pIter).get2_first_intrfc();
              ifpIter != (*pIter).intrfc_list_end(); ifpIter++)
         {
#if DEBUG >= 1
             if ((*ifpIter) == NULL)
                cerr << "\n<<=== " << other.m_name << ":"
                     << (*pIter).get_name()
                     << " has null pointer in the intrfcL\n";
#endif
             AX_InterfaceCP nIntrfcP;
             nIntrfcP = get_intrfc_by_id((*ifpIter)->get_id());
             (*newPortI).add_new_intrfc(nIntrfcP);
         }

         // adding output buses with partial information
         for (bpIter = (*pIter).get2_first_bus();
              bpIter != (*pIter).bus_list_end(); bpIter++)
         {
            busP = get_bus_by_id((*bpIter)->get_id());
            if (busP == NULL)
            {
#if DEBUG >= 1
               cerr << "Cannot find bus by id\n";
#endif
               continue;
            } // cannot find bus by id
            else
            {
               AX_ConnPntC nConnPnt;
               nConnPnt.set_ag(this);
               nConnPnt.set_comp(NULL);
               nConnPnt.set_port(&(*newPortI));
               newConnPntI = busP->add_new_connpnt(nConnPnt);
               (*newPortI).add_new_bus(busP);
            } // bus is reconstructed
         }
      }

      AX_FuncBlkLCIter fIter;
      AX_FuncBlkLCIter newFuncI;

      // reconstructing functional block list
      m_fncblkL.clear();
      for (fIter = other.m_fncblkL.begin();
           fIter != other.m_fncblkL.end(); fIter++)
      {
         AX_FuncBlkC nFunc;
         m_fncblkL.push_back(nFunc);
         newFuncI = --m_fncblkL.end();
         (*newFuncI).copyBasic(*fIter);

         // reconstruct the platform pointer
         AX_PlatformCP oPlatfmP, nPlatfmP = NULL;
         oPlatfmP = (*fIter).get_platfm();
         if (oPlatfmP != NULL)
            nPlatfmP = get_platfm_by_id(oPlatfmP->get_id());
         (*newFuncI).set_platfm(nPlatfmP);

         // reconstruct the input ports
         for (ppIter = (*fIter).get2_first_inport();
              ppIter != (*fIter).inport_list_end(); ppIter++)
         {
            AX_CompPortC nPort;
            m_compportL.push_back(nPort);
            newPortI = --m_compportL.end();
            (*newPortI).copy(*(*ppIter));
            (*newPortI).remove_all_buses();
            (*newPortI).remove_all_intrfcs();
            (*newPortI).set_sw_port(NULL);
            (*newPortI).set_refCnt(0);
            (*newPortI).set_comp((AX_CompCP)&(*newFuncI));
            AX_CompPortCP nportP = &(*newPortI);
            (*newFuncI).add_new_inport(nportP);

            // reconstruct the interface pointers
            AX_InterfacePLCIter ifpIter;
            for (ifpIter = (*ppIter)->get2_first_intrfc();
                 ifpIter != (*ppIter)->intrfc_list_end(); ifpIter++)
            {
#if DEBUG >= 1
                if ((*ifpIter) == NULL)
                   cerr << "\n<<=== " << (*fIter).get_name() << ":"
                        << (*ppIter)->get_name()
                        << " has null pointer in the intrfcL\n";
#endif
                AX_InterfaceCP nIntrfcP;
                nIntrfcP = get_intrfc_by_id((*ifpIter)->get_id());
                (*newPortI).add_new_intrfc(nIntrfcP);
            }

            // reconstruct buses
            for (bpIter = (*ppIter)->get2_first_bus();
                 bpIter != (*ppIter)->bus_list_end(); bpIter++)
            {
               // check to see if the bus already existed
               busP = get_bus_by_id((*bpIter)->get_id());
               // bus is not reconstructed yet
               if (busP == NULL)
               {
#if DEBUG >= 1
               cerr << "Cannot find bus by id\n";
#endif
               continue;
            } // cannot find bus by id
               else // bus is reconstructed
               {
                  AX_ConnPntC nConnPnt;
                  nConnPnt.set_ag(NULL);
                  nConnPnt.set_comp((AX_CompCP)&(*newFuncI));
                  nConnPnt.set_port(&(*newPortI));
                  newConnPntI = busP->add_new_connpnt(nConnPnt);
                  (*newPortI).add_new_bus(busP);
               }
            } // end bus for
         } // end inport for

         // reconstruct the output ports
         for (ppIter = (*fIter).get2_first_outport();
              ppIter != (*fIter).outport_list_end(); ppIter++)
         {
            AX_CompPortC nPort;
            m_compportL.push_back(nPort);
            newPortI = --m_compportL.end();
            (*newPortI).copy(*(*ppIter));
            (*newPortI).remove_all_buses();
            (*newPortI).remove_all_intrfcs();
            (*newPortI).set_sw_port(NULL);
            (*newPortI).set_refCnt(0);
            (*newPortI).set_comp((AX_CompCP)&(*newFuncI));
            AX_CompPortCP nportP = &(*newPortI);
            (*newFuncI).add_new_outport(nportP);

            // reconstruct the interface pointers
            AX_InterfacePLCIter ifpIter;
            for (ifpIter = (*ppIter)->get2_first_intrfc();
                 ifpIter != (*ppIter)->intrfc_list_end(); ifpIter++)
            {
#if DEBUG >= 1
                if ((*ifpIter) == NULL)
                   cerr << "\n<<=== " << (*fIter).get_name() << ":"
                        << (*ppIter)->get_name()
                        << " has null pointer in the intrfcL\n";
#endif
                AX_InterfaceCP nIntrfcP;
                nIntrfcP = get_intrfc_by_id((*ifpIter)->get_id());
                (*newPortI).add_new_intrfc(nIntrfcP);
            }

            // reconstruct buses
            for (bpIter = (*ppIter)->get2_first_bus();
                 bpIter != (*ppIter)->bus_list_end(); bpIter++)
            {
               // check to see if the bus already existed
               busP = get_bus_by_id((*bpIter)->get_id());
               // bus is not reconstructed yet
               if (busP == NULL)
               {
#if DEBUG >= 1
                  cerr << "Cannot find bus by id\n";
#endif
                  continue;
               } // cannot find bus by id
               else // bus is reconstructed
               {
                  AX_ConnPntC nConnPnt;
                  nConnPnt.set_ag(NULL);
                  nConnPnt.set_comp((AX_CompCP)&(*newFuncI));
                  nConnPnt.set_port(&(*newPortI));
                  newConnPntI = busP->add_new_connpnt(nConnPnt);
                  (*newPortI).add_new_bus(busP);
               }
            } // end bus for
         } // end outport for

         // reconstruct the io ports
         for (ppIter = (*fIter).get2_first_ioport();
              ppIter != (*fIter).ioport_list_end(); ppIter++)
         {
            AX_CompPortC nPort;
            m_compportL.push_back(nPort);
            newPortI = --m_compportL.end();
            (*newPortI).copy(*(*ppIter));
            (*newPortI).remove_all_buses();
            (*newPortI).remove_all_intrfcs();
            (*newPortI).set_sw_port(NULL);
            (*newPortI).set_refCnt(0);
            (*newPortI).set_comp((AX_CompCP)&(*newFuncI));
            AX_CompPortCP nportP = &(*newPortI);
            (*newFuncI).add_new_ioport(nportP);

            // reconstruct the interface pointers
            AX_InterfacePLCIter ifpIter;
            for (ifpIter = (*ppIter)->get2_first_intrfc();
                 ifpIter != (*ppIter)->intrfc_list_end(); ifpIter++)
            {
#if DEBUG >= 1
                if ((*ifpIter) == NULL)
                   cerr << "\n<<=== " << (*fIter).get_name() << ":"
                        << (*ppIter)->get_name()
                        << " has null pointer in the intrfcL\n";
#endif
                AX_InterfaceCP nIntrfcP;
                nIntrfcP = get_intrfc_by_id((*ifpIter)->get_id());
                (*newPortI).add_new_intrfc(nIntrfcP);
            }

            // reconstruct buses
            for (bpIter = (*ppIter)->get2_first_bus();
                 bpIter != (*ppIter)->bus_list_end(); bpIter++)
            {
               // check to see if the bus already existed
               busP = get_bus_by_id((*bpIter)->get_id());
               // bus is not reconstructed yet
               if (busP == NULL)
               {
#if DEBUG >= 1
                  cerr << "Cannot find bus by id\n";
#endif
                  continue;
               } // cannot find bus by id
               else // bus is reconstructed
               {
                  AX_ConnPntC nConnPnt;
                  nConnPnt.set_ag(NULL);
                  nConnPnt.set_comp((AX_CompCP)&(*newFuncI));
                  nConnPnt.set_port(&(*newPortI));
                  newConnPntI = busP->add_new_connpnt(nConnPnt);
                  (*newPortI).add_new_bus(busP);
               }
            } // end bus for
         } // end ioport for
      } // end fncblk for

      // the second pass is to fill up the dependency and reference list
      for (fIter = other.m_fncblkL.begin();
           fIter != other.m_fncblkL.end(); fIter++)
      {
         AX_FuncBlkPLCIter fpIter;
         AX_FuncBlkCP funcP  = get_fncblk_by_id((*fIter).get_id());

         // reconstruct the dependency list
         funcP->remove_all_deps();
         for (fpIter = (*fIter).get2_first_dep();
              fpIter != (*fIter).dep_list_end(); fpIter++)
         {
             AX_FuncBlkCP rfuncP = get_fncblk_by_id((*fpIter)->get_id());
             funcP->add_new_dep(rfuncP);
         }

         // reconstruct the reference list
         funcP->remove_all_refs();
         for (fpIter = (*fIter).get2_first_ref();
              fpIter != (*fIter).ref_list_end(); fpIter++)
         {
             AX_FuncBlkCP rfuncP = get_fncblk_by_id((*fpIter)->get_id());
             funcP->add_new_ref(rfuncP);
         } // end ref for

      } // end fncblk second for

      // the second pass for internal compport to fill up
      // m_sw_portP, the sw port pointer to its referenced hw port
      for (pIter = other.m_compportL.begin();
           pIter != other.m_compportL.end(); pIter++)
      {
         if ((*pIter).get_sw_port() != NULL)
         {
            AX_CompPortCP swPortP;
            swPortP = get_compport_by_id((*pIter).get_sw_port()->get_id());
            if (swPortP == NULL)
            {
#if DEBUG >= 1
               cerr << "(a)Get compport by id failed for ";
               if ((*pIter).get_sw_port()->get_ag() != NULL)
               {
                  cerr << "ERROR PORT: "
                       << (*pIter).get_sw_port()->get_ag()->get_name();
               }
               else
                  cerr << (*pIter).get_sw_port()->get_comp()->get_name();
               cerr << "." << (*pIter).get_sw_port()->get_name()
                    << endl;
#endif
            }
            AX_CompPortCP newPortP;
            newPortP = get_compport_by_id((*pIter).get_id());
            if (newPortP == NULL)
            {
#if DEBUG >= 1
               cerr << "(b)Get compport by id failed for ";
               if ((*pIter).get_ag() != NULL)
               {
                  cerr << "ERROR PORT: "
                       << (*pIter).get_ag()->get_name();
               }
               else
                  cerr << (*pIter).get_comp()->get_name();
               cerr << "." << (*pIter).get_name() << endl;
#endif
            }
            newPortP->set_sw_port(swPortP);
            swPortP->inc_refCnt();
         }
      }

      // copy schedule time
      AX_SchdTimeLSIter stimeI;
      remove_all_schdtimes();
      for (stimeI = other.m_schdtimeL.begin();
           stimeI != other.m_schdtimeL.end(); stimeI++)
      {
         AX_SchdTimeS nTime;
         AX_SchdTimeLSIter nTimeI;

         m_schdtimeL.push_back(nTime);
         nTimeI = --m_schdtimeL.end();
         (*nTimeI).m_taskP = (*stimeI).m_taskP;
         (*nTimeI).m_fncblkP = (*stimeI).m_fncblkP;
         (*nTimeI).m_connP = (*stimeI).m_connP;
         (*nTimeI).m_busP = (*stimeI).m_busP;
         (*nTimeI).m_start = (*stimeI).m_start;
         (*nTimeI).m_finish = (*stimeI).m_finish;
         (*nTimeI).m_temp = (*stimeI).m_temp;
      }

      m_latency = other.m_latency;
      m_area = other.m_area;
      m_power = other.m_power;
      m_cost = other.m_cost;
      m_fitness = other.m_fitness;
      m_temp = other.m_temp;

      return;
   }
// ------------------------------------------------------------
   bool AX_ArchGraphC::operator== (const AX_ArchGraphC & other)
   {
      if (&other == this)
         return (true);

      return (false);
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Pointer List Manipulations
// ------------------------------------------------------------
   AX_FuncBlkLCIter AX_ArchGraphC::add_new_fncblk(AX_FuncBlkC & fncblk)
   {
      AX_FuncBlkLCIter fIter;

      for (fIter = m_fncblkL.begin();
           fIter != m_fncblkL.end(); fIter++)
          if (fncblk == (*fIter))
             return fIter;

      m_fncblkL.push_back(fncblk);
      fIter = --m_fncblkL.end();

      return fIter;
   }
// ------------------------------------------------------------
   AX_FuncBlkLCIter AX_ArchGraphC::add_fncblk(AX_FuncBlkC & fncblk)
   {
      AX_FuncBlkLCIter fIter;

      m_fncblkL.push_back(fncblk);
      fIter = --m_fncblkL.end();

      return fIter;
   }
// ------------------------------------------------------------
   AX_FuncBlkCP AX_ArchGraphC::get_fncblk(int index)
   {
      AX_FuncBlkCP fncblkP;

      if ((index < 0)||((unsigned int)index > m_fncblkL.size()-1))
         fncblkP = NULL;
      else
      {
         AX_FuncBlkLCIter fIter;
         int i;

         fIter = m_fncblkL.begin();
         for(i = 0; i < index; i++) fIter++;

         fncblkP = &(*fIter);
      }

      return fncblkP;
   }
// ------------------------------------------------------------
   AX_FuncBlkCP AX_ArchGraphC::get_fncblk(string name)
   {
      if (name.empty())
         return NULL;
      else
      {
         AX_FuncBlkLCIter fIter;

         for (fIter = m_fncblkL.begin();
              fIter != m_fncblkL.end(); fIter++)
            if ((*fIter).get_name() == name)
               return &(*fIter);
      }

      return NULL;
   }
// ------------------------------------------------------------
   AX_FuncBlkCP AX_ArchGraphC::get_fncblk_by_id(int id)
   {

      if (id < 0)
         return NULL;

      AX_FuncBlkLCIter fIter;
      for (fIter = m_fncblkL.begin();
           fIter != m_fncblkL.end(); fIter++)
         if ((*fIter).get_id() == id)
            return &(*fIter);

      return NULL;
   }
// ------------------------------------------------------------
   AX_FuncBlkCP AX_ArchGraphC::get_fncblk_by_task(AX_TaskCP taskP)
   {
      if (taskP == NULL)
         return NULL;

      AX_FuncBlkLCIter fIter;
      for (fIter = m_fncblkL.begin();
           fIter != m_fncblkL.end(); fIter++)
         if ((*fIter).is_mapped_task(taskP))
            return &(*fIter);

      return NULL;
   }
// ------------------------------------------------------------
   void AX_ArchGraphC::remove_fncblk(AX_FuncBlkCP fncblkP)
   {
      AX_FuncBlkLCIter fIter;

      for (fIter = m_fncblkL.begin();
           fIter != m_fncblkL.end(); fIter++)
          if (&(*fIter) == fncblkP)
          {
             AX_FuncBlkC fTmp;
             (*fIter) = fTmp;
             m_fncblkL.erase(fIter);
             return;
          }
      return;
   }
// ------------------------------------------------------------
   AX_FuncBlkLCIter AX_ArchGraphC::delete_fncblk(AX_FuncBlkLCIter & iter)
   {
      AX_FuncBlkLCIter retIter;

      if (m_fncblkL.end() == iter)
         return iter;
      else if (m_fncblkL.size() == 0)
         return m_fncblkL.end();
      else if ((--m_fncblkL.end()) == iter)
      {
         AX_FuncBlkC fTmp;
         (*iter) = fTmp;
         m_fncblkL.erase(iter);
         retIter = m_fncblkL.end();
      }
      else
      {
         retIter = ++iter;
         iter--;
         AX_FuncBlkC fTmp;
         (*iter) = fTmp;
         m_fncblkL.erase(iter);
      }

      return retIter;
   }
// ------------------------------------------------------------
   AX_FuncBlkLCIter AX_ArchGraphC::delete_fncblk(AX_FuncBlkCP funcP)
   {
      AX_FuncBlkLCIter fIter;

      if (m_fncblkL.size() == 0)
         return m_fncblkL.end();

      if (&(*(--m_fncblkL.end())) == funcP)
      {
         fIter = --m_fncblkL.end();
         AX_FuncBlkC fTmp;
         (*fIter) = fTmp;
         m_fncblkL.erase(fIter);
         return m_fncblkL.end();
      }

      for (fIter = m_fncblkL.begin();
           fIter != m_fncblkL.end(); fIter++)
         if (&(*fIter) == funcP)
         {
            AX_FuncBlkLCIter retIter = ++fIter;
            fIter--;
            AX_FuncBlkC fTmp;
            (*fIter) = fTmp;
            m_fncblkL.erase(fIter);
            return retIter;
         }
      return fIter;
   }
// ------------------------------------------------------------
   void AX_ArchGraphC::remove_all_fncblks()
   {
      AX_FuncBlkLCIter fIter;

      AX_FuncBlkC fTmp;
      while ((fIter = m_fncblkL.begin()) != m_fncblkL.end())
      {
         (*fIter) = fTmp;
         m_fncblkL.erase(fIter);
      }

      m_fncblkL.clear();
   }
// ------------------------------------------------------------
   AX_FuncBlkCP AX_ArchGraphC::find_fncblk_by_task(AX_TaskCP task)
   {
      AX_FuncBlkLCIter fIter;
      for (fIter = get2_first_fncblk();
           fIter != fncblk_list_end(); fIter++)
      {
	 if ((*fIter).get_functype() == PROCESSOR)
	     continue;
         AX_TaskRefLSIter taskrefI;
         for (taskrefI = (*fIter).get2_first_taskref();
              taskrefI != (*fIter).taskref_list_end(); taskrefI++)
            if ((*taskrefI).m_taskP == task)
               return &(*fIter);
      }

      return NULL;
   }
// ------------------------------------------------------------
// This function erase the m_depPL in the given funcblk funcP.
// While process through m_depPL, funcP is removed from m_refPL
// of the funcblk, say depP, referred in funcP's m_depPL.
// At this point if no other funcblks depend on depP anymore,
// depP is removed from the architecture graph.
// ------------------------------------------------------------
   void AX_ArchGraphC::erase_dep_fm_fncblk(AX_FuncBlkCP funcP,
                                            AX_TaskCP taskP)
   {
      AX_FuncBlkPLCIter fpIter;

      for (fpIter = funcP->get2_first_dep();
           fpIter != funcP->dep_list_end(); fpIter++)
      {
         if ((*fpIter)->get_functype() == MEMORY ||
             (*fpIter)->get_functype() == MEMORY_CORE)
         {
            AX_UAttrCP uattrP;
            uattrP = (*fpIter)->get_uattr(UATTRGENCAPREM);
            if (uattrP == NULL)
            {
#if DEBUG >= 1
               cerr << this->m_name << ": "
                    << (*fpIter)->get_name()
                    << " does not have GenCapRem user attr\n";
#endif
               continue;
            }
            char * cstr = new char[(uattrP->get_value()).size()+1];
            strcpy(cstr, (uattrP->get_value()).c_str());
            float mCap = atof(cstr);
            delete cstr;

            AX_FuncBlkPLCIter mScanPI;
            for (mScanPI = funcP->get2_first_fncblk();
                 mScanPI != funcP->fncblk_list_end(); mScanPI++)
            {
               // special treatment for memory requirement
               if ((*mScanPI)->get_functype() == MEMREQ)
               {
                  bool codeMem = true;
                  float oneSize = mem_parse((*mScanPI)->get_name(),
                                            codeMem);
                  mCap += (int)oneSize;
               }
            }
            AX_UAttrC nUattr;
            string nTmpS = UATTRGENCAPREM;
            nUattr.set_name(nTmpS);
            nUattr.set_type(T_FLOAT);
            char uVal[20];
            my_itoa((int)mCap, uVal);
            nUattr.set_value(uVal);
            (*fpIter)->delete_uattr(uattrP);
            (*fpIter)->add_new_uattr(nUattr);
         }
         (*fpIter)->delete_ref(funcP);

         // if the funcblk does not reference to a task
         // and no other funcblk reference to this funblk,
         // then delete this funcblk and the compports
         if ((*fpIter)->get_no_taskrefs() == 0 &&
             (*fpIter)->get_no_refs() == 0)
         {
            // first we remove the dep pointer in the funcblks
            // in its depPL. The dep pointer in the other funcblk
            // is added in AX_EvolveC::connect() for processors
            // and the memory the processor uses. Notice that
            // depPL is used by other funcblks as actual dependency
            // list in the ag.
            AX_FuncBlkPLCIter depPI;
            for (depPI = (*fpIter)->get2_first_dep();
                 depPI != (*fpIter)->dep_list_end(); depPI++)
            {
               (*depPI)->delete_dep(*fpIter);
            }
            delete_compports_by_fncblk((*fpIter), funcP, NULL);
            delete_fncblk(*fpIter);
         }
      }
      funcP->remove_all_deps();

      return;
   }
// ------------------------------------------------------------
   AX_FuncBlkCP AX_ArchGraphC::pick_largest_mem(int & capRem)
   {
      AX_FuncBlkLCIter fIter;
      AX_FuncBlkCP maxMem = NULL;
      int max = -1;

      for (fIter = m_fncblkL.begin();
           fIter != m_fncblkL.end(); fIter++)
      {
         if ((*fIter).get_functype() == MEMORY ||
             (*fIter).get_functype() == MEMORY_CORE)
         {
            if ((*fIter).get_temp() == -1)
            {
               AX_UAttrCP uattrP;
               uattrP = (*fIter).get_uattr(UATTRONCHIPMEM);

               char * cstr = new char[(uattrP->get_value()).size()+1];
               strcpy(cstr, (uattrP->get_value()).c_str());
               int ftmp = atoi(cstr);
               delete cstr;

               if (ftmp > max)
               {
                  max = ftmp;
                  maxMem = &(*fIter);
               }
            }
            else
            {
               if ((*fIter).get_temp() > max)
               {
                  max = (*fIter).get_temp();
                  maxMem = &(*fIter);
               }
            }

         }
      }
      capRem = max;

      return maxMem;
   }

// ------------------------------------------------------------
   int AX_ArchGraphC::get_latency_by_task(AX_TaskCP taskP)
   {
      AX_FuncBlkLCIter fIter;
      int latency = -1;

      for (fIter = m_fncblkL.begin();
           fIter != m_fncblkL.end(); fIter++)
      {
         if ((*fIter).is_mapped_task(taskP))
         {
            AX_UAttrCP uattrP;
            uattrP = (*fIter).get_uattr(UATTRDATACOMPDELAY);

            char * cstr = new char[(uattrP->get_value()).size()+1];
            strcpy(cstr, (uattrP->get_value()).c_str());
            latency = atoi(cstr);
            delete cstr;

            return latency;
         }
      }

      return latency;
   }
// ------------------------------------------------------------
   bool AX_ArchGraphC::fncblk_exists(AX_FuncBlkCP funcP)
   {
      AX_FuncBlkLCIter fIter;

      for (fIter = m_fncblkL.begin();
           fIter != m_fncblkL.end(); fIter++)
      {
         if (&(*fIter) == funcP)
            return true;
      }

      return false;
   }
// ------------------------------------------------------------
   void AX_ArchGraphC::clean_taskref_in_proc()
   {
      AX_FuncBlkLCIter fIter;

      for (fIter = m_fncblkL.begin();
           fIter != m_fncblkL.end(); fIter++)
      {
	 if ((*fIter).get_functype() == PROCESSOR)
	    (*fIter).remove_all_taskrefs();
      }

      return;
   }
// ------------------------------------------------------------
   AX_InterfaceLCIter AX_ArchGraphC::add_new_intrfc(AX_InterfaceC & intrfc)
   {
      AX_InterfaceLCIter iIter;

      for (iIter = m_intrfcL.begin();
           iIter != m_intrfcL.end(); iIter++)
          if (intrfc == (*iIter))
             return iIter;

      m_intrfcL.push_back(intrfc);
      iIter = --m_intrfcL.end();

    return iIter;
   }
// ------------------------------------------------------------
   AX_InterfaceCP AX_ArchGraphC::get_intrfc(int index)
   {
      AX_InterfaceCP intrfcP;

      if ((index < 0)||((unsigned int)index > m_intrfcL.size()-1))
         intrfcP = NULL;
      else
      {
         AX_InterfaceLCIter iIter;
         int i;

         iIter = m_intrfcL.begin();
         for(i = 0; i < index; i++) iIter++;

         intrfcP = &(*iIter);
      }

      return intrfcP;
   }
// ------------------------------------------------------------
   AX_InterfaceCP AX_ArchGraphC::get_intrfc(string name)
   {
      if (name.empty())
         return NULL;

      AX_InterfaceLCIter iIter;
      for (iIter = m_intrfcL.begin();
           iIter != m_intrfcL.end(); iIter++)
         if ((*iIter).get_name() == name)
            return &(*iIter);

      return NULL;
   }
// ------------------------------------------------------------
   AX_InterfaceCP AX_ArchGraphC::get_intrfc_by_id(int id)
   {
      if (id < 0)
         return NULL;

      AX_InterfaceLCIter iIter;
      for (iIter = m_intrfcL.begin();
           iIter != m_intrfcL.end(); iIter++)
         if ((*iIter).get_id() == id)
            return &(*iIter);

      return NULL;
   }
// ------------------------------------------------------------
   int AX_ArchGraphC::get_intrfc_candidates(AX_CompPortCP srcPortP,
                                             AX_CompPortCP sinkPortP,
                                             AX_InterfacePLC & intrfcPL,
                                             CandidateTypeE cType)
   {
      AX_InterfacePLCIter ipIter;

      if (srcPortP->get_comp() == NULL)
      {
         // source is a graph port, so only sink function
         // interfaces is copied
         if (!get_intrfcPL_fm_port(sinkPortP, intrfcPL, true, cType))
            return -1;
      }
      else if (sinkPortP->get_comp() == NULL)
      {
         // sink is a graph port, so only source function
         // interfaces is copied
         if (!get_intrfcPL_fm_port(srcPortP, intrfcPL, false, cType))
            return -1;
      }
      else
      {
         // both source and sink are funcblks. Then we need
         // to copy only the common interfaces
         if (!get_intrfcPL_fm_port(srcPortP, sinkPortP, intrfcPL, cType))
            return -1;
      }

      return intrfcPL.size();
   }

// ------------------------------------------------------------
   bool AX_ArchGraphC::get_intrfcPL_fm_port(AX_CompPortCP portP,
                                              AX_InterfacePLC & intrfcPL,
                                              bool isSrc,
                                              CandidateTypeE cType)
   {
      if (portP->get_comp() == NULL)
         return true;

      AX_FuncBlkCP funcP = (AX_FuncBlkC *)portP->get_comp();
      AX_InterfacePLCIter ipIter;
      AX_InterfacePLCIter newIter;
      if (funcP->get_type() == HARDWARE)
      {
         // for a hardware component 
         // we simply copy the interfaces
         for (ipIter = portP->get2_first_intrfc();
              ipIter != portP->intrfc_list_end(); ipIter++)
         {
            // Since a graph output port may have multiple inputs,
            // we allow only bus interface being used here
            if (!isSrc && (*ipIter)->get_intrtype() != INTRFC_BUS)
               continue;
            if (cType == GET_BUS_ONLY &&
                (*ipIter)->get_intrtype() != INTRFC_BUS)
               continue;
            if (cType == GET_WIRE_ONLY &&
                (*ipIter)->get_intrtype() != INTRFC_WIRE)
               continue;
            bool found = false;
            for (newIter = intrfcPL.begin();
                 newIter != intrfcPL.end(); newIter++)
               if ((*newIter) == (*ipIter))
               {
                  found = true;
                  break;
               }
            if (!found)
               intrfcPL.push_back(*ipIter);
         }

         return (bool)intrfcPL.size();
      } // end funcP type == HARDWARE
      else if (funcP->get_type() == SOFTWARE)
      {
#if DEBUG >= 1
         cerr << "A software "
              << funcP->get_name()
              << " incorrectly calls get_intrfc_fm_port()" << endl;
#endif
         return false;
      }
      else
      {
#if DEBUG >= 1
         cerr << "Illegal type of "
              << funcP->get_name() << endl;
#endif
         return false;
      } // end funcP type selection

      return true;
   }

// ------------------------------------------------------------
   bool AX_ArchGraphC::get_intrfcPL_fm_port(AX_CompPortCP srcPortP,
                                              AX_CompPortCP sinkPortP,
                                              AX_InterfacePLC & intrfcPL,
                                              CandidateTypeE cType)
   {
      AX_InterfacePLCIter ipIter;
      AX_InterfacePLCIter newIter;
      AX_FuncBlkCP srcFuncP = (AX_FuncBlkC *)srcPortP->get_comp();
      AX_FuncBlkCP sinkFuncP = (AX_FuncBlkC *)sinkPortP->get_comp();

      // determine which is the true target
      if (srcFuncP->get_type() == SOFTWARE)
      {
#if DEBUG >= 1
         cerr << "A software "
              << srcFuncP->get_name()
              << " incorrectly calls get_intrfc_fm_port()" << endl;
#endif
         return false;
      }
      // determine which is the true target
      if (sinkFuncP->get_type() == SOFTWARE)
      {
#if DEBUG >= 1
         cerr << "A software "
              << sinkFuncP->get_name()
              << " incorrectly calls get_intrfc_fm_port()" << endl;
#endif
         return false;
      }

      // copy common interfaces from two funcblks
      for (ipIter = srcPortP->get2_first_intrfc();
           ipIter != srcPortP->intrfc_list_end(); ipIter++)
      {
         if (cType == GET_BUS_ONLY &&
             (*ipIter)->get_intrtype() != INTRFC_BUS)
            continue;
         if (cType == GET_WIRE_ONLY &&
             (*ipIter)->get_intrtype() != INTRFC_WIRE)
            continue;
         for (newIter = sinkPortP->get2_first_intrfc();
              newIter != sinkPortP->intrfc_list_end(); newIter++)
            if ((*ipIter) == (*newIter))
            {
               intrfcPL.push_back(*ipIter);
               break;
            }
      }

      return true;
   }

// ------------------------------------------------------------
   bool AX_ArchGraphC::collect_all_bus_intrfcs(AX_InterfacePLC & intrfcPL)
   {
      AX_InterfaceLCIter intrfcI;
      bool found = false;

      for (intrfcI = m_intrfcL.begin();
           intrfcI != m_intrfcL.end(); intrfcI++)
      {
         if ((*intrfcI).get_intrtype() == INTRFC_BUS)
         {
            intrfcPL.push_back(&(*intrfcI));
            found = true;
         }
      }

      return found;
   }

// ------------------------------------------------------------
   bool AX_ArchGraphC::collect_all_wire_intrfcs(AX_InterfacePLC & intrfcPL)
   {
      AX_InterfaceLCIter intrfcI;
      bool found = false;

      for (intrfcI = m_intrfcL.begin();
           intrfcI != m_intrfcL.end(); intrfcI++)
      {
         if ((*intrfcI).get_intrtype() == INTRFC_WIRE)
         {
            intrfcPL.push_back(&(*intrfcI));
            found = true;
         }
      }

      return found;
   }

// ------------------------------------------------------------
   void AX_ArchGraphC::remove_intrfc(AX_InterfaceCP intrfcP)
   {
      AX_InterfaceLCIter iIter;

      for (iIter = m_intrfcL.begin();
           iIter != m_intrfcL.end(); iIter++)
          if (&(*iIter) == intrfcP)
          {
             m_intrfcL.erase(iIter);
             return;
          }
      return;
   }
// ------------------------------------------------------------
   AX_InterfaceLCIter AX_ArchGraphC::delete_intrfc(AX_InterfaceLCIter & iter)
   {
      AX_InterfaceLCIter retIter;

      if (m_intrfcL.end() == iter)
         return iter;
      else if ((--m_intrfcL.end()) == iter)
      {
         AX_InterfaceC iTmp;
         (*iter) = iTmp;
         m_intrfcL.erase(iter);
         retIter = m_intrfcL.end();
      }
      else
      {
         retIter = ++iter;
         iter--;
         AX_InterfaceC iTmp;
         (*iter) = iTmp;
         m_intrfcL.erase(iter);
      }

      return retIter;
   }
// ------------------------------------------------------------
   void AX_ArchGraphC::remove_all_intrfcs()
   {
      AX_InterfaceLCIter iIter;

      AX_InterfaceC iTmp;
      while ((iIter = m_intrfcL.begin()) != m_intrfcL.end())
      {
         (*iIter) = iTmp;
         m_intrfcL.erase(iIter);
      }

      m_intrfcL.clear();
   }
// ------------------------------------------------------------
   AX_PlatformLCIter AX_ArchGraphC::add_new_platfm(AX_PlatformC & platfm)
   {
      AX_PlatformLCIter pIter;

      for (pIter = m_platfmL.begin();
           pIter != m_platfmL.end(); pIter++)
          if (platfm == (*pIter))
             return pIter;

      m_platfmL.push_back(platfm);
      pIter = --m_platfmL.end();

      return pIter;
   }
// ------------------------------------------------------------
   AX_PlatformCP AX_ArchGraphC::get_platfm(int index)
   {
      AX_PlatformCP platfmP;

      if ((index < 0)||((unsigned int)index > m_platfmL.size()-1))
         platfmP = NULL;
      else
      {
         AX_PlatformLCIter pIter;
         int i;

         pIter = m_platfmL.begin();
         for(i = 0; i < index; i++) pIter++;

         platfmP = &(*pIter);
      }

      return platfmP;
   }
// ------------------------------------------------------------
   AX_PlatformCP AX_ArchGraphC::get_platfm(string name)
   {
      if (name.empty())
         return NULL;

      AX_PlatformLCIter pIter;
      for (pIter = m_platfmL.begin();
           pIter != m_platfmL.end(); pIter++)
         if ((*pIter).get_name() == name)
            return &(*pIter);

      return NULL;
   }
// ------------------------------------------------------------
   AX_PlatformCP AX_ArchGraphC::get_platfm_by_id(int id)
   {
      if (id < 0)
         return NULL;

      AX_PlatformLCIter pIter;
      for (pIter = m_platfmL.begin();
           pIter != m_platfmL.end(); pIter++)
         if ((*pIter).get_id() == id)
            return &(*pIter);

      return NULL;
   }
// ------------------------------------------------------------
   void AX_ArchGraphC::remove_platfm(AX_PlatformCP platfmP)
   {
      AX_PlatformLCIter pIter;

      for (pIter = m_platfmL.begin();
           pIter != m_platfmL.end(); pIter++)
          if (&(*pIter) == platfmP)
          {
             m_platfmL.erase(pIter);
             return;
          }
      return;
   }
// ------------------------------------------------------------
   AX_PlatformLCIter AX_ArchGraphC::delete_platfm(AX_PlatformLCIter & iter)
   {
      AX_PlatformLCIter retIter;

      if (m_platfmL.end() == iter)
         return iter;
      else if ((--m_platfmL.end()) == iter)
      {
         AX_PlatformC pTmp;
         (*iter) = pTmp;
         m_platfmL.erase(iter);
         retIter = m_platfmL.end();
      }
      else
      {
         retIter = ++iter;
         iter--;
         AX_PlatformC pTmp;
         (*iter) = pTmp;
         m_platfmL.erase(iter);
      }

      return retIter;
   }
// ------------------------------------------------------------
   void AX_ArchGraphC::remove_all_platfms()
   {
      AX_PlatformLCIter pIter;

      AX_PlatformC pTmp;
      while ((pIter = m_platfmL.begin()) != m_platfmL.end())
      {
         (*pIter) = pTmp;
         m_platfmL.erase(pIter);
      }

      m_platfmL.clear();
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Instance List Manipulations
// ------------------------------------------------------------
   AX_CompPortLCIter AX_ArchGraphC::create_new_ag_inport()
   {
      AX_CompPortC compport;

      m_ag_inportL.push_back(compport);

      return(--m_ag_inportL.end());
   }
// ------------------------------------------------------------
   AX_CompPortLCIter AX_ArchGraphC::add_new_ag_inport(AX_CompPortC & compport)
   {
      AX_CompPortLCIter cIter;

      for (cIter = m_ag_inportL.begin();
           cIter != m_ag_inportL.end(); cIter++)
          if (compport == (*cIter))
             return cIter;

      m_ag_inportL.push_back(compport);

      return (--m_ag_inportL.end());
   }
// ------------------------------------------------------------
   AX_CompPortLCIter AX_ArchGraphC::add_new_ag_inport(string name)
   {
      AX_CompPortLCIter cIter;

      for (cIter = m_ag_inportL.begin();
           cIter != m_ag_inportL.end(); cIter++)
          if (name== (*cIter).get_name())
             return cIter;

      AX_CompPortC nPort;
      AX_CompPortLCIter retI;
      m_ag_inportL.push_back(nPort);
      retI = --m_ag_inportL.end();
      (*retI).set_name(name);
      (*retI).set_type(PORT_IN);
      (*retI).set_comp(NULL);
      (*retI).set_ag(this);

      return (retI);
   }
// ------------------------------------------------------------
   AX_CompPortCP AX_ArchGraphC::get_ag_inport(int index)
   {
      AX_CompPortCP compportP;

      if ((index < 0)||((unsigned int)index > m_ag_inportL.size()-1))
         compportP = NULL;
      else
      {
         AX_CompPortLCIter cIter;
         int i;

         cIter = m_ag_inportL.begin();
         for(i = 0; i < index; i++) cIter++;

         compportP = &(*cIter);
      }

      return compportP;
   }
// ------------------------------------------------------------
   AX_CompPortCP AX_ArchGraphC::get_ag_inport(string name)
   {
      AX_CompPortCP compportP = NULL;
      AX_CompPortLCIter cIter;

      for (cIter = m_ag_inportL.begin();
           cIter != m_ag_inportL.end(); cIter++)
          if (name == (*cIter).get_name())
          {
             compportP = &(*cIter);
             return compportP;
          }

      return compportP;
   }
// ------------------------------------------------------------
   void AX_ArchGraphC::remove_ag_inport(AX_CompPortCP compportP)
   {
       AX_CompPortLCIter cIter;

       for (cIter = m_ag_inportL.begin();
            cIter != m_ag_inportL.end(); cIter++)
           if (&(*cIter) == compportP)
           {
               m_ag_inportL.erase(cIter);
               return;
           }
       return;
   }
// ------------------------------------------------------------
   AX_CompPortLCIter AX_ArchGraphC::delete_ag_inport(AX_CompPortLCIter & iter)
   {
      AX_CompPortLCIter cIter;

      if (m_ag_inportL.end() == iter)
         return iter;
      else if ((--m_ag_inportL.end()) == iter)
      {
         AX_CompPortC pTmp;
         (*iter) = pTmp;
         m_ag_inportL.erase(iter);
         cIter = m_ag_inportL.end();
      }
      else
      {
         cIter = ++iter;
         iter--;
         AX_CompPortC pTmp;
         (*iter) = pTmp;
         m_ag_inportL.erase(iter);
      }

      return cIter;
   }
// ------------------------------------------------------------
   void AX_ArchGraphC::remove_all_ag_inports()
   {
      AX_CompPortLCIter cIter;

      AX_CompPortC pTmp;
      while ((cIter = m_ag_inportL.begin()) != m_ag_inportL.end())
      {
         (*cIter) = pTmp;
         m_ag_inportL.erase(cIter);
      }

      m_ag_inportL.clear();
   }
// ------------------------------------------------------------
   void AX_ArchGraphC::reset_ag_inport_temp()
   {
      AX_CompPortLCIter cIter;

      for (cIter = m_ag_inportL.begin();
           cIter != m_ag_inportL.end(); cIter++)
         (*cIter).set_temp(-1);
   }
// ------------------------------------------------------------
   AX_CompPortLCIter AX_ArchGraphC::create_new_ag_outport()
   {
      AX_CompPortC compport;

      m_ag_outportL.push_back(compport);

      return(--m_ag_outportL.end());
   }
// ------------------------------------------------------------
   AX_CompPortLCIter AX_ArchGraphC::add_new_ag_outport(AX_CompPortC & compport)
   {
      AX_CompPortLCIter cIter;

      for (cIter = m_ag_outportL.begin();
           cIter != m_ag_outportL.end(); cIter++)
          if (compport == (*cIter))
             return cIter;

      m_ag_outportL.push_back(compport);

      return (--m_ag_outportL.end());
   }
// ------------------------------------------------------------
   AX_CompPortLCIter AX_ArchGraphC::add_new_ag_outport(string name)
   {
      AX_CompPortLCIter cIter;

      for (cIter = m_ag_outportL.begin();
           cIter != m_ag_outportL.end(); cIter++)
          if (name== (*cIter).get_name())
             return cIter;

      AX_CompPortC nPort;
      AX_CompPortLCIter retI;
      m_ag_outportL.push_back(nPort);
      retI = --m_ag_outportL.end();
      (*retI).set_name(name);
      (*retI).set_type(PORT_OUT);
      (*retI).set_comp(NULL);
      (*retI).set_ag(this);

      return (retI);
   }
// ------------------------------------------------------------
   AX_CompPortCP AX_ArchGraphC::get_ag_outport(int index)
   {
      AX_CompPortCP compportP;

      if ((index < 0)||((unsigned int)index > m_ag_outportL.size()-1))
         compportP = NULL;
      else
      {
         AX_CompPortLCIter cIter;
         int i;

         cIter = m_ag_outportL.begin();
         for(i = 0; i < index; i++) cIter++;

         compportP = &(*cIter);
      }

      return compportP;
   }
// ------------------------------------------------------------
   AX_CompPortCP AX_ArchGraphC::get_ag_outport(string name)
   {
      AX_CompPortCP compportP = NULL;
      AX_CompPortLCIter cIter;

      for (cIter = m_ag_outportL.begin();
           cIter != m_ag_outportL.end(); cIter++)
          if (name == (*cIter).get_name())
          {
             compportP = &(*cIter);
             return compportP;
          }

      return compportP;
   }
// ------------------------------------------------------------
   void AX_ArchGraphC::remove_ag_outport(AX_CompPortCP compportP)
   {
       AX_CompPortLCIter cIter;

       for (cIter = m_ag_outportL.begin();
            cIter != m_ag_outportL.end(); cIter++)
           if (&(*cIter) == compportP)
           {
               AX_CompPortC pTmp;
               (*cIter) = pTmp;
               m_ag_outportL.erase(cIter);
               return;
           }
       return;
   }
// ------------------------------------------------------------
   AX_CompPortLCIter AX_ArchGraphC::delete_ag_outport(AX_CompPortLCIter & iter)
   {
      AX_CompPortLCIter cIter;

      if (m_ag_outportL.end() == iter)
         return iter;
      else if ((--m_ag_outportL.end()) == iter)
      {
         AX_CompPortC pTmp;
         (*iter) = pTmp;
         m_ag_outportL.erase(iter);
         cIter = m_ag_outportL.end();
      }
      else
      {
         cIter = ++iter;
         iter--;
         AX_CompPortC pTmp;
         (*iter) = pTmp;
         m_ag_outportL.erase(iter);
      }

      return cIter;
   }
// ------------------------------------------------------------
   void AX_ArchGraphC::remove_all_ag_outports()
   {
      AX_CompPortLCIter cIter;

      AX_CompPortC pTmp;
      while ((cIter = m_ag_outportL.begin()) != m_ag_outportL.end())
      {
         (*cIter) = pTmp;
         m_ag_outportL.erase(cIter);
      }

      m_ag_outportL.clear();
   }
// ------------------------------------------------------------
   void AX_ArchGraphC::reset_ag_outport_temp()
   {
      AX_CompPortLCIter cIter;

      for (cIter = m_ag_outportL.begin();
           cIter != m_ag_outportL.end(); cIter++)
         (*cIter).set_temp(-1);
   }
// ------------------------------------------------------------
   AX_CompPortLCIter AX_ArchGraphC::create_new_compport()
   {
      AX_CompPortC compport;

      m_compportL.push_back(compport);

      return(--m_compportL.end());
   }
// ------------------------------------------------------------
   AX_CompPortLCIter AX_ArchGraphC::add_new_compport(AX_CompPortC & compport)
   {
      AX_CompPortLCIter cIter;

      for (cIter = m_compportL.begin();
           cIter != m_compportL.end(); cIter++)
          if (compport == (*cIter))
             return cIter;

      m_compportL.push_back(compport);

      return (--m_compportL.end());
   }
// ------------------------------------------------------------
   AX_CompPortLCIter AX_ArchGraphC::add_compport(AX_CompPortC & compport)
   {
      m_compportL.push_back(compport);

      return (--m_compportL.end());
   }
// ------------------------------------------------------------
   void AX_ArchGraphC::add_compports_by_fncblk(AX_FuncBlkCP oFuncP,
                                                AX_FuncBlkCP nFuncP,
                                                int & portId)
   {
      AX_CompPortPLCIter cpIter;
      AX_CompPortLCIter newIter;

#if DEBUG >= 4
      cerr << "Printing intrfcL: ";
      AX_InterfaceLCIter intrI;
      for (intrI = m_intrfcL.begin();
           intrI != m_intrfcL.end(); intrI++)
         cerr << (*intrI).get_name() << " ";
      cerr << endl;
#endif

      // get the comp inports
      for (cpIter = oFuncP->get2_first_inport();
           cpIter != oFuncP->inport_list_end(); cpIter++)
      {
         AX_CompPortC nPort;
         newIter = add_new_compport(nPort);
         (*newIter) = *(*cpIter);
         (*newIter).remove_all_buses();
         (*newIter).remove_all_intrfcs();
         (*newIter).set_comp((AX_CompCP)nFuncP);
         (*newIter).set_id(portId++);
         nFuncP->add_new_inport(&(*newIter));
         (*newIter).copy_intrfcs(&m_intrfcL, (*cpIter));
      }

      // get the comp outports
      for (cpIter = oFuncP->get2_first_outport();
           cpIter != oFuncP->outport_list_end(); cpIter++)
      {
         AX_CompPortC nPort;
         newIter = add_new_compport(nPort);
         (*newIter) = *(*cpIter);
         (*newIter).remove_all_buses();
         (*newIter).remove_all_intrfcs();
         (*newIter).set_comp((AX_CompCP)nFuncP);
         (*newIter).set_id(portId++);
         nFuncP->add_new_outport(&(*newIter));
         (*newIter).copy_intrfcs(&m_intrfcL, (*cpIter));
      }

      // get the comp ioports
      for (cpIter = oFuncP->get2_first_ioport();
           cpIter != oFuncP->ioport_list_end(); cpIter++)
      {
         AX_CompPortC nPort;
         newIter = add_new_compport(nPort);
         (*newIter) = *(*cpIter);
         (*newIter).remove_all_buses();
         (*newIter).remove_all_intrfcs();
         (*newIter).set_comp((AX_CompCP)nFuncP);
         (*newIter).set_id(portId++);
         nFuncP->add_new_ioport(&(*newIter));
         (*newIter).copy_intrfcs(&m_intrfcL, (*cpIter));
      }
   }
// ------------------------------------------------------------
// this function is only called for adding compports for
// collector and distributor
// ------------------------------------------------------------
   void AX_ArchGraphC::add_compports_by_task(AX_TaskCP taskP,
                                              AX_FuncBlkCP oFuncP,
                                              AX_FuncBlkCP nFuncP,
                                              int & portId)
   {
      AX_TaskPortPLCIter tpIter;
      AX_CompPortLCIter newIter;

      // get the task inports and put them into the comp
      for (tpIter = taskP->get2_first_inport();
           tpIter != taskP->inport_list_end(); tpIter++)
      {
         AX_CompPortC nPort;
         newIter = add_new_compport(nPort);
         string nTmpS = (*tpIter)->get_name();
         (*newIter).set_name(nTmpS);
         if (taskP->get_type() == COLLECTOR)
            (*newIter).set_type(PORT_BI);
         else if (taskP->get_type() == DISTRIBUTOR)
            (*newIter).set_type(PORT_IN);
         else
         {
#if DEBUG >= 5
            cerr << "Only task type COLLECTOR or DISTRIBUTOR then "
                 << "this will be called." << endl;
#endif
            (*newIter).set_type(PORT_TYPE_UNKNOWN);
         }
         (*newIter).remove_all_buses();
         (*newIter).remove_all_sample_rates();
         (*newIter).add_new_sample_rate((*tpIter)->get_no_samples());
         (*newIter).set_sample_size((*tpIter)->get_sample_size());
         (*newIter).set_comp((AX_CompCP)nFuncP);
         (*newIter).set_id(portId++);
         (*newIter).remove_all_intrfcs();
         AX_InterfaceLCIter intrfcI;
         for (intrfcI = m_intrfcL.begin();
              intrfcI != m_intrfcL.end(); intrfcI++)
            (*newIter).add_new_intrfc(&(*intrfcI));
         nFuncP->add_new_inport(&(*newIter));
      }

      // get the task outports and put them into the comp
      for (tpIter = taskP->get2_first_outport();
           tpIter != taskP->outport_list_end(); tpIter++)
      {
         AX_CompPortC nPort;
         newIter = add_new_compport(nPort);
         string nTmpS = (*tpIter)->get_name();
         (*newIter).set_name(nTmpS);
         if (taskP->get_type() == COLLECTOR)
            (*newIter).set_type(PORT_OUT);
         else if (taskP->get_type() == DISTRIBUTOR)
            (*newIter).set_type(PORT_BI);
         else
         {
#if DEBUG >= 5
            cerr << "Only task type COLLECTOR or DISTRIBUTOR then "
                 << "this will be called." << endl;
#endif
            (*newIter).set_type(PORT_TYPE_UNKNOWN);
         }
         (*newIter).remove_all_buses();
         (*newIter).remove_all_sample_rates();
         (*newIter).add_new_sample_rate((*tpIter)->get_no_samples());
         (*newIter).set_sample_size((*tpIter)->get_sample_size());
         (*newIter).set_comp((AX_CompCP)nFuncP);
         (*newIter).set_id(portId++);
         (*newIter).remove_all_intrfcs();
         AX_InterfaceLCIter intrfcI;
         for (intrfcI = m_intrfcL.begin();
              intrfcI != m_intrfcL.end(); intrfcI++)
            (*newIter).add_new_intrfc(&(*intrfcI));
         nFuncP->add_new_outport(&(*newIter));
      }
   }
// ------------------------------------------------------------
   bool AX_ArchGraphC::change_compports_owner(AX_FuncBlkCP oldFuncP,
                                                  AX_FuncBlkCP newFuncP,
                                                  AX_FuncBlkCP platFuncP,
                                                  AX_TaskCP taskP)
   {
      AX_CompPortLCIter cpIter;

      for (cpIter = get2_first_compport();
           cpIter != compport_list_end(); cpIter++)
      {
         if ((*cpIter).get_comp() == (AX_CompCP)oldFuncP)
         {
            (*cpIter).set_comp((AX_CompCP)newFuncP);
            (*cpIter).remove_all_intrfcs();
            PortTypeE type = (*cpIter).get_type();
            AX_CompPortCP portP;
            portP = &(*cpIter);
            if (type == PORT_IN)
               newFuncP->add_new_inport(portP);
            else if (type == PORT_OUT)
               newFuncP->add_new_outport(portP);
            else if (type == PORT_BI)
               newFuncP->add_new_ioport(portP);
            else
            {
               cerr << (*cpIter).get_comp()->get_name()
                    << "." << (*cpIter).get_name()
                    << ": port type not defined" << endl;
               return false;
            }
         } // end if comp
      } // end for compport loop
      if (newFuncP->get_type() == HARDWARE)
         newFuncP->copy_intrfcs(&m_intrfcL, platFuncP);

      return true;
   }
// ------------------------------------------------------------
   void AX_ArchGraphC::collect_oth_ports(AX_TaskPortCP portP,
                                          AX_ConnPntLC & connPntL,
                                          AX_ConnPLC & connL,
                                          bool input)
   {
      AX_ConnPLCIter tConnPI, newConnPI;
      AX_TaskGraphCP endTgP;
      AX_TaskCP endTaskP;
      AX_TaskPortCP endPortP;

      for (tConnPI = portP->get2_first_conn();
           tConnPI != portP->conn_list_end(); tConnPI++)
      {
         connL.push_back(*tConnPI);
         if (input)
         {
            if ((*tConnPI)->get_src_port()->get_no_conns() > 1)
               continue;
            endTgP = (*tConnPI)->get_src_tg();
            endTaskP = (*tConnPI)->get_src_task();
            endPortP = (*tConnPI)->get_src_port();
         }
         else
         {
            endTgP = (*tConnPI)->get_sink_tg();
            endTaskP = (*tConnPI)->get_sink_task();
            endPortP = (*tConnPI)->get_sink_port();
         }
         AX_ConnPntC newPntC;
         AX_ConnPntLCIter newPntI;
         if (endTgP != NULL)
         {
            AX_CompPortCP fPortP;
            connPntL.push_back(newPntC);
            newPntI = --connPntL.end();

            (*newPntI).set_ag(this);
            (*newPntI).set_comp(NULL);
            if (input)
               fPortP = get_ag_inport(endPortP->get_name());
            else
               fPortP = get_ag_outport(endPortP->get_name());
            (*newPntI).set_port(fPortP);
         }
         else
         {
            AX_FuncBlkCP othFuncP;
            othFuncP = find_fncblk_by_task(endTaskP);
            // we do not look for software ports. For we don't
            // want to include processor ports for deleting.
            // But even for hardware port we need to careful
            // about what to include. NEED WORK HERE!!!!
            if ((othFuncP->get_type() == HARDWARE &&
                endTaskP->get_no_succs() <= 1 && input) ||
                (othFuncP->get_type() == HARDWARE && !input))
            {
               // the other end is a hardware but not a distributor
               connPntL.push_back(newPntC);
               newPntI = --connPntL.end();

               (*newPntI).set_ag(NULL);
               (*newPntI).set_comp(AX_CompCP(othFuncP));
               (*newPntI).set_port(get_compport(othFuncP, endPortP->get_name()));
            }
         }
      }
      return;
   }
// ------------------------------------------------------------
   AX_CompPortCP AX_ArchGraphC::get_compport(int index)
   {
      if ((index < 0)||((unsigned int)index > m_compportL.size()-1))
         return NULL;
      else
      {
         AX_CompPortLCIter cIter;
         int i;

         cIter = m_compportL.begin();
         for(i = 0; i < index; i++) cIter++;

         return &(*cIter);
      }

      return NULL;
   }
// ------------------------------------------------------------
   AX_CompPortCP AX_ArchGraphC::get_compport(AX_FuncBlkCP funcP, string name)
   {
      AX_CompPortCP compportP = NULL;
      AX_CompPortLCIter cIter;

      for (cIter = m_compportL.begin();
           cIter != m_compportL.end(); cIter++)
      {
          AX_FuncBlkCP fncblkP = (AX_FuncBlkCP)(*cIter).get_comp();
          if (fncblkP == funcP && name == (*cIter).get_name())
             return &(*cIter);
      }

      return compportP;
   }
// ------------------------------------------------------------
   AX_CompPortCP AX_ArchGraphC::get_compport(AX_TaskCP taskP, string name)
   {
      AX_CompPortLCIter cIter;

      for (cIter = m_compportL.begin();
           cIter != m_compportL.end(); cIter++)
      {
          if ((*cIter).get_comp() == NULL)
             continue;
          AX_FuncBlkCP fncblkP = (AX_FuncBlkCP)(*cIter).get_comp();
          if (fncblkP->get_no_taskrefs() == 0)
             continue;
          AX_TaskRefLSIter taskrefI;
          for (taskrefI = fncblkP->get2_first_taskref();
               taskrefI != fncblkP->taskref_list_end(); taskrefI++)
          {
             if ((*taskrefI).m_taskP == taskP &&
                 (*cIter).get_name() == name)
                return &(*cIter);
          }
      }

      return NULL;
   }
// ------------------------------------------------------------
   AX_CompPortCP AX_ArchGraphC::get_compport_by_id(int id)
   {
      AX_CompPortLCIter cIter;

      for (cIter = m_compportL.begin();
           cIter != m_compportL.end(); cIter++)
      {
          if ((*cIter).get_id() == id)
             return &(*cIter);
      }

      return NULL;
   }
// ------------------------------------------------------------
   void AX_ArchGraphC::remove_compport(AX_CompPortCP compportP)
   {
       AX_CompPortLCIter cIter;

       for (cIter = m_compportL.begin();
            cIter != m_compportL.end(); cIter++)
           if (&(*cIter) == compportP)
           {
               AX_CompPortC pTmp;
               (*cIter) = pTmp;
               m_compportL.erase(cIter);
               return;
           }
       return;
   }
// ------------------------------------------------------------
   void AX_ArchGraphC::delete_compports_by_fncblk(AX_FuncBlkCP funcP)
   {
      if (funcP == NULL)
         return;

      AX_CompPortLCIter portI;
      bool StepBack = false;
      for (portI = get2_first_compport();
           portI != compport_list_end(); portI++)
      {
         if (StepBack)
         {
            portI = m_compportL.begin();
            StepBack = false;
         }
         if ((*portI).get_comp() == funcP)
         {
            portI = delete_compport(portI);
            if (portI != m_compportL.begin())
               portI--;
            else
               StepBack = true;
         }
      }
   }
// ------------------------------------------------------------
   void AX_ArchGraphC::delete_compports_by_fncblk(AX_FuncBlkCP funcP,
                                                   AX_FuncBlkCP orgFuncP,
                                                   AX_TaskCP taskP)
   {
      AX_CompPortLCIter cIter;
      bool StepBack = false;
      for (cIter = get2_first_compport();
           cIter != compport_list_end(); cIter++)
      {
         if (StepBack)
         {
            cIter = m_compportL.begin();
            StepBack = false;
         }
         if ((AX_FuncBlkCP)(*cIter).get_comp() == funcP)
         {
            AX_BusPLCIter busPI;
            bool stepback = false;
            for (busPI = (*cIter).get2_first_bus();
                 busPI != (*cIter).bus_list_end(); busPI++)
            {
               if (stepback)
               {
                  busPI--;
                  stepback = true;
               }
               (*busPI)->delete_connpnt(NULL, (AX_CompCP)funcP, &(*cIter));
               if ((*busPI)->get_no_connpnts() == 1)
               {
                  AX_BusCP delBusP = (*busPI);
                  AX_ConnPntLCIter conPntI;

                  conPntI = (*busPI)->get2_first_connpnt();
                  (*conPntI).get_port()->delete_bus(*busPI);
                  (*busPI)->remove_all_connpnts();
                  (*busPI)->remove_all_connrefs();
                  busPI = (*cIter).delete_bus(*busPI);
                  if ((*cIter).get_no_buses() == 0)
                     busPI = (*cIter).bus_list_end();
                  else if (busPI != (*cIter).get2_first_bus())
                     busPI--;
                  else
                     stepback = true;

                  delete_bus_fm_ports(delBusP);
                  delete_bus(delBusP);
               }
               else if ((*busPI)->get_no_connpnts() == 0)
               {
                  AX_BusCP delBusP = (*busPI);

                  (*busPI)->remove_all_connrefs();
                  busPI = (*cIter).delete_bus(*busPI);
                  if (busPI != (*cIter).get2_first_bus())
                     busPI--;
                  else
                     stepback = true;

                  delete_bus(delBusP);
               }
            }
            AX_CompPortCP delPortP = &(*cIter);
            AX_CompPortLCIter cleanI;
            for (cleanI = m_compportL.begin();
                 cleanI != m_compportL.end(); cleanI++)
               if ((*cleanI).get_sw_port() == delPortP)
               {
                  delPortP->dec_refCnt();
                  (*cleanI).set_sw_port(NULL);
               }
            funcP->delete_ioport(delPortP);
            cIter = delete_compport(cIter);
            if (cIter != m_compportL.begin())
               cIter--;
            else
               StepBack = true;
         } // end if comp
      } // end for compport loop

      if (taskP == NULL)
         return;

      // remove connpnts of other ends
      AX_BusPLCIter busPI;
      AX_ConnPntLC connPntL;
      AX_ConnPLC connL;
      AX_TaskPortPLCIter tPortPI;

      connPntL.clear();
      for (tPortPI = taskP->get2_first_inport();
           tPortPI != taskP->inport_list_end(); tPortPI++)
      {
         AX_CompPortPLCIter srchPI;
         bool found = false;
         for (srchPI = orgFuncP->get2_first_inport();
              srchPI != orgFuncP->inport_list_end(); srchPI++)
         {
            if ((*tPortPI)->get_name() == (*srchPI)->get_name())
            {
               collect_oth_ports((*tPortPI), connPntL, connL, true);
               found = true;
               break;
            }
         } // end for orgFuncP inport loop
         if (found)
            continue;
         for (srchPI = orgFuncP->get2_first_ioport();
              srchPI != orgFuncP->ioport_list_end(); srchPI++)
         {
            if ((*tPortPI)->get_name() == (*srchPI)->get_name())
            {
               collect_oth_ports((*tPortPI), connPntL, connL, true);
               break;
            }
         } // end for orgFuncP ioport loop
      } // end for task inport loop

      for (tPortPI = taskP->get2_first_outport();
           tPortPI != taskP->outport_list_end(); tPortPI++)
      {
         AX_CompPortPLCIter srchPI;
         bool found = false;
         for (srchPI = orgFuncP->get2_first_outport();
              srchPI != orgFuncP->outport_list_end(); srchPI++)
         {
            if ((*tPortPI)->get_name() == (*srchPI)->get_name())
            {
               collect_oth_ports((*tPortPI), connPntL, connL, false);
               found = true;
               break;
            }
         }
         if (found)
            continue;
         for (srchPI = orgFuncP->get2_first_ioport();
              srchPI != orgFuncP->ioport_list_end(); srchPI++)
         {
            if ((*tPortPI)->get_name() == (*srchPI)->get_name())
            {
               collect_oth_ports((*tPortPI), connPntL, connL, false);
               break;
            }
         } // end for orgFuncP ioport loop
      } // end for task outport loop

      AX_ConnPntLCIter connPntI, cpyI;
      AX_ConnPLCIter iCntIter = connL.begin();
      for (cpyI = connPntL.begin(); cpyI != connPntL.end(); cpyI++)
      {
         AX_BusPLCIter srchBusPI;
         AX_BusCP delBusP = NULL;
         bool found = false;
         bool stepback = false;

         for (srchBusPI = (*cpyI).get_port()->get2_first_bus();
              srchBusPI != (*cpyI).get_port()->bus_list_end(); srchBusPI++)
         {
            if (stepback)
            {
               srchBusPI--;
               stepback = false;
            }
            for (connPntI = (*srchBusPI)->get2_first_connpnt();
                 connPntI != (*srchBusPI)->connpnt_list_end(); connPntI++)
            {
               if ((*cpyI).get_ag() == (*connPntI).get_ag() &&
                   (*cpyI).get_comp() == (*connPntI).get_comp() &&
                   (*cpyI).get_port() == (*connPntI).get_port())
               {
                  delBusP = (*srchBusPI);
                  (*srchBusPI)->delete_connpnt(connPntI);
                  (*srchBusPI)->delete_connref(*iCntIter);
                  srchBusPI = (*cpyI).get_port()->delete_bus(*srchBusPI);
                  if (srchBusPI != (*cpyI).get_port()->get2_first_bus())
                     srchBusPI--;
                  else
                     stepback = true;
                  found = true;
                  iCntIter++;
                  break;
               }
            } // end for connPntL loop
            if (found)
            {
               if (delBusP->get_no_connpnts() == 1)
               {
                  AX_ConnPntLCIter conPntI;
                  conPntI = delBusP->get2_first_connpnt();
                  (*conPntI).get_port()->delete_bus(delBusP);
                  delBusP->remove_all_connpnts();
                  delBusP->remove_all_connrefs();

                  delete_bus_fm_ports(delBusP);
                  delete_bus(delBusP);
               }
               else if (delBusP->get_no_connpnts() == 0)
               {
                  delBusP->remove_all_connrefs();
                  delete_bus(delBusP);
               }
               break;
            }
         } // end busL loop
#if 0
         if (!found)
         {
            cerr << "ArchGraph::Cannot find other port ";
            if ((*cpyI).get_ag() != NULL)
              cerr << (char *)(*cpyI).get_ag()->get_name() << ".";
            else
              cerr << (char *)(*cpyI).get_comp()->get_name() << ".";
            cerr << (char *)(*cpyI).get_port()->get_name() << endl;
            return;
         }
#endif
      } // end for connpnt loop

      return;
   }
// ------------------------------------------------------------
   AX_CompPortLCIter AX_ArchGraphC::delete_compport(AX_CompPortLCIter & iter)
   {
      AX_CompPortLCIter cIter;

      if (m_compportL.end() == iter)
         return iter;
      else if ((--m_compportL.end()) == iter)
      {
         AX_CompPortC pTmp;
         (*iter) = pTmp;
         m_compportL.erase(iter);
         cIter = m_compportL.end();
      }
      else
      {
         cIter = ++iter;
         iter--;
         AX_CompPortC pTmp;
         (*iter) = pTmp;
         m_compportL.erase(iter);
      }

      return cIter;
   }
// ------------------------------------------------------------
   void AX_ArchGraphC::remove_all_compports()
   {
      AX_CompPortLCIter cIter;

      AX_CompPortC pTmp;
      while ((cIter = m_compportL.begin()) !=  m_compportL.end())
      {
         (*cIter) = pTmp;
         m_compportL.erase(cIter);
      }

      m_compportL.clear();
   }
// ------------------------------------------------------------
   void AX_ArchGraphC::reset_compport_temp()
   {
      AX_CompPortLCIter cIter;

      for (cIter = m_compportL.begin();
           cIter != m_compportL.end(); cIter++)
         (*cIter).set_temp(-1);
   }
// ------------------------------------------------------------
   void AX_ArchGraphC::adjust_refCnt(AX_CompPortCP portP)
   {
      AX_CompPortPLCIter portPI;

      int refCnt = 0;
      AX_CompPortLCIter swPortI;
      for (swPortI = get2_first_compport();
           swPortI != compport_list_end(); swPortI++)
      {
         if ((*swPortI).get_sw_port() == portP)
            refCnt++;
      }
      if (refCnt != portP->get_refCnt())
         portP->set_refCnt(refCnt);
   }
// ------------------------------------------------------------
   AX_BusLCIter AX_ArchGraphC::create_new_bus()
   {
      AX_BusC bus;

      m_busL.push_back(bus);

      return(--m_busL.end());
   }
// ------------------------------------------------------------
   AX_BusLCIter AX_ArchGraphC::add_new_bus(AX_BusC & bus)
   {
      AX_BusLCIter bIter;

      for (bIter = m_busL.begin();
           bIter != m_busL.end(); bIter++)
          if (bus == (*bIter))
             return bIter;

      m_busL.push_back(bus);

      return (--m_busL.end());
   }
// ------------------------------------------------------------
   AX_BusCP AX_ArchGraphC::get_bus(int index)
   {
      AX_BusCP busP;

      if ((index < 0)||((unsigned int)index > m_busL.size()-1))
         busP = NULL;
      else
      {
         AX_BusLCIter bIter;
         int i;

         bIter = m_busL.begin();
         for(i = 0; i < index; i++) bIter++;

         busP = &(*bIter);
      }

      return busP;
   }
// ------------------------------------------------------------
   AX_BusCP AX_ArchGraphC::get_bus_by_id(int id)
   {
      AX_BusLCIter bIter;

      if (id < 0)
         return NULL;
      for (bIter = m_busL.begin();
           bIter != m_busL.end(); bIter++)
          if ((*bIter).get_id() == id)
             return &(*bIter);

      return NULL;
   }
// ------------------------------------------------------------
   AX_BusCP AX_ArchGraphC::get_bus_by_intrfc(int index,
                                               AX_InterfaceCP intrP)
   {
      AX_BusCP busP;

      if ((index < 0)||((unsigned int)index > m_busL.size()-1))
         busP = NULL;
      else
      {
         AX_BusLCIter bIter;
         int i, j = 0, max = m_busL.size();

         bIter = m_busL.begin();
         for (i = 0; i < max; i++)
         {
            if ((*bIter).get_intrfc() == intrP &&
                j == index)
               return &(*bIter);

            if ((*bIter).get_intrfc() == intrP)
               j++;

            bIter++;
         }

         busP = &(*bIter);
      }

      return busP;
   }
// ------------------------------------------------------------
   int AX_ArchGraphC::get_no_buses_by_intrfc(AX_InterfaceCP intrP)
   {
      AX_BusLCIter busI;
      int Cnt = 0;

      for (busI = m_busL.begin(); busI != m_busL.end(); busI++)
         if ((*busI).get_intrfc() == intrP)
            Cnt++;

      return Cnt;
   }
// ------------------------------------------------------------
   AX_BusCP AX_ArchGraphC::get_bus_by_conn(AX_ConnCP connP)
   {
      AX_BusLCIter bIter;

      if (connP == NULL)
         return NULL;

      for (bIter = m_busL.begin();
           bIter != m_busL.end(); bIter++)
          if ((*bIter).is_mapped_conn(connP))
             return &(*bIter);

      return NULL;
   }
// ------------------------------------------------------------
// this random function first roll the die to determine, in 25%
// chance, if a new bus to be created. If failed then randomly
// select one existing bus for the connection.
// ------------------------------------------------------------
   AX_BusCP AX_ArchGraphC::ran_pick_bus(AX_InterfaceCP intrP, int & Cnt)
   {
      int bNum;
      AX_BusLCIter busI;
      AX_BusCP busP;

      bNum = get_no_buses_by_intrfc(intrP);
    
      if (bNum <= 0)
      {
         // no such bus, create a new one
         AX_BusC nBus;
         m_busL.push_back(nBus);
         busI = --m_busL.end();
         (*busI).set_intrfc(intrP);
         (*busI).set_id(Cnt++);
         busP = &(*busI);
      }
      else
      {
         int sNum = my_rand(100);

         if (sNum >= 0 && sNum < 25)
         {
            // create a new bus by chance
            AX_BusC nBus;
            m_busL.push_back(nBus);
            busI = --m_busL.end();
            (*busI).set_intrfc(intrP);
            (*busI).set_id(Cnt++);
            busP = &(*busI);
         } // end creating new bus in 25% chance
         else
         {
            int rNum = my_rand(bNum);
            busP = get_bus_by_intrfc(rNum, intrP);
         } // end using old bus
      }

      return busP;
   }
// ------------------------------------------------------------
   void AX_ArchGraphC::remove_bus(AX_BusCP busP)
   {
       AX_BusLCIter bIter;

       for (bIter = m_busL.begin();
            bIter != m_busL.end(); bIter++)
           if (&(*bIter) == busP)
           {
               m_busL.erase(bIter);
               return;
           }
       return;
   }
// ------------------------------------------------------------
   AX_BusLCIter AX_ArchGraphC::delete_bus(AX_BusLCIter & iter)
   {
      AX_BusLCIter bIter;

      if (m_busL.end() == iter)
         return iter;
      else if ((--m_busL.end()) == iter)
      {
         AX_BusC bTmp;
         (*iter) = bTmp;
         m_busL.erase(iter);
         bIter = m_busL.end();
      }
      else
      {
         bIter = ++iter;
         iter--;
         AX_BusC bTmp;
         (*iter) = bTmp;
         m_busL.erase(iter);
      }

      return bIter;
   }
// ------------------------------------------------------------
   AX_BusLCIter AX_ArchGraphC::delete_bus(AX_BusCP busP)
   {
      AX_BusLCIter iter, bIter = m_busL.end();

      if (busP == NULL)
         return m_busL.end();
      else if (&(*(--m_busL.end())) == busP)
      {
         iter = --m_busL.end();
         AX_BusC bTmp;
         (*iter) = bTmp;
         m_busL.erase(iter);
         return m_busL.end();
      }
      else
      {
         for (iter = m_busL.begin(); iter != m_busL.end(); iter++)
            if (&(*iter) == busP)
            {
               bIter = ++iter;
               iter--;
               AX_BusC bTmp;
               (*iter) = bTmp;
               m_busL.erase(iter);
               return bIter;
            }
      }

      return bIter;
   }
// ------------------------------------------------------------
   void AX_ArchGraphC::remove_all_buses()
   {
      AX_BusLCIter bIter;

      AX_BusC bTmp;
      while ((bIter = m_busL.begin()) != m_busL.end())
      {
         (*bIter) = bTmp;
         m_busL.erase(bIter);
      }

      m_busL.clear();
   }
// ------------------------------------------------------------
   void AX_ArchGraphC::clean_all_bus_connrefs()
   {
      AX_BusLCIter bIter;

      for (bIter = m_busL.begin(); bIter != m_busL.end(); bIter++)
         (*bIter).clean_connrefs();
   }

// ------------------------------------------------------------
   AX_SchdTimeLSIter AX_ArchGraphC::create_new_schdtime()
   {
      AX_SchdTimeS schdtime;

      schdtime.m_taskP = NULL;
      schdtime.m_fncblkP = NULL;
      schdtime.m_connP = NULL;
      schdtime.m_busP = NULL;
      schdtime.m_start = -1;
      schdtime.m_finish = -1;
      schdtime.m_temp = -1;

      m_schdtimeL.push_back(schdtime);

      return(--m_schdtimeL.end());
   }
// ------------------------------------------------------------
   AX_SchdTimeLSIter AX_ArchGraphC::add_new_schdtime(AX_SchdTimeS schdtime)
   {
      AX_SchdTimeLSIter tIter;

      for (tIter = m_schdtimeL.begin();
           tIter != m_schdtimeL.end(); tIter++)
          if (&schdtime == &(*tIter))
             return tIter;

      m_schdtimeL.push_back(schdtime);

      return (--m_schdtimeL.end());
   }
// ------------------------------------------------------------
   AX_SchdTimeSP AX_ArchGraphC::get_schdtime(int index)
   {
      AX_SchdTimeSP schdtimeP = NULL;

      if ((index < 0)||((unsigned int)index > m_schdtimeL.size()-1))
         schdtimeP = NULL;
      else
      {
         AX_SchdTimeLSIter tIter;
         int i;

         tIter = m_schdtimeL.begin();
         for(i = 0; i < index; i++) tIter++;

         schdtimeP = &(*tIter);
      }

      return schdtimeP;
   }
// ------------------------------------------------------------
   void AX_ArchGraphC::remove_schdtime(AX_SchdTimeSP schdtimeP)
   {
       AX_SchdTimeLSIter tIter;

       for (tIter = m_schdtimeL.begin();
            tIter != m_schdtimeL.end(); tIter++)
           if (&(*tIter) == schdtimeP)
           {
               (*tIter).m_taskP = NULL;
               (*tIter).m_fncblkP = NULL;
               (*tIter).m_connP = NULL;
               (*tIter).m_busP = NULL;
               (*tIter).m_start = 0;
               (*tIter).m_finish = 0;
               (*tIter).m_temp = 0;
               m_schdtimeL.erase(tIter);
               return;
           }
       return;
   }
// ------------------------------------------------------------
   AX_SchdTimeLSIter AX_ArchGraphC::delete_schdtime(AX_SchdTimeLSIter & iter)
   {
      AX_SchdTimeLSIter tIter;

      if (m_schdtimeL.end() == iter)
         return iter;
      else if ((--m_schdtimeL.end()) == iter)
      {
         m_schdtimeL.erase(iter);
         tIter = m_schdtimeL.end();
      }
      else
      {
         tIter = ++iter;
         iter--;
         (*iter).m_taskP = NULL;
         (*iter).m_fncblkP = NULL;
         (*iter).m_connP = NULL;
         (*iter).m_busP = NULL;
         (*iter).m_start = 0;
         (*iter).m_finish = 0;
         (*iter).m_temp = 0;
         m_schdtimeL.erase(iter);
      }

      return tIter;
   }
// ------------------------------------------------------------
   void AX_ArchGraphC::remove_all_schdtimes()
   {
      AX_SchdTimeLSIter tIter;

      while ((tIter = m_schdtimeL.begin()) != m_schdtimeL.end())
      {
         (*tIter).m_taskP = NULL;
         (*tIter).m_fncblkP = NULL;
         (*tIter).m_connP = NULL;
         (*tIter).m_busP = NULL;
         (*tIter).m_start = 0;
         (*tIter).m_finish = 0;
         (*tIter).m_temp = 0;
         m_schdtimeL.erase(tIter);
      }

      m_schdtimeL.clear();
   }

// ------------------------------------------------------------
   void AX_ArchGraphC::schdtimes_dump()
   {
      AX_SchdTimeLSIter tIter;

      cerr << "  SchdTimeL:" << endl;
      for (tIter = m_schdtimeL.begin();
           tIter != m_schdtimeL.end(); tIter++)
      {
         cerr << "    Task: " << (*tIter).m_taskP->get_name();
         cerr << ", FuncBlk: " << (*tIter).m_fncblkP->get_name() << endl;
         if ((*tIter).m_connP != NULL)
            (*tIter).m_connP->dump_simple();
         if ((*tIter).m_busP != NULL)
            (*tIter).m_busP->dump_simple();
         cerr << "          start: " << (*tIter).m_start;
         cerr << " finish: " << (*tIter).m_finish << endl;
      }
      cerr << endl;
   }

// ------------------------------------------------------------
   AX_SchdTimeLSIter AX_ArchGraphC::get_schdtime_by_task(AX_TaskCP taskP)
   {
      AX_SchdTimeLSIter tIter;

      for (tIter = m_schdtimeL.begin();
           tIter != m_schdtimeL.end(); tIter++)
      {
         if ((*tIter).m_taskP == taskP)
            return tIter;
      }

      return m_schdtimeL.end();
   }
// ------------------------------------------------------------
   AX_SchdTimeLSIter AX_ArchGraphC::get_schdtime_by_fncblk(AX_FuncBlkCP funcP)
   {
      AX_SchdTimeLSIter tIter;

      for (tIter = m_schdtimeL.begin();
           tIter != m_schdtimeL.end(); tIter++)
      {
         if ((*tIter).m_fncblkP == funcP)
            return tIter;
      }

      return m_schdtimeL.end();
   }
// ------------------------------------------------------------
   AX_SchdTimeLSIter AX_ArchGraphC::get_schdtime_by_conn(AX_ConnCP connP)
   {
      AX_SchdTimeLSIter tIter;

      for (tIter = m_schdtimeL.begin();
           tIter != m_schdtimeL.end(); tIter++)
      {
         if ((*tIter).m_connP == connP)
            return tIter;
      }

      return m_schdtimeL.end();
   }
// ------------------------------------------------------------
   AX_SchdTimeLSIter AX_ArchGraphC::get_schdtime_by_bus(AX_BusCP busP)
   {
      AX_SchdTimeLSIter tIter;

      for (tIter = m_schdtimeL.begin();
           tIter != m_schdtimeL.end(); tIter++)
      {
         if ((*tIter).m_busP == busP)
            return tIter;
      }

      return m_schdtimeL.end();
   }
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Sorting Functions

// ------------------------------------------------------------
   bool compare_fncblk_id(AX_FuncBlkC const & f1,
                          AX_FuncBlkC const & f2)
   {
      int arg1 = f1.m_id;
      int arg2 = f2.m_id;

      if (arg1 == arg2)
         return false;
      else if (arg1 < arg2)
         return false;
      else
         return true;
   }
// ------------------------------------------------------------
   void AX_ArchGraphC::sort_fncblk_by_id()
   {
      if (m_fncblkL.size() <= 0)
         return;

      m_fncblkL.sort(compare_fncblk_id);
   }
// ------------------------------------------------------------
   bool compare_compport_id(AX_CompPortC const & c1,
                            AX_CompPortC const & c2)
   {
      int arg1 = c1.m_id;
      int arg2 = c2.m_id;

      if (arg1 == arg2)
         return false;
      else if (arg1 < arg2)
         return false;
      else
         return true;
   }
// ------------------------------------------------------------
   void AX_ArchGraphC::sort_compport_by_id()
   {
      if (m_compportL.size() <= 0)
         return;

      m_compportL.sort(compare_compport_id);
   }
// ------------------------------------------------------------
   bool compare_bus_id(AX_BusC const & b1,
                       AX_BusC const & b2)
   {
      int arg1 = b1.m_id;
      int arg2 = b2.m_id;

      if (arg1 == arg2)
         return false;
      else if (arg1 < arg2)
         return false;
      else
         return true;
   }
// ------------------------------------------------------------
   void AX_ArchGraphC::sort_bus_by_id()
   {
      if (m_busL.size() <= 0)
         return;

      m_busL.sort(compare_bus_id);
   }
// ------------------------------------------------------------
   void AX_ArchGraphC::renum_fncblk_id()
   {
      AX_FuncBlkLCIter funcI;
      int fCnt = 0;
      char buf1[20];

      for (funcI = m_fncblkL.begin();
           funcI != m_fncblkL.end(); funcI++)
      {
         string extName = trim_id((*funcI).get_name());
         my_itoa(fCnt, buf1);
         extName = extName + "_";
         extName = extName + buf1;
         (*funcI).set_name(extName);
         (*funcI).set_id(fCnt++);
      }
   }
// ------------------------------------------------------------
   void AX_ArchGraphC::renum_compport_id()
   {
      AX_CompPortLCIter portI;
      int pCnt = 0;

      for (portI = m_compportL.begin();
           portI != m_compportL.end(); portI++)
         (*portI).set_id(pCnt++);
   }
// ------------------------------------------------------------
   void AX_ArchGraphC::renum_bus_id()
   {
      AX_BusLCIter busI;
      int bCnt = 0;

      for (busI = m_busL.begin();
           busI != m_busL.end(); busI++)
         (*busI).set_id(bCnt++);
   }

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Special Functions

// ------------------------------------------------------------
   void AX_ArchGraphC::delete_bus_fm_ports(AX_BusCP busP)
   {
      AX_CompPortLCIter portI;
      for (portI = m_ag_inportL.begin();
           portI != m_ag_inportL.end(); portI++)
         (*portI).scan_delete_bus(busP);

      for (portI = m_ag_outportL.begin();
           portI != m_ag_outportL.end(); portI++)
         (*portI).scan_delete_bus(busP);

      for (portI = m_compportL.begin();
           portI != m_compportL.end(); portI++)
         (*portI).scan_delete_bus(busP);
   }
// ------------------------------------------------------------
   int AX_ArchGraphC::get_sharable_lists(AX_TaskGraphC & tg,
                                          AX_ShareLC & shareL)
   {
      AX_TaskLCIter taskI;
      for (taskI = tg.get2_first_task();
           taskI != tg.task_list_end(); taskI++)
      {
         AX_FuncBlkCP funcP;
         funcP = get_fncblk_by_task(&(*taskI));

         // only get software component for sharing
         if (funcP->get_type() == HARDWARE)
            continue;

         AX_FuncBlkPLCIter depFuncPI, masFuncPI;
         bool depFound = false, funcFound = false;
         string orgProcName;
         // first get the real processor used in the AG
         for (depFuncPI = funcP->get2_first_dep();
              depFuncPI != funcP->dep_list_end(); depFuncPI++)
         {
            if ((*depFuncPI)->get_functype() == PROCESSOR)
            {
               // Use function (family) name instead of funcblk name
               // to look for sharable processors
               //orgProcName = trim_id((*depFuncPI)->get_name());
               orgProcName = (*depFuncPI)->get_function();
               depFound = true;
               break;
            }
         }
         // then we find the processor referenced in the dependency list
         for (masFuncPI = funcP->get2_first_fncblk();
              masFuncPI != funcP->fncblk_list_end(); masFuncPI++)
            if ((*masFuncPI)->get_functype() == PROCESSOR)
            {
               funcFound = true;
               break;
            }

         if (!depFound)
         {
#if DEBUG >= 1
            cerr << m_name << ": Cannot find mapping processor for "
                 << funcP->get_name() << endl;
#endif
            shareL.clear();
            return 0;
         }
         if (!funcFound)
         {
#if DEBUG >= 1
            cerr << m_name
                 << ": Cannot find processor in dependency list of "
                 << funcP->get_name() << endl;
#endif
            shareL.clear();
            return 0;
         }
         if ((*masFuncPI)->get_function() != orgProcName)
         {
#if DEBUG >= 1
            cerr << m_name
                 << " - " << funcP->get_name()
                 << ": Dependent processor "
                 << (*depFuncPI)->get_name()
                 << " does not match with mapping processor "
                 << (*masFuncPI)->get_name() << endl;
#endif
            shareL.clear();
            return 0;
         }
         bool shareFound = false;
         AX_ShareLCIter shareI;
         for (shareI = shareL.begin();
              shareI != shareL.end(); shareI++)
         {
            if ((*shareI).get_fncblk() == (*masFuncPI))
            {
               int orgCnt = (*shareI).get_no_fncblks();
               (*shareI).add_new_fncblk(*depFuncPI);
               if (orgCnt < (*shareI).get_no_fncblks())
               {
                  (*shareI).add_task(&(*taskI));
                  (*shareI).add_sw_fncblk(funcP);
               }
               shareFound = true;
            }
         }
         if (shareFound)
            continue;

         // no AX_ShareC found in the list, add one
         AX_ShareC nShare;
         AX_ShareLCIter nShareI;
         shareL.push_back(nShare);
         nShareI = --shareL.end();
         (*nShareI).set_fncblk(*masFuncPI);
         (*nShareI).add_task(&(*taskI));
         (*nShareI).add_fncblk(*depFuncPI);
         (*nShareI).add_sw_fncblk(funcP);
      }

      return shareL.size();
   }
// ------------------------------------------------------------
   int AX_ArchGraphC::get_arch_latency(AX_TaskGraphC & tg)
   {
      int latency = -99999;
      AX_TaskPortLCIter tportI;

      for (tportI = tg.get2_first_tg_outport();
           tportI != tg.tg_outport_list_end(); tportI++)
      {
         AX_ConnPLCIter connPI;
         for (connPI = (*tportI).get2_first_conn();
              connPI != (*tportI).conn_list_end(); connPI++)
         {
            AX_TaskCP taskP = (*connPI)->get_src_task();
            if (taskP == NULL)
            {
               cerr << "Task graph outport "
                    << (*tportI).get_name()
                    << " does not connect to a task\n";
               continue;
            }
            AX_FuncBlkCP funcP;
            funcP = get_fncblk_by_task(taskP);

            AX_TaskRefLSIter taskrefI;
            for (taskrefI = funcP->get2_first_taskref();
                 taskrefI != funcP->taskref_list_end(); taskrefI++)
            {
               if ((*taskrefI).m_finish > latency)
                  latency = (*taskrefI).m_finish;
            }
         }
      }
      m_latency = latency;

      return latency;
   }
// ------------------------------------------------------------
   void AX_ArchGraphC::compute_utilization(AX_TaskGraphC & tg)
   {
      // computing funcblk utilization
      //   1. if is a generic HW, find it's taskrefL and add all used time
      //   2. if is a processor, scan it's refPL, and scan taskrefL of
      //      each referencing funcblk to sum up the used time
      //   3. if is a memory, the utilization is the capacity utilized,
      //      not the time used.
#if DEBUG >= 5
      if ((*this).get_name() == "Edge_Gen1_Mu0.4.7")
         cerr << "AG: " << (*this).get_name() << " computes utilization" << endl;
#endif
       AX_FuncBlkLCIter funcI;
      for (funcI = m_fncblkL.begin();
           funcI != m_fncblkL.end(); funcI++)
      {
         if ((*funcI).get_type() != HARDWARE)
            continue;
         int usetime = 0;
         if ((*funcI).get_functype() != PROCESSOR &&
             (*funcI).get_functype() != MEMORY &&
             (*funcI).get_functype() != MEMORY_CORE)
         {
            AX_TaskRefLSIter trefI;
            for (trefI = (*funcI).get2_first_taskref();
                 trefI != (*funcI).taskref_list_end(); trefI++)
            {
               int period = (*trefI).m_finish - (*trefI).m_start + 1;
               usetime += period;
            } // end taskref for
         } // end if
         else if ((*funcI).get_functype() == PROCESSOR)
         {
#if DEBUG >= 5
            if ((*this).get_name() == "Edge_Gen1_Mu0.4.7" &&
                (*funcI).get_name() == "LU_DSP16210_9")
            {
                cerr << "    " << (*funcI).get_name() << ": ";
                cerr << endl;
                cerr << "    m_refPL size is " << (*funcI).get_no_refs() << endl;
                (*funcI).dump();
            }
#endif
            AX_FuncBlkPLCIter refI;
            AX_IterBlkPLS iblkPL;
            for (refI = (*funcI).get2_first_ref();
                 refI != (*funcI).ref_list_end(); refI++)
            {
               AX_TaskRefLSIter trefI;
               AX_IterBlkSP iblkP;
               for (trefI = (*refI)->get2_first_taskref();
                    trefI != (*refI)->taskref_list_end(); trefI++)
               {
                  AX_IterBlkLSIter ibI;
                  bool inIterBlk = false;
                  for (ibI = tg.get2_first_iterblk();
                       ibI != tg.iterblk_list_end(); ibI++)
                  {
                     AX_TaskPLCIter taskPI;
                     for (taskPI = (*ibI).m_taskPL.begin();
                          taskPI != (*ibI).m_taskPL.end(); taskPI++)
                        if ((*taskPI) == (*trefI).m_taskP)  // goes bad here
                        {
                           iblkP = &(*ibI);
                           inIterBlk = true;
                           break;
                        }
                     if (inIterBlk)
                        break;
                  }

                  bool repIterBlk = false;
                  if (inIterBlk)
                  {
                     AX_IterBlkPLSIter ibPI;
                     for (ibPI = iblkPL.begin();
                          ibPI != iblkPL.end(); ibPI++)
                     {
                        if ((*ibPI) == iblkP)
                        {
                           repIterBlk = true;
                           break;
                        }
                     }
                     if (repIterBlk)
                        continue;
                     iblkPL.push_back(iblkP);
                  }
                  int period = (*trefI).m_finish - (*trefI).m_start + 1;
                  usetime += period;
               }
            }
#if DEBUG >= 5
            if ((*this).get_name() == "Edge_Gen1_Mu0.4.7" &&
                (*funcI).get_name() == "LU_DSP16210_9")
               cerr << "use time = " << usetime << endl;
#endif
            iblkPL.clear();
         }
         else if ((*funcI).get_functype() == MEMORY ||
                  (*funcI).get_functype() == MEMORY_CORE)
         {
            AX_UAttrCP uattrP;
            int cap;
            uattrP = (*funcI).get_uattr(UATTRONCHIPMEM);

            char * cstr;
            cstr = new char[(uattrP->get_value()).size()+1];
            strcpy(cstr, (uattrP->get_value()).c_str());
            cap = atoi(cstr) * 1024;
            delete cstr;

            int capRem;
            uattrP = (*funcI).get_uattr(UATTRGENCAPREM);
            cstr = new char[(uattrP->get_value()).size()+1];
            strcpy(cstr, (uattrP->get_value()).c_str());
            capRem = atoi(cstr);
            delete cstr;

            int capUsed = cap - capRem;
            (*funcI).set_util((float)capUsed / (float)cap);
            continue;
         }
         (*funcI).set_util((float)usetime / (float)m_latency);
      }

      // computing bus utilization
      AX_BusLCIter busI;
      for (busI = m_busL.begin();
           busI != m_busL.end(); busI++)
      {
         int usetime = 0;
         AX_ConnRefLSIter conrefI;
         for (conrefI = (*busI).get2_first_connref();
              conrefI != (*busI).connref_list_end(); conrefI++)
         {
            int period = (*conrefI).m_finish - (*conrefI).m_start + 1;
            usetime += period;
         }
         (*busI).set_util((float)usetime / (float)m_latency);
      }
   }

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Miscellaneous Functions
// ------------------------------------------------------------
   void AX_ArchGraphC::clean_up()
   {
      m_name.clear();
      m_id = 0;
      m_dsnP = NULL;
      m_status = UNASSEMBLED;
      m_latency = 0;
      m_area = 0;
      m_power = 0;
      m_cost = 0.0;
      m_temp = 0;
   }
// ------------------------------------------------------------
   void AX_ArchGraphC::dump()
   {
      cerr.setf(ios::fixed,ios::floatfield);
      cerr.precision(8);
      cerr << "  Architecture: " << m_name  << endl
           << "  fitness: "<< m_fitness << endl;
      cerr.precision(2);
      cerr << "  latency: " << m_latency
           << "  area: " << m_area
           << "  power: " << m_power
           << "  cost: " << m_cost << endl;

      int nProcs = 0, nMem = 0, nHW = 0, nSW = 0; 
      AX_FuncBlkLCIter fcIter;
      for (fcIter = m_fncblkL.begin();
           fcIter != m_fncblkL.end(); fcIter++) {
         if ((*fcIter).get_functype() == PROCESSOR)
            nProcs++;
         else if (((*fcIter).get_functype() == MEMORY) ||
                  ((*fcIter).get_functype() == MEMORY_CORE))
            nMem++;
         else if (((*fcIter).get_functype() == FUNC_DISTRIBUTOR) ||
                  ((*fcIter).get_functype() == FUNC_COLLECTOR))
            nHW++;
         else if ((*fcIter).get_functype() == FUNCTION) {
            if ((*fcIter).get_type() == HARDWARE)
               nHW++;
            else if ((*fcIter).get_type() == SOFTWARE)
               nSW++;
         }
      }

      cerr << "  # of processors: " << nProcs << endl;
      cerr << "  # of memory blocks: " << nMem << endl;
      cerr << "  # of HW tasks: " << nHW << endl;
      cerr << "  # of SW tasks: " << nSW << endl;

      cerr << endl;
      cerr << "  Platforms: " << endl;
      AX_PlatformLCIter pIter;
      for (pIter = m_platfmL.begin();
           pIter != m_platfmL.end(); pIter++)
          (*pIter).dump();

      cerr << endl;
      cerr << "  Interfaces: " << endl;
      AX_InterfaceLCIter iIter;
      for (iIter = m_intrfcL.begin();
           iIter != m_intrfcL.end(); iIter++)
          (*iIter).dump();

      cerr << endl;
      cerr << "  Functional Blocks: " << endl;
      AX_FuncBlkLCIter fIter;
      for (fIter = m_fncblkL.begin();
           fIter != m_fncblkL.end(); fIter++)
          (*fIter).dump();

      cerr << endl;
      cerr << "  Internal Ports: " << endl;
      AX_CompPortLCIter cIter;
      for (cIter = m_compportL.begin();
           cIter != m_compportL.end(); cIter++)
          (*cIter).dump();

      cerr << endl;
      cerr << "  External Input Ports: " << endl;
      for (cIter = m_ag_inportL.begin();
           cIter != m_ag_inportL.end(); cIter++)
          (*cIter).dump();

      cerr << endl;
      cerr << "  External Output Ports: " << endl;
      for (cIter = m_ag_outportL.begin();
           cIter != m_ag_outportL.end(); cIter++)
          (*cIter).dump();

      cerr << endl;
      cerr << "  Buses: " << endl;
      AX_BusLCIter bIter;
      for (bIter = m_busL.begin();
           bIter != m_busL.end(); bIter++)
          (*bIter).dump();

      cerr << endl;
      cerr << "  Architecture Description Done\n\n" << endl;
   }
// ------------------------------------------------------------
// ------------------------------------------------------------
   void AX_ArchGraphC::dump_simple()
   {
      cerr.setf(ios::fixed,ios::floatfield);
      cerr.precision(8);
      cerr << "  Architecture: " << m_name  << endl
           << "  fitness: "<< m_fitness << endl;
      cerr.precision(2);
      cerr << "  latency: " << m_latency
           << "  area: " << m_area
           << "  power: " << m_power
           << "  cost: " << m_cost << endl;

      int nProcs = 0, nMem = 0, nHW = 0, nSW = 0; 
      AX_FuncBlkLCIter fcIter;
      for (fcIter = m_fncblkL.begin();
           fcIter != m_fncblkL.end(); fcIter++) {
         if ((*fcIter).get_functype() == PROCESSOR)
            nProcs++;
         else if (((*fcIter).get_functype() == MEMORY) ||
                  ((*fcIter).get_functype() == MEMORY_CORE))
            nMem++;
         else if (((*fcIter).get_functype() == FUNC_DISTRIBUTOR) ||
                  ((*fcIter).get_functype() == FUNC_COLLECTOR))
            nHW++;
         else if ((*fcIter).get_functype() == FUNCTION) {
            if ((*fcIter).get_type() == HARDWARE)
               nHW++;
            else if ((*fcIter).get_type() == SOFTWARE)
               nSW++;
         }
      }

      cerr << "  # of processors: " << nProcs << endl;
      cerr << "  # of memory blocks: " << nMem << endl;
      cerr << "  # of HW tasks: " << nHW << endl;
      cerr << "  # of SW tasks: " << nSW << endl;

      cerr << "  Functional Blocks: " << endl;
      AX_FuncBlkLCIter fIter;
      for (fIter = m_fncblkL.begin();
           fIter != m_fncblkL.end(); fIter++)
          (*fIter).dump();
   }
// ------------------------------------------------------------
   void AX_ArchGraphC::dump_small()
   {
      cerr.setf(ios::fixed,ios::floatfield);
      cerr.precision(8);
      cerr << "  Architecture: " << m_name  << endl
             << "  fitness: "<< m_fitness << endl;
      cerr.precision(2);
      cerr << "  latency: " << m_latency
             << "  area: " << m_area
             << "  power: " << m_power
             << "  cost: " << m_cost << endl;

      int nProcs = 0, nMem = 0, nHW = 0, nSW = 0; 
      AX_FuncBlkLCIter fcIter;
      for (fcIter = m_fncblkL.begin();
           fcIter != m_fncblkL.end(); fcIter++) {
         if ((*fcIter).get_functype() == PROCESSOR)
            nProcs++;
         else if (((*fcIter).get_functype() == MEMORY) ||
                  ((*fcIter).get_functype() == MEMORY_CORE))
            nMem++;
         else if (((*fcIter).get_functype() == FUNC_DISTRIBUTOR) ||
                  ((*fcIter).get_functype() == FUNC_COLLECTOR))
            nHW++;
         else if ((*fcIter).get_functype() == FUNCTION) {
            if ((*fcIter).get_type() == HARDWARE)
               nHW++;
            else if ((*fcIter).get_type() == SOFTWARE)
               nSW++;
         }
      }

      cerr << "  # of processors: " << nProcs << endl;
      cerr << "  # of memory blocks: " << nMem << endl;
      cerr << "  # of HW tasks: " << nHW << endl;
      cerr << "  # of SW tasks: " << nSW << endl;

      cerr << endl;
      cerr << "  Functional Blocks: " << endl;
      AX_FuncBlkLCIter fIter;
      for (fIter = m_fncblkL.begin();
           fIter != m_fncblkL.end(); fIter++)
          (*fIter).dump();

      cerr << endl;
      cerr << "  External Input Ports: " << endl;
      AX_CompPortLCIter cIter;
      for (cIter = m_ag_inportL.begin();
           cIter != m_ag_inportL.end(); cIter++)
          (*cIter).dump_simple();

      cerr << endl;
      cerr << "  External Output Ports: " << endl;
      for (cIter = m_ag_outportL.begin();
           cIter != m_ag_outportL.end(); cIter++)
          (*cIter).dump_simple();

      cerr << endl;
      cerr << "  Buses: " << endl;
      AX_BusLCIter bIter;
      for (bIter = m_busL.begin();
           bIter != m_busL.end(); bIter++)
          (*bIter).dump();

      cerr << endl;
      cerr << "  Architecture Description Done\n\n" << endl;
   }

// ------------------------------------------------------------
// ------------------------------------------------------------
   void AX_ArchGraphC::write(fstream & agFile)
   {
      agFile.setf(ios::fixed,ios::floatfield);
      agFile.precision(8);
      agFile << "  Architecture: " << m_name  << endl
             << "  fitness: "<< m_fitness << endl;
      agFile.precision(2);
      agFile << "  latency: " << m_latency
             << "  area: " << m_area
             << "  power: " << m_power
             << "  cost: " << m_cost << endl;

      int nProcs = 0, nMem = 0, nHW = 0, nSW = 0; 
      AX_FuncBlkLCIter fcIter;
      for (fcIter = m_fncblkL.begin();
           fcIter != m_fncblkL.end(); fcIter++) {
         if ((*fcIter).get_functype() == PROCESSOR)
            nProcs++;
         else if (((*fcIter).get_functype() == MEMORY) ||
                  ((*fcIter).get_functype() == MEMORY_CORE))
            nMem++;
         else if (((*fcIter).get_functype() == FUNC_DISTRIBUTOR) ||
                  ((*fcIter).get_functype() == FUNC_COLLECTOR))
            nHW++;
         else if ((*fcIter).get_functype() == FUNCTION) {
            if ((*fcIter).get_type() == HARDWARE)
               nHW++;
            else if ((*fcIter).get_type() == SOFTWARE)
               nSW++;
         }
      }

      agFile << "  # of processors: " << nProcs << endl;
      agFile << "  # of memory blocks: " << nMem << endl;
      agFile << "  # of HW tasks: " << nHW << endl;
      agFile << "  # of SW tasks: " << nSW << endl;

      agFile << endl;
      agFile << "  Platforms: " << endl;
      AX_PlatformLCIter pIter;
      for (pIter = m_platfmL.begin();
           pIter != m_platfmL.end(); pIter++)
          (*pIter).write(agFile);

      agFile << endl;
      agFile << "  Interfaces: " << endl;
      AX_InterfaceLCIter iIter;
      for (iIter = m_intrfcL.begin();
           iIter != m_intrfcL.end(); iIter++)
          (*iIter).write(agFile);

      agFile << endl;
      agFile << "  Functional Blocks: " << endl;
      AX_FuncBlkLCIter fIter;
      for (fIter = m_fncblkL.begin();
           fIter != m_fncblkL.end(); fIter++)
          (*fIter).write(agFile);

      agFile << endl;
      agFile << "  Internal Ports: " << endl;
      AX_CompPortLCIter cIter;
      for (cIter = m_compportL.begin();
           cIter != m_compportL.end(); cIter++)
          (*cIter).write(agFile);

      agFile << endl;
      agFile << "  External Input Ports: " << endl;
      for (cIter = m_ag_inportL.begin();
           cIter != m_ag_inportL.end(); cIter++)
          (*cIter).write(agFile);

      agFile << endl;
      agFile << "  External Output Ports: " << endl;
      for (cIter = m_ag_outportL.begin();
           cIter != m_ag_outportL.end(); cIter++)
          (*cIter).write(agFile);

      agFile << endl;
      agFile << "  Buses: " << endl;
      AX_BusLCIter bIter;
      for (bIter = m_busL.begin();
           bIter != m_busL.end(); bIter++)
          (*bIter).write(agFile);

      agFile << endl;
      agFile << "  Architecture Description Done\n\n" << endl;
   }
// ------------------------------------------------------------
   void AX_ArchGraphC::write_small(fstream & agFile)
   {
      agFile.setf(ios::fixed,ios::floatfield);
      agFile.precision(8);
      agFile << "  Architecture: " << m_name  << endl
             << "  fitness: "<< m_fitness << endl;
      agFile.precision(2);
      agFile << "  latency: " << m_latency
             << "  area: " << m_area
             << "  power: " << m_power
             << "  cost: " << m_cost << endl;

      agFile << endl;
      agFile << "  Functional Blocks: " << endl;
      AX_FuncBlkLCIter fIter;
      for (fIter = m_fncblkL.begin();
           fIter != m_fncblkL.end(); fIter++)
          (*fIter).write(agFile);

      agFile << endl;
      agFile << "  External Input Ports: " << endl;
      AX_CompPortLCIter cIter;
      for (cIter = m_ag_inportL.begin();
           cIter != m_ag_inportL.end(); cIter++)
          (*cIter).write_simple(agFile);

      agFile << endl;
      agFile << "  External Output Ports: " << endl;
      for (cIter = m_ag_outportL.begin();
           cIter != m_ag_outportL.end(); cIter++)
          (*cIter).write_simple(agFile);

      agFile << endl;
      agFile << "  Buses: " << endl;
      AX_BusLCIter bIter;
      for (bIter = m_busL.begin();
           bIter != m_busL.end(); bIter++)
          (*bIter).write(agFile);

      agFile << endl;
      agFile << "  Architecture Description Done\n\n" << endl;
   }
// ------------------------------------------------------------
   void AX_ArchGraphC::write_SystemC( string folder )
   {
      string cmd("mkdir -p ");
      cmd += folder;
      system( cmd.c_str() );

      m_gsccL.clear();

      // Create a Gen_SystemC object for each functional block
      AX_FuncBlkLCIter fIter;
      for (fIter = m_fncblkL.begin();
           fIter != m_fncblkL.end(); fIter++)
      {
          AX_Gen_SystemCCP gen_sc_obj = new AX_Gen_SystemCC( fIter->get_function() , folder );
          m_gsccL.push_back( gen_sc_obj );
          fIter->write_SystemC( gen_sc_obj );
      }

 
     // Write SystemC files (*.h and *.cpp) 

      AX_Gen_SystemCPLCIter gscIter;
      for ( gscIter = m_gsccL.begin();
            gscIter != m_gsccL.end(); gscIter++ )
      {
          (*gscIter)->write();
          delete(*gscIter);
      }

   }
// ------------------------------------------------------------
   void AX_ArchGraphC::write_simple(fstream & agFile)
   {
      agFile.setf(ios::fixed,ios::floatfield);
      agFile.precision(8);
      agFile << "  Architecture: " << m_name  << endl
             << "  fitness: "<< m_fitness << endl;
      agFile.precision(2);
      agFile << "  latency: " << m_latency
             << "  area: " << m_area
             << "  power: " << m_power
             << "  cost: " << m_cost << endl;

      agFile << "  Functional Blocks: " << endl;
      AX_FuncBlkLCIter fIter;
      for (fIter = m_fncblkL.begin();
           fIter != m_fncblkL.end(); fIter++)
          (*fIter).write(agFile);
   }


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~End of Function Definitions
