#!/bin/sh
#SBATCH --nodes=2
#SBATCH --ntasks=64
#SBATCH --ntasks-per-node=32
#SBATCH --account=snap
#SBATCH --mail-type=FAIL
#SBATCH --mail-user=username@alaska.edu
#SBATCH --output=/path/to/output/alfresco_slurm_%j.out
#SBATCH -p main

echo Start slurm && date

export PATH=$PATH:/usr/lib64/openmpi/bin

# This assumes that fresco-api is built and installed locally in ~/local/bin
# See README.md for build and install instructions.
export PATH=$HOME/local/bin:$PATH

# Running without the --nostats option causes ALFRESCO to segfault.
mpirun fresco-mpi -f alfresco.json --nostats

echo End slurm && date
