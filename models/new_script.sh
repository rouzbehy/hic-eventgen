#!/usr/bin/env bash
#SBATCH --account=rrg-jeon-ac
#SBATCH --job-name=smoothHydro
#SBATCH --time=15:30:00
#SBATCH --cpus-per-task=1
#SBATCH --mem-per-cpu=3G
#SBATCH --mail-type=ALL
#SBATCH --mail-user=rouzbeh.modarresi-yazdi@mail.mcgill.ca
#SBATCH --output=./out.dat

source /home/rmyazdi/scratch/smooth_hydro/script.sh
cd /home/rmyazdi/scratch/smooth_hydro/hic-eventgen/local/testvenv/bin/
source activate

cd /home/rmyazdi/scratch/smooth_hydro/hic-eventgen/models

$PWD/run-events-wrapper
for syst in "AuAu200" "PbPb2760" "PbPb5020"
    do
        innerLoc=$PWD
        for cent in "00-05" "05-10" "10-20" "20-30" "30-40" "40-50"
            do
                echo "System: " $syst " and centrality: " $cent
                loc="$syst-results/$syst-Avg-$cent"
                mkdir -p $loc
                parameterFileName="input_${syst}_${cent}.dat"
                cp ./new_inputs/${parameterFileName} ./
                resultfile=$PWD/"result.dat"
                ./run-events @${parameterFileName} $resultfile > "code_output_${syst}_${cent}.out"
                ## move the results:
                echo "  Done with the calculation. We have: "
                ls -lhtS
                echo "  Move all and prepare for the next calc."
                mv EventPlanesFrzout.dat ./${loc}/
                mv initial.hdf5 ./${loc}/
                mv final_hadrons.hdf ./${loc}/
                mv JetData.h5 ./${loc}/
                mv ${parameterFileName} ./${loc}/
                mv surface.dat ./${loc}/
                mv result.dat ./${loc}/
                mv "code_output_${syst}_${cent}.out" ./${loc}/
             done
     done
