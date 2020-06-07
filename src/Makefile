#
#  Makefile for Architecture Explorer
#
#  by Alan Su, OddPoint
#  alansuphd@gmail.com
#


CC = g++
CFLAG = -O2 -Wall -static -static-libgcc -c
OFLAG = -O2 -Wall
PCFLAG = -O2 -Wall -c
POFLAG = -O2 -Wall
HDR = enum.h type.h 
OBJ = comp.o compport.o bus.o interface.o connpnt.o funcblk.o task.o \
      taskport.o conn.o uattr.o share.o util.o gen_sc.o taskgraph.o archgraph.o \
      platform.o dsnspec.o dsn.o schedule.o evolve.o dsnread.o synthesis.o \
      main.o
LIB = -static-libgcc
TARGET = ArchExp
PROJECT_HOME = .

all: $(OBJ) $(TARGET)
comp.o: comp.cpp comp.h $(HDR)
	$(CC) $(CFLAG) $(@:.o=.cpp)
compport.o: compport.cpp compport.h $(HDR) comp.h bus.h interface.h
	$(CC) $(CFLAG) $(@:.o=.cpp)
bus.o: bus.cpp bus.h $(HDR) interface.h comp.h archgraph.h connpnt.h
	$(CC) $(CFLAG) $(@:.o=.cpp)
interface.o: interface.cpp interface.h $(HDR) uattr.h comp.h platform.h
	$(CC) $(CFLAG) $(@:.o=.cpp)
connpnt.o: connpnt.cpp connpnt.h $(HDR) comp.h archgraph.h compport.h
	$(CC) $(CFLAG) $(@:.o=.cpp)
funcblk.o: funcblk.cpp funcblk.h $(HDR) uattr.h task.h comp.h interface.h \
           platform.h compport.h
	$(CC) $(CFLAG) $(@:.o=.cpp)
task.o: task.cpp task.h $(HDR) taskgraph.h taskport.h funcblk.h
	$(CC) $(CFLAG) $(@:.o=.cpp)
taskport.o: taskport.cpp taskport.h $(HDR) conn.h taskgraph.h task.h
	$(CC) $(CFLAG) $(@:.o=.cpp)
conn.o: conn.cpp conn.h $(HDR) task.h taskport.h taskgraph.h
	$(CC) $(CFLAG) $(@:.o=.cpp)
uattr.o: uattr.cpp uattr.h $(HDR)
	$(CC) $(CFLAG) $(@:.o=.cpp)
share.o: share.cpp share.h $(HDR) funcblk.h task.h
	$(CC) $(CFLAG) $(@:.o=.cpp)
util.o: util.cpp util.h
	$(CC) $(CFLAG) $(@:.o=.cpp)
gen_sc.o: gen_sc.cpp gen_sc.h
	$(CC) $(CFLAG) $(@:.o=.cpp)
taskgraph.o: taskgraph.cpp taskgraph.h $(HDR) task.h taskport.h conn.h
	$(CC) $(CFLAG) $(@:.o=.cpp)
archgraph.o: archgraph.cpp archgraph.h $(HDR) bus.h compport.h funcblk.h \
             interface.h platform.h task.h share.h util.h
	$(CC) $(CFLAG) $(@:.o=.cpp)
platform.o: platform.cpp platform.h $(HDR) uattr.h comp.h interface.h
	$(CC) $(CFLAG) $(@:.o=.cpp)
dsnspec.o: dsnspec.cpp dsnspec.h $(HDR) dsnspec.h funcblk.h interface.h platform.h \
       taskgraph.h archgraph.h
	$(CC) $(CFLAG) $(@:.o=.cpp)
dsn.o: dsn.cpp dsn.h $(HDR) dsnspec.h funcblk.h interface.h platform.h \
       util.h taskgraph.h archgraph.h
	$(CC) $(CFLAG) $(@:.o=.cpp)
schedule.o: schedule.cpp schedule.h $(HDR) extern.h util.h dsn.h 
	$(CC) $(CFLAG) $(@:.o=.cpp)
evolve.o: evolve.cpp evolve.h $(HDR) util.h dsn.h share.h schedule.h
	$(CC) $(CFLAG) $(@:.o=.cpp)
dsnread.o: dsnread.cpp $(HDR) dsn.h
	$(CC) $(CFLAG) $(@:.o=.cpp)
synthesis.o: synthesis.cpp $(HDR) synthesis.h evolve.h dsn.h
	$(CC) $(CFLAG) $(@:.o=.cpp)
main.o: main.cpp $(HDR) synthesis.h evolve.h
	$(CC) $(CFLAG) $(@:.o=.cpp)
ArchExp: $(OBJ)
	$(CC) $(OFLAG) $(LIB) $(OBJ) -o $@

bin:
	cp $(TARGET) $(PROJECT_HOME)/../bin

clean:
	rm -f *.o
	rm -f $(TARGET)
