#ifndef AX_CONFIG_H
#define AX_CONFIG_H

// Configurable values
int MAX_TASK_NUM = 200;
int MAX_TASK_DEPEND = 20;
int MAX_CONN_NUM = 1000;
int POP_SIZE = 10;
int NO_GENERATION = 1000;
int ASIC_COMP_METHOD = 1;
int MAXGATECOUNT = 500000;
int ASIC_AREA = 400;
int MEMCORE_AREA = 160;
int FIT_FUNC = 0;
long RAN_SEED = 123456789;
float ALPHA_NORM = 1.0;
float BETA_NORM = 1.0;
float GAMA_NORM = 1.0;
float BUS_CLK_RATE = 100.0;
int BUS_WIDTH = 1;
float DSP_UTIL_MARGIN = 0.9;
bool ACTIVE_NORM = false;

#endif
