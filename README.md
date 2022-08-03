# Boreal ALFRESCO (Alaska Frame-Based Ecosystem Code)

## About

ALFRESCO was originally developed to simulate the response of subarctic vegetation to a changing climate and disturbance regime (Rupp et al. 2000a, 2000b). Previous research has highlighted both direct and indirect (through changes in fire regime) effects of climate on the expansion rate, species composition, and extent of treeline in Alaska (Rupp et al. 2000b, 2001, Lloyd et al. 2003). Additional research, focused on boreal forest vegetation dynamics, has emphasized that fire frequency changes – both direct (climate-driven or anthropogenic) and indirect (as a result of vegetation succession and species composition) – strongly influence landscape-level vegetation patterns and associated feedbacks to future fire regime (Rupp et al. 2002, Chapin et al. 2003, Turner et al. 2003). A detailed description of ALFRESCO can be obtained from the literature (Rupp et al. 2000a, 200b, 2001, 2002). The boreal forest version of ALFRESCO was developed to explore the interactions and feedbacks between fire, climate, and vegetation in interior Alaska (Rupp et al. 2002, 2007, Duffy et al. 2005, 2007) and associated impacts to natural resources (Rupp et al. 2006, Butler et al. 2007).

ALFRESCO is a state-and-transition model of successional dynamics that explicitly represents the spatial processes of fire and vegetation recruitment across the landscape (Rupp et al. 2000a). ALFRESCO does not model fire behavior, but rather models the empirical relationship between growing-season climate (e.g., average temperature and total precipitation) and total annual area burned (i.e., the footprint of fire on the landscape). ALFRESCO also models the changes in vegetation flammability that occurs during succession through a flammability coefficient that changes with vegetation type and stand age (Chapin et al. 2003).

The fire regime is simulated stochastically and is driven by climate, vegetation type, and time since last fire (Rupp et al. 2000a, 2007). ALFRESCO employs a cellular automaton approach, where an ignited pixel may spread to any of the eight surrounding pixels. ‘Ignition’ of a pixel is determined using a random number generator and as a function of the flammability value of that pixel. Fire ‘spread’ depends on the flammability of the receptor pixel and any effects of natural firebreaks including non-vegetated mountain slopes and large water bodies, which do not burn.

Version 1.0.1 can operate at any time step and pixel resolution, however the current model calibration and parameterization was conducted at an annual time step and 1 km2 pixel resolution. A 30 m2 calibration and parameterization is currently underway. Other model developments include refined tundra transition stages, fire suppression effects on fire size, simulated fire severity patterns and fire severity effects on successional rates and trajectories.

## Build process

The following instructions show how to build the ALFRESCO code for either global or local installation on Atlas.

First, you will need to get onto a compute node. Unlike the login nodes that run CentOS 6, the compute nodes run
CentOS 7. This code will need to be compiled from a CentOS 7 system. Run the following command to log into a compute node:

```
$ srun -p main -N 1 -n 32 --pty /bin/bash
```

Then, clone this repo:

```
$ git clone git@github.com:ua-snap/alfresco.git
$ cd alfresco
```

Set the following environment variables to pick up the MPI C++ compiler and executables:

```
$ export CXX=/usr/lib64/openmpi/bin/mpic++
$ export PATH=$PATH:/usr/lib64/openmpi/bin
```

Run `autogen.sh` to generate the `configure` script that will be needed in the next step:

```
$ ./autogen.sh
```

This will output a handful of warnings that you can ignore as long as a `configure` file is successfully generated. Run the `configure` script. Omit the `--prefix=$HOME/local` option if you intend to install ALFRESCO globally:

```
$ ./configure --prefix=$HOME/local
```

Run `make` to compile the code:

```
$ make
```

Install the executables:

```
$ make install
```

This will install the executables in `$HOME/local` (specified with the `--prefix` option above), or globally if the `--prefix` option was omitted. MPI and non-MPI versions of the ALFRESCO executable should now be present in this directory. For example:

```
$ ls -1 $HOME/local/bin
fresco
fresco-mpi
```

## Running

Previous versions of ALFRESCO read their run configurations from FIF files, of which there are many examples in the `Deprecated/Runs/TestCases` and `Deprecated/Runs/Demo` directories. However, ALFRESCO executables built from the latest code base now read their run configurations from JSON files. FIF files are no longer compatible with the latest code base but are perserved in the `Deprecated` directory for reference.

An example JSON parameter file is provided in `examples/alfresco.json`. Before running ALFRESCO, make sure to update the following line in `alfresco.json` with the directory where you would like the output GeoTIFFs to be written:

```
{
  ...
  "PathSetup": {	
    ...
    "ClientOutputBasePath": "/atlas_scratch/username/run/directory",
    ...
  }
  ...
},
```

### Running ALFRESCO interactively on one cluster node

To run a locally installed version of ALFRESCO interactively on a single node, first make sure you are logged into a compute node:

```
$ srun -p main -N 1 -n 32 --pty /bin/bash
```

Then, make sure the local ALFRESCO install directory is in your PATH. For example:

```
$ export PATH=$PATH:$HOME/local/bin
```

Then run the non-MPI version of ALFRESCO:

```
$ fresco -f alfresco.json
```

Output from each ALFRESCO replicate will show up in the `logs` directory.

### Running ALFRESCO on multiple cluster nodes

To run across multiple cluster nodes, you will need to run the MPI version of ALFRESCO submitted via a Slurm script. An example Slurm script is included in `examples/alfresco.slurm`. Change these two lines of the Slurm script to reflect your email address and where you would like the Slurm output to be written:

```
#SBATCH --mail-user=username@alaska.edu
#SBATCH --output=/path/to/output/alfresco_slurm_%j.out
```

Make sure the `alfresco.json` run configuration file is in the same directory as `alfresco.slurm`. If you have renamed the `alfresco.json` file or put it in a different directory, make sure these changes are reflected in the `alfresco.slurm` script. Then submit the Slurm job like this:

```
$ sbatch alfresco.slurm
```

You can check the status of the Slurm job like this:

```
$ squeue
```

Output from each ALFRESCO replicate will show up in the `logs` directory.