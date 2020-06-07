#include "dsn.h"
#include <iostream>
#include <fstream>
#include "util.h"
using namespace std;

// -----------------------------------------------------------
// -----------------------------------------------------------
bool AX_DsnC::read_dsnspec(std::string FileName)
{
    AX_DsnSpecC spec;
    string rdStr;
    char rdBuf[256];
    bool dsnRead = false;
    
    fstream DSNFile;
    DSNFile.open(FileName.c_str(), fstream::in);
    if (!DSNFile)
    {
       cerr << "Cannot open " << FileName << endl;
       return false;
    }

    // read design name
    DSNFile >> rdBuf;
    rdStr = rdBuf;

    while (!DSNFile.eof())
    {
       if ((strncmp(rdStr.c_str(), "//", 2) == 0) ||
           (rdStr[0] == '\n'))
       {
          // A comment line. Consume the line
          char getc = '0';
          while (getc != '\0' && getc != '\n')
             DSNFile.get(getc);

          // Get next item
          DSNFile >> rdBuf;
          rdStr = rdBuf;
#if DEBUG >= 5
          cerr << "dsnspec reads 0.0: " << rdStr << endl;
#endif
          continue;
       }
       if (!dsnRead)
       {
#if DEBUG >= 5
          cerr << "dsnspec reads 1 design: " << rdStr << endl;
#endif
          m_name = rdStr;
          dsnRead = true;
          DSNFile >> rdBuf;
          rdStr = rdBuf;
#if DEBUG >= 5
          cerr << "dsnspec reads 0.1: " << rdStr << endl;
#endif
          continue;
       }

       // spec name is retrived
       spec.set_name(rdStr);
#if DEBUG >= 5
       cerr << "dsnspec reads 2 spec: " << rdStr << endl;
#endif

       // read id
       DSNFile >> rdBuf;
       rdStr = rdBuf;
       int iNum = atoi(rdStr.c_str());
       spec.set_id(iNum);
#if DEBUG >= 5
       cerr << "dsnspec reads 3: " << rdStr << " id: " << iNum << endl;
#endif

       // read maximum memory
       DSNFile >> rdBuf;
       rdStr = rdBuf;
       iNum = atoi(rdStr.c_str());
       spec.set_max_memory(iNum);
#if DEBUG >= 5
       cerr << "dsnspec reads 4: " << rdStr << " max_memory: " << iNum << endl;
#endif

       // read maximum latency
       DSNFile >> rdBuf;
       rdStr = rdBuf;
       iNum = atoi(rdStr.c_str());
       spec.set_max_latency(iNum);
#if DEBUG >= 5
       cerr << "dsnspec reads 5: " << rdStr << " max_latency: " << iNum << endl;
#endif

       // read maximum periodicity
       DSNFile >> rdBuf;
       rdStr = rdBuf;
       iNum = atoi(rdStr.c_str());
       spec.set_max_periodicity(iNum);
#if DEBUG >= 5
       cerr << "dsnspec reads 6: " << rdStr << " max_periodicity: " << iNum << endl;
#endif

       // read maximum power
       DSNFile >> rdBuf;
       rdStr = rdBuf;
       float fNum = atof(rdStr.c_str());
       spec.set_max_power(fNum);
#if DEBUG >= 5
       cerr << "dsnspec reads 7: " << rdStr << " max_power: " << fNum << endl;
#endif

       // read maximum area
       DSNFile >> rdBuf;
       rdStr = rdBuf;
       fNum = atof(rdStr.c_str());
       spec.set_max_area(fNum);
#if DEBUG >= 5
       cerr << "dsnspec reads 8: " << rdStr << " max_area: " << fNum << endl;
#endif

       // read maximum cost
       DSNFile >> rdBuf;
       rdStr = rdBuf;
       fNum = atof(rdStr.c_str());
       spec.set_max_cost(fNum);
#if DEBUG >= 5
       cerr << "dsnspec reads 9: " << rdStr << " max_cost: " << fNum << endl;
#endif

       // set dsn pointer
       spec.set_dsn(this);

       // add dsnspec
       add_new_dsnspec(spec);

       // get next item
       DSNFile >> rdBuf;
       rdStr = rdBuf;
    }

    return true;
}

