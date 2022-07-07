# Boreal ALFRESCO (Alaska Frame-Based Ecosystem Code)

## About

An understanding of the processes that control wildland fuel accumulation, including the roles played by climate change and fire management activities, is crucial for designing wildland management policies. Boreal ALFRESCO simulates the responses of subarctic and boreal vegetation to transient climatic changes. The model assumptions reflect the hypothesis that fire regime and climate are the primary drivers of landscape-level changes in the distribution of vegetation in the circumpolar arctic/boreal zone. Furthermore, it assumes that vegetation composition and continuity serve as a major determinant of large, landscape-level fires. Boreal ALFRESCO operates on an annual time step, in a landscape composed of 1 × 1 km pixels, a scale appropriate for interfacing with mesoscale climate and carbon models. The model simulates five major subarctic/boreal ecosystem types: upland tundra, black spruce forest, white spruce forest, deciduous forest, and grassland-steppe. These ecosystem types represent a generalized classification of the complex vegetation mosaic characteristic of the circumpolar arctic and boreal zones of Alaska. SNAP climate data can be used as ALFRESCO inputs, thus creating projections of the impacts of changing climate on fire regime. 

## Build process

The following instructions show how to build the ALFRESCO code for either global or local installation on Atlas.

First, you will need to get onto a compute node. Unlike the login nodes that run CentOS 6, the compute nodes run
CentOS 7. This code will need to be compiled from a CentOS 7 system.


To log into atlas15 for example:

```
$ srun -p main -N 1 -n 32 -w atlas15 --pty /bin/bash
```

Download, build, and install version 1.9.0 of the POCO C++ Libraries. Newer versions of the POCO C++ Libraries do not appear to be compatible with the C++ compiler installed on Atlas, so an older version is chosen deliberately here:

```
wget 'https://github.com/pocoproject/poco/archive/refs/tags/poco-1.9.0-release.tar.gz'
tar zxvf poco-1.9.0-release.tar.gz
cd poco-poco-1.9.0-release
./configure --prefix=$HOME/local
make
make install
```

From the compute node, clone this repo:

```
$ git clone git@github.com:ua-snap/alfresco.git
$ cd alfresco
```

Set the following environment variables to pick up the MPI C++ compiler and executables:

```
$ export CXX=/usr/lib64/openmpi/bin/mpic++
$ export PATH=$PATH:/usr/lib64/openmpi/bin
$ export CPLUS_INCLUDE_PATH=$HOME/local/include
$ export LD_LIBRARY_PATH=$HOME/local/lib
```

Run `autogen.sh` to generate the `configure` script that will be needed in the next step:

```
$ ./autogen.sh
```

Run the `configure` script. Omit the `--prefix=$HOME/local` option if you intend to install alfresco globally:

```
$ ./configure --prefix=$HOME/local
```

Run `make` to compile the code:

```
$ make
```

Run `make install` to install the executables in `$HOME/local` (specified with the `--prefix` option above), or globally if the `--prefix` option was omitted. MPI and non-MPI versions of the ALFRESCO executable should now be present in this directory. For example:

```
$ ls -1 $HOME/local/bin
fresco
fresco-mpi
```

## Running

At appears that previous versions of ALFRESCO read run parameters from `*.fif` files, of which there are many examples in the `Runs/TestCases/FIF` and `/Runs/Demo/FIF`. However, the ALFRESCO executables now expect parameters to be provided from `*.json` files. An example JSON parameter file is provided in `Runs/Demo/JSON`. The FIF files have been preserved for reference.


To run a locally installed version of ALFRESCO, make sure the install diretory is in your PATH. For example:

```
$ export PATH=$PATH:$HOME/local/bin
```

To run the non-MPI version of ALFRESCO:

```
$ ./fresco -f Alfresco.JSON
```

To run the MPI version of ALFRESCO, run the following from a Slurm script/job:

```
$ mpiexec fresco-mpi -f Alfresco.JSON
```