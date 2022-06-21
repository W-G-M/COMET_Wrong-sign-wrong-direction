#!/bin/sh

export SH_NUM_EVENTS=2000
export SH_RUN_NUMBER=0
export SH_SUBRUN=1
export SH_RANDOM_SEED=327682
export ICEDUSTCONTROLPATH=$ICEDUSTINSTALLPATH/IcedustControl

# mkdir -p workdir_test_electron
# cd workdir_test_electron
# $ICEDUSTCONTROLPATH/app/RunIcedustControl -c ../config/particleGun_electron.cfg

mkdir -p workdir_test_positron
cd workdir_test_positron
$ICEDUSTCONTROLPATH/app/RunIcedustControl -c ../config/particleGun_positron.cfg
