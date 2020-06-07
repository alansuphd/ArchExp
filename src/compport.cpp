// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// compport.cpp 
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <iostream>
#include "compport.h"
using namespace std;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructor
// ------------------------------------------------------------
   AX_CompPortC::AX_CompPortC()
   {
      m_name.clear();
      m_id = -1;
      m_db_idx = -1;
      m_type = PORT_TYPE_UNKNOWN;
      m_compP = NULL;
      m_agP = NULL;
      m_sample_size = -1;
      m_assign_sample_rate = -1;
      m_intrfcPL.clear();
      m_sw_portP = NULL;
      m_refCnt = 0;
      m_temp = -1;
   }
// ------------------------------------------------------------
   AX_CompPortC::AX_CompPortC(const AX_CompPortC & other)
   {
      copy(other);
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Destructor
// ------------------------------------------------------------
   AX_CompPortC::~AX_CompPortC()
   {
      remove_all_buses();
      remove_all_sample_rates();
      remove_all_intrfcs();
      clean_up();
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Assignator & Comparators
// ------------------------------------------------------------
   AX_CompPortC & AX_CompPortC::operator= (const AX_CompPortC & other)
   {
      if (&other == this)
         return(*this);
 
      copy(other);

      return(*this);
   }
// ------------------------------------------------------------
   void AX_CompPortC::copy(const AX_CompPortC & other)
   {
      m_name = other.m_name;
      m_id = other.m_id;
      m_db_idx = other.m_db_idx;
      m_type = other.m_type;
      m_compP = other.m_compP;
      m_agP = other.m_agP;
      m_busPL = other.m_busPL;
      m_sample_size = other.m_sample_size;
      m_sample_rateL = other.m_sample_rateL;
      m_intrfcPL = other.m_intrfcPL;
      m_assign_sample_rate = other.m_assign_sample_rate;
      m_sw_portP = other.m_sw_portP;
      m_refCnt = other.m_refCnt;
      m_temp = other.m_temp;
   }
// ------------------------------------------------------------
   bool AX_CompPortC::operator== (const AX_CompPortC & other)
   {
      if (&other == this)
         return(true);
 
      if (m_name == other.m_name &&
          m_compP == other.m_compP &&
          m_agP == other.m_agP)
         return(true);
 
      return(false);
   }
// ------------------------------------------------------------

// ~~~~~~~~~~~~~~~~~~~~~~ Interface Pointer List Manipulations
// ------------------------------------------------------------
   AX_InterfacePLCIter AX_CompPortC::add_new_intrfc(AX_InterfaceCP intrfcP)
   {
      AX_InterfacePLCIter iIter;

      for (iIter = m_intrfcPL.begin();
           iIter != m_intrfcPL.end(); iIter++)
          if (intrfcP == (*iIter))
             return iIter;

      m_intrfcPL.push_back(intrfcP);
      iIter = --m_intrfcPL.end();

    return iIter;
   }
// ------------------------------------------------------------
   AX_InterfaceCP AX_CompPortC::get_intrfc(int index)
   {
      AX_InterfaceCP intrfcP;

      if ((index < 0)||((unsigned int)index > m_intrfcPL.size()-1))
         intrfcP = NULL;
      else
      {
         AX_InterfacePLCIter iIter;
         int i;

         iIter = m_intrfcPL.begin();
         for(i = 0; i < index; i++) iIter++;

         intrfcP = (*iIter);
      }

      return intrfcP;
   }
// ------------------------------------------------------------
   AX_InterfaceCP AX_CompPortC::get_intrfc(string name)
   {
      if (name.empty())
         return NULL;

      AX_InterfacePLCIter iIter;
      for (iIter = m_intrfcPL.begin();
           iIter != m_intrfcPL.end(); iIter++)
         if ((*iIter)->get_name() == name)
            return (*iIter);

      return NULL;
   }
// ------------------------------------------------------------
   void AX_CompPortC::remove_intrfc(AX_InterfaceCP intrfcP)
   {
      AX_InterfacePLCIter iIter;

      for (iIter = m_intrfcPL.begin();
           iIter != m_intrfcPL.end(); iIter++)
          if ((*iIter) == intrfcP)
          {
             m_intrfcPL.erase(iIter);
             return;
          }
      return;
   }
// ------------------------------------------------------------
   AX_InterfacePLCIter AX_CompPortC::delete_intrfc(AX_InterfacePLCIter & iter)

   {
      AX_InterfacePLCIter retIter;

      if (m_intrfcPL.end() == iter)
         return iter;
      else if ((--m_intrfcPL.end()) == iter)
      {
         m_intrfcPL.erase(iter);
         retIter = m_intrfcPL.end();
      }
      else
      {
         retIter = ++iter;
         iter--;
         m_intrfcPL.erase(iter);
      }

      return retIter;
   }
// ------------------------------------------------------------
   bool AX_CompPortC::copy_intrfcs(AX_InterfaceLC * intrfcL,
                                       AX_CompPortCP oPortP)
   {
      AX_InterfacePLCIter oIntrPI;
      AX_InterfaceLCIter nIntrI;
      AX_InterfaceCP intrfcP;

      for (oIntrPI = oPortP->get2_first_intrfc();
           oIntrPI != oPortP->intrfc_list_end(); oIntrPI++)
      {
          intrfcP = NULL;
          for (nIntrI = intrfcL->begin();
               nIntrI != intrfcL->end(); nIntrI++)
             if ((*nIntrI).get_name() == (*oIntrPI)->get_name())
             {
                intrfcP = &(*nIntrI);
                break;
             }

          if (intrfcP == NULL)
          {
#if DEBUG >= 1
             cerr << "Cannot find intrfc '"
                  << (*oIntrPI)->get_name() << "' resides in "
                  << oPortP->get_comp()->get_name()
                  << "." << oPortP->get_name()
                  << endl;
#endif
             return false;
          }
          add_new_intrfc(intrfcP);
      }

      return true;
   }
// ------------------------------------------------------------
   AX_InterfaceCP AX_CompPortC::ran_pick_intrfc()
   {
      int iNum;

      if (m_intrfcPL.size() == 0)
         return NULL;
      iNum = my_rand(m_intrfcPL.size());

      return get_intrfc(iNum);
   }
// ------------------------------------------------------------
   bool AX_CompPortC::find_common_intrfc(AX_InterfacePLC & intrfcPL)
   {
      bool nonempty = false, stepback = false;
      AX_InterfacePLCIter iter1, iter2, iter3;

      for (iter1 = intrfcPL.begin();
           iter1 != intrfcPL.end(); iter1++)
      {
         if (stepback)
         {
            iter1 = intrfcPL.begin();
            stepback = false;
         }
         // search the fncblk's list to see if this interface exists
         bool found = false;
         for (iter2 = m_intrfcPL.begin();
              iter2 != m_intrfcPL.end(); iter2++)
         {
            if ((*iter1) == (*iter2))
            {
               found = true;
               nonempty = true;
               break;
            }
         }
         // interface exists, to the next one
         if (found)
            continue;

         // interface does not exist, delete it from intrfcPL
         if ((--intrfcPL.end()) == iter1)
         {
            intrfcPL.erase(iter1);
            iter1 = intrfcPL.end();
         }
         else
         {
            iter3 = ++iter1;
            iter1--;
            intrfcPL.erase(iter1);
            iter1 = iter3;
         }
         if (iter1 != intrfcPL.begin())
            iter1--;
         else
            stepback = true;
      }

      return nonempty;
   }

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Bus Pointer List Manipulations
// ------------------------------------------------------------
   AX_BusPLCIter AX_CompPortC::add_new_bus(AX_BusCP busP)
   {
      if (busP == NULL)
         return m_busPL.end();

      AX_BusPLCIter bIter;

      for (bIter = m_busPL.begin(); 
           bIter != m_busPL.end(); bIter++)
          if (busP == (*bIter))
             return bIter;

      if ((m_type == PORT_IN) && (m_busPL.size() > 0))
      {
         // This means that the component port is an IN port. In hardware
         // design, IN port allows only ehtier a BUS or a WIRE connected 
         // to it. 
         //
         // (m_busPL.size() > 0) implies there is already some BUS/WIRE
         // connected to this port, so no more BUS/WIRE is allowed to be
         // added into this port. We have to return --m_busPL.end() or
         // m_busPL.end().
         //
         AX_BusCP tBusP = m_busPL.front();
         if (tBusP->get_intrfc()->get_intrtype() == INTRFC_BUS &&
             busP->get_intrfc()->get_intrtype() == INTRFC_BUS)
         {
            if (tBusP->get_intrfc()->get_name() ==
                busP->get_intrfc()->get_name())
         // Return --m_busPL.end() means both the input AX_BusC and the 
         // AX_BusC in m_busPL.front() are all INTRFC_BUS type and have
         // the same name, they are the same thing.
               return --m_busPL.end();
            else
         // Return m_busPL.end() means both the input AX_BusC and the 
         // AX_BusC in m_busPL.front() are all INTRFC_BUS type. But they
         // have have different name, they are NOT the same thing.
               return m_busPL.end();
         }
         else
         // Return m_busPL.end() means the input AX_BusC and the AX_BusC
         // in m_busPL.front() are different type. 
         // They are NOT the same thing.
            return m_busPL.end();
      }

      if (( m_type == PORT_OUT ) 
       && ( m_busPL.size() > 0 )
       && ( busP->get_intrfc()->get_intrtype() == INTRFC_BUS ))
      {
         // This means that the component port is an OUT port. In hardware
         // design, OUT port allows multiple fan-out. Therefore, it allows
         // more than one BUS/WIRE connected into it. 
         //
         // (m_busPL.size() > 0) implies that there is already some BUS/WIRE
         // connected to this port, so we are going to check if there exist an
         // AX_BusC in m_busPL, which same as the input AX_BusC in interface
         // type and name.
         //
         std::string bus_intrfc_name = busP->get_intrfc()->get_name();

         for (bIter = m_busPL.begin(); 
              bIter != m_busPL.end(); bIter++)
         {
            if (((*bIter)->get_intrfc()->get_intrtype() == INTRFC_BUS)
             && ((*bIter)->get_intrfc()->get_name() == bus_intrfc_name ))
               return bIter;
         }
        
         // After the above for loop, no the same INTRFC_BUS object found.
         // So we go to the following code to add the new BUS/WIRE into 
         // this component port.
      }

#if DEBUG >= 3
      if (busP->get_intrfc()->get_intrtype() == INTRFC_BUS &&
          m_busPL.size() > 1)
      {
         cerr << "Mult-bus: ";
         AX_BusPLCIter bIter;
         for (bIter = m_busPL.begin();
              bIter != m_busPL.end(); bIter++)
            cerr << (*bIter)->get_intrfc()->get_name() << " ";
         cerr << endl;
      }
#endif
      m_busPL.push_back(busP);

      return (--m_busPL.end());
   }
// ------------------------------------------------------------
   AX_BusCP AX_CompPortC::get_bus(int index)
   {
      AX_BusCP busP = NULL;

      if ((index < 0)||((unsigned int)index > m_busPL.size()-1))
         busP = NULL;
      else
      {
         AX_BusPLCIter bIter;
         int i;

         bIter = m_busPL.begin();
         for(i = 0; i < index; i++) bIter++;

         busP = (*bIter);
      }

      return busP;
   }
// ------------------------------------------------------------
   void AX_CompPortC::remove_bus(AX_BusCP busP)
   {
       AX_BusPLCIter bIter;

       for (bIter = m_busPL.begin(); 
            bIter != m_busPL.end(); bIter++)
           if ((*bIter) == busP)
           {
               m_busPL.erase(bIter);
               return;
           }
       return;
   }
// ------------------------------------------------------------
   AX_BusPLCIter AX_CompPortC::delete_bus(AX_BusPLCIter & iter)
   {
      AX_BusPLCIter retIter;

      if (m_busPL.end() == iter)
         return iter;
      else if ((--m_busPL.end()) == iter)
      {
         m_busPL.erase(iter);
         retIter = m_busPL.end();
      }
      else
      {
         retIter = ++iter;
         iter--;
         m_busPL.erase(iter);
      }
      
      return retIter;
   }
// ------------------------------------------------------------
   AX_BusPLCIter AX_CompPortC::delete_bus(AX_BusCP busP)
   {
      AX_BusPLCIter iter, retIter;

      if (busP == NULL)
         return m_busPL.end();
      else if ((*(--m_busPL.end())) == busP)
      {
         iter = --m_busPL.end();
         m_busPL.erase(iter);
         return m_busPL.end();
      }
      else
      {
         retIter = m_busPL.end();
         for (iter = m_busPL.begin(); iter != m_busPL.end(); iter++)
            if ((*iter) == busP)
            {
               retIter = ++iter;
               iter--;
               m_busPL.erase(iter);
               return retIter;
            }
      }
      return retIter;
   }
// ------------------------------------------------------------
   void AX_CompPortC::scan_delete_bus(AX_BusCP busP)
   {
      AX_BusPLCIter busPI;
      bool stepback = false;
      for (busPI = m_busPL.begin();
           busPI != m_busPL.end(); busPI++)
      {
         if (stepback)
         {
            busPI--;
            stepback = false;
         }
         if ((*busPI) == busP)
         {
            busPI = delete_bus(busP);
            if (busPI != (--m_busPL.end()))
               busPI--;
            else
               stepback = true;
         }
      }
   }
// ------------------------------------------------------------
   AX_BusCP AX_CompPortC::func_on_port(AX_FuncBlkCP funcP)
   {
      AX_BusPLCIter iter;

      for (iter = m_busPL.begin(); iter != m_busPL.end(); iter++)
      {
         AX_ConnPntLCIter conPntI;
         for (conPntI = (*iter)->get2_first_connpnt();
              conPntI != (*iter)->connpnt_list_end(); conPntI++)
            if ((*conPntI).get_comp() == funcP)
               return (*iter);
      }

      return NULL;
   }

// ~~~~~~~~~~~~~~~~~~~~ Sample Rate Instance List Manipulations
// ------------------------------------------------------------
   AX_IntLCIter AX_CompPortC::add_new_sample_rate(int rate)
   {
      if (rate == 0)
         return m_sample_rateL.end();

      AX_IntLCIter rIter;

      for (rIter = m_sample_rateL.begin(); 
           rIter != m_sample_rateL.end(); rIter++)
          if (rate == (*rIter))
             return rIter;

      m_sample_rateL.push_back(rate);

      return (--m_sample_rateL.end());
   }
// ------------------------------------------------------------
   AX_IntCP AX_CompPortC::get_sample_rate(int index)
   {
      AX_IntCP intP;

      if ((index < 0)||((unsigned int)index > m_sample_rateL.size()-1))
         intP = NULL;
      else
      {
         AX_IntLCIter iIter;
         int i;

         iIter = m_sample_rateL.begin();
         for(i = 0; i < index; i++) iIter++;

         intP = &(*iIter);
      }

      return intP;
   }
// ------------------------------------------------------------
   void AX_CompPortC::remove_sample_rate(int rate)
   {
       AX_IntLCIter rIter;

       for (rIter = m_sample_rateL.begin(); 
            rIter != m_sample_rateL.end(); rIter++)
           if ((*rIter) == rate)
           {
               rIter = delete_sample_rate(rIter);
               return;
           }
       return;
   }
// ------------------------------------------------------------
   AX_IntLCIter AX_CompPortC::delete_sample_rate(AX_IntLCIter & iter)
   {
      AX_IntLCIter rIter;

      if (m_sample_rateL.end() == iter)
         return iter;
      else if ((--m_sample_rateL.end()) == iter)
      {
         m_sample_rateL.erase(iter);
         rIter = m_sample_rateL.end();
      }
      else
      {
         rIter = ++iter;
         iter--;
         m_sample_rateL.erase(iter);
      }
     
      return rIter;
   }
// ------------------------------------------------------------
   void AX_CompPortC::remove_all_sample_rates()
   {
      AX_IntLCIter iIter;
 
      while ((iIter = m_sample_rateL.begin()) != m_sample_rateL.end())
         m_sample_rateL.erase(iIter);

      m_sample_rateL.clear();
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Structure List Manipulations


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Special Functions


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Miscellaneous Functions
// ------------------------------------------------------------
   void AX_CompPortC::clean_up()
   {
      m_name.clear();
      m_id = 0;
      m_db_idx = 0;
      m_type = PORT_TYPE_UNKNOWN;
      m_compP = NULL;
      m_agP = NULL;
      m_sample_size = 0;
      m_assign_sample_rate = 0;
      m_sw_portP = NULL;
      m_refCnt = 0;
      m_temp = 0;
   }
// ------------------------------------------------------------
   void AX_CompPortC::dump()
   {
      if (m_agP != NULL)
         cerr << "        " << m_agP->get_name() << ".";
      else
         cerr << "        " << m_compP->get_name() << ".";
      cerr << m_name << endl;
      cerr << "        id " << m_id
           << "  db_idx " << m_db_idx
           << "  type " << m_type
           << "  sample size " << m_sample_size
           << "  refCnt " << m_refCnt << endl;

      if (m_sw_portP != NULL)
      {
         cerr << "        sw_portP: ";
         if (m_sw_portP->m_agP != NULL)
         {
            cerr << "ERROR PORT: "
                 << m_sw_portP->m_agP->get_name();
         }
         else
            cerr << m_sw_portP->m_compP->get_name();

         cerr << "." << m_sw_portP->m_name << endl;
      }

      if (m_sample_rateL.size() > 0)
      {
         cerr << "        Sample rates: ";
         AX_IntLCIter rIter;
         for (rIter = m_sample_rateL.begin();
              rIter != m_sample_rateL.end(); rIter++)
            cerr << (*rIter) << " ";
         cerr << endl;
      }

      cerr << "        assigned sample rate: "
           << m_assign_sample_rate << endl;

      // dumping interfaces
      AX_InterfacePLCIter iIter;
      if (m_intrfcPL.size() > 0)
      {
         cerr << "        Interfaces:" << endl; 
         for (iIter = m_intrfcPL.begin();
              iIter != m_intrfcPL.end(); iIter++)
             (*iIter)->dump_simple();
      }

      if (m_busPL.size() > 0)
      {
         cerr << "        Buses:" << endl;
         AX_BusPLCIter bIter;
         for (bIter = m_busPL.begin();
              bIter != m_busPL.end(); bIter++)
            (*bIter)->dump_simple();
      }
      cerr << endl;
   }
// ------------------------------------------------------------
   void AX_CompPortC::dump_simple()
   {
      if (m_agP != NULL)
         cerr << "        " << m_agP->get_name() << ".";
      else
         cerr << "        " << m_compP->get_name() << ".";
      cerr << m_name << endl;
      cerr << "          id " << m_id
           << "  db_idx " << m_db_idx
           << "  type " << m_type
           << "  sample size " << m_sample_size
           << "  refCnt " << m_refCnt << endl;
      if (m_sw_portP != NULL)
      {
         cerr << "          sw_portP: ";
         if (m_sw_portP->m_agP != NULL)
         {
            cerr << "ERROR PORT: "
                 << m_sw_portP->m_agP->get_name();
         }
         else
            cerr << m_sw_portP->m_compP->get_name();

         cerr << "." << m_sw_portP->m_name << endl;
      }

      // writing buses
      AX_BusPLCIter bIter;
      if (m_busPL.size() > 0)
      {
          cerr << "          Bus id's: ";
          for (bIter = m_busPL.begin();
               bIter != m_busPL.end(); bIter++)
              cerr << (*bIter)->get_id() << " ";
          cerr << endl;
      }
   }
// ------------------------------------------------------------
   void AX_CompPortC::write(fstream & File)
   {
      if (m_agP != NULL)
         File << "        " << m_agP->get_name() << ".";
      else
         File << "        " << m_compP->get_name() << ".";
      File << m_name << endl;
      File << "        id " << m_id
           << "  db_idx " << m_db_idx
           << "  type " << m_type
           << "  sample size " << m_sample_size
           << "  refCnt " << m_refCnt << endl;

      if (m_sw_portP != NULL)
      {
         File << "        sw_portP: ";
         if (m_sw_portP->m_agP != NULL)
         {
            File << "ERROR PORT: "
                 << m_sw_portP->m_agP->get_name();
         }
         else
            File << m_sw_portP->m_compP->get_name();

         File << "." << m_sw_portP->m_name << endl;
      }

      if (m_sample_rateL.size() > 0)
      {
         File << "        Sample rates: ";
         AX_IntLCIter rIter;
         for (rIter = m_sample_rateL.begin();
              rIter != m_sample_rateL.end(); rIter++)
            File << (*rIter) << " ";
         File << endl;
      }

      File << "        assigned sample rate "
           << m_assign_sample_rate << endl;

      // writing interfaces
      AX_InterfacePLCIter iIter;
      if (m_intrfcPL.size() > 0)
      {
         File << "        Interfaces:" << endl; 
         for (iIter = m_intrfcPL.begin();
              iIter != m_intrfcPL.end(); iIter++)
             (*iIter)->write_simple(File);
      }

      if (m_busPL.size() > 0)
      {
         File << "        Buses:" << endl;
         AX_BusPLCIter bIter;
         for (bIter = m_busPL.begin();
              bIter != m_busPL.end(); bIter++)
            (*bIter)->write_simple(File);
      }
      File << endl;
   }
// ------------------------------------------------------------
   void AX_CompPortC::write_simple(fstream & File)
   {
      if (m_agP != NULL)
         File << "        " << m_agP->get_name() << ".";
      else
         File << "        " << m_compP->get_name() << ".";
      File << m_name << endl;
      File << "        id " << m_id
           << "  db_idx " << m_db_idx
           << "  type " << m_type
           << "  sample size " << m_sample_size
           << "  refCnt " << m_refCnt << endl;
      if (m_sw_portP != NULL)
      {
         File << "        sw_portP: ";
         if (m_sw_portP->m_agP != NULL)
         {
            File << "ERROR PORT: "
                 << m_sw_portP->m_agP->get_name();
         }
         else
            File << m_sw_portP->m_compP->get_name();

         File << "." << m_sw_portP->m_name << endl;
      }

      // writing buses
      AX_BusPLCIter bIter;
      if (m_busPL.size() > 0)
      {
          File << "          Bus id's: ";
          for (bIter = m_busPL.begin();
               bIter != m_busPL.end(); bIter++)
              File << (*bIter)->get_id() << " ";
          File << endl;
      }
   }
// ------------------------------------------------------------
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~End of Function Definitions
