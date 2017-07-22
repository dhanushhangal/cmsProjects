from WMCore.Configuration import Configuration
config = Configuration()

subScript = "scanDataTree.C"
config.section_("General")
config.General.requestName = 'trackingMiniTreeScan_cymbal'
config.General.workArea = 'TrkingCorrection' 
config.General.transferLogs = True

config.section_("JobType")
config.JobType.inputFiles = ['FrameworkJobReport.xml','inTree.h',subScript,'miniTreeScan.h']
config.JobType.outputFiles = ['tracker.root']
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'PSet.py'
config.JobType.scriptExe = 'runScript.sh'
config.JobType.scriptArgs = ['script='+subScript]
config.JobType.maxJobRuntimeMin = 1800

config.section_("Data")
config.Data.userInputFiles = open('list_file.txt').readlines()
#config.Data.userInputFiles = open('list_drum.txt').readlines()
#config.Data.userInputFiles = open('Data2015_PbPb_List.txt').readlines()
#config.Data.userInputFiles = open('test_crab.txt').readlines()
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.totalUnits = len(config.Data.userInputFiles)
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

