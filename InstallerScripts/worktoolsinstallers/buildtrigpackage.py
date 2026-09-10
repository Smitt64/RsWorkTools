import os
from shutil import copyfile
from datetime import date
from config.configobj import ConfigObj
from installer.installer import InstallerPackageInfoBase

class BuildTrigComponent(InstallerPackageInfoBase):
    def __init__(self):
        today = date.today()
        self.__filesToCopy = ['BuildTrig/{}/BuildTrig.exe']
        super(BuildTrigComponent, self).__init__()
        self.DisplayName = 'Build Trig'
        self.Description = 'Мастер проверки макросов *.mac на изменения в git/svn репозитории с помощью MacTriggerGit.exe'
        self.Name = 'com.rs.fmt.workfmt.buildtrig'
        self.Dependencies = ['com.rs.fmt.workfmt', 'com.rs.worktools.runtime']
        self.ReleaseDate = today.strftime('%Y-%m-%d')

    def makeData(self, datadir):
        fmtdir = ConfigObj.inst().getWorkFmtSourceDir()
        for cpfiletemplate in self.__filesToCopy:
            filetocopy = cpfiletemplate.format(ConfigObj.inst().getBinaryType())
            srcexefile = os.path.join(fmtdir, filetocopy)
            dstexefile = os.path.join(self.DataPath, os.path.basename(filetocopy))
            copyfile(srcexefile, dstexefile)

    def getVersion(self):
        releasedir = os.path.join(ConfigObj.inst().getWorkFmtSourceDir(), self.__filesToCopy[0].format(ConfigObj.inst().getBinaryType()))
        try:
            ver = super(BuildTrigComponent, self).getExeVersion(releasedir)
            print(ver)
            return ver
        except:
            return 'Unknown version'