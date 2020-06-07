// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// interface.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <iostream>
#include "interface.h"
using namespace std;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructor
// ------------------------------------------------------------
   AX_InterfaceC::AX_InterfaceC()
   {
       this->set_category(INTERFACE);
       m_platfmP = NULL;
       m_intrtype = INTRFC_TYPE_UNKNOWN;
   }
// ------------------------------------------------------------
   AX_InterfaceC::AX_InterfaceC(const AX_InterfaceC & other)
   {
      copy(other);
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Destructor
// ------------------------------------------------------------
   AX_InterfaceC::~AX_InterfaceC()
   {
      this->AX_CompC::~AX_CompC();

      clean_up();
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Assignator & Comparators
// ------------------------------------------------------------
   AX_InterfaceC & AX_InterfaceC::operator= (const AX_InterfaceC & other)
   {
      if (&other == this)
         return(*this);

      copy(other);

      return(*this);
   }
// ------------------------------------------------------------
   void AX_InterfaceC::copy(const AX_InterfaceC & other)
   {
      AX_CompC::copy(other);
      m_platfmP = other.m_platfmP;
      m_intrtype = other.m_intrtype;

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
   bool AX_InterfaceC::operator== (const AX_InterfaceC & other)
   {
      return (AX_CompC::operator== (other));
   }
// ------------------------------------------------------------
   bool AX_InterfaceC::operator== (const string name)
   {
      return (AX_CompC::operator== (name));
   }
// ------------------------------------------------------------
   bool AX_InterfaceC::operator!= (const string name)
   {
       return (!(*this == name));
   }
// ------------------------------------------------------------

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Instance List Manipulations
// ------------------------------------------------------------
   AX_UAttrLCIter AX_InterfaceC::add_new_uattr(AX_UAttrC & uattr)
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
   AX_UAttrCP AX_InterfaceC::get_uattr(int index)
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
   void AX_InterfaceC::remove_uattr(AX_UAttrCP uattrP)
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
   AX_UAttrLCIter AX_InterfaceC::delete_uattr(AX_UAttrLCIter & iter)
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
   void AX_InterfaceC::remove_all_uattrs()
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


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Miscellaneous Functions
// ------------------------------------------------------------
   void AX_InterfaceC::clean_up()
   {
       m_platfmP = NULL; 
       m_intrtype = INTRFC_TYPE_UNKNOWN;
       m_uattrL.clear();
   }
// ------------------------------------------------------------
   void AX_InterfaceC::dump()
   {
       AX_CompC::dump();
       AX_InterfacePLCIter iIter;
       AX_UAttrLCIter uIter;

       // dumping interface type
       cerr << "    intrtype " << m_intrtype << endl;

       // dumping platform
       if (m_platfmP != NULL)
       {
           cerr << "  ";
           m_platfmP->dump_simple();
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
   void AX_InterfaceC::dump_simple()
   {
      AX_CompC::dump();
   }
// ------------------------------------------------------------
   void AX_InterfaceC::write(std::fstream & File)
   {
       AX_CompC::write(File);
       AX_InterfacePLCIter iIter;
       AX_UAttrLCIter uIter;

       // writeing interface type
       File << "    intrtype " << m_intrtype << endl;

       // writeing platform
       if (m_platfmP != NULL)
       {
           File << "  ";
           m_platfmP->write_simple(File);
       }

       // writeing user attributes
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
   void AX_InterfaceC::write_simple(std::fstream & File)
   {
      AX_CompC::write(File);
   }
// ------------------------------------------------------------
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~End of Function Definitions