// -----------------------------------------------------------
// -----------------------------------------------------------
bool AX_DsnC::read_tg(std::string FileName)
{
    AX_TaskLCIter tIter;
    AX_ConnLCIter cIter;
    string rdStr;
    int tNum = 0, pNum = 0, cNum = 0;
    char rdBuf[256];

    fstream TGFile;
    TGFile.open(FileName.c_str(), fstream::in);
    if (!TGFile)
    {
       cerr << "Cannot open " << FileName << endl;
       return false;
    }

    TGFile >> rdBuf;
    rdStr = rdBuf;
    while(!TGFile.eof())
    {
       if (strncmp(rdStr.c_str(), "//", 2) == 0)
       {
          // A comment line. Consume the line
          char getc = '0';
          while (getc != '\0' && getc != '\n')
             TGFile.get(getc);

          // Get next item
          TGFile >> rdBuf;
          rdStr = rdBuf;
       }
       else if (strcmp(rdStr.c_str(),"TaskGraph") == 0)
       {
#if DEBUG >= 5
          cerr << "read_tg-0 taskgraph label: " << rdStr << endl;
#endif
          // get taskgraph name
          TGFile >> rdBuf;
          rdStr = rdBuf;
#if DEBUG >= 5
          cerr << "read_tg-1 task graph name: " << rdStr << endl;
#endif
          m_tg.set_name(rdStr);
          TGFile >> rdBuf;
          rdStr = rdBuf;
       }
       else if (strcmp(rdStr.c_str(),"Task") == 0)
       {
#if DEBUG >= 5
          cerr << "read_tg-0 task label: " << rdStr << endl;
#endif
          tIter = m_tg.create_new_task();
          // get task name
          TGFile >> rdBuf;
          rdStr = rdBuf;
#if DEBUG >= 5
          cerr << "read_tg-2 task name: " << rdStr << endl;
#endif
          (*tIter).set_name(rdStr);
          (*tIter).set_id(tNum++);

          // get functional name
          TGFile >> rdBuf;
          rdStr = rdBuf;
          (*tIter).set_function(rdStr);
#if DEBUG >= 5
          cerr << "read_tg-3 function type: " << rdStr << endl;
#endif

          // get taskgraph name
          TGFile >> rdBuf;
          rdStr = rdBuf;
          (*tIter).set_parent_tg(&m_tg);
#if DEBUG >= 5
          cerr << "read_tg-4 parent taskgraph: " << rdStr << endl;
#endif

          // get list of components (implementations)
          TGFile >> rdBuf;
          rdStr = rdBuf;

          char *cstr, *tokStr;
          cstr = new char[rdStr.size()+1];
          strcpy(cstr, rdStr.c_str());
          tokStr = strtok(cstr,",");

          while (tokStr != NULL)
          {
             AX_FuncBlkLCIter comp, comp1;
             string fTmpS = tokStr;
             comp = get_fncblk(fTmpS);
#if DEBUG >= 5
          cerr << "read_tg-5 task comp: " << fTmpS << endl;
#endif
             int oldFncCnt = (*tIter).get_no_fncblks();
             if (comp != m_fncblkL.end())
             {
                (*tIter).add_new_fncblk(&(*comp));
             }
             else
             {
                comp1 = comp;
                comp1++;
                for (comp = get_first_fncblk_by_func(tokStr);
                     comp != m_fncblkL.end();
                     comp = get_next_fncblk_by_func(comp1, tokStr))
                {
                   (*tIter).add_new_fncblk(&(*comp));
                   comp1 = comp;
                   comp1++;
                }
             }
             if (oldFncCnt == (*tIter).get_no_fncblks())
             {
                cerr << "Cannot find " << tokStr
                     << " in m_fncblkL" << endl;
                continue;
             }
             tokStr = strtok(NULL,",");
          }

          // get task type
          TGFile >> rdBuf;
          rdStr = rdBuf;
          if (strcmp(rdStr.c_str(),"Atomic") == 0)
             (*tIter).set_type(ATOMIC);
          else if (strcmp(rdStr.c_str(),"Hierarchical") == 0)
             (*tIter).set_type(HIERARCHICAL);
          else if (strcmp(rdStr.c_str(),"Delay") == 0)
             (*tIter).set_type(DELAY);
          else if (strcmp(rdStr.c_str(),"Distributor") == 0)
             (*tIter).set_type(DISTRIBUTOR);
          else if (strcmp(rdStr.c_str(),"Collector") == 0)
             (*tIter).set_type(COLLECTOR);
          else
             (*tIter).set_type(TASK_TYPE_UNKNOWN);
#if DEBUG >= 5
          cerr << "read_tg-6 task type: " << rdStr << endl;
#endif

          // get delay number for a delay task
          if ((*tIter).get_type() == DELAY)
          {
             TGFile >> rdBuf;
             rdStr = rdBuf;

             int dNum = atoi(rdStr.c_str());
             (*tIter).set_delay(dNum);
#if DEBUG >= 5
             cerr << "read_tg-7 delay length: " << dNum << endl;
#endif
          }

          // next item
          TGFile >> rdBuf;
          rdStr = rdBuf;
       }
       else if (strcmp(rdStr.c_str(),"Port") == 0)
       {
#if DEBUG >= 5
          cerr << "read_tg-0 port label: " << rdStr << endl;
#endif
          AX_TaskPortLCIter pIter;
          string pName, tName;
          AX_TaskPortC rdPort;

          TGFile >> rdBuf;
          pName = rdBuf;
          rdPort.set_name(pName);
          TGFile >> rdBuf;
          tName = rdBuf;
#if DEBUG >= 5
          cerr << "read_tg-8 port name: " << tName << "." << pName << endl;
#endif
          string dirS;
          TGFile >> rdBuf;
          dirS = rdBuf;
          if (tName == m_tg.get_name())
          {
             rdPort.set_parent_tg(&m_tg);
             if (strcmp(dirS.c_str(),"In") == 0)
                pIter = m_tg.create_new_tg_inport(pName);
             else if (strcmp(dirS.c_str(),"Out") == 0)
                pIter = m_tg.create_new_tg_outport(pName);
#if DEBUG >= 5
          cerr << "read_tg-9 graph port: " << tName << ":" << dirS << ":" << pName << endl;
#endif
          }
          else
          {
             AX_TaskLCIter temp;
             temp = m_tg.get_task(tName);
             if (temp == m_tg.task_list_end())
             {
                AX_TaskC nTask;
                nTask.set_name(tName);
                temp = m_tg.add_new_task(nTask);
             }
             pIter = m_tg.create_new_taskport(&(*temp), pName);
             if (strcmp((char *)dirS.c_str(),"In") == 0)
             {
                (*pIter).set_type(PORT_IN);
                (*temp).add_new_inport(&(*pIter));
             }
             else if (strcmp(dirS.c_str(),"Out") == 0)
             {
                (*pIter).set_type(PORT_OUT);
                (*temp).add_new_outport(&(*pIter));
             }
             else if (strcmp(dirS.c_str(),"Bi") == 0)
             {
                (*pIter).set_type(PORT_BI);
                (*temp).add_new_outport(&(*pIter));
             }
             else
             {
                cerr << "Incorrect direction to port "
                     << tName << ":" << pName << endl;
                (*pIter).set_type(PORT_TYPE_UNKNOWN);
             }
#if DEBUG >= 5
             cerr << "read_tg-10 task port: " << tName << ":" << dirS << ":" << pName << endl;
#endif
          }
          (*pIter).set_id(pNum++);

          // get sample size
          TGFile >> rdBuf;
          rdStr = rdBuf;
          int iNum = atoi(rdStr.c_str());
          (*pIter).set_sample_size(iNum);
#if DEBUG >= 5
          cerr << "read_tg-11 sample size: " << iNum << endl;
#endif

          // get sample rate
          TGFile >> rdBuf;
          rdStr = rdBuf;
          iNum = atoi(rdStr.c_str());
          (*pIter).set_no_samples(iNum);
#if DEBUG >= 5
          cerr << "read_tg-12 sample rate: " << iNum << endl;
#endif

          // next item
          TGFile >> rdBuf;
          rdStr = rdBuf;
       }
       else if (strcmp(rdStr.c_str(),"Conn") == 0)
       {
#if DEBUG >= 5
          cerr << "read_tg-13 conn label: " << rdStr << endl;
#endif
          AX_ConnC rdConn;
          rdConn.set_type(DATA);
          rdConn.set_avg_activ(100);
          rdConn.set_pk_activ(100);

          TGFile >> rdBuf;
          rdStr = rdBuf;
          string tokStr, tokStr1;
          string taskName, portName;

          char * cstr;
          cstr = new char[rdStr.size()+1];
          strcpy(cstr, rdStr.c_str());
          tokStr = strtok(cstr,",");
          tokStr1 = strtok(NULL,",");

          delete cstr;
          cstr = new char[tokStr.size()+1];
          strcpy(cstr, tokStr.c_str());
          taskName = strtok(cstr,":");
          portName = strtok(NULL,":");
#if DEBUG >= 5
          cerr << "read_tg-14 source: " << taskName << ":" << portName << endl;
#endif

          AX_TaskCP srcTask = NULL; 
          AX_TaskCP sinkTask = NULL;

          if (taskName == m_tg.get_name())
          {
             AX_TaskPortLCIter portI;

             rdConn.set_src_tg(&m_tg);
             portI = m_tg.get_tg_inport(portName);
             if (portI == m_tg.tg_inport_list_end())
             {
                cerr << "Conn read: Connot find port "
                     << tokStr << endl;
                TGFile >> rdBuf;
                rdStr = rdBuf;
                continue;
             }
             rdConn.set_src_port(&(*portI));
             cIter = m_tg.add_new_conn(rdConn);
             (*portI).add_new_conn(&(*cIter));
          }
          else
          {
             AX_TaskLCIter taskI;
             AX_TaskPortPLCIter portPI;
             taskI = m_tg.get_task(taskName);
             if (taskI == m_tg.task_list_end())
             {
                cerr << "Conn read: Cannot find task "
                     << taskName << endl;
                TGFile >> rdBuf;
                rdStr = rdBuf;
                continue;
             }
             srcTask = &(*taskI);
             portPI = (*taskI).get_outport(portName);
             rdConn.set_src_task(&(*taskI));
             rdConn.set_src_port((*portPI));
             cIter = m_tg.add_new_conn(rdConn);
             (*portPI)->add_new_conn(&(*cIter));
          }
          (*cIter).set_id(cNum++);

          delete cstr;
          cstr = new char[tokStr1.size()+1];
          strcpy(cstr, tokStr1.c_str());
          taskName = strtok(cstr,":");
          portName = strtok(NULL,":");
#if DEBUG >= 5
          cerr << "read_tg-15 sink: " << taskName << ":" << portName << endl;
#endif

          if (taskName == m_tg.get_name())
          {
#if DEBUG >= 5
             cerr << "read_tg-16 sink taskgraph port: " << taskName << ":" << portName << endl;
#endif
             AX_TaskPortLCIter portI;

             (*cIter).set_sink_tg(&m_tg);
             portI = m_tg.get_tg_outport(portName);
             if (portI == m_tg.tg_outport_list_end())
             {
                cerr << "Conn read: Connot find port "
                     << tokStr << endl;
                TGFile >> rdBuf;
                rdStr = rdBuf;
                continue;
             }
             (*cIter).set_sink_port(&(*portI));
             (*portI).add_new_conn(&(*cIter));
          }
          else
          {
#if DEBUG >= 5
             cerr << "read_tg-17 sink task port: " << taskName << ":" << portName << endl;
#endif
             AX_TaskLCIter taskI;
             AX_TaskPortPLCIter portPI;
             taskI = m_tg.get_task(taskName);
             if (taskI == m_tg.task_list_end())
             {
                cerr << "Conn read: Cannot find task "
                     << taskName << endl;
                TGFile >> rdBuf;
                rdStr = rdBuf;
                continue;
             }
#if DEBUG >= 5
             cerr << "read_tg-18 sink task retrived: " << (*taskI).get_name() << endl;
#endif
             sinkTask = &(*taskI);
             portPI = (*taskI).get_inport(portName);
             (*cIter).set_sink_task(&(*taskI));
             (*cIter).set_sink_port(*portPI);
             (*portPI)->add_new_conn(&(*cIter));
#if DEBUG >= 5
             cerr << "read_tg-19 sink port retrived: " << (*portPI)->get_name() << endl;
#endif
          }
          delete cstr;

#if DEBUG >= 5
          (*cIter).dump_simple();
#endif
          // next item
          TGFile >> rdBuf;
          rdStr = rdBuf;

          // appending the succ and pred lists
          if (srcTask == NULL && sinkTask == NULL)
          {
                cerr << "Conn read: both source and sink tasks are null"
                     << endl;
                continue;
          }
          if (srcTask != NULL)
          {
             if (sinkTask != NULL)
                sinkTask->add_new_pred(srcTask);
          }
          if (sinkTask != NULL)
          {
             if (srcTask != NULL)
                srcTask->add_new_succ(sinkTask);
          }
       }
       else if (strcmp(rdStr.c_str(),"IterBlk") == 0)
       {
          AX_IterBlkS nIterBlk;
          nIterBlk.m_itercnt = -1;
          nIterBlk.m_ii = -1;
          nIterBlk.m_latency = -1;
          nIterBlk.m_basedelay= -1;
          nIterBlk.m_temp = -1;
          AX_IterBlkLSIter nIterBlkI;
          nIterBlkI = m_tg.add_new_iterblk(nIterBlk);

          // get IterBlk name
          TGFile >> rdBuf;
          rdStr = rdBuf;
          (*nIterBlkI).m_name = rdStr;
#if DEBUG >= 5
          cerr << "read_tg-20 IterBlk name: " << (*nIterBlkI).m_name << endl;
#endif

          // get iteration count
          TGFile >> rdBuf;
          rdStr = rdBuf;

          int itNum;
          itNum = atoi(rdStr.c_str());
          (*nIterBlkI).m_itercnt = itNum;
#if DEBUG >= 5
          cerr << "read_tg-21 IterBlk count: " << (*nIterBlkI).m_itercnt << endl;
#endif

          // get task list
          TGFile >> rdBuf;
          rdStr = rdBuf;

          (*nIterBlkI).m_latency = -1;
          (*nIterBlkI).m_basedelay = -1;
          if (!parse_iterblk(rdStr, m_tg, &(*nIterBlkI)))
             return false;
#if DEBUG >= 5
          cerr << "read_tg-22 IterBlk parsed: " << endl;
#endif

          // get next item
          TGFile >> rdBuf;
          rdStr = rdBuf;
       }
       else if (strcmp(rdStr.c_str(),"CycleHead") == 0)
       {
          // get cycle head list
          TGFile >> rdBuf;
          rdStr = rdBuf;
#if DEBUG >= 5
          cerr << "read_tg-23 cycle head: " << rdStr << endl;
#endif

          if (!parse_cyclehead(rdStr, m_tg))
             return false;
#if DEBUG >= 5
          cerr << "read_tg-24 cycle head parsed" << endl;
#endif

          // get next item
          TGFile >> rdBuf;
          rdStr = rdBuf;
       }
    }
 
    return(true);
}
// -----------------------------------------------------------
bool AX_DsnC::parse_iterblk(string & ibStr, AX_TaskGraphC & tg,
                                AX_IterBlkSP iterblkP)
{
    char *cstr, *tokStr;
    cstr = new char[ibStr.size()+1];
    strcpy(cstr, ibStr.c_str());
    tokStr = strtok(cstr,",");

    while (tokStr != NULL)
    {
       AX_TaskLCIter nTaskI;
       string tTmpS = tokStr;
       nTaskI = tg.get_task(tTmpS);
       if (nTaskI == tg.task_list_end())
       {
          cerr << "Library err: IterBlk spec err - "
               << tokStr << " is not a task\n";
          return false;
       }
       iterblkP->m_taskPL.push_back(&(*nTaskI));
       (*nTaskI).set_iterblk(iterblkP);
       tokStr = strtok(NULL,",");
    }

    return true;
}
// -----------------------------------------------------------
bool AX_DsnC::parse_cyclehead(string & cyStr, AX_TaskGraphC & tg)
{
    char *cstr, *tokStr;
    cstr = new char[cyStr.size()+1];
    strcpy(cstr, cyStr.c_str());
    tokStr = strtok(cstr,",");

    while (tokStr != NULL)
    {
       AX_TaskLCIter nTaskI;
       AX_CycleS nCycle;

       string tTmpS = tokStr;
       nTaskI = tg.get_task(tTmpS);
       if (nTaskI == tg.task_list_end())
       {
          cerr << "Library err: CycleHead spec err - "
               << tokStr << " is not a task\n";
          return false;
       }
       nCycle.m_headP = &(*nTaskI);
       nCycle.m_cycle_latency = -1;
       nCycle.m_lcd = -1;
       tg.add_new_cycle(nCycle);
       tokStr = strtok(NULL,",");
    }

    return true;
}

// -----------------------------------------------------------
// -----------------------------------------------------------
bool AX_DsnC::read_comps(std::string FileName)
{
    AX_CompPortC rdPort;
    AX_UAttrC rdAttr;
    string rdStr;
    string rdName;
    char rdBuf[256];

    fstream CompFile;
    CompFile.open(FileName.c_str(), fstream::in);
    if (!CompFile)
    {
       cerr << "Cannot open " << FileName << endl;
       return false;
    }

    CompFile >> rdBuf;
    rdStr = rdBuf;
    while(!CompFile.eof())
    {
       if (strncmp(rdStr.c_str(), "//", 2) == 0)
       {
          // A comment line. Consume the line
          char getc = '0';
          while (getc != '\0' && getc != '\n')
             CompFile.get(getc);

          // Get next item
          CompFile >> rdBuf;
          rdStr = rdBuf;
       }
       else if (strcmp(rdStr.c_str(),"Comp") == 0)
       {
#if DEBUG >= 5
             cerr << "read_comp-0 label: " << rdStr << endl;
#endif
          CompFile >> rdBuf;
          rdStr = rdBuf;
          if (strcmp(rdStr.c_str(),"Platform") == 0)
          {
             CompFile >> rdBuf;
             rdName = rdBuf;
#if DEBUG >= 5
             cerr << "read_comp-1 platform name: " << rdName << endl;
#endif
             AX_PlatformLCIter pcIter;
             pcIter = create_new_platfm(rdName);

             // read platform level
             CompFile >> rdBuf;
             rdStr = rdBuf;
             if (strcmp(rdStr.c_str(),"Cabinet") == 0)
                (*pcIter).set_level(CABINET);
             else if (strcmp(rdStr.c_str(),"PCB") == 0)
                (*pcIter).set_level(PCB);
             else if (strcmp(rdStr.c_str(),"Die") == 0)
                (*pcIter).set_level(DIE);
             else if (strcmp(rdStr.c_str(),"Core") == 0)
                (*pcIter).set_level(CORE);
             else
                cerr << "Error: Incorrect platform level" << endl;
#if DEBUG >= 5
             cerr << "read_comp-2 platform level: " << rdStr << endl;
#endif

             // read list of sub-platforms
             CompFile >> rdBuf;
             rdStr = rdBuf;
             if (strcmp(rdStr.c_str(),"null") != 0)
             {
                char *cstr, *tokStr;
                cstr = new char[rdStr.size()+1];
                strcpy(cstr, rdStr.c_str());
                tokStr = strtok(cstr,",");

                while (tokStr != NULL)
                {
#if DEBUG >= 5
                   cerr << "read_comp-3 sub-platform: " << tokStr << endl;
#endif
                   AX_PlatformLCIter nTemp;
                   string pTmpS = tokStr;
                   nTemp = create_new_platfm(pTmpS);
                   (*pcIter).add_new_subpltfm(&(*nTemp));
                   tokStr = strtok(NULL,",");
                }
             }

             // read list of interfaces
             CompFile >> rdBuf;
             rdStr = rdBuf;
             if (strcmp(rdStr.c_str(),"null") != 0)
             {
                char *cstr, *tokTmp;
                cstr = new char[rdStr.size()+1];
                strcpy(cstr, rdStr.c_str());
                tokTmp = strtok(cstr,",");

                while (tokTmp != NULL)
                {
#if DEBUG >= 5
                   cerr << "read_comp-4 interface: " << tokTmp << endl;
#endif
                   AX_InterfaceLCIter nTemp;
                   string iTmpS = tokTmp;
                   nTemp = create_new_intrfc(iTmpS);
                   (*pcIter).add_new_intrfc(&(*nTemp));
                   tokTmp = strtok(NULL,",");
                }
             }

             // read list of user attributes
             CompFile >> rdBuf;
             rdStr = rdBuf;
             while (strcmp(rdStr.c_str(),"Comp") != 0 &&
                    strcmp(rdStr.c_str(),"Port") != 0 &&
                    strncmp(rdStr.c_str(),"//",2) != 0)
             {
#if DEBUG >= 5
                cerr << "read_comp-5 uattr: " << rdStr << endl;
#endif
                AX_UAttrC uattr;
                uattr.parse_attr(rdStr);
                (*pcIter).add_new_uattr(uattr);
                CompFile >> rdBuf;
                rdStr = rdBuf;
             }
             continue;
          }
          else if (strcmp(rdStr.c_str(),"Interface") == 0)
          {
#if DEBUG >= 5
             cerr << "read_comp-0 label: " << rdStr << endl;
#endif
             CompFile >> rdBuf;
             rdName = rdBuf;
#if DEBUG >= 5
             cerr << "read_comp-6 interface name: " << rdName << endl;
#endif
             AX_InterfaceLCIter icIter;
             icIter = create_new_intrfc(rdName);

             // read platform
             CompFile >> rdBuf;
             rdStr = rdBuf;
             AX_PlatformLCIter pTemp;
             pTemp = create_new_platfm(rdStr);
             (*icIter).set_platfm(&(*pTemp));
#if DEBUG >= 5
             cerr << "read_comp-7 platform: " << rdStr << endl;
#endif

             // read interface type
             CompFile >> rdBuf;
             rdStr = rdBuf;
             if (strcmp(rdStr.c_str(),"Bus") == 0)
                (*icIter).set_intrtype(INTRFC_BUS);
             else if (strcmp(rdStr.c_str(),"Wire") == 0)
                (*icIter).set_intrtype(INTRFC_WIRE);
#if DEBUG >= 5
             cerr << "read_comp-8 interface type: " << rdStr << endl;
#endif

             // read list of user attributes
             CompFile >> rdBuf;
             rdStr = rdBuf;
             while (strcmp(rdStr.c_str(),"Comp") != 0 &&
                    strcmp(rdStr.c_str(),"Port") != 0 &&
                    strncmp(rdStr.c_str(),"//",2) != 0)
             {
#if DEBUG >= 5
             cerr << "read_comp-9 uattr: " << rdStr << endl;
#endif
                AX_UAttrC uattr;
                uattr.parse_attr(rdStr);
                (*icIter).add_new_uattr(uattr);
                CompFile >> rdBuf;
                rdStr = rdBuf;
             }
             continue;
          }
          else if (strcmp(rdStr.c_str(),"FuncBlk") == 0)
          {
#if DEBUG >= 5
             cerr << "read_comp-0 label: " << rdStr << endl;
#endif
             CompFile >> rdBuf;
             rdName = rdBuf;
#if DEBUG >= 5
             cerr << "read_comp-10 FuncBlk name: " << rdName << endl;
#endif
             AX_FuncBlkLCIter fcIter;
             fcIter = create_new_fncblk(rdName);

             // read function
             CompFile >> rdBuf;
             rdStr = rdBuf;
             (*fcIter).set_function(rdStr);
#if DEBUG >= 5
             cerr << "read_comp-11 function: " << rdStr << endl;
#endif

             // read type 
             CompFile >> rdBuf;
             rdStr = rdBuf;
             if (strcmp(rdStr.c_str(),"Hardware") == 0)
                (*fcIter).set_type(HARDWARE);
             else if (strcmp(rdStr.c_str(),"Software") == 0)
                (*fcIter).set_type(SOFTWARE);
             else
                (*fcIter).set_type(NULLTYPE);
#if DEBUG >= 5
             cerr << "read_comp-12 type: " << rdStr << endl;
#endif

             // read functype 
             CompFile >> rdBuf;
             rdStr = rdBuf;
             if (strcmp(rdStr.c_str(),"Function") == 0)
                (*fcIter).set_functype(FUNCTION);
             else if (strcmp(rdStr.c_str(),"Processor") == 0)
                (*fcIter).set_functype(PROCESSOR);
             else if (strcmp(rdStr.c_str(),"Memory") == 0)
                (*fcIter).set_functype(MEMORY);
             else if (strcmp(rdStr.c_str(),"MemoryCore") == 0)
                (*fcIter).set_functype(MEMORY_CORE);
             else if (strcmp(rdStr.c_str(),"Distributor") == 0)
                (*fcIter).set_functype(FUNC_DISTRIBUTOR);
             else if (strcmp(rdStr.c_str(),"Collector") == 0)
                (*fcIter).set_functype(FUNC_COLLECTOR);
             else if (strcmp(rdStr.c_str(),"Delay") == 0)
                (*fcIter).set_functype(FUNC_DELAY);
             else
                (*fcIter).set_functype(FUNC_TYPE_UNKNOWN);
#if DEBUG >= 5
             cerr << "read_comp-13 functype: " << rdStr << endl;
#endif

             // read max clock rate
             CompFile >> rdBuf;
             rdStr = rdBuf;
             int iNum;
             if (strcmp(rdStr.c_str(),"null") == 0)
                (*fcIter).set_max_clk_rate(0);
             else
             {
                iNum = atoi(rdStr.c_str());
                (*fcIter).set_max_clk_rate(iNum);
             }
#if DEBUG >= 5
             cerr << "read_comp-14 max clock rate: " << rdStr << endl;
#endif

             // read no of stages
             CompFile >> rdBuf;
             rdStr = rdBuf;
             if (strcmp(rdStr.c_str(),"null") == 0)
                (*fcIter).set_no_stages(1);
             else
             {
                iNum = atoi(rdStr.c_str());
                (*fcIter).set_no_stages(iNum);
             }
#if DEBUG >= 5
             cerr << "read_comp-15 # stages: " << rdStr << endl;
#endif

             // read leakage current
             float fNum;
             CompFile >> rdBuf;
             rdStr = rdBuf;
             if (strcmp(rdStr.c_str(),"null") == 0)
                (*fcIter).set_leak_current(0);
             else
             {
                fNum = (float)atof(rdStr.c_str());
                (*fcIter).set_leak_current((int)fNum);
             }
#if DEBUG >= 5
             cerr << "read_comp-16 leakage: " << rdStr << endl;
#endif

             // read runtime power
             CompFile >> rdBuf;
             rdStr = rdBuf;
             if (strcmp(rdStr.c_str(),"null") == 0)
                (*fcIter).set_run_pwr(0);
             else
             {
                fNum = (float)atof(rdStr.c_str());
                (*fcIter).set_run_pwr((int)fNum);
             }
#if DEBUG >= 5
             cerr << "read_comp-17 dynamic power: " << rdStr << endl;
#endif

             // read platform
             CompFile >> rdBuf;
             rdStr = rdBuf;
             if (strcmp(rdStr.c_str(),"null") != 0)
             {
                AX_PlatformLCIter pTemp;
                pTemp = create_new_platfm(rdStr);
                (*fcIter).set_platfm(&(*pTemp));
             }
#if DEBUG >= 5
             cerr << "read_comp-18 platform: " << rdStr << endl;
#endif

             // read list of dependent funcs/comps
             CompFile >> rdBuf;
             rdStr = rdBuf;
             if (strcmp(rdStr.c_str(),"null") != 0)
             {
                char *cstr, *tokTmp1;
                cstr = new char[rdStr.size()+1];
                strcpy(cstr, rdStr.c_str());
                tokTmp1 = strtok(cstr,",");

                while (tokTmp1 != NULL)
                {
#if DEBUG >= 5
                   cerr << "read_comp-19 dep-func/comp: " << tokTmp1 << endl;
#endif
                   AX_FuncBlkLCIter fTemp;
                   string fTmpS = tokTmp1;

                   /* Alan - 2012.9.15, adding v02 feature
                      For a software component, its first item in
                      the dependent list should always be the processor
                      family it runs on. Thus, get all the processor
                      family when (*fcIter).m_fncblkPL is of size 0
                   */
                   if (((*fcIter).get_type() == SOFTWARE) &&
                       ((*fcIter).get_no_fncblks() == 0))
                   {
                      if (!set_proc_family(fTmpS, &(*fcIter)))
                         cerr << "Cannot find processor(s) of family "
                              << fTmpS << " for " << (*fcIter).get_name()
                              << endl;
                      fTemp = fcIter;
                   }
                   else
                   {
                      fTemp = create_new_fncblk(fTmpS);
                      (*fcIter).add_new_fncblk(&(*fTemp));
                   }

                   // if this is a memory capacity needed
                   if (strncmp("MEM_",tokTmp1,4) == 0)
                      (*fTemp).set_functype(MEMREQ);

                   tokTmp1 = strtok(NULL,",");
                }
             }

             // read list of user attributes
             CompFile >> rdBuf;
             rdStr = rdBuf;
             while (strcmp(rdStr.c_str(),"Comp") != 0 &&
                    strcmp(rdStr.c_str(),"Port") != 0 &&
                    strncmp(rdStr.c_str(),"//",2) != 0)
             {
#if DEBUG >= 5
                cerr << "read_comp-20 uattr: " << rdStr << endl;
#endif
                AX_UAttrC uattr;
                uattr.parse_attr(rdStr);
                (*fcIter).add_new_uattr(uattr);
                CompFile >> rdBuf;
                rdStr = rdBuf;
             }
             continue;
          }
       }
       else if (strcmp(rdStr.c_str(),"Port") == 0)
       {
#if DEBUG >= 5
             cerr << "read_comp-0 label: " << rdStr << endl;
#endif
          AX_CompPortC port;
          AX_CompPortLCIter pIter;
          string compName, catName, portName;

          CompFile >> rdBuf;
          catName = rdBuf;
#if DEBUG >= 5
          cerr << "read_comp-21 port category: " << catName << endl;
#endif
          CompFile >> rdBuf;
          compName = rdBuf;
#if DEBUG >= 5
          cerr << "read_comp-22 component name: " << compName << endl;
#endif

          // read port name
          CompFile >> rdBuf;
          rdStr = rdBuf;
          port.set_name(rdStr);
#if DEBUG >= 5
          cerr << "read_comp-23 port name: " << rdStr << endl;
#endif

          // read direction
          CompFile >> rdBuf;
          rdStr = rdBuf;
          if (strcmp(rdStr.c_str(),"In") == 0)
             port.set_type(PORT_IN);
          else if (strcmp(rdStr.c_str(),"Out") == 0)
             port.set_type(PORT_OUT);
          else if (strcmp(rdStr.c_str(),"Bi") == 0)
             port.set_type(PORT_BI);
          else
             port.set_type(PORT_TYPE_UNKNOWN);
#if DEBUG >= 5
          cerr << "read_comp-24 direction: " << rdStr << endl;
#endif

          // read no of bits per sample
          CompFile >> rdBuf;
          rdStr = rdBuf;
          int iNum = atoi(rdStr.c_str());
          port.set_sample_size(iNum);
#if DEBUG >= 5
          cerr << "read_comp-25 sample size: " << rdStr << endl;
#endif

          // read list of sample rates
          CompFile >> rdBuf;
          rdStr = rdBuf;

          char *cstr, *tokStr;
          cstr = new char[rdStr.size()+1];
          strcpy(cstr, rdStr.c_str());
          tokStr = strtok(cstr,",");

          while (tokStr != NULL)
          {
             iNum = atoi(tokStr);
             port.add_new_sample_rate(iNum);
             tokStr = strtok(NULL,",");
          }
#if DEBUG >= 5
          cerr << "read_comp-26 sample rates: " << rdStr << endl;
#endif

          // read list of interfaces
          CompFile >> rdBuf;
          rdStr = rdBuf;
          if (strcmp(rdStr.c_str(),"null") != 0)
          {
             char *tokTmp;

             cstr = new char[rdStr.size()+1];
             strcpy(cstr, rdStr.c_str());
             tokTmp = strtok(cstr,",");

             while (tokTmp != NULL)
             {
#if DEBUG >= 5
                cerr << "read_comp-27 interface: " << tokTmp << endl;
#endif
                AX_InterfaceLCIter nTemp;
                string tokS = tokTmp;
                nTemp = create_new_intrfc(tokS);
                port.add_new_intrfc(&(*nTemp));
                tokTmp = strtok(NULL,",");
             }
          }

          AX_CompC * compP;
          if (strcmp(catName.c_str(),"FuncBlk") == 0)
          {
             AX_FuncBlkLCIter fTemp;
             fTemp = create_new_fncblk(compName);
             compP = (AX_CompC *)(&(*fTemp));
             port.set_comp(compP);
             pIter = add_new_compport(port);
             if (port.get_type() == PORT_IN)
                (*fTemp).add_new_inport(&(*pIter));
             else if (port.get_type() == PORT_OUT)
                (*fTemp).add_new_outport(&(*pIter));
             else if (port.get_type() == PORT_BI)
                (*fTemp).add_new_ioport(&(*pIter));
             if ((*fTemp).get_type() == SOFTWARE)
             {
                // get the sw_port info
                CompFile >> rdBuf;
                rdStr = rdBuf;
                if (strcmp(rdStr.c_str(),"null") != 0)
                {
                   char *tokTmp;

                   cstr = new char[rdStr.size()+1];
                   strcpy(cstr, rdStr.c_str());
                   tokTmp = strtok(cstr,",");

                   string tokS = tokTmp;
                   AX_FuncBlkLCIter procI;
                   //procI = create_new_fncblk(tokS);
                   procI = get_first_fncblk_by_func(tokS);
                   if (procI == m_fncblkL.end())
                   {
                      cerr << "dsnread:: Cannot find " << tokTmp
                           << " in the library\n";
                      return false;
                   }
                   tokTmp = strtok(NULL,",");
                   AX_CompPortLCIter pPortI;
                   tokS = tokTmp;
                   pPortI = get_compport(&(*procI), tokS);
                   if (pPortI == compport_list_end())
                   {
                      cerr << "dsnread:: Cannot find "
                           << (*procI).get_name()
                           << "." << tokTmp
                           << " in the library\n";
                      return false;
                   }
                   (*pIter).set_sw_port(&(*pPortI));
#if DEBUG >= 5
                cerr << "read_comp-28 FuncBlk SW port:"
                     << (*procI).get_name() << "." << (*pPortI).get_name() << endl;
#endif
                } // end if rdstr == null
             } // end if fTempP == SOFTWARE
          }

          CompFile >> rdBuf;
          rdStr = rdBuf;
       }
    }

    return(true);
}
