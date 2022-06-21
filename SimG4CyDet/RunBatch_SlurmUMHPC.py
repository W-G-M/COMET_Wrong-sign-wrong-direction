import sys
import glob
import os
import subprocess

cwd = os.getcwd()
#
# NOTE: Specify your own ICEDUST setup script
#
setupEnvPath = "/home/user/gmwong/comet/icedust/setupICEDUST_UMHPC.sh"
#
#
#
NEVENTS=10000 #Each skimmed rootracker files should have about less than 2000 events. 
#
#
#
ICEDUST_RUNNUMBER=0
ICEDUST_SUBRUN_MIN=0
ICEDUST_SUBRUN_MAX=100
#
#
#
JOBS = [
  ("electron",     "./config/particleGun_electron.cfg",  "workdir_electron"),
  ("positron",     "./config/particleGun_positron.cfg",  "workdir_positron"),
]

#
# Make the bash script
#
for job in JOBS:
  JOB_NAME = job[0]
  CFG_FILE = job[1]
  WORK_DIR = job[2]
  #
  # Make the bash script
  #
  for iSubRun in xrange(ICEDUST_SUBRUN_MIN, ICEDUST_SUBRUN_MAX):
    #
    # Specify random seed.
    #
    rndmSeed = (5<<16)+2+iSubRun
    #
    #
    #
    SUBJOB_DIR = 'Job_{:s}_{:d}'.format(JOB_NAME,iSubRun)
    #
    #
    #
    fName      = 'Job_{:s}_{:d}.sh'.format(JOB_NAME,iSubRun)
    f = open(fName, 'w')
    f.write('#!/bin/bash -l\n')  
    f.write('#SBATCH --partition=cpu-epyc\n')
    f.write('#SBATCH --output=BatchLog/%x.%j.out\n')
    f.write('#SBATCH --error=BatchLog/%x.%j.err\n')
    f.write('#SBATCH --mem=4G\n')
    f.write('#SBATCH --ntasks=1\n')
    f.write('#SBATCH --nodes=1\n')
    f.write('\n')
    f.write('cd {:s} \n'.format(cwd))
    f.write('source {:s} \n'.format(setupEnvPath))
    f.write('export SH_NUM_EVENTS={:s}\n'.format(str(NEVENTS)))
    f.write('export SH_RUN_NUMBER={:s}\n'.format(str(ICEDUST_RUNNUMBER)))
    f.write('export SH_SUBRUN={:s}\n'.format(str(iSubRun)))
    f.write('export SH_RANDOM_SEED={:s}\n'.format(str(rndmSeed)))
    f.write('export ICEDUSTCONTROLPATH=$ICEDUSTINSTALLPATH/IcedustControl\n')
    f.write('mkdir -p {:s} \n'.format(WORK_DIR))
    f.write('cd {:s} \n'.format(WORK_DIR))
    f.write('mkdir -p {:s} \n'.format(SUBJOB_DIR))
    f.write('cd {:s} \n'.format(SUBJOB_DIR))
    command = '$ICEDUSTCONTROLPATH/app/RunIcedustControl -c {:s}/{:s}'.format(cwd, CFG_FILE)
    f.write(command + ' \n')
    f.close()
    #
    preliminary_command = 'chmod +x {:s}'.format(fName)
    os.system(preliminary_command)
    # 
    print 'Sending Job Name: {:s}'.format(fName)
    sendBatchScript_command = 'sbatch %s' % fName
    os.system(sendBatchScript_command)
    # 
    os.system('rm -f ' + fName)
