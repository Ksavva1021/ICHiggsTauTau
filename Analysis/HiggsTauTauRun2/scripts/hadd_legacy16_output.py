#!/usr/bin/env python

import sys
import os
from optparse import OptionParser
import math
import fnmatch
import glob

parser = OptionParser()

parser.add_option("--folder", dest = "folder",
                  help="Specify folder that contains the output to be hadded")
parser.add_option("--ignore_nfiles", dest= "ignore", default=False, action='store_true',
                  help="Ignore number of files per sample")
parser.add_option("--sample_list", dest = "samplelist", default="./jobs/files_per_sample_2016.txt",
                  help="list of files per sample you want to use for hadding")
parser.add_option("--batch", dest= "batch", default=False, action='store_true',
                  help="Submit as batch jobs")

def list_paths(path):
    directories = []
    for item in os.listdir(path):
      if os.path.isdir(os.path.join(path, item)):
        directories.append(item)
    return directories


(options,args) = parser.parse_args()

if not options.folder:
  parser.error('No folder specified')

outputf = options.folder
samplelist = options.samplelist
ignore = options.ignore
batch = options.batch

JOBWRAPPER      = './scripts/generate_job.sh'
JOBSUBMIT       = './scripts/submit_ic_batch_job.sh "hep.q -l h_rt=0:180:0"'


