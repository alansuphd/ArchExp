// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// uattr.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <iostream>
#include "uattr.h"
using namespace std;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Constructor
// ------------------------------------------------------------
   AX_UAttrC::AX_UAttrC()
   {
      m_name.clear();
      m_type = T_VOID;
      m_value.clear();
   }
// ------------------------------------------------------------
   AX_UAttrC::AX_UAttrC(const AX_UAttrC & other)
   {
      m_name = other.m_name;
      m_type = other.m_type;
      m_value = other.m_value;
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Destructor
// ------------------------------------------------------------
   AX_UAttrC::~AX_UAttrC()
   {
      clean_up();
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Assignator & Comparators
// ------------------------------------------------------------
   AX_UAttrC & AX_UAttrC::operator= (const AX_UAttrC & other)
   {
      if (&other == this)
         return(*this);
   
      m_name = other.m_name;
      m_type = other.m_type;
      m_value = other.m_value;

      return(*this);
   }
// ------------------------------------------------------------
   void AX_UAttrC::copy(const AX_UAttrC & other)
   {
      m_name = other.m_name;
      m_type = other.m_type;
      m_value = other.m_value;
   }
// ------------------------------------------------------------
   bool AX_UAttrC::operator== (const AX_UAttrC & other)
   {
      if (&other == this)
         return(true);

      if (m_name == other.m_name &&
          m_type == other.m_type &&
          m_value == other.m_value)
         return(true);

      return(false);
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Pointer List Manipulations


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Instance List Manipulations


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Structure List Manipulations


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Special Functions
// ------------------------------------------------------------
   void AX_UAttrC::parse_attr(string attrStr)
   {
      string tokStr, tokStr1;
   
      char * cstr = new char[attrStr.size()+1];
      strcpy(cstr, attrStr.c_str());
      tokStr = strtok(cstr,",");

      this->set_name(tokStr);
      tokStr = strtok(NULL,",");

      if (tokStr == "int")
         this->set_type(T_INT);
      else if (tokStr == "long")
         this->set_type(T_LONG);
      else if (tokStr == "double")
         this->set_type(T_DOUBLE);
      else if (tokStr == "float")
         this->set_type(T_FLOAT);
      else if (tokStr == "char")
         this->set_type(T_CHAR);
      else if (tokStr == "char*")
         this->set_type(T_CHARP);
      else if (tokStr == "StringC")
         this->set_type(T_STRINGC);
      else if (tokStr == "StringAC")
         this->set_type(T_STRINGAC);
      else
         this->set_type(T_VOID);

      tokStr = strtok(NULL,",");
      this->set_value(tokStr);
      delete[] cstr;
   
      return;
   }
// ------------------------------------------------------------


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Miscellaneous Functions
// ------------------------------------------------------------
   void AX_UAttrC::clean_up()
   {
      m_name.clear();
      m_type = T_VOID;
      m_value.clear();
   }
// ------------------------------------------------------------
   void AX_UAttrC::dump()
   {
      cerr << "      Usr Attr: " << m_name << ".  type ";
      switch (m_type)
      {
         case T_VOID:
            cerr << "void";
            break;
         case T_INT:
            cerr << "int";
            break;
         case T_LONG:
            cerr << "long";
            break;
         case T_DOUBLE:
            cerr << "double";
            break;
         case T_FLOAT:
            cerr << "float";
            break;
         case T_CHAR:
            cerr << "char";
            break;
         case T_CHARP:
            cerr << "char *";
            break;
         case T_STRINGC:
            cerr << "StringC";
            break;
         case T_STRINGAC:
            cerr << "StringAC";
            break;
      }
      cerr << "  value " << m_value << endl;
   }
// ------------------------------------------------------------
   void AX_UAttrC::write(fstream & File)
   {
      File << "      Usr Attr: " << m_name << ".  type ";
      switch (m_type)
      {
         case T_VOID:
            File << "void";
            break;
         case T_INT:
            File << "int";
            break;
         case T_LONG:
            File << "long";
            break;
         case T_DOUBLE:
            File << "double";
            break;
         case T_FLOAT:
            File << "float";
            break;
         case T_CHAR:
            File << "char";
            break;
         case T_CHARP:
            File << "char *";
            break;
         case T_STRINGC:
            File << "StringC";
            break;
         case T_STRINGAC:
            File << "StringAC";
            break;
      }
      File << "  value " << m_value << endl;
   }
// ------------------------------------------------------------
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~End of Function Definitions
