# ArchExp
Architecture Exploration, an ESL synthesis tool that takes in a consistent Synchronous Data Flow Graph (SDF), a library and a cost function. The synthesis result is a set of architectures with least costs and a matching set of schedule. The synthesis heuristic is genetic algorithm.
There are three folders, src, bin, and projects.

src:
This directory contains the source code with a Makefile. First run 'make' to
compile the engine, ArchExp. After the compilation is done run 'make bin'
to copy ArchExp to ../bin.

bin:
This directory is where ArchExp resides.

projects:
In this directory there is the Edge project. For each project, it needs to have
these directories: archgraphs, data, datasmall, lib, spec, and SystemC:

archgraphs: a new directory created to place architecture graphs synthesized
data      : contains the complete synthesized architecture detail descriptions
datasmall : contains the abstracted synthesized architecture descriptions
lib       : the component library
spec      : where the task graph, spec and control files are
SystemC   : SystemC codes of synthesized architectures

Other directories are pre-mature experiments.

A Makefile is provided to run ArchExp. Please run "make dall" or "make p100"
to see how they go.

An example to run ArchExp goes:
../../bin/ArchExp EDGE_spec EDGE_complib_dall EDGE_taskgraph EDGE_control_f0

The format is
path-to-ArchExp spec_file complib_file taskgraph_file control_file

Where complib_file must resides in lib/ directory, and spec_file,
taskgraph_file and control_file must reside in spec directory.