sample_list = [
     'WJetsToLNu-LO',
     'WJetsToLNu-LO-ext',
     'W1JetsToLNu-LO',
     'W2JetsToLNu-LO',
     'W2JetsToLNu-LO-ext',
     'W3JetsToLNu-LO',
     'W3JetsToLNu-LO-ext',
     'W4JetsToLNu-LO',
     'W4JetsToLNu-LO-ext1',
     'W4JetsToLNu-LO-ext2',
     'WGToLNuG-ext1',
     'WGToLNuG-ext2',
     'WGToLNuG-ext3',
     'WGstarToLNuMuMu',
     'WGstarToLNuEE',
     'DYJetsToLL',
     'DYJetsToLL-LO-ext1',
     'DYJetsToLL-LO-ext2',
     'DY1JetsToLL-LO',
     'DY2JetsToLL-LO',
     'DY3JetsToLL-LO',
     'DY4JetsToLL-LO',
     'DYJetsToLL_M-10-50-LO',
     'VVTo2L2Nu',
     'VVTo2L2Nu-ext1',
     'ZZTo2L2Q',
     'WWTo1L1Nu2Q',
     'WWToLNuQQ',
     'WWToLNuQQ-ext',
     'WZTo2L2Q',
     'WZTo1L3Nu',
     'WZTo1L1Nu2Q',
     'WZJToLLLNu',
     'ZZTo4L',
     'QCDMuEnrichedPt15',
     'Tbar-tW',
     'T-tW',
     'Tbar-t',
     'T-t',
     'TT',
     "TTTo2L2Nu",
     "TTToHadronic",
     "TTToSemiLeptonic",
     'WJetsToLNu',
     'WJetsToLNu-ext',
     'EWKWMinus2Jets_WToLNu',
     'EWKWMinus2Jets_WToLNu-ext1',
     'EWKWMinus2Jets_WToLNu-ext2',
     'EWKWPlus2Jets_WToLNu',
     'EWKWPlus2Jets_WToLNu-ext1',
     'EWKWPlus2Jets_WToLNu-ext2',
     'EWKZ2Jets_ZToLL',
     'EWKZ2Jets_ZToLL-ext1',
     'EWKZ2Jets_ZToLL-ext2',
     'EWKZ2Jets_ZToNuNu',
     'EWKZ2Jets_ZToNuNu-ext1',
     'EWKZ2Jets_ZToNuNu-ext2',
     'ZZTo4L-amcat',
     'TauB',
     'TauC',
     'TauD',
     'TauE',
     'TauF',
     'TauG',
     'TauH',
     'SingleElectronB',
     'SingleElectronC',
     'SingleElectronD',
     'SingleElectronE',
     'SingleElectronF',
     'SingleElectronG',
     'SingleElectronH',
     'SingleMuonB',
     'SingleMuonC',
     'SingleMuonD',
     'SingleMuonE',
     'SingleMuonF',
     'SingleMuonG',
     'SingleMuonH',
     'MuonEGB',
     'MuonEGC',
     'MuonEGD',
     'MuonEGE',
     'MuonEGF',
     'MuonEGG',
     'MuonEGH',
     'EmbeddingElElB',
     'EmbeddingElElC',
     'EmbeddingElElD',
     'EmbeddingElElE',
     'EmbeddingElElF',
     'EmbeddingElElG',
     'EmbeddingElElH',
     'EmbeddingElMuB',
     'EmbeddingElMuC',
     'EmbeddingElMuD',
     'EmbeddingElMuE',
     'EmbeddingElMuF',
     'EmbeddingElMuG',
     'EmbeddingElMuH',
     'EmbeddingElTauB',
     'EmbeddingElTauC',
     'EmbeddingElTauD',
     'EmbeddingElTauE',
     'EmbeddingElTauF',
     'EmbeddingElTauG',
     'EmbeddingElTauH',
     'EmbeddingMuMuB',
     'EmbeddingMuMuC',
     'EmbeddingMuMuD',
     'EmbeddingMuMuE',
     'EmbeddingMuMuF',
     'EmbeddingMuMuG',
     'EmbeddingMuMuH',
     'EmbeddingMuTauB',
     'EmbeddingMuTauC',
     'EmbeddingMuTauD',
     'EmbeddingMuTauE',
     'EmbeddingMuTauF',
     'EmbeddingMuTauG',
     'EmbeddingMuTauH',
     'EmbeddingTauTauB',
     'EmbeddingTauTauC',
     'EmbeddingTauTauD',
     'EmbeddingTauTauE',
     'EmbeddingTauTauF',
     'EmbeddingTauTauG',
     'EmbeddingTauTauH',
     'GluGluHToTauTauUncorrelatedDecay',
     'GluGluHToTauTauUncorrelatedDecay_Filtered',
     'GluGluToHToTauTauPlusTwoJets_M125_amcatnloFXFX',
     'GluGluToHToTauTau_M125_amcatnloFXFX',
     'GluGluToMaxmixHToTauTauPlusTwoJets_M125_amcatnloFXFX',
     'GluGluToMaxmixHToTauTau_M125_amcatnloFXFX',
     'GluGluToPseudoscalarHToTauTauPlusTwoJets_M125_amcatnloFXFX',
     'GluGluToPseudoscalarHToTauTau_M125_amcatnloFXFX',
     'JJH0MToTauTauPlusOneJets',
     'JJH0MToTauTauPlusOneJets_Filtered',
     'JJH0MToTauTauPlusTwoJets',
     'JJH0MToTauTauPlusTwoJets_Filtered',
     'JJH0MToTauTauPlusZeroJets',
     'JJH0MToTauTauPlusZeroJets_Filtered',
     'JJH0Mf05ph0ToTauTauPlusOneJets',
     'JJH0Mf05ph0ToTauTauPlusOneJets_Filtered',
     'JJH0Mf05ph0ToTauTauPlusTwoJets',
     'JJH0Mf05ph0ToTauTauPlusTwoJets_Filtered',
     'JJH0Mf05ph0ToTauTauPlusZeroJets',
     'JJH0Mf05ph0ToTauTauPlusZeroJets_Filtered',
     'JJH0PMToTauTauPlusOneJets',
     'JJH0PMToTauTauPlusOneJets_Filtered',
     'JJH0PMToTauTauPlusTwoJets',
     'JJH0PMToTauTauPlusTwoJets_Filtered',
     'JJH0PMToTauTauPlusZeroJets',
     'JJH0PMToTauTauPlusZeroJets_Filtered',
     'JJHiggs0MToTauTau',
     'JJHiggs0Mf05ph0ToTauTau',
     'VBFHToTauTauUncorrelatedDecay',
     'VBFHToTauTauUncorrelatedDecay_Filtered',
     'VBFHiggs0L1_HToTauTau',
     'VBFHiggs0L1f05ph0_HToTauTau',
     'VBFHiggs0M_HToTauTau',
     'VBFHiggs0Mf05ph0_HToTauTau',
     'VBFHiggs0PH_HToTauTau',
     'VBFHiggs0PHf05ph0_HToTauTau',
     'VBFHiggs0PM_HToTauTau',
     'WHiggs0L1_HToTauTau',
     'WminusHToTauTauUncorrelatedDecay',
     'WminusHToTauTauUncorrelatedDecay_Filtered',
     'WplusHToTauTauUncorrelatedDecay',
     'WplusHToTauTauUncorrelatedDecay_Filtered',
     'ZHToTauTauUncorrelatedDecay',
     'ZHToTauTauUncorrelatedDecay_Filtered',
     'ZHiggs0L1fZH05ph0_HToTauTau',
     'ttHiggs0MToTauTau',
     'ttHiggs0Mf05ph0ToTauTau',
     'ttHiggs0PMToTauTau',
     'SUSYGluGluToBBHToTauTau_M-80',
     'SUSYGluGluToBBHToTauTau_M-90',
     'SUSYGluGluToBBHToTauTau_M-100',
     'SUSYGluGluToBBHToTauTau_M-110',
     'SUSYGluGluToBBHToTauTau_M-120',
     'SUSYGluGluToBBHToTauTau_M-130',
     'SUSYGluGluToBBHToTauTau_M-140',
     'SUSYGluGluToBBHToTauTau_M-160',
     'SUSYGluGluToBBHToTauTau_M-180',
     'SUSYGluGluToBBHToTauTau_M-200',
     'SUSYGluGluToBBHToTauTau_M-250',
     'SUSYGluGluToBBHToTauTau_M-300',
     'SUSYGluGluToBBHToTauTau_M-350',
     'SUSYGluGluToBBHToTauTau_M-400',
     'SUSYGluGluToBBHToTauTau_M-450',
     'SUSYGluGluToBBHToTauTau_M-500',
     'SUSYGluGluToBBHToTauTau_M-600',
     'SUSYGluGluToBBHToTauTau_M-700',
     'SUSYGluGluToBBHToTauTau_M-800',
     'SUSYGluGluToBBHToTauTau_M-900',
     'SUSYGluGluToBBHToTauTau_M-1000',
     'SUSYGluGluToBBHToTauTau_M-1200',
     'SUSYGluGluToBBHToTauTau_M-1400',
     'SUSYGluGluToBBHToTauTau_M-1500',
     'SUSYGluGluToBBHToTauTau_M-1600',
     'SUSYGluGluToBBHToTauTau_M-1800',
     'SUSYGluGluToBBHToTauTau_M-2000',
     'SUSYGluGluToBBHToTauTau_M-2300',
     'SUSYGluGluToBBHToTauTau_M-2600',
     'SUSYGluGluToBBHToTauTau_M-2900',
     'SUSYGluGluToBBHToTauTau_M-3200',
     'SUSYGluGluToBBHToTauTau_M-80-NLO',
     'SUSYGluGluToBBHToTauTau_M-90-NLO',
     'SUSYGluGluToBBHToTauTau_M-100-NLO',
     'SUSYGluGluToBBHToTauTau_M-110-NLO',
     'SUSYGluGluToBBHToTauTau_M-120-NLO',
     'SUSYGluGluToBBHToTauTau_M-130-NLO',
     'SUSYGluGluToBBHToTauTau_M-140-NLO',
     'SUSYGluGluToBBHToTauTau_M-160-NLO',
     'SUSYGluGluToBBHToTauTau_M-180-NLO',
     'SUSYGluGluToBBHToTauTau_M-200-NLO',
     'SUSYGluGluToBBHToTauTau_M-250-NLO',
     'SUSYGluGluToBBHToTauTau_M-300-NLO',
     'SUSYGluGluToBBHToTauTau_M-350-NLO',
     'SUSYGluGluToBBHToTauTau_M-400-NLO',
     'SUSYGluGluToBBHToTauTau_M-450-NLO',
     'SUSYGluGluToBBHToTauTau_M-500-NLO',
     'SUSYGluGluToBBHToTauTau_M-600-NLO',
     'SUSYGluGluToBBHToTauTau_M-700-NLO',
     'SUSYGluGluToBBHToTauTau_M-800-NLO',
     'SUSYGluGluToBBHToTauTau_M-900-NLO',
     'SUSYGluGluToBBHToTauTau_M-1000-NLO',
     'SUSYGluGluToBBHToTauTau_M-1200-NLO',
     'SUSYGluGluToBBHToTauTau_M-1400-NLO',
     'SUSYGluGluToBBHToTauTau_M-1500-NLO',
     'SUSYGluGluToBBHToTauTau_M-1600-NLO',
     'SUSYGluGluToBBHToTauTau_M-1800-NLO',
     'SUSYGluGluToBBHToTauTau_M-2000-NLO',
     'SUSYGluGluToBBHToTauTau_M-2300-NLO',
     'SUSYGluGluToBBHToTauTau_M-2600-NLO',
     'SUSYGluGluToBBHToTauTau_M-2900-NLO',
     'SUSYGluGluToBBHToTauTau_M-3200-NLO',
     'SUSYGluGluToHToTauTau_M-80',
     'SUSYGluGluToHToTauTau_M-90',
     'SUSYGluGluToHToTauTau_M-100',
     'SUSYGluGluToHToTauTau_M-110',
     'SUSYGluGluToHToTauTau_M-120',
     'SUSYGluGluToHToTauTau_M-130',
     'SUSYGluGluToHToTauTau_M-140',
     'SUSYGluGluToHToTauTau_M-160',
     'SUSYGluGluToHToTauTau_M-180',
     'SUSYGluGluToHToTauTau_M-200',
     'SUSYGluGluToHToTauTau_M-250',
     'SUSYGluGluToHToTauTau_M-300',
     'SUSYGluGluToHToTauTau_M-350',
     'SUSYGluGluToHToTauTau_M-400',
     'SUSYGluGluToHToTauTau_M-450',
     'SUSYGluGluToHToTauTau_M-500',
     'SUSYGluGluToHToTauTau_M-600',
     'SUSYGluGluToHToTauTau_M-700',
     'SUSYGluGluToHToTauTau_M-800',
     'SUSYGluGluToHToTauTau_M-900',
     'SUSYGluGluToHToTauTau_M-1000',
     'SUSYGluGluToHToTauTau_M-1200',
     'SUSYGluGluToHToTauTau_M-1400',
     'SUSYGluGluToHToTauTau_M-1500',
     'SUSYGluGluToHToTauTau_M-1600',
     'SUSYGluGluToHToTauTau_M-1800',
     'SUSYGluGluToHToTauTau_M-2000',
     'SUSYGluGluToHToTauTau_M-2300',
     'SUSYGluGluToHToTauTau_M-2600',
     'SUSYGluGluToHToTauTau_M-2900',
     'SUSYGluGluToHToTauTau_M-3200',
     'GluGluHToWWTo2L2Nu_M-125',
     'VBFHToWWTo2L2Nu_M-125',
     'GluGluZH_HToWW',
     'HZJ_HToWW',
     'HWminusJ_HToWW',
     'HWplusJ_HToWW',
     'GluGluToHToTauTau_M-125',
     'VBFHToTauTau_M-125',
     'WplusHToTauTau_M-125',
     'WminusHToTauTau_M-125',
     'ZHToTauTau_M-125',
     'ttHJetToTT',
    'SUSYGluGluToBBHToTauTau_M-1000_powheg',
    'SUSYGluGluToBBHToTauTau_M-100_powheg',
    'SUSYGluGluToBBHToTauTau_M-1200_powheg',
    'SUSYGluGluToBBHToTauTau_M-120_powheg',
    'SUSYGluGluToBBHToTauTau_M-125_powheg',
    'SUSYGluGluToBBHToTauTau_M-130_powheg',
    'SUSYGluGluToBBHToTauTau_M-1400_powheg',
    'SUSYGluGluToBBHToTauTau_M-140_powheg',
    'SUSYGluGluToBBHToTauTau_M-1600_powheg',
    'SUSYGluGluToBBHToTauTau_M-160_powheg',
    'SUSYGluGluToBBHToTauTau_M-1800_powheg',
    'SUSYGluGluToBBHToTauTau_M-180_powheg',
    'SUSYGluGluToBBHToTauTau_M-2000_powheg',
    'SUSYGluGluToBBHToTauTau_M-200_powheg',
    'SUSYGluGluToBBHToTauTau_M-2300_powheg',
    'SUSYGluGluToBBHToTauTau_M-250_powheg',
    'SUSYGluGluToBBHToTauTau_M-2600_powheg',
    'SUSYGluGluToBBHToTauTau_M-2900_powheg',
    'SUSYGluGluToBBHToTauTau_M-300_powheg',
    'SUSYGluGluToBBHToTauTau_M-3200_powheg',
    'SUSYGluGluToBBHToTauTau_M-3500_powheg',
    'SUSYGluGluToBBHToTauTau_M-350_powheg',
    'SUSYGluGluToBBHToTauTau_M-400_powheg',
    'SUSYGluGluToBBHToTauTau_M-450_powheg',
    'SUSYGluGluToBBHToTauTau_M-500_powheg',
    'SUSYGluGluToBBHToTauTau_M-600_powheg',
    'SUSYGluGluToBBHToTauTau_M-60_powheg',
    'SUSYGluGluToBBHToTauTau_M-700_powheg',
    'SUSYGluGluToBBHToTauTau_M-800_powheg',
    'SUSYGluGluToBBHToTauTau_M-80_powheg',
    'SUSYGluGluToBBHToTauTau_M-900_powheg',
    'SUSYGluGluToHToTauTau_M-1000_powheg',
    'SUSYGluGluToHToTauTau_M-100_powheg',
    'SUSYGluGluToHToTauTau_M-1200_powheg',
    'SUSYGluGluToHToTauTau_M-120_powheg',
    'SUSYGluGluToHToTauTau_M-125_powheg',
    'SUSYGluGluToHToTauTau_M-130_powheg',
    'SUSYGluGluToHToTauTau_M-1400_powheg',
    'SUSYGluGluToHToTauTau_M-140_powheg',
    'SUSYGluGluToHToTauTau_M-1600_powheg',
    'SUSYGluGluToHToTauTau_M-160_powheg',
    'SUSYGluGluToHToTauTau_M-1800_powheg',
    'SUSYGluGluToHToTauTau_M-180_powheg',
    'SUSYGluGluToHToTauTau_M-2000_powheg',
    'SUSYGluGluToHToTauTau_M-200_powheg',
    'SUSYGluGluToHToTauTau_M-2300_powheg',
    'SUSYGluGluToHToTauTau_M-250_powheg',
    'SUSYGluGluToHToTauTau_M-2600_powheg',
    'SUSYGluGluToHToTauTau_M-2900_powheg',
    'SUSYGluGluToHToTauTau_M-300_powheg',
    'SUSYGluGluToHToTauTau_M-3200_powheg',
    'SUSYGluGluToHToTauTau_M-3500_powheg',
    'SUSYGluGluToHToTauTau_M-350_powheg',
    'SUSYGluGluToHToTauTau_M-400_powheg',
    'SUSYGluGluToHToTauTau_M-450_powheg',
    'SUSYGluGluToHToTauTau_M-500_powheg',
    'SUSYGluGluToHToTauTau_M-600_powheg',
    'SUSYGluGluToHToTauTau_M-60_powheg',
    'SUSYGluGluToHToTauTau_M-700_powheg',
    'SUSYGluGluToHToTauTau_M-800_powheg',
    'SUSYGluGluToHToTauTau_M-80_powheg',
    'SUSYGluGluToHToTauTau_M-900_powheg',
    'WHiggs0MfWH05ph0',
    'WHiggs0PH',
    'WHiggs0PM',
    'WHiggs0PHfWH05ph0',
    'WHiggs0M',
    'WHiggs0L1fWH05ph0',
    'ZHiggs0PH',
    'ZHiggs0L1',
    'ZHiggs0PHfZH05ph0',
    'ZHiggs0MfZH05ph0',
    'ZHiggs0PM',
    'ZHiggs0M',
    ]

