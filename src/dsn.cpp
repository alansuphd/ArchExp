// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// dsn.c
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <iostream>
#include "dsn.h"
#include "extern.h"
using namespace std;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructor
// ------------------------------------------------------------
   AX_DsnC::AX_DsnC()
   {
      m_name.clear();
      m_id = -1;
      m_temp = -1;
   }
// ------------------------------------------------------------
   AX_DsnC::AX_DsnC(AX_DsnC & other)
   {
      m_name = other.m_name;
      m_id = other.m_id;
      m_platfmL = other.m_platfmL;
      m_intrfcL = other.m_intrfcL;
      m_fncblkL = other.m_fncblkL;
      m_tg = other.m_tg;
      m_dsnspecL = other.m_dsnspecL;
      m_temp = -1;
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Destructor
// ------------------------------------------------------------
   AX_DsnC::~AX_DsnC()
   {
      remove_all_platfms();
      remove_all_intrfcs();
      remove_all_fncblks();
      remove_all_dsnspecs();
      clean_up();
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Assignator & Comparators
// ------------------------------------------------------------
   AX_DsnC & AX_DsnC::operator= (AX_DsnC & other)
   {
      if (&other == this)
         return(*this);

      m_name = other.m_name;
      m_id = other.m_id;
      m_platfmL = other.m_platfmL;
      m_intrfcL = other.m_intrfcL;
      m_fncblkL = other.m_fncblkL;
      m_tg = other.m_tg;
      m_dsnspecL = other.m_dsnspecL;
      m_temp = -1;

      return(*this);
   }
// ------------------------------------------------------------
   bool AX_DsnC::operator== (const AX_DsnC & other)
   {
      if (&other == this)
         return(true); 

      return(false);
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Pointer List Manipulations


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Instance List Manipulations
// ------------------------------------------------------------
   AX_FuncBlkLCIter AX_DsnC::create_new_fncblk()
   {
      AX_FuncBlkC fncblk;

      m_fncblkL.push_back(fncblk);

      return(--m_fncblkL.end());
   }
// ------------------------------------------------------------
   AX_FuncBlkLCIter AX_DsnC::create_new_fncblk(string & name)
   {
      if (name.empty())
         return m_fncblkL.end();

      AX_FuncBlkLCIter fIter;
      for (fIter = m_fncblkL.begin();
           fIter != m_fncblkL.end(); fIter++)
         if ((*fIter).get_name() == name)
         {
            return fIter;
         }

      AX_FuncBlkC nFncBlk;
      m_fncblkL.push_back(nFncBlk);
      fIter = --m_fncblkL.end();
      (*fIter).set_name(name);

      return(fIter);
   }
// ------------------------------------------------------------
   AX_FuncBlkLCIter AX_DsnC::get_fncblk(std::string & name)
   {
      if (name.empty())
         return m_fncblkL.end();

      AX_FuncBlkLCIter fIter;
      for (fIter = m_fncblkL.begin();
           fIter != m_fncblkL.end(); fIter++)
         if ((*fIter).get_name() == name)
            return fIter;

     return fIter;
   }
// ------------------------------------------------------------
   AX_FuncBlkLCIter AX_DsnC::add_new_fncblk(AX_FuncBlkC & fncblk)
   {
      AX_FuncBlkLCIter fIter;

      for (fIter = m_fncblkL.begin();
           fIter != m_fncblkL.end(); fIter++)
          if (fncblk == (*fIter))
             return fIter;

      m_fncblkL.push_back(fncblk);

      return (--m_fncblkL.end());
   }
// ------------------------------------------------------------
   bool AX_DsnC::is_new_fncblk(std::string & name)
   {
      if (name.empty())
         return false;

      AX_FuncBlkLCIter fIter;
      for (fIter = m_fncblkL.begin();
           fIter != m_fncblkL.end(); fIter++)
         if ((*fIter).get_name() == name)
            return false;

     return true;
   }
// ------------------------------------------------------------
   AX_FuncBlkCP AX_DsnC::get_fncblk(int index)
   {
      AX_FuncBlkCP fncblkP = NULL;

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
   int AX_DsnC::get_fncblk_id(string name)
   {
      if (name.empty())
         return -1;

      AX_FuncBlkLCIter fIter;

      for (fIter = m_fncblkL.begin();
           fIter != m_fncblkL.end(); fIter++)
          if ((*fIter).get_name() == name)
             return (*fIter).get_id();

      return -1;
   }
// ------------------------------------------------------------
   AX_FuncBlkLCIter AX_DsnC::get_first_fncblk_by_func(string func)
   {
      if (func.empty())
         return m_fncblkL.end();

      AX_FuncBlkLCIter fIter;

      for (fIter = m_fncblkL.begin();
           fIter != m_fncblkL.end(); fIter++)
          if ((*fIter).get_function() == func)
             return fIter;

      return m_fncblkL.end();
   }
// ------------------------------------------------------------
   AX_FuncBlkLCIter AX_DsnC::get_next_fncblk_by_func(AX_FuncBlkLCIter iter,
                                                       string func)
   {
      if (func.empty())
         return m_fncblkL.end();

      AX_FuncBlkLCIter fIter;

      for (fIter = iter; fIter != m_fncblkL.end(); fIter++)
          if ((*fIter).get_function() == func)
             return fIter;

      return fIter;
   }
// ------------------------------------------------------------
// This function finds all processors of the same family then
// add them into the dependent list of funcP
   bool AX_DsnC::set_proc_family(std::string & family,
                                 AX_FuncBlkCP funcP)
   {
      if (family.empty())
         return false;

      AX_FuncBlkLCIter fIter;
      bool found = false;
      for (fIter = m_fncblkL.begin();
           fIter != m_fncblkL.end(); fIter++)
         if ((*fIter).get_function() == family)
         {
            found = true;
            funcP->add_new_fncblk(&(*fIter));
#if DEBUG >= 3
            cerr << "Processor found for " << funcP->get_name() << endl;
            (*fIter).dump();
#endif
         }

     return found;
   }
// ------------------------------------------------------------
   void AX_DsnC::remove_fncblk(AX_FuncBlkCP fncblkP)
   {
       AX_FuncBlkLCIter fIter;

       for (fIter = m_fncblkL.begin();
            fIter != m_fncblkL.end(); fIter++)
           if (&(*fIter) == fncblkP)
           {
               m_fncblkL.erase(fIter);
               return;
           }
       return;
   }
// ------------------------------------------------------------
   AX_FuncBlkLCIter AX_DsnC::delete_fncblk(AX_FuncBlkLCIter & iter)
   {
      AX_FuncBlkLCIter fIter;

      if (m_fncblkL.end() == iter)
         return iter;
      else if ((--m_fncblkL.end()) == iter)
      {
         AX_FuncBlkC fTmp;
         (*iter) = fTmp;
         m_fncblkL.erase(iter);
         fIter = m_fncblkL.end();
      }
      else
      {
         fIter = ++iter;
         iter--;
         AX_FuncBlkC fTmp;
         (*iter) = fTmp;
         m_fncblkL.erase(iter);
      }

      return fIter;
   }
// ------------------------------------------------------------
   void AX_DsnC::remove_all_fncblks()
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
   AX_FuncBlkCP AX_DsnC::ran_pick_mem(int & cap, int min, int archwidth)
   {
      AX_FuncBlkLCIter fIter;
      AX_FuncBlkCP *memPool;
      int memCnt = 0;
      int nextwidth;
      bool found = false;

      // find feasible memory modules
      memPool = new AX_FuncBlkCP[m_fncblkL.size()];
      nextwidth = next_grade(archwidth);
      for (fIter = m_fncblkL.begin();
           fIter != m_fncblkL.end(); fIter++)
      {
         if ((*fIter).get_functype() == MEMORY ||
             (*fIter).get_functype() == MEMORY_CORE)
         {
            AX_UAttrCP uattrP;
            uattrP = (*fIter).get_uattr(UATTRONCHIPMEM);
            int memCap;
            if ((*fIter).get_functype() == MEMORY)
               memCap = my_atoi(uattrP->get_value()) * 1024;
            else
               memCap = my_atoi(uattrP->get_value());
            uattrP = (*fIter).get_uattr(UATTRARCHWIDTH);
            int memwidth = my_atoi(uattrP->get_value());
            if (((*fIter).get_functype() == MEMORY &&
                 memwidth >= archwidth && memwidth < nextwidth &&
                 memCap > min) ||
                ((*fIter).get_functype() == MEMORY_CORE &&
                 memwidth >= archwidth && memwidth < nextwidth))
            {
               memPool[memCnt++] = &(*fIter);
               found = true;
            }
         }
      }

      if (!found)
      {
         cap = 0;
         return NULL;
      }

      // randomly select a memory module
      int pick = my_rand(memCnt);

      // get the memory capacity
      AX_UAttrCP uattrP;
      AX_FuncBlkCP memPick = memPool[pick];
      uattrP = memPick->get_uattr(UATTRONCHIPMEM);
      if (memPick->get_functype() == MEMORY)
         cap = my_atoi(uattrP->get_value()) * 1024;
      else
         cap = my_atoi(uattrP->get_value());

      delete [] memPool;

      return memPick;
   }
// ------------------------------------------------------------
   bool AX_DsnC::fncblk_exists(AX_FuncBlkCP funcP)
   {
      AX_FuncBlkLCIter fIter;

      for (fIter = m_fncblkL.begin();
           fIter != m_fncblkL.end(); fIter++)
         if (&(*fIter) == funcP)
            return true;

      return false;
   }
// ------------------------------------------------------------
   AX_InterfaceLCIter AX_DsnC::create_new_intrfc()
   {
      AX_InterfaceC intrfc;

      m_intrfcL.push_back(intrfc);

      return(--m_intrfcL.end());
   }
// ------------------------------------------------------------
   AX_InterfaceLCIter AX_DsnC::create_new_intrfc(string & name)
   {
      if (name.empty())
         return m_intrfcL.end();

      AX_InterfaceLCIter iIter;
      for (iIter = m_intrfcL.begin();
           iIter != m_intrfcL.end(); iIter++)
         if ((*iIter).get_name() == name)
            return iIter;

      AX_InterfaceC nIntrfc;
      m_intrfcL.push_back(nIntrfc);
      iIter = --m_intrfcL.end();
      (*iIter).set_name(name);

      return(iIter);

   }
// ------------------------------------------------------------
   AX_InterfaceLCIter AX_DsnC::get_intrfc(string & name)
   {
      if (name.empty())
         return m_intrfcL.end();

      AX_InterfaceLCIter iIter;
      for (iIter = m_intrfcL.begin();
           iIter != m_intrfcL.end(); iIter++)
         if ((*iIter).get_name() == name)
            return iIter;

      return iIter;
   }
// ------------------------------------------------------------
   AX_InterfaceLCIter AX_DsnC::add_new_intrfc(AX_InterfaceC & intrfc)
   {
      AX_InterfaceLCIter iIter;

      for (iIter = m_intrfcL.begin();
           iIter != m_intrfcL.end(); iIter++)
          if (intrfc == (*iIter))
             return iIter;

      m_intrfcL.push_back(intrfc);

      return (--m_intrfcL.end());
   }
// ------------------------------------------------------------
   bool AX_DsnC::is_new_intrfc(string & name)
   {
      if (name.empty())
         return false;

      AX_InterfaceLCIter iIter;
      for (iIter = m_intrfcL.begin();
           iIter != m_intrfcL.end(); iIter++)
         if ((*iIter).get_name() == name)
            return false;

      return true;
   }
// ------------------------------------------------------------
   AX_InterfaceCP AX_DsnC::get_intrfc(int index)
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
   int AX_DsnC::get_intrfc_id(string name)
   {
      if (name.empty())
         return -1;

      AX_InterfaceLCIter iIter;

      for (iIter = m_intrfcL.begin();
           iIter != m_intrfcL.end(); iIter++)
          if ((*iIter).get_name() == name)
             return (*iIter).get_id();

      return -1;
   }
// ------------------------------------------------------------
   void AX_DsnC::remove_intrfc(AX_InterfaceCP intrfcP)
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
   AX_InterfaceLCIter AX_DsnC::delete_intrfc(AX_InterfaceLCIter & iter)
   {
      AX_InterfaceLCIter iIter;

      if (m_intrfcL.end() == iter)
         return iter;
      else if ((--m_intrfcL.end()) == iter)
      {
         AX_InterfaceC iTmp;
         (*iter) = iTmp;
         m_intrfcL.erase(iter);
         iIter = m_intrfcL.end();
      }
      else
      {
         iIter = ++iter;
         iter--;
         AX_InterfaceC iTmp;
         (*iter) = iTmp;
         m_intrfcL.erase(iter);
      }

      return iIter;
   }
// ------------------------------------------------------------
   void AX_DsnC::remove_all_intrfcs()
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
   AX_PlatformLCIter AX_DsnC::create_new_platfm()
   {
      AX_PlatformC platfm;

      m_platfmL.push_back(platfm);

      return(--m_platfmL.end());
   }
// ------------------------------------------------------------
   AX_PlatformLCIter AX_DsnC::create_new_platfm(string & name)
   {
      if (name.empty())
         return m_platfmL.end();

      AX_PlatformLCIter pIter;
      for (pIter = m_platfmL.begin();
           pIter != m_platfmL.end(); pIter++)
         if ((*pIter).get_name() == name)
            return pIter;

      AX_PlatformC nPlatfm;
      m_platfmL.push_back(nPlatfm);
      pIter = --m_platfmL.end();
      (*pIter).set_name(name);

      return(pIter);
   }
// ------------------------------------------------------------
   AX_PlatformLCIter AX_DsnC::get_platfm(string & name)
   {
      if (name.empty())
         return m_platfmL.end();

      AX_PlatformLCIter pIter;
      for (pIter = m_platfmL.begin();
           pIter != m_platfmL.end(); pIter++)
         if ((*pIter).get_name() == name)
            return pIter;

      return pIter;
   }
// ------------------------------------------------------------
   AX_PlatformLCIter AX_DsnC::add_new_platfm(AX_PlatformC & platfm)
   {
      AX_PlatformLCIter pIter;

      for (pIter = m_platfmL.begin();
           pIter != m_platfmL.end(); pIter++)
          if (platfm == (*pIter))
             return pIter;

      m_platfmL.push_back(platfm);

      return (--m_platfmL.end());
   }
// ------------------------------------------------------------
   bool AX_DsnC::is_new_platfm(string & name)
   {
      if (name.empty())
         return false;

      AX_PlatformLCIter pIter;
      for (pIter = m_platfmL.begin();
           pIter != m_platfmL.end(); pIter++)
         if ((*pIter).get_name() == name)
            return false;

      return true;
   }
// ------------------------------------------------------------
   AX_PlatformCP AX_DsnC::get_platfm(int index)
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
   int AX_DsnC::get_platfm_id(string name)
   {
      if (name.empty())
         return -1;

      AX_PlatformLCIter pIter;

      for (pIter = m_platfmL.begin();
           pIter != m_platfmL.end(); pIter++)
          if ((*pIter).get_name() == name)
             return (*pIter).get_id();

      return -1;
   }
// ------------------------------------------------------------
   void AX_DsnC::remove_platfm(AX_PlatformCP platfmP)
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
   AX_PlatformLCIter AX_DsnC::delete_platfm(AX_PlatformLCIter & iter)
   {
      AX_PlatformLCIter pIter;

      if (m_platfmL.end() == iter)
         return iter;
      else if ((--m_platfmL.end()) == iter)
      {
         m_platfmL.erase(iter);
         pIter = m_platfmL.end();
      }
      else
      {
         pIter = ++iter;
         iter--;
         m_platfmL.erase(iter);
      }

      return pIter;
   }
// ------------------------------------------------------------
   void AX_DsnC::remove_all_platfms()
   {
      AX_PlatformLCIter pIter;

      while ((pIter = m_platfmL.begin()) != m_platfmL.end())
         m_platfmL.erase(pIter);

      m_platfmL.clear();
   }
// ------------------------------------------------------------
   AX_CompPortLCIter AX_DsnC::create_new_compport()
   {
      AX_CompPortC compport;

      m_compportL.push_back(compport);

      return(--m_compportL.end());
   }
// ------------------------------------------------------------
   AX_CompPortLCIter AX_DsnC::create_new_compport(string & name)
   {
      if (name.empty())
         return m_compportL.end();

      AX_CompPortLCIter cIter;
      for (cIter = m_compportL.begin();
           cIter != m_compportL.end(); cIter++)
         if ((*cIter).get_name() == name)
            return cIter;

      AX_CompPortC nCompPort;
      m_compportL.push_back(nCompPort);
      cIter = --m_compportL.end();
      (*cIter).set_name(name);

      return(cIter);

   }
// ------------------------------------------------------------
   AX_CompPortLCIter AX_DsnC::get_compport(string & name)
   {
      if (name.empty())
         return m_compportL.end();

      AX_CompPortLCIter cIter;
      for (cIter = m_compportL.begin();
           cIter != m_compportL.end(); cIter++)
         if ((*cIter).get_name() == name)
            return cIter;

      return cIter;
   }
// ------------------------------------------------------------
   AX_CompPortLCIter AX_DsnC::add_new_compport(AX_CompPortC & compport)
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
   bool AX_DsnC::is_new_compport(string & name)
   {
      if (name.empty())
         return false;

      AX_CompPortLCIter cIter;
      for (cIter = m_compportL.begin();
           cIter != m_compportL.end(); cIter++)
         if ((*cIter).get_name() == name)
            return false;

      return true;
   }
// ------------------------------------------------------------
   AX_CompPortCP AX_DsnC::get_compport(int index)
   {
      AX_CompPortCP compportP;

      if ((index < 0)||((unsigned int)index > m_compportL.size()-1))
         compportP = NULL;
      else
      {
         AX_CompPortLCIter cIter;
         int i;

         cIter = m_compportL.begin();
         for(i = 0; i < index; i++) cIter++;

         compportP = &(*cIter);
      }

      return compportP;
   }
// ------------------------------------------------------------
   AX_CompPortLCIter AX_DsnC::get_compport(AX_FuncBlkCP funcP,
                                             string name)
   {
      if (name.empty())
         return m_compportL.end();

      AX_CompPortLCIter cIter;

      for (cIter = m_compportL.begin();
           cIter != m_compportL.end(); cIter++)
          if ((*cIter).get_comp() == AX_CompCP(funcP) &&
              (*cIter).get_name() == name)
             return cIter;

      return cIter;
   }
// ------------------------------------------------------------
   void AX_DsnC::remove_compport(AX_CompPortCP compportP)
   {
       AX_CompPortLCIter cIter;

       for (cIter = m_compportL.begin();
            cIter != m_compportL.end(); cIter++)
           if (&(*cIter) == compportP)
           {
               m_compportL.erase(cIter);
               return;
           }
       return;
   }
// ------------------------------------------------------------
   AX_CompPortLCIter AX_DsnC::delete_compport(AX_CompPortLCIter & iter)
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
   void AX_DsnC::remove_all_compports()
   {
      AX_CompPortLCIter cIter;

      AX_CompPortC pTmp;
      while ((cIter = m_compportL.begin()) != m_compportL.end())
      {
         (*cIter) = pTmp;
         m_compportL.erase(cIter);
      }

      m_compportL.clear();
   }
// ------------------------------------------------------------
   AX_DsnSpecLCIter AX_DsnC::create_new_dsnspec()
   {
      AX_DsnSpecC dsnspec;

      m_dsnspecL.push_back(dsnspec);

      return(--m_dsnspecL.end());
   }
// ------------------------------------------------------------
   AX_DsnSpecLCIter AX_DsnC::create_new_dsnspec(string & name)
   {
      if (name.empty())
         return m_dsnspecL.end();

      AX_DsnSpecLCIter dIter;
      for (dIter = m_dsnspecL.begin();
           dIter != m_dsnspecL.end(); dIter++)
         if ((*dIter).get_name() == name)
            return dIter;

      AX_DsnSpecC nDsnInf;
      m_dsnspecL.push_back(nDsnInf);
      dIter = --m_dsnspecL.end();
      (*dIter).set_name(name);

      return(dIter);

   }
// ------------------------------------------------------------
   AX_DsnSpecLCIter AX_DsnC::get_dsnspec(string & name)
   {
      if (name.empty())
         return m_dsnspecL.end();

      AX_DsnSpecLCIter dIter;
      for (dIter = m_dsnspecL.begin();
           dIter != m_dsnspecL.end(); dIter++)
         if ((*dIter).get_name() == name)
            return dIter;

      return dIter;
   }
// ------------------------------------------------------------
   AX_DsnSpecLCIter AX_DsnC::add_new_dsnspec(AX_DsnSpecC & dsnspec)
   {
      AX_DsnSpecLCIter dIter;

      for (dIter = m_dsnspecL.begin();
           dIter != m_dsnspecL.end(); dIter++)
          if (dsnspec == (*dIter))
             return dIter;

      m_dsnspecL.push_back(dsnspec);

      return (--m_dsnspecL.end());
   }
// ------------------------------------------------------------
   bool AX_DsnC::is_new_dsnspec(string & name)
   {
      if (name.empty())
         return false;

      AX_DsnSpecLCIter dIter;
      for (dIter = m_dsnspecL.begin();
           dIter != m_dsnspecL.end(); dIter++)
         if ((*dIter).get_name() == name)
            return false;

      return true;
   }
// ------------------------------------------------------------
   AX_DsnSpecCP AX_DsnC::get_dsnspec(int index)
   {
      AX_DsnSpecCP dsnspecP;

      if ((index < 0)||((unsigned int)index > m_dsnspecL.size()-1))
         dsnspecP = NULL;
      else
      {
         AX_DsnSpecLCIter dIter;
         int i;

         dIter = m_dsnspecL.begin();
         for(i = 0; i < index; i++) dIter++;

         dsnspecP = &(*dIter);
      }

      return dsnspecP;
   }
// ------------------------------------------------------------
   AX_DsnSpecLCIter AX_DsnC::get_dsnspec(string name)
   {
      if (name.empty())
         return m_dsnspecL.end();

      AX_DsnSpecLCIter dIter;

      for (dIter = m_dsnspecL.begin();
           dIter != m_dsnspecL.end(); dIter++)
          if ((*dIter).get_name() == name)
             return dIter;

      return dIter;
   }
// ------------------------------------------------------------
   void AX_DsnC::remove_dsnspec(AX_DsnSpecCP dsnspecP)
   {
       AX_DsnSpecLCIter dIter;

       for (dIter = m_dsnspecL.begin();
            dIter != m_dsnspecL.end(); dIter++)
           if (&(*dIter) == dsnspecP)
           {
               m_dsnspecL.erase(dIter);
               return;
           }
       return;
   }
// ------------------------------------------------------------
   AX_DsnSpecLCIter AX_DsnC::delete_dsnspec(AX_DsnSpecLCIter & iter)
   {
      AX_DsnSpecLCIter dIter;

      if (m_dsnspecL.end() == iter)
         return iter;
      else if ((--m_dsnspecL.end()) == iter)
      {
         AX_DsnSpecC dTmp;
         (*iter) = dTmp;
         m_dsnspecL.erase(iter);
         dIter = m_dsnspecL.end();
      }
      else
      {
         dIter = ++iter;
         iter--;
         AX_DsnSpecC dTmp;
         (*iter) = dTmp;
         m_dsnspecL.erase(iter);
      }

      return dIter;
   }
// ------------------------------------------------------------
   void AX_DsnC::remove_all_dsnspecs()
   {
      AX_DsnSpecLCIter dIter;

      AX_DsnSpecC dTmp;
      while ((dIter = m_dsnspecL.begin()) != m_dsnspecL.end())
      {
         (*dIter) = dTmp;
         m_dsnspecL.erase(dIter);
      }

      m_dsnspecL.clear();
   }
// ------------------------------------------------------------
   AX_ArchGraphLCIter AX_DsnC::create_new_ag()
   {
      AX_ArchGraphC ag;

      m_agL.push_back(ag);

      return(--m_agL.end());
   }
// ------------------------------------------------------------
   AX_ArchGraphLCIter AX_DsnC::create_new_ag(string & name)
   {
      if (name.empty())
         return m_agL.end();

      AX_ArchGraphLCIter dIter;
      for (dIter = m_agL.begin();
           dIter != m_agL.end(); dIter++)
         if ((*dIter).get_name() == name)
            return dIter;

      AX_ArchGraphC nDsnInf;
      m_agL.push_back(nDsnInf);
      dIter = --m_agL.end();
      (*dIter).set_name(name);

      return(dIter);

   }
// ------------------------------------------------------------
   AX_ArchGraphLCIter AX_DsnC::get_ag(string & name)
   {
      if (name.empty())
         return m_agL.end();

      AX_ArchGraphLCIter dIter;
      for (dIter = m_agL.begin();
           dIter != m_agL.end(); dIter++)
         if ((*dIter).get_name() == name)
            return dIter;

      return dIter;
   }
// ------------------------------------------------------------
   AX_ArchGraphLCIter AX_DsnC::add_new_ag(AX_ArchGraphC & ag)
   {
      AX_ArchGraphLCIter dIter;

      for (dIter = m_agL.begin();
           dIter != m_agL.end(); dIter++)
          if (ag.get_name() == (*dIter).get_name())
             return dIter;

      m_agL.push_back(ag);

      return (--m_agL.end());
   }
// ------------------------------------------------------------
   AX_ArchGraphLCIter AX_DsnC::add_ag(AX_ArchGraphC & ag)
   {
      m_agL.push_back(ag);

      return (--m_agL.end());
   }
// ------------------------------------------------------------
   bool AX_DsnC::is_new_ag(string & name)
   {
      if (name.empty())
         return false;

      AX_ArchGraphLCIter dIter;
      for (dIter = m_agL.begin();
           dIter != m_agL.end(); dIter++)
         if ((*dIter).get_name() == name)
            return false;

      return true;
   }
// ------------------------------------------------------------
   AX_ArchGraphCP AX_DsnC::get_ag(int index)
   {
      AX_ArchGraphCP agP;

      if ((index < 0)||((unsigned int)index > m_agL.size()-1))
         agP = NULL;
      else
      {
         AX_ArchGraphLCIter dIter;
         int i;

         dIter = m_agL.begin();
         for(i = 0; i < index; i++) dIter++;

         agP = &(*dIter);
      }

      return agP;
   }
// ------------------------------------------------------------
   void AX_DsnC::remove_ag(AX_ArchGraphCP agP)
   {
       AX_ArchGraphLCIter dIter;

       for (dIter = m_agL.begin();
            dIter != m_agL.end(); dIter++)
           if (&(*dIter) == agP)
           {
               m_agL.erase(dIter);
               return;
           }
       return;
   }
// ------------------------------------------------------------
   void AX_DsnC::remove_ag(string name)
   {
       AX_ArchGraphLCIter dIter;

       for (dIter = m_agL.begin();
            dIter != m_agL.end(); dIter++)
           if ((*dIter).get_name() == name)
           {
               m_agL.erase(dIter);
               return;
           }
       return;
   }
// ------------------------------------------------------------
   AX_ArchGraphLCIter AX_DsnC::delete_ag(AX_ArchGraphLCIter & iter)
   {
      AX_ArchGraphLCIter dIter;

      if (m_agL.end() == iter)
         return iter;
      else if ((--m_agL.end()) == iter)
      {
         //string sTmp = "";
         //(*iter).set_name(sTmp);
         m_agL.erase(iter);
         dIter = m_agL.end();
      }
      else
      {
         dIter = ++iter;
         iter--;
         //string sTmp = "";
         //(*iter).set_name(sTmp);
         m_agL.erase(iter);
      }

      return dIter;
   }
// ------------------------------------------------------------
   AX_ArchGraphLCIter AX_DsnC::delete_ag(AX_ArchGraphCP agP)
   {
      AX_ArchGraphLCIter dIter, aIter;

      if (agP == NULL)
         return m_agL.end();

      for (aIter = m_agL.begin(); aIter != m_agL.end(); aIter++)
      {
          if (&(*aIter) == agP)
          {
             if ((--m_agL.end()) == aIter)
             {
                //string sTmp = "";
                //(*aIter).set_name(sTmp);
                m_agL.erase(aIter);
                dIter = m_agL.end();
             }
             else
             {
                dIter = ++aIter;
                aIter--;
                //string sTmp = "";
                //(*aIter).set_name(sTmp);
                m_agL.erase(aIter);
             }
             break;
          }
      }

      return dIter;
   }
// ------------------------------------------------------------
   AX_ArchGraphLCIter AX_DsnC::delete_ag(string name)
   {
      AX_ArchGraphLCIter dIter, aIter;

      if (name.empty())
         return m_agL.end();

      for (aIter = m_agL.begin(); aIter != m_agL.end(); aIter++)
      {
          if ((*aIter).get_name() == name)
          {
             if ((--m_agL.end()) == aIter)
             {
                string sTmp = "";
                (*aIter).set_name(sTmp);
                m_agL.erase(aIter);
                dIter = m_agL.end();
             }
             else
             {
                dIter = ++aIter;
                aIter--;
                string sTmp = "";
                (*aIter).set_name(sTmp);
                m_agL.erase(aIter);
             }
             break;
          }
      }

      return dIter;
   }
// ------------------------------------------------------------
   void AX_DsnC::remove_all_ags()
   {
      AX_ArchGraphLCIter dIter;

      string sTmp = "";
      while ((dIter = m_agL.begin()) != m_agL.end())
      {
         (*dIter).set_name(sTmp);
         m_agL.erase(dIter);
      }

      m_agL.clear();
   }
// ------------------------------------------------------------
   bool AX_DsnC::check_ags()
   {
      AX_ArchGraphLCIter agI;

      for (agI = m_agL.begin();
           agI != m_agL.end(); agI++)
      {
         // check tasks
         AX_TaskLCIter taskI;
         AX_FuncBlkLCIter funcI;
         for (taskI = m_tg.get2_first_task();
              taskI != m_tg.task_list_end(); taskI++)
         {
            AX_FuncBlkCP firstFuncP = NULL;;
            bool found = false, first = false;
            for (funcI = (*agI).get2_first_fncblk();
                 funcI != (*agI).fncblk_list_end(); funcI++)
            {
	       if ((*funcI).get_functype() == PROCESSOR)
		  continue;
               if ((*funcI).is_mapped_task(&(*taskI)))
               {
                  if (found && first)
                  {
                     cerr << (*agI).get_name() << ": "
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
                cerr << (*agI).get_name() << ": "
                     << "Cannot find mapping for "
                     << (*taskI).get_name() << endl;
            }
         } // end task for

         // check conns
         AX_ConnLCIter connI;
         for (connI = m_tg.get2_first_conn();
              connI != m_tg.conn_list_end(); connI++)
         {
            AX_BusLCIter busI;
            AX_BusCP firstBusP = NULL;;
            bool found = false, first = false;

            for (busI = (*agI).get2_first_bus();
                 busI != (*agI).bus_list_end(); busI++)
            {
               if ((*busI).is_mapped_conn(&(*connI)))
               {
                  if (found && first)
                  {
                     cerr << (*agI).get_name() << ": ";
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
                  srcFuncP = (*agI).get_fncblk_by_task(srcTaskP);
                  sinkFuncP = (*agI).get_fncblk_by_task(sinkTaskP);
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
                  if ((*srcPI) == (*sinkPI))
                     continue;
               }
               cerr << (*agI).get_name() << ": "
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
            AX_BusCP fBusP = (*agI).get_bus_by_conn(&(*connI));
            if ((*connI).get_src_task() != NULL)
            {
               AX_FuncBlkCP fFuncP;
               AX_CompPortCP fPortP, sPortP;
               string pName = (*connI).get_src_port()->get_name();
               fFuncP = (*agI).get_fncblk_by_task((*connI).get_src_task());
               fPortP = fFuncP->get_outport(pName);
               if (fPortP == NULL)
                  cerr << (*agI).get_name() << ": "
                       << " cannot find port " << pName
                       << " in " << fFuncP->get_name();
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
                  cerr << (*agI).get_name() << ":"
                       << " cannot find matching connpnt "
                       << fFuncP->get_name() << "."
                       << fPortP->get_name() << " on bus "
                       << fBusP->get_id() << " for "
                       << (*connI).get_src_task()->get_name()
                       << "." << pName << endl;
            }
            if ((*connI).get_sink_task() != NULL)
            {
               AX_FuncBlkCP fFuncP;
               AX_CompPortCP fPortP, sPortP;
               string pName = (*connI).get_sink_port()->get_name();
               fFuncP = (*agI).get_fncblk_by_task((*connI).get_sink_task());
               fPortP = fFuncP->get_inport(pName);
               if (fPortP == NULL)
                  cerr << (*agI).get_name() << ": "
                       << " cannot find port " << pName
                       << " in " << fFuncP->get_name();
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
                  cerr << (*agI).get_name() << ": "
                       << " cannot find matching connpnt "
                       << fFuncP->get_name() << "."
                       << fPortP->get_name() << " on bus "
                       << fBusP->get_id() << " for "
                       << (*connI).get_sink_task()->get_name()
                       << "." << pName << endl;
            }
         } // end conn for

         { // check for one or none connection buses
            AX_BusLCIter busI;
            for (busI = (*agI).get2_first_bus();
                 busI != (*agI).bus_list_end(); busI++)
            {
               if ((*busI).get_no_connpnts() <= 1)
               {
                  cerr << (*agI).get_name()
                       << ": Useless bus found:\n";
                  (*busI).dump();
               }
               if ((*busI).get_intrfc()->get_intrtype() != INTRFC_WIRE &&
                   (*busI).get_intrfc()->get_intrtype() != INTRFC_BUS)
               {
                  cerr << (*agI).get_name()
                       << ": Incorrect interface type found:\n";
                  (*busI).dump();
               }
               if ((*busI).get_intrfc()->get_intrtype() != INTRFC_WIRE &&
                   (*busI).get_intrfc()->get_intrtype() != INTRFC_BUS)
               {
                  cerr << (*agI).get_name()
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
                     cerr << (*agI).get_name()
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
                  {
                     if ((*nxtPntI).get_ag() == (*conpntI).get_ag() &&
                         (*nxtPntI).get_comp() == (*conpntI).get_comp() &&
                         (*nxtPntI).get_port() == (*conpntI).get_port())
                     {
                        cerr << (*agI).get_name()
                             << ": bus " << (*busI).get_id()
                             << " has duplicate connpnts ";
                        if ((*conpntI).get_ag() != NULL)
                           cerr << (*conpntI).get_ag()->get_name();
                        else
                           cerr << (*conpntI).get_comp()->get_name();
                        cerr << "."
                             << (*conpntI).get_port()->get_name()
                             << endl;
                     }
                  }
               }
            }
         } // end checking useless buses

         // checking compport's bus list
         AX_CompPortLCIter portI;
         for (portI = (*agI).get2_first_compport();
              portI != (*agI).compport_list_end(); portI++)
         {
            AX_BusPLCIter busPI;
            bool errPrted = false;
            for (busPI = (*portI).get2_first_bus();
                 busPI != (*portI).bus_list_end(); busPI++)
            {
               if ((*busPI)->get_intrfc()->get_intrtype() != INTRFC_WIRE &&
                   (*busPI)->get_intrfc()->get_intrtype() != INTRFC_BUS)
               {
                  cerr << (*agI).get_name()
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
                  cerr << (*agI).get_name() 
                       << ": multi-bus: ";
                  for (int i = 0; i < (*portI).get_no_buses(); i++)
                  {
                     AX_InterfaceCP intrP;
                     intrP = (*portI).get_bus(i)->get_intrfc();
                     cerr << intrP->get_name()
                          << " ";
                  }
                  cerr << endl;
                  errPrted = true;
               }

               AX_InterfaceLCIter intrfcI;
               bool found = false;
               for (intrfcI = (*agI).get2_first_intrfc();
                    intrfcI != (*agI).intrfc_list_end(); intrfcI++)
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
                  cerr << (*agI).get_name() << ": ";
                  if (fncblkP == NULL)
                     cerr << (*agI).get_name();
                  else
                     cerr << fncblkP->get_name();
                  cerr << "." << (*portI).get_name()
                       << "'s bus " << (*busPI)->get_id()
                       << "'s interface "
                       << (*busPI)->get_intrfc()->get_name()
                       << " does not belong to this graph\n";
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
                  cerr << (*agI).get_name() << ": ";
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
                  cerr << (*agI).get_name()
                       << ": Incorrect taskref in "
                       << checkP->get_name() << "."
                       << (*portI).get_name() << endl;
            }
            if (AX_FuncBlkCP((*portI).get_comp())->get_type() == SOFTWARE)
               continue;
            if ((*portI).get_no_intrfcs() == 0)
            {
               cerr << (*agI).get_name()
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
               for (swPortI = (*agI).get2_first_compport();
                    swPortI != (*agI).compport_list_end(); swPortI++)
               {
                  if ((*swPortI).get_sw_port() == (*portPI))
                     refCnt++;
               }
               if (refCnt != (*portPI)->get_refCnt())
               {
                  cerr << (*agI).get_name() << ": "
                       << procP->get_name() << "."
                       << (*portPI)->get_name()
                       << "'s refCnt " << (*portPI)->get_refCnt()
                       << " does not match real refCnt " << refCnt << endl;
               }
            }
         }

         // check the fncblkPL depPL and refPL links
         for (funcI = (*agI).get2_first_fncblk();
              funcI != (*agI).fncblk_list_end(); funcI++)
         {
            AX_FuncBlkPLCIter funcPI;
            if ((*funcI).get_functype() != PROCESSOR)
               continue;
            if ((*funcI).get_no_deps() > 1)
            {
               cerr << (*agI).get_name() << ": ";
               cerr << (*funcI).get_name()
                    << " has more than one entry in depPL\n";
            }
            if ((*funcI).get_no_refs() <= 0)
            {
               cerr << (*agI).get_name() << ": ";
               cerr << (*funcI).get_name()
                    << " has no fncblk in refPL\n";
            }
            for (funcPI = (*funcI).get2_first_dep();
                 funcPI != (*funcI).dep_list_end(); funcPI++)
            {
               if ((*funcPI)->get_functype() != MEMORY &&
                   (*funcPI)->get_functype() != MEMORY_CORE)
               {
                  cerr << (*agI).get_name() << ": ";
                  cerr << (*funcI).get_name()
                       << " does not have memory in depPL\n";
               }
               // MEMORY & MEMORY_CORE may have multiple dependents
               if ((*funcPI)->get_functype() == MEMORY ||
                   (*funcPI)->get_functype() == MEMORY_CORE) {
                  AX_FuncBlkPLCIter depPI;
                  bool depFlag = false;
                  for (depPI = (*funcPI)->get2_first_dep();
                       depPI != (*funcPI)->dep_list_end(); depPI++) {
                     if ((*depPI) == &(*funcI))
                        depFlag = true;
                  }
                  if (!depFlag) {
                     cerr << (*agI).get_name() << ": ";
                     cerr << (*funcI).get_name()
                          << " has "
                          << (*depPI)->get_name()
                          << " in depPL but not vise versa\n";
                  }
               }
            }
         }
         for (funcI = (*agI).get2_first_fncblk();
              funcI != (*agI).fncblk_list_end(); funcI++)
         {
            AX_FuncBlkPLCIter funcPI;
            if ((*funcI).get_functype() != MEMORY &&
                (*funcI).get_functype() != MEMORY_CORE)
               continue;
            if ((*funcI).get_no_deps() > 1)
            {
               cerr << (*agI).get_name() << ": ";
               cerr << (*funcI).get_name()
                    << " has more than one entry in depPL\n";
            }
            if ((*funcI).get_no_refs() <= 0)
            {
               cerr << (*agI).get_name() << ": ";
               cerr << (*funcI).get_name()
                    << " has no fncblk in refPL\n";
            }
            for (funcPI = (*funcI).get2_first_dep();
                 funcPI != (*funcI).dep_list_end(); funcPI++)
            {
               if ((*funcPI)->get_functype() != PROCESSOR)
               {
                  cerr << (*agI).get_name() << ": ";
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
                        cerr << (*agI).get_name() << ": ";
                        cerr << (*funcI).get_name()
                             << " has "
                             << (*depPI)->get_name()
                             << " in depPL but not vise versa\n";
                     }
               }
            }
         }
         for (funcI = (*agI).get2_first_fncblk();
              funcI != (*agI).fncblk_list_end(); funcI++)
         {
            AX_FuncBlkPLCIter funcPI;
            if ((*funcI).get_functype() != FUNCTION)
               continue;
            if ((*funcI).get_no_taskrefs() <= 0)
            {
               cerr << (*agI).get_name() << ": ";
               cerr << (*funcI).get_name()
                    << " has no task ref to it\n";
            }
            if ((*funcI).get_type() == SOFTWARE)
               if ((*funcI).get_no_deps() <= 0)
               {
                  cerr << (*agI).get_name() << ": ";
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
                  cerr << (*agI).get_name() << ": ";
                  cerr << (*funcI).get_name()
                       << " has " << (*funcPI)->get_name()
                       << " in depPL but not vise versa in refPL\n";
               }
            }
         }

         // check port for bus links
         for (funcI = (*agI).get2_first_fncblk();
              funcI != (*agI).fncblk_list_end(); funcI++)
         {
            AX_CompPortPLCIter portPI;
            for (portPI = (*funcI).get2_first_inport();
                 portPI != (*funcI).inport_list_end(); portPI++)
            {
               AX_BusPLCIter busPI;
               for (busPI = (*portPI)->get2_first_bus();
                    busPI != (*portPI)->bus_list_end(); busPI++)
               {
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
                      cerr << (*agI).get_name() << ": ";
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
                      cerr << (*agI).get_name() << ": ";
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
                      cerr << (*agI).get_name() << ": ";
                      cerr << "Bus ID " << (*busPI)->get_id();
                         cerr << " does not have connpnt "
                           << (*funcI).get_name() << "."
                           << (*portPI)->get_name() << endl;
                   }
               } // end busPI for
            } // end ioport for
         } // end funcI loop
      } // end agI loop

      return true;
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Structure List Manipulations


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Special Functions


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Miscellaneous Functions
// ------------------------------------------------------------
   void AX_DsnC::clean_up()
   {
      m_name.clear();
      m_id = -1;
      m_temp = -1;
   }
// ------------------------------------------------------------
   void AX_DsnC::dump()
   {
      cerr << "Design : " << m_name << " .ID: " << m_id << endl;
      cerr << endl;

      cerr << "Design Specs : " << endl;
      AX_DsnSpecLCIter dIter;
      for (dIter = m_dsnspecL.begin();
           dIter != m_dsnspecL.end(); dIter++)
          (*dIter).dump();

      cerr << "Component List : " << endl;
      AX_FuncBlkLCIter fIter;
      for (fIter = m_fncblkL.begin();
           fIter != m_fncblkL.end(); fIter++)
         (*fIter).dump();

      cerr << "Platform List : " << endl;
      AX_PlatformLCIter pIter;
      for (pIter = m_platfmL.begin();
           pIter != m_platfmL.end(); pIter++)
         (*pIter).dump();

      cerr << "Interface List : " << endl;
      AX_InterfaceLCIter iIter;
      for (iIter = m_intrfcL.begin();
           iIter != m_intrfcL.end(); iIter++)
         (*iIter).dump();

      cerr << endl;
      cerr << "Task graph : " << endl;
      m_tg.dump();

      return;
   }
// ------------------------------------------------------------
   void AX_DsnC::write_agL_files_SystemC()
   {
      cout << "Generating SystemC files..." << endl;
      AX_ArchGraphLCIter agLI;
     
      for (agLI = m_agL.begin(); agLI != m_agL.end(); agLI++)
      {
         string folder("SystemC/");
         folder += agLI->get_name();
         folder += "/";
         agLI->write_SystemC( folder );
      }
   }
// ------------------------------------------------------------
   void AX_DsnC::write_agL_files()
   {
      AX_ArchGraphLCIter agLI;

      for (agLI = m_agL.begin(); agLI != m_agL.end(); agLI++)
      {
         string FileName;

         FileName = "data/";
         FileName += (*agLI).get_name();
         fstream agFile;
         agFile.open(FileName.c_str(), fstream::out|fstream::app);
         (*agLI).write(agFile);

         FileName = "datasmall/";
         FileName += (*agLI).get_name();
         fstream smallFile;
         smallFile.open(FileName.c_str(), fstream::out|fstream::app);
         (*agLI).write_small(smallFile);
      }
   }
// ------------------------------------------------------------
   void AX_DsnC::write(fstream & File)
   {
      File << "Design : " << m_name << " .ID: " << m_id << endl;
      File << endl;

      File << endl;
      File << "Design Specs : " << endl;
      AX_DsnSpecLCIter dIter;
      for (dIter = m_dsnspecL.begin();
           dIter != m_dsnspecL.end(); dIter++)
          (*dIter).write(File);

      File << endl;
      File << "Task graph : " << endl;
      m_tg.write(File);

      File << "Component List : " << endl;
      AX_FuncBlkLCIter fIter;
      for (fIter = m_fncblkL.begin();
           fIter != m_fncblkL.end(); fIter++)
         (*fIter).write(File);
   }
// ------------------------------------------------------------
   void AX_DsnC::write_bkui_dom_file()
   {
      string FileName;

      FileName = "data/";
      FileName += m_name;
      FileName += ".dom";

      fstream domFile;
      domFile.open(FileName.c_str(), fstream::out|fstream::app);
      domFile << "# feature\tnumber\tnickname\tmultiplicity\tmapping\n";
      domFile << "# -------\t------\t--------\t------------\t-------\n";
      int id = 0;

      // writing functions
      AX_FuncBlkLCIter funcI;
      for (funcI = m_fncblkL.begin();
           funcI != m_fncblkL.end(); funcI++)
      {
         if ((*funcI).get_functype() == MEMREQ)
            continue;
         (*funcI).set_id(id);
         domFile << (*funcI).get_name();
         domFile << "\t" << id++ << "\t";
         domFile << (*funcI).get_name();
         domFile << "\t1\t0:1:1\n";
      }

/*
      // writing platforms
      AX_PlatformLCIter platI;
      for (platI = m_platfmL.begin();
           platI != m_platfmL.end(); platI++)
      {
         (*platI).set_id(id);
         domFile << (char *)(*platI).get_name();
         domFile << "\t" << id++ << "\t";
         domFile << (char *)(*platI).get_name();
         domFile << "\t1\t0:1:1\n";
      }

      // writing interfaces
      AX_InterfaceLCIter intrI;
      for (intrI = m_intrfcL.begin();
           intrI != m_intrfcL.end(); intrI++)
      {
         (*intrI).set_id(id);
         domFile << (*intrI).get_name();
         domFile << "\t" << id++ << "\t";
         domFile << (*intrI).get_name();
         domFile << "\t1\t0:1:1\n";
      }
*/

      domFile.close();
   }
// ------------------------------------------------------------
   void AX_DsnC::write_bkui_data_file()
   {
      string FileName;

      FileName = "data/";
      FileName += m_name;
      FileName += ".data";

      fstream dataFile;
      dataFile.open(FileName.c_str(), fstream::out|fstream::app);
      dataFile.setf(ios::fixed,ios::floatfield);
      dataFile.precision(16);

      int *cntAry;
      int arySize = get_no_fncblks();
      cntAry = new int[arySize];

      AX_ArchGraphLCIter agI;
      for (agI = m_agL.begin(); agI != m_agL.end(); agI++)
      {
         dataFile << (*agI).get_name();
         dataFile << ", " << (*agI).get_fitness();

         for (int i = 0; i < arySize; i++)
            cntAry[i] = 0;

         AX_FuncBlkLCIter funcI;
         for (funcI = (*agI).get2_first_fncblk();
              funcI != (*agI).fncblk_list_end(); funcI++)
         {
            string orgName = trim_id((*funcI).get_name());
            int curID = get_fncblk_id(orgName);
            cntAry[curID]++;
         }

         for (int i = 0; i < arySize; i++)
            if (cntAry[i] > 0)
               dataFile << ", <" << i << "," << cntAry[i] << ">";
         dataFile << endl;
      } // end ag loop

      // delete [] cntAry;
   }
// ------------------------------------------------------------
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~End of Function Definitions
