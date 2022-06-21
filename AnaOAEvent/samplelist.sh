#!/bin/bash -l
 
#SBATCH --partition=cpu-epyc
#SBATCH --output=%x.%j.out
#SBATCH --error=%x.%j.err
#SBATCH --mem=4G
#SBATCH --ntasks=1
#SBATCH --nodes=1


# i=0

for i in {10..99}; do

for file in lustre/user/gmwong/basic_studies-master-CyDetWrongSign/CyDetWrongSign/SimG4CyDet/PartGunElecFromDisk/workdir_electron/Job_electron_"$i"/; do

cd /lustre/user/gmwong/basic_studies-master-CyDetWrongSign/CyDetWrongSign/SimG4CyDet/PartGunElecFromDisk/workdir_electron/Job_electron_"$i"/

cp -r  oa_g4_em_00000000-00"$i"_*.root  /lustre/user/gmwong/basic_studies-master-CyDetWrongSign/CyDetWrongSign/SimG4CyDet/PartGunElecFromDisk/workdir_electron

# cp -r  oa_g4_em_00000000-000"$i"_*.root  ../workdir_electron

# cp -r  /lustre/user/gmwong/basic_studies-master-CyDetWrongSign/CyDetWrongSign/SimG4CyDet/PartGunElecFromDisk/workdir_electron/Job_electron_"$i"/oa_g4_em_00000000-000"$i"_*.root ../workdir_electron

# for file in lustre/user/gmwong/basic_studies-master-CyDetWrongSign/CyDetWrongSign/SimG4CyDet/PartGunElecFromDisk/workdir_electron/Job_electron_"$i"/oa_g4_em_00000000-000"$i"_*.root; do

# ls test.txt > lustre/user/gmwong/basic_studies-master-CyDetWrongSign/CyDetWrongSign/SimG4CyDet/PartGunElecFromDisk/workdir_electron/Job_electron_"$i"/oa_g4_em_00000000-000"$i"_*.root

done 

done