// ------------------------------------------------------------------
// util.cpp
// ------------------------------------------------------------------

#include <iostream>
#include <math.h>
#include "util.h"
using namespace std;

// ------------------------------------------------------------------
// given an integer, this procedure return an ascii string for the no.
// ------------------------------------------------------------------
void my_itoa(int num, char * buf1)
{
    char buf[50];
    int j, k, idx;
    
    if (num == 0)
    {
        strcpy(buf1, "0");
        return;
    }
    
    idx = 0;
    j = num;
    if (num < 0)
        j = -num;
    while(j > 0)
    {
        k = j % 10;
        j = j / 10;
        buf[idx] = '0' + k;
        idx++;
    }
    for (j = 0; j < idx; j++)
        buf1[j] = buf[idx - j - 1];
    buf1[idx] = '\0';
}

// ------------------------------------------------------------------
// given an std::string, this procedure return the integer number it
// represents
// ------------------------------------------------------------------
int my_atoi(string data)
{
   char * cstr;
   cstr  = new char[data.size()+1];
   strcpy(cstr, data.c_str());

   return(atoi(cstr));
}

// ------------------------------------------------------------------
// get the absolute value of a float number
// ------------------------------------------------------------------
float my_fabs(float val)
{
    if (val < 0.0)
       val *= -1.0;

    return val;
}

// ------------------------------------------------------------------
// Get a random number i such that 0 <= i < max
// ------------------------------------------------------------------
int my_rand(int max)
{
    return rand() % max;
}

// ------------------------------------------------------------------
// Given a pool of positive integers of size pSize, randomly select
// sSize of numbers and place them into the array sel. And erase the
// selecte numbers from pool (by placing a -1)
// ------------------------------------------------------------------
void pick_numbers(int pool[], int pSize, int sel[], int sSize)
{
   for (int i = 0; i < sSize; i++)
   {
      while (1)
      {
         int iNum = my_rand(pSize);
         if (pool[iNum] >= 0)
         {
            sel[i] = pool[iNum];
            pool[iNum] = -1;
            break;
         }
      }
   }
}

// ------------------------------------------------------------------
int mem_parse(string name, bool & codeMem)
{
   int mSize;
   string mTmpS, tTmpS;

   char *cstr, *memStr, *typeStr, *sizeStr;
   cstr  = new char[name.size()+1];
   strcpy(cstr, name.c_str());
   memStr = strtok(cstr,"_");
   mTmpS = memStr;
   typeStr = strtok(NULL,"_");
   tTmpS = typeStr;
   sizeStr = strtok(NULL,"_");

#if DEBUG >= 4
   cerr << "mem_parse-1 still here!" << endl;
#endif
   if (mTmpS != "MEM")
      return 0;

#if DEBUG >= 4
   cerr << "mem_parse-2 still here!" << endl;
#endif
   if (tTmpS == "DATA")
      codeMem = false;
#if DEBUG >= 4
   cerr << "mem_parse-3 still here!" << endl;
#endif

   delete cstr;
   //cstr  = new char[name.size()+1];
   //strcpy(cstr, sizeStr.c_str());
   mSize = atoi(sizeStr);
   //delete cstr;

   return mSize;
}
// ------------------------------------------------------------------
int next_grade(int curGrade)
{
   float fLog = 0, ftemp;
   float fGrade;
   int nextGrade;

   ftemp = (float)curGrade;
   while (ftemp >= 2.0)
   {
      fLog += 1.0;
      ftemp /= 2.0;
   }
   fLog += 1.0;
   fGrade = pow(2.0, fLog);
   nextGrade = (int)fGrade;

   return nextGrade;
}
// ------------------------------------------------------------------
string trim_id(string name)
{
   int size = name.size();

   int pos = size - 1;
   for (int i = size-1; i >= 0; i--)
   {
      if (name[i] == '_')
         break;
      pos--;
   }

   string retStr;
   retStr.clear();
   for (int i = 0; i < pos; i++)
      retStr += name[i];

   return retStr;
}
// ------------------------------------------------------------------
bool isInStrA(StringAC & strA, const string & str)
{
   for (unsigned int i = 0; i < strA.size(); i++)
   {
      if (str == strA[i])
         return(true);
   }

   return(false);
}
