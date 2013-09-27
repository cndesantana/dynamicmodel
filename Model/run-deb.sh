#/bin/bash

#!/bin/bash 
seed=17
#seed=1007
rm SOC*.dat
rm erro
rm AverIndInTime*.dat
rm FoodWeb_seed*.net
rm realMig*.dat
touch erro
#tail -f erro &
#50 realizations
while [  $seed -lt 37 ]; do
	echo The seed is $seed
#while [  $seed -lt 50 ]; do
# 	rm SOC*.dat
#	rm out*.dat
#	rm over*.net

#NITE      - Number of Iterations
#FWNF      - Food-Web Network File
#SNNF      - Spatial Neighborhood Network File
#TM        - Time for Migration
#TCN       - Time for Generate Coexistence Networks
#SEED      - Seed for Random Function
#SHOW-EACH - Time for Output
#SAVE-EACH - Time for Partial Saved File
#EXIST_THR - Minimal threshold above which the species is considered as alive in the site.	
#./fweb 101 fwf.net snnf.net 1 2000 $seed 1 100 0.05 2> erro > /dev/null #&
./fweb 1001 fwf.net snnf.net 1 2000 $seed 2 500 0.05 2> erro > /dev/null #&
#./fweb 1000 fwf.net snnf.net 1 2000 $seed 1 1000 0.05 2>> erro > /dev/null &
# tail -f erro
	seed=$((seed+10)) 
done


