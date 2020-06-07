// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// platform.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <iostream>
#include "platform.h"
using namespace std;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructor
// ------------------------------------------------------------
   AX_PlatformC::AX_PlatformC()
   {
      this->set_category(PLATFORM);
   }
// ------------------------------------------------------------
   AX_PlatformC::AX_PlatformC(const AX_PlatformC & other)
   {
      copy(other);
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Destructor
// ------------------------------------------------------------
   AX_PlatformC::~AX_PlatformC()
   {
      this->AX_CompC::~AX_CompC();
      remove_all_subpltfms();
      remove_all_impl_subpltfms();
      remove_all_intrfcs();
      remove_all_impl_intrfcs();
      remove_all_uattrs();
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Assignator & Comparators
// ------------------------------------------------------------
   AX_PlatformC & AX_PlatformC::operator= (const AX_PlatformC & other)
   {
      if (&other == this)
         return(*this);

      copy(other);

      return(*this);
   }
// ------------------------------------------------------------
   void AX_PlatformC::copy(const AX_PlatformC & other)
   {
      AX_CompC::copy(other);
      AX_PlatformPLCCnstIter pIter;
      AX_InterfacePLCCnstIter iIter;

      m_subpltfmPL.clear();
      for (pIter = other.m_subpltfmPL.begin();
           pIter != other.m_subpltfmPL.end(); pIter++)
          m_subpltfmPL.push_back(*pIter);

      m_impl_subpltfmPL.clear();
      for (pIter = other.m_impl_subpltfmPL.begin();
           pIter != other.m_impl_subpltfmPL.end(); pIter++)
          m_impl_subpltfmPL.push_back(*pIter);

      m_intrfcPL.clear();
      for (iIter = other.m_intrfcPL.begin();
           iIter != other.m_intrfcPL.end(); iIter++)
          m_intrfcPL.push_back(*iIter);

      m_impl_intrfcPL.clear();
      for (iIter = other.m_impl_intrfcPL.begin();
           iIter != other.m_impl_intrfcPL.end(); iIter++)
          m_impl_intrfcPL.push_back(*iIter);

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
   void AX_PlatformC::copyBasic(const AX_PlatformC & other)
   {
      AX_CompC::copy(other);

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
   bool AX_PlatformC::operator== (const AX_PlatformC & other)
   {
      return (AX_CompC::operator== (other));
   }
// ------------------------------------------------------------
   bool AX_PlatformC::operator== (const string name)
   {
      return (AX_CompC::operator== (name));
   }
// ------------------------------------------------------------
   bool AX_PlatformC::operator!= (const string name)
   {
      return (!(*this == name));
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Pointer List Manipulations
// ------------------------------------------------------------
   AX_PlatformPLCIter AX_PlatformC::add_new_subpltfm(AX_PlatformCP subpltfmP)
   {
      AX_PlatformPLCIter pIter;

      for (pIter = m_subpltfmPL.begin();
           pIter != m_subpltfmPL.end(); pIter++)
          if (subpltfmP == (*pIter))
             return pIter;

      m_subpltfmPL.push_back(subpltfmP);
      pIter = --m_subpltfmPL.end();

      return pIter;
   }
// ------------------------------------------------------------
   AX_PlatformCP AX_PlatformC::get_subpltfm(int index)
   {
      AX_PlatformCP subpltfmP;

      if ((index < 0)||((unsigned int)index > m_subpltfmPL.size()-1))
         subpltfmP = NULL;
      else
      {
         AX_PlatformPLCIter pIter;
         int i;

         pIter = m_subpltfmPL.begin();
         for(i = 0; i < index; i++) pIter++;

         subpltfmP = (*pIter);
      }

      return subpltfmP;
   }
// ------------------------------------------------------------
   void AX_PlatformC::remove_subpltfm(AX_PlatformCP subpltfmP)
   {
      AX_PlatformPLCIter pIter;

      for (pIter = m_subpltfmPL.begin();
           pIter != m_subpltfmPL.end(); pIter++)
          if ((*pIter) == subpltfmP)
          {
             m_subpltfmPL.erase(pIter);
             return;
          }
      return;
   }
// ------------------------------------------------------------
   AX_PlatformPLCIter AX_PlatformC::delete_subpltfm(AX_PlatformPLCIter & iter)
   {
      AX_PlatformPLCIter retIter;

      if (m_subpltfmPL.end() == iter)
         return iter;
      else if ((--m_subpltfmPL.end()) == iter)
      {
         m_subpltfmPL.erase(iter);
         retIter = m_subpltfmPL.end();
      }
      else
      {
         retIter = ++iter;
         iter--;
         m_subpltfmPL.erase(iter);
      }

      return retIter;
   }
// ------------------------------------------------------------
   AX_PlatformPLCIter AX_PlatformC::add_new_impl_subpltfm(AX_PlatformCP subpltfmP)
   {
      AX_PlatformPLCIter pIter;

      for (pIter = m_impl_subpltfmPL.begin();
           pIter != m_impl_subpltfmPL.end(); pIter++)
          if (subpltfmP == (*pIter))
             return pIter;

      m_impl_subpltfmPL.push_back(subpltfmP);
      pIter = --m_impl_subpltfmPL.end();

      return pIter;
   }
// ------------------------------------------------------------
   AX_PlatformCP AX_PlatformC::get_impl_subpltfm(int index)
   {
      AX_PlatformCP subpltfmP;

      if ((index < 0)||((unsigned int)index > m_impl_subpltfmPL.size()-1))
         subpltfmP = NULL;
      else
      {
         AX_PlatformPLCIter pIter;
         int i;

         pIter = m_impl_subpltfmPL.begin();
         for(i = 0; i < index; i++) pIter++;

         subpltfmP = (*pIter);
      }

      return subpltfmP;
   }
// ------------------------------------------------------------
   void AX_PlatformC::remove_impl_subpltfm(AX_PlatformCP subpltfmP)
   {
      AX_PlatformPLCIter pIter;

      for (pIter = m_impl_subpltfmPL.begin();
           pIter != m_impl_subpltfmPL.end(); pIter++)
          if ((*pIter) == subpltfmP)
          {
             m_impl_subpltfmPL.erase(pIter);
             return;
          }
      return;
   }
// ------------------------------------------------------------
   AX_PlatformPLCIter AX_PlatformC::delete_impl_subpltfm(AX_PlatformPLCIter & iter)
   {
      AX_PlatformPLCIter retIter;

      if (m_impl_subpltfmPL.end() == iter)
         return iter;
      else if ((--m_impl_subpltfmPL.end()) == iter)
      {
         m_impl_subpltfmPL.erase(iter);
         retIter = m_impl_subpltfmPL.end();
      }
      else
      {
         retIter = ++iter;
         iter--;
         m_impl_subpltfmPL.erase(iter);
      }

      return retIter;
   }
// ------------------------------------------------------------
   AX_InterfacePLCIter AX_PlatformC::add_new_intrfc(AX_InterfaceCP intrfcP)
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
   AX_InterfaceCP AX_PlatformC::get_intrfc(int index)
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
   void AX_PlatformC::remove_intrfc(AX_InterfaceCP intrfcP)
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
   AX_InterfacePLCIter AX_PlatformC::delete_intrfc(AX_InterfacePLCIter & iter)
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
   AX_InterfacePLCIter AX_PlatformC::add_new_impl_intrfc(AX_InterfaceCP intrfcP)
   {
      AX_InterfacePLCIter iIter;

      for (iIter = m_impl_intrfcPL.begin();
           iIter != m_impl_intrfcPL.end(); iIter++)
          if (intrfcP == (*iIter))
             return iIter;

      m_impl_intrfcPL.push_back(intrfcP);
      iIter = --m_impl_intrfcPL.end();

      return iIter;
   }
// ------------------------------------------------------------
   AX_InterfaceCP AX_PlatformC::get_impl_intrfc(int index)
   {
      AX_InterfaceCP intrfcP;

      if ((index < 0)||((unsigned int)index > m_impl_intrfcPL.size()-1))
         intrfcP = NULL;
      else
      {
         AX_InterfacePLCIter iIter;
         int i;

         iIter = m_impl_intrfcPL.begin();
         for(i = 0; i < index; i++) iIter++;

         intrfcP = (*iIter);
      }

      return intrfcP;
   }
// ------------------------------------------------------------
   void AX_PlatformC::remove_impl_intrfc(AX_InterfaceCP intrfcP)
   {
      AX_InterfacePLCIter iIter;

      for (iIter = m_impl_intrfcPL.begin();
           iIter != m_impl_intrfcPL.end(); iIter++)
          if ((*iIter) == intrfcP)
          {
             m_impl_intrfcPL.erase(iIter);
             return;
          }
      return;
   }
// ------------------------------------------------------------
   AX_InterfacePLCIter AX_PlatformC::delete_impl_intrfc(AX_InterfacePLCIter & iter)
   {
      AX_InterfacePLCIter retIter;

      if (m_impl_intrfcPL.end() == iter)
         return iter;
      else if ((--m_impl_intrfcPL.end()) == iter)
      {
         m_impl_intrfcPL.erase(iter);
         retIter = m_impl_intrfcPL.end();
      }
      else
      {
         retIter = ++iter;
         iter--;
         m_impl_intrfcPL.erase(iter);
      }

      return retIter;
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Instance List Manipulations
// ------------------------------------------------------------
   AX_UAttrLCIter AX_PlatformC::add_new_uattr(AX_UAttrC & uattr)
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
   AX_UAttrCP AX_PlatformC::get_uattr(int index)
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
   void AX_PlatformC::remove_uattr(AX_UAttrCP uattrP)
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
   AX_UAttrLCIter AX_PlatformC::delete_uattr(AX_UAttrLCIter & iter)
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
   void AX_PlatformC::remove_all_uattrs()
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

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Structure List Manipulations


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Special Functions


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Miscellaneous Functions
// ------------------------------------------------------------
   void AX_PlatformC::clean_up()
   {

   }
// ------------------------------------------------------------
   void AX_PlatformC::dump()
   {
      AX_CompC::dump();
      AX_PlatformPLCIter pIter;
      AX_InterfacePLCIter iIter;
      AX_UAttrLCIter uIter;

      // dumping sub platforms
      if (m_subpltfmPL.size() > 0)
      {
          cerr << "    Sub platforms:" << endl;
          for (pIter = m_subpltfmPL.begin();
               pIter != m_subpltfmPL.end(); pIter++)
          {
              cerr << "  ";
              (*pIter)->dump_simple();
          }
      }
      if (m_impl_subpltfmPL.size() > 0)
      {
          cerr << "    Sub platforms implemented:" << endl;
          for (pIter = m_impl_subpltfmPL.begin();
               pIter != m_impl_subpltfmPL.end(); pIter++)
          {
              cerr << "  ";
              (*pIter)->dump_simple();
          }
      }

      // dumping interfaces
      if (m_intrfcPL.size() > 0)
      {
          cerr << "    Interfaces:" << endl;
          for (iIter = m_intrfcPL.begin();
               iIter != m_intrfcPL.end(); iIter++)
          {
              cerr << "  ";
              (*iIter)->dump_simple();
          }
      }
      if (m_impl_intrfcPL.size() > 0)
      {
          cerr << "  Interfaces implemented:" << endl;
          for (iIter = m_impl_intrfcPL.begin();
               iIter != m_impl_intrfcPL.end(); iIter++)
          {
              cerr << "  ";
              (*iIter)->dump_simple();
          }
      }
  
      // dumping user attributes
      if (m_uattrL.size() > 0)
      {
          cerr << "   User defined attrs:" << endl;
          for (uIter = m_uattrL.begin();
               uIter != m_uattrL.end(); uIter++)
              (*uIter).dump();
      }
      cerr << endl;
   }
// ------------------------------------------------------------
   void AX_PlatformC::dump_simple()
   {
      AX_CompC::dump();
   }
// ------------------------------------------------------------
   void AX_PlatformC::write(fstream & File)
   {
      AX_CompC::write(File);
      AX_PlatformPLCIter pIter;
      AX_InterfacePLCIter iIter;
      AX_UAttrLCIter uIter;

      // writeing sub platforms
      if (m_subpltfmPL.size() > 0)
      {
          File << "    Sub platforms:" << endl;
          for (pIter = m_subpltfmPL.begin();
               pIter != m_subpltfmPL.end(); pIter++)
          {
              File << "  ";
              (*pIter)->write_simple(File);
          }
      }
      if (m_impl_subpltfmPL.size() > 0)
      {
          File << "    Sub platforms implemented:" << endl;
          for (pIter = m_impl_subpltfmPL.begin();
               pIter != m_impl_subpltfmPL.end(); pIter++)
          {
              File << "  ";
              (*pIter)->write_simple(File);
          }
      }

      // writeing interfaces
      if (m_intrfcPL.size() > 0)
      {
          File << "    Interfaces:" << endl;
          for (iIter = m_intrfcPL.begin();
               iIter != m_intrfcPL.end(); iIter++)
          {
              File << "  ";
              (*iIter)->write_simple(File);
          }
      }
      if (m_impl_intrfcPL.size() > 0)
      {
          File << "  Interfaces implemented:" << endl;
          for (iIter = m_impl_intrfcPL.begin();
               iIter != m_impl_intrfcPL.end(); iIter++)
          {
              File << "  ";
              (*iIter)->write_simple(File);
          }
      }
  
      // writeing user attributes
      if (m_uattrL.size() > 0)
      {
          File << "   User defined attrs:" << endl;
          for (uIter = m_uattrL.begin();
               uIter != m_uattrL.end(); uIter++)
              (*uIter).write(File);
      }
      File << endl;
   }
// ------------------------------------------------------------
   void AX_PlatformC::write_simple(fstream & File)
   {
      AX_CompC::write(File);
   }
// ------------------------------------------------------------
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~End of Function Definitions
