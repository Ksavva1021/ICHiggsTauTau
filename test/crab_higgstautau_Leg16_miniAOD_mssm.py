from CRABClient.UserUtilities import config
from CRABClient.UserUtilities import getUsernameFromCRIC
from multiprocessing import Process

config = config()

config.General.transferOutputs = True
config.General.workArea='May21_MC_102X_2016'

config.JobType.psetName = 'higgstautau_cfg_102X_Aug19_Leg2016.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['EventTree.root']
config.JobType.maxMemoryMB = 4000
cfgParams = ['isData=0' ,'doHT=0' , 'globalTag=102X_mcRun2_asymptotic_v7']
config.JobType.allowUndistributedCMSSW = True

#config.Data.unitsPerJob = 100000
#config.Data.splitting = 'EventAwareLumiBased'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.publication = False
config.Data.outLFNDirBase='/store/user/{}/{}/'.format(getUsernameFromCRIC(), config.General.workArea)
config.Data.allowNonValidInputDataset = True
# config.Data.inputDBS = 'phys03'
#config.Data.ignoreLocality= True
config.Site.whitelist   = ['T2_*','T1_*','T3_*']

config.Site.storageSite = 'T2_UK_London_IC'

if __name__ == '__main__':

    from CRABAPI.RawCommand import crabCommand
    from httplib import HTTPException

    # We want to put all the CRAB project directories from the tasks we submit here into one common directory.
    # That's why we need to set this parameter (here or above in the configuration file, it does not matter, we will not overwrite it).

    def submit(config):
        try:
            crabCommand('submit', config = config)
        except HTTPException, hte:
            print hte.headers

    #############################################################################################
    ## From now on that's what users should modify: this is the a-la-CRAB2 configuration part. ##
    #############################################################################################

    tasks=list()

    # ggH-LO
    ggH_LO_masses = [80,90,100,110,120,130,140,160,180,200,250,300,350,400,450,500,600,700,800,900,1000,1200,1400,1500,1600,1800,2000,2300,2600,2900,3200]
    for mass in ggH_LO_masses:
        task_name = 'SUSYGluGluToHToTauTau_M-{}'.format(mass)
        file_location = '/SUSYGluGluToHToTauTau_M-{}_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'.format(mass)
        tasks.append((task_name,file_location))

    # bbH-LO
    bbH_LO_masses = [80,90,100,110,120,130,140,160,180,200,250,300,350,400,450,500,600,700,800,900,1000,1200,1400,1500,1600,1800,2000,2300,2600,2900,3200]
    for mass in bbH_LO_masses:
        task_name = 'SUSYGluGluToBBHToTauTau_M-{}'.format(mass)
        file_location = '/SUSYGluGluToBBHToTauTau_M-{}_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'.format(mass)
        tasks.append((task_name,file_location))

    # bbH-NLO
    # Named samples v3-v2
    bbH_NLO_masses = [80,90,110,120,130,140,160,180,200,250,350,400,450,500,600,700,800,900,1000,1400,1600,1800,2000,2900]
    for mass in bbH_NLO_masses:
        task_name = 'SUSYGluGluToBBHToTauTau_M-{}-NLO'.format(mass)
        file_location = '/SUSYGluGluToBBHToTauTau_M-{}_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv3-94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'.format(mass)
        tasks.append((task_name,file_location))

    # bbH-NLO
    # Named samples v3-v3
    bbH_NLO_masses = [1200,2300,2600,3200]
    for mass in bbH_NLO_masses:
        task_name = 'SUSYGluGluToBBHToTauTau_M-{}-NLO'.format(mass)
        file_location = '/SUSYGluGluToBBHToTauTau_M-{}_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv3-94X_mcRun2_asymptotic_v3-v3/MINIAODSIM'.format(mass)
        tasks.append((task_name,file_location))
    
    for task in tasks:
        print task[0]
        config.General.requestName = task[0]
        config.Data.inputDataset = task[1]

        config.JobType.pyCfgParams = cfgParams
        print(config)

        p = Process(target=submit, args=(config,))
        p.start()
        p.join()
