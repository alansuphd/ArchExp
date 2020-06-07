// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// util.h
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef AX_UTIL_H_INCLUDED
#define AX_UTIL_H_INCLUDED 1

#include <string.h>
#include <stdlib.h>
#include <vector>
#define StringAC std::vector<std::string>

void my_itoa(int num, char * buf1);
int my_atoi(std::string);
float my_fabs(float val);
int my_rand(int max);
void pick_numbers(int *pool, int pSize, int *sel, int sSize);
int mem_parse(std::string name, bool & codeMem);
int next_grade(int curGrade);
std::string trim_id(std::string name);
bool isInStrA(StringAC & strA, const std::string & str);

#endif /* AX_UTIL_H_INCLUDED */

