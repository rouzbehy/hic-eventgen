source /home/rmyazdi/scratch/smooth_hydro/script.sh
cd /home/rmyazdi/scratch/smooth_hydro/hic-eventgen/local/testvenv/bin/
source activate

cd /home/rmyazdi/scratch/smooth_hydro/hic-eventgen/models


#export PYTHONPATH="${PYTHONPATH}:/home/rmyazdi/scratch/smooth_hydro/hic-eventgen/models/frzout"

outerLoc=$PWD
for syst in "AuAu200" "PbPb2760" "PbPb5020"
    do
        innerLoc=$PWD
        for cent in "00-05" "05-10" "10-20" "20-30" "30-40"
            do
                echo "\tSystem: " $syst " and centrality: " $cent
                loc="$syst-results/$syst-Avg-$cent"
                mkdir -p $loc
                #echo "\tMade folder ${loc}"
                cp run-events ./$loc
                cp run-events-wrapper ./$loc
                parameterFileName="${syst}_Avg_${cent}"
                cp ./inputs/${parameterFileName} ./$loc
                cd $loc
                #echo "\t$PWD"
                touch "${parameterFileName}.dat"
                resultfile=$PWD/"result.dat"
                $PWD/run-events-wrapper
                ./run-events @${parameterFileName} $resultfile > "$parameterFileName.out"
                cd ${innerLoc}
                #echo "\t$PWD"
             done
     cd ${outerLoc}
     done
