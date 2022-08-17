# Boreal ALFRESCO (Alaska Frame-Based Ecosystem Code)

## About

An understanding of the processes that control wildland fuel accumulation, including the roles played by climate change and fire management activities, is crucial for designing wildland management policies. Boreal ALFRESCO simulates the responses of subarctic and boreal vegetation to transient climatic changes. The model assumptions reflect the hypothesis that fire regime and climate are the primary drivers of landscape-level changes in the distribution of vegetation in the circumpolar arctic/boreal zone. Furthermore, it assumes that vegetation composition and continuity serve as a major determinant of large, landscape-level fires. Boreal ALFRESCO operates on an annual time step, in a landscape composed of 1 × 1 km pixels, a scale appropriate for interfacing with mesoscale climate and carbon models. The model simulates five major subarctic/boreal ecosystem types: upland tundra, black spruce forest, white spruce forest, deciduous forest, and grassland-steppe. These ecosystem types represent a generalized classification of the complex vegetation mosaic characteristic of the circumpolar arctic and boreal zones of Alaska. SNAP climate data can be used as ALFRESCO inputs, thus creating projections of the impacts of changing climate on fire regime. 

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

Output from each ALFRESCO replicate will show up in the `logs` directory. Output GeoTIFFs will show up in the `Maps` directory, separated into subdirectories for each year.

## Using historical fire data

Historical fire polygon data can be downloaded from the [Maps/Imagery/Geospatial page of the AICC website](https://fire.ak.blm.gov/predsvcs/maps.php) as a file geodatabase (see "AlaskaFireHistory_Polygons_1940_*"). Polygons from each year of the file geodatabase can be extracted, rasterized, and used as historical fire inputs for ALFRESCO.

The following Bash command will created GeoTIFFs from the AICC fire history polygons geodatabase for each year from 1950 to 2021, for example:

```
for i in {1950..2021}; do \
gdal_rasterize AlaskaFireHistory_Polygons.gdb ~/FireHistoryYears/AlaskaFireHistory_Polygons_${i}.tif \
-a_srs EPSG:3338 -burn 255 -ts 5528 2223 -te -1725223.205807 321412.933 3802776.794 2544412.932644 \
-ot Byte -sql "SELECT * FROM AK_fire_location_polygons WHERE FIREYEAR = '${i}'"; \
done
```

To input these GeoTIFFs into ALFRESCO, make sure the historical transition type and year are set properly in the JSON configuration, and the historical fire GeoTIFF base path is set. For example:

```
{
    ...
    "Fire": {
        "Types": ["SPATIAL", "HISTORICAL", "SPATIAL"],
        "TypeTransitionYears": [1000, 1950, 2022],
        "Historical": ["", "/atlas_scratch/username/AlaskaFireHistory/AlaskaFireHistory_Polygons.tif", ""],
        ...
    }
    ...
}
```