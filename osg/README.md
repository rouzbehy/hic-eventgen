# Open Science Grid (OSG)

See https://support.opensciencegrid.org for general information.

## Building

The [makepkg](makepkg) script creates a package `hic-osg.tar.gz` containing all the model code to be distributed to each job.
Run it on OSG submit host such as XSEDE (xd-login.opensciencegrid.org) or Duke CI-connect (duke-login.osgconnect.net)

## Submitting jobs

The script [hic-wrapper](hic-wrapper) is the Condor executable.
It sets environment variables, calls `run-events`, and copies the results file to the final destination via GridFTP (see [data transfer](#data-transfer) below).

Each job runs 10 events.

The script [submit](submit) generates the Condor job files and submits them.
__Please read the script and understand what it's doing.
In particular, check the GridFTP destination and OSG project name.__

`submit` uses a Condor DAG for managing jobs.
The DAG smoothly submits jobs to the queue and throttles the total number of idle jobs, so that many jobs can be submitted at once without overloading the queue.

The script is designed for submitting many jobs for each of a set of input files.
Its usage is

    submit batch_label jobs_per_input_file input_files...

where

- `batch_label` is a human-readable label which also sets the destination folder for all job results files in this batch
- `jobs_per_input_file` is the number of jobs to run for each input file
- `input_files...` are the paths to each input file to run

For example, I have a set of input files for running LHC events in `~/inputs/lhc`.
To run 10,000 events (1000 jobs) for each input file:

    ./condor/submit lhc 1000 inputs/lhc/*

## Data transfer

The data files are returned to the submitting directory. 
On OSGCONNECT, the submitting directory resides in the /stash/user/$USER directory. It offers a long term data storage, and it is also an endpoint of globus cloud service.
On OSGXSEDE, one may use the old globus toolkit.

