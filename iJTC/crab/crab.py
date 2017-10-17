from WMCore.Configuration import Configuration
config = Configuration()

subScript = "HT_Correlation_bjet.C"
#subScript = "tracker.C"
#subScript = "closMaker.C"

config.section_("General")
config.General.requestName = 'GenRec_5TeV_bJTC_looseTrkCorr_HPOn'
#config.General.requestName = 'GenGen_5TeV_bJTC_looseTrkCorr_HPOn'
#config.General.requestName = 'test_correlation'
config.General.workArea = 'bJTC_5TeV_Study'
config.General.transferOutputs = True
config.General.transferLogs = False

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'PSet.py'
config.JobType.scriptExe = 'runScript.sh'
config.JobType.scriptArgs = ['script='+subScript]
config.JobType.inputFiles = ['FrameworkJobReport.xml','TrkCorr_Jun7_Iterative_PbPb_etaLT2p4.tar.gz',subScript,'lib.tar.gz', 'corrTable.tar.gz', 'TrkCorr_July22_Iterative_pp_eta2p4.tar.gz']
#config.JobType.outputFiles = ['corr.root']
config.JobType.outputFiles = ['correlation.root']
#config.JobType.outputFiles = ['trkClos.root']
config.JobType.maxJobRuntimeMin = 1800

config.section_("Data")
config.Data.userInputFiles = open('list_cymbal.txt').readlines()
#config.Data.userInputFiles = open('list_test.txt').readlines()
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.totalUnits = len(config.Data.userInputFiles)
config.Data.outputPrimaryDataset = 'PbPb_5TeV_AnalysisHistoOutput_looseMerge'
config.Data.outLFNDirBase = '/store/user/wangx/'+config.General.requestName
config.Data.publication = False
config.Data.ignoreLocality = True

config.section_("Site")
config.Site.whitelist = ['T2_US_Purdue']
config.Site.storageSite = 'T3_US_FNALLPC'
config.Site.ignoreGlobalBlacklist = True

#"really" force crab to only run at whitelisted sites
config.section_("Debug")
config.Debug.extraJDL = ['+CMS_ALLOW_OVERFLOW=False']