sample_list = list(set(sample_list))

out=''
for s in sorted(sample_list): out+='    - %s\n' %s

print out
#print sample_list

#channel = ['em','et','mt','tt','zee','zmm','wmnu','tpzee','tpzmm','tpmt','tpem']
channel = ['et','em','mt','tt','zmm']

with open("%(samplelist)s"%vars(),"r") as inf:
  lines = inf.readlines()

subdirs=['']
subdirs+=list_paths(outputf)

new_subdirs=[]
for d in subdirs:
  infi=os.listdir('%(outputf)s/%(d)s' % vars())
  if infi: new_subdirs.append((d,infi))
subdirs=new_subdirs

print subdirs


nfiles={}


def FindMissingFiles(outf, d, samp, chan, infiles):
  files=fnmatch.filter(infiles,'%(samp)s_2016_%(chan)s_*'%vars())
  nums = [int(x.split('_')[-1].replace('.root','')) for x in files]
  nums.sort()
  res = [ele for ele in range(max(nums)+1) if ele not in nums]

  if len(res) !=0:
    print "Some files are missing for sample %(samp)s_2016_%(chan)s! in %(d)s:"%vars()
    for x in res: print '%(samp)s_2016_%(chan)s_%(x)i.root' % vars()
    return False
  else:
    return True

for ind in range(0,len(lines)):
  nfiles[lines[ind].split()[0]]=int(lines[ind].split()[1])
for sa in sample_list:
  remove=True
  to_remove=[]
  hadd_dirs=[]
  command=''
  if batch:
    JOB='jobs/hadd_%s.sh' % sa
    os.system('%(JOBWRAPPER)s "" %(JOB)s' %vars())
  for ch in channel:
    for jsdir in subdirs:
      sdir = jsdir[0]
      infiles=jsdir[1]
      if os.path.isfile('%(outputf)s/%(sdir)s/%(sa)s_2016_%(ch)s_0.root'%vars()):
        if "%(sa)s_2016"%vars() in nfiles or ignore==True:
#          files=glob.glob('%(outputf)s/%(sdir)s/%(sa)s_2016_%(ch)s_*.root'%vars())
          no_missing_files = FindMissingFiles(outputf, sdir, sa, ch,infiles) 
          if no_missing_files and (ignore ==True or len(fnmatch.filter(infiles,'%(sa)s_2016_%(ch)s_*'%vars())) == nfiles["%(sa)s_2016"%vars()]):
            if not batch:  
              print "Hadding in subdir %(sdir)s"%vars()
              print "Hadding %(sa)s_%(ch)s in %(sdir)s"%vars()
              os.system('hadd -f %(outputf)s/%(sdir)s/%(sa)s_%(ch)s_2016.root %(outputf)s/%(sdir)s/%(sa)s_2016_%(ch)s_* &> ./haddout.txt'% vars()) 
              os.system("sed -i '/Warning in <TInterpreter::ReadRootmapFile>/d' ./haddout.txt")
              filetext = open("./haddout.txt").read()
              if 'Warning' in filetext or 'Error' in filetext:
                print "Hadd had a problem:"
                print filetext 
                remove=False
              else :
                to_remove.append('rm %(outputf)s/%(sdir)s/%(sa)s_2016_%(ch)s_*' %vars())
            else:
              haddout='haddout_%s_%s_%s.txt' % (sa,ch,sdir)  
              hadd_dirs.append((haddout, 'rm %(outputf)s/%(sdir)s/%(sa)s_2016_%(ch)s_*' %vars())) 
              command+="echo \"Hadding %(sa)s_%(ch)s in %(sdir)s\"\necho \"Hadding %(sa)s_%(ch)s\"\nhadd -f %(outputf)s/%(sdir)s/%(sa)s_%(ch)s_2016.root %(outputf)s/%(sdir)s/%(sa)s_2016_%(ch)s_* &> ./%(haddout)s\nsed -i '/Warning in <TInterpreter::ReadRootmapFile>/d' ./%(haddout)s\n" % vars()     
          else :
            print "Incorrect number of files for sample %(sa)s_2016_%(ch)s! in %(sdir)s"%vars()
            remove=False

  if not batch and remove:
    # if all channels and systematics were hadded sucsessfully then remove the input files
    for x in to_remove: 
      os.system(x)

  if batch and command:
    with open(JOB, "a") as file: 
      file.write("\n%s" % command)
      rm_command = 'y=1\n' % vars()
      for i in hadd_dirs:
        hadd_file  = i[0]
        rm_command+='if [ \"$(cat %(hadd_file)s | grep -e Warning -e Error)\"  != \"\" ]; then y=0; fi\n' % vars()
      rm_command+='if [ $y == 1 ]; then\n'
      for i in hadd_dirs:
        input_file = i[1]
        rm_command+=input_file+'\n'
      rm_command+='fi'
      if remove: file.write("\n%s" % rm_command)
      file.write('\nEnd of job')
    os.system('%(JOBSUBMIT)s %(JOB)s' % vars())

