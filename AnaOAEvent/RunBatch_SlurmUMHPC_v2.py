import sys
import os
import subprocess

cwd = os.getcwd()
setupEnvPath = "/home/user/gmwong/comet/icedust/setupICEDUST_UMHPC.sh"
#
# Specify macro name
#
macroName = "Analyzer.C"

txtFileDir  = './TempSampleList/'

# JOBS=[
#   ("electron", "partGunElecFromDisk_v0_electron"),
#   ("positron", "partGunElecFromDisk_v0_positron"),
# ]

JOBS=[
  ("electron_part0", "partGunElecFromDisk_v0_electron_part0"),
  ("electron_part1", "partGunElecFromDisk_v0_electron_part1"),
  ("electron_part2", "partGunElecFromDisk_v0_electron_part2"),
  ("electron_part3", "partGunElecFromDisk_v0_electron_part3"),
  ("electron_part4", "partGunElecFromDisk_v0_electron_part4"),
  ("positron_part0", "partGunElecFromDisk_v0_positron_part0"),
  ("positron_part1", "partGunElecFromDisk_v0_positron_part1"),
  ("positron_part2", "partGunElecFromDisk_v0_positron_part2"),
  ("positron_part3", "partGunElecFromDisk_v0_positron_part3"),
  ("positron_part4", "partGunElecFromDisk_v0_positron_part4"),
]

#
# Make the bash script
#
for iJob,Job in enumerate(JOBS):
  jobName = Job[0]
  txtFileName = Job[1]
  #
  #
  #
  outDir="./histos_{:s}/".format(jobName)
  #
  #
  #
  fName = 'Batch_{:s}.sh'.format(jobName)
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
  command = 'python -u RunLocal.py -m {:s} -d {:s} -t {:s} -o {:s}'.format(macroName,txtFileDir,txtFileName,outDir)
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
