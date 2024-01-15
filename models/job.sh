#!/usr/bin/env bash
#SBATCH --account=rrg-jeon-ac
#SBATCH --job-name=smoothHydro
#SBATCH --time=12:30:00
#SBATCH --cpus-per-task=1
#SBATCH --mem-per-cpu=3G
#SBATCH --mail-type=ALL
#SBATCH --mail-user=rouzbeh.modarresi-yazdi@mail.mcgill.ca
#SBATCH --output=./out.dat

source /home/rmyazdi/scratch/smooth_hydro/script.sh
cd /home/rmyazdi/scratch/smooth_hydro/hic-eventgen/local/testvenv/bin/
source activate

cd /home/rmyazdi/scratch/smooth_hydro/hic-eventgen/models

outerLoc=$PWD
for syst in "AuAu200" "PbPb2760" "PbPb5020"
    do
        innerLoc=$PWD
        for cent in "00-05" "05-10" "10-20" "20-30" "30-40"
            do
                echo "System: " $syst " and centrality: " $cent
                loc="$syst-results/$syst-Avg-$cent"
                mkdir -p $loc
                cp run-events ./$loc
                cp run-events-wrapper ./$loc
                parameterFileName="${syst}_Avg_${cent}"
                cp ./inputs/${parameterFileName} ./$loc
                cd $loc
                touch "${parameterFileName}.dat"
                resultfile=$PWD/"result.dat"
                $PWD/run-events-wrapper
                ./run-events @${parameterFileName} $resultfile > "$parameterFileName.out"
                cd ${innerLoc}
             done
     cd ${outerLoc}
     done
