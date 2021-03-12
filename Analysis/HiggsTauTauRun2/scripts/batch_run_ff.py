#!/usr/bin/env python

import sys
from optparse import OptionParser
import os
import string
import shlex
from subprocess import Popen, PIPE

parser = OptionParser()
parser.add_option("--year", dest="year", type='string', default='all',help="years to run fake factors for")
parser.add_option("--channel", dest="channel", type='string', default='all',help="channels to run fake factors for")
parser.add_option("--dry_run", dest="dry_run", action='store_true',help="will only create only job and will not run")
parser.add_option("--force_pol1", dest="force_pol1", action='store_true',help="use force pol1 option")
(options, args) = parser.parse_args()

year = options.year
channel = options.channel
dry_run = options.dry_run

#cmssw_base = "/vols/cms/gu18/AnalyserCMSSW/CMSSW_8_0_25"
#cmssw_base = "/vols/cms/gu18/CrabCMSSW/CMSSW_10_2_19"
cmssw_base = os.environ['CMSSW_BASE']

fake_factor_output = 'fake_factor_output'
if options.force_pol1: fake_factor_output = 'fake_factor_output_pol1'

#os.system('rm -r %(fake_factor_output)s/*/*/*.pdf' % vars())
#os.system('rm -r %(fake_factor_output)s/workspaces/*' % vars())
#os.system('rm -r %(fake_factor_output)s/jobs/*' % vars())


if year == "all":
  year_list = ["2016","2017","2018"]
else:
  year_list = year.split(",")

if channel == "all":
  channel_list = ["et","mt","tt"]
else:
  channel_list = channel.split(",")

if not os.path.isdir('%(fake_factor_output)s' % vars()):
  os.system("mkdir %(fake_factor_output)s" % vars() )

if not os.path.isdir('%(fake_factor_output)s/jobs' % vars()):
  os.system("mkdir %(fake_factor_output)s/jobs" % vars())

if not os.path.isdir('%(fake_factor_output)s/workspaces' % vars()):
  os.system("mkdir %(fake_factor_output)s/workspaces" % vars())

for ch in channel_list:
  if not os.path.isdir('%(fake_factor_output)s/%(ch)s' % vars()):
    os.system("mkdir %(fake_factor_output)s/%(ch)s" % vars())  
  for yr in year_list:
    if not os.path.isdir('%(fake_factor_output)s/%(ch)s/%(yr)s' % vars()):
      os.system("mkdir %(fake_factor_output)s/%(ch)s/%(yr)s" % vars())
      os.system("mkdir %(fake_factor_output)s/%(ch)s/%(yr)s" % vars())


for ch in channel_list:
  for yr in year_list:
    
    if ch in ["et","mt"]:
      run_cmd_ff = "python scripts/mt_et_ffs_deeptau_mssm.py --channel=%(ch)s --year=%(yr)s --output_folder='%(fake_factor_output)s/%(ch)s/%(yr)s/'" % vars()
      if options.force_pol1: run_cmd_ff+=' --force_pol1'
      run_cmd_ws = "python scripts/make_ff_workspace_mssm_lt.py --channel=%(ch)s --year=%(yr)s --output='%(fake_factor_output)s/workspaces/' --input='%(fake_factor_output)s/%(ch)s/%(yr)s/'" % vars()
    elif ch in ["tt"]:
      run_cmd_ff = "python scripts/tt_ffs_deeptau_2018_jetpt_and_pt.py --year=%(yr)s --output_folder='%(fake_factor_output)s/%(ch)s/%(yr)s/'" % vars()
      if options.force_pol1: run_cmd_ff+=' --force_pol1'
      run_cmd_ws = "python scripts/make_ff_workspace_mssm_tt.py --year=%(yr)s --output='%(fake_factor_output)s/workspaces/' --input='%(fake_factor_output)s/%(ch)s/%(yr)s/'" % vars()

    job_file = "%(fake_factor_output)s/jobs/batch_ff_%(ch)s_%(yr)s.sh" % vars()
    if os.path.exists(job_file): os.system('rm %(job_file)s' % vars())

    os.system('echo "#!/bin/bash" >> %(job_file)s' % vars())
    os.system('echo "cd %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2" >> %(job_file)s' % vars())
    os.system('echo "source /vols/grid/cms/setup.sh" >> %(job_file)s' % vars()) 
    os.system('echo "export SCRAM_ARCH=slc6_amd64_gcc481" >> %(job_file)s' % vars())
    os.system('echo "eval \'scramv1 runtime -sh\'" >> %(job_file)s' % vars())
    os.system('echo "source %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/setup_libs.sh" >> %(job_file)s' % vars())
    os.system('echo "ulimit -c 0" >> %(job_file)s' % vars())
    os.system('echo "%(run_cmd_ff)s" >> %(job_file)s' % vars())

    if options.force_pol1:
      output_folder='%(fake_factor_output)s/%(ch)s/%(yr)s/' % vars()
      output_folder_nom=output_folder.replace('_pol1','')
      os.system('mkdir -p %(output_folder_nom)s' % vars())
      os.system('echo "cp %(output_folder)s/fakefactor_fits_%(ch)s_medium_%(yr)s_forcepol1.root %(output_folder_nom)s/." >> %(job_file)s' % vars())

    #os.system('echo "%(run_cmd_ws)s" >> %(job_file)s' % vars())
    os.system('chmod +x %(job_file)s' % vars())

    error_file = job_file.replace('.sh','_error.log')
    output_file = job_file.replace('.sh','_output.log')
    if os.path.exists(error_file): os.system('rm %(error_file)s' % vars())
    if os.path.exists(output_file): os.system('rm %(output_file)s' % vars())
    
    if not dry_run:
      os.system('qsub -e %(error_file)s -o %(output_file)s -V -q hep.q -l h_rt=0:180:0 -cwd %(job_file)s' % vars())



      
  

