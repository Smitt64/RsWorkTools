import os
from shutil import copyfile
from datetime import date
from config.configobj import ConfigObj
from installer.installer import InstallerPackageInfoBase

class DumpToolComponent(InstallerPackageInfoBase):
    def __init__(self):
        today = date.today()
        self.__filesToCopy = ['DumpTool/{}/DumpTool.exe',
            'fmtdatapumpwrp/{}/fmtdatapumpwrp.exe']
        
        super(DumpToolComponent, self).__init__()

        self.DisplayName = 'Dump Tool'
        self.Description = 'Мастер импорта/экспорта файла дампа'
        self.Name = 'com.rs.fmt.workfmt.dumptool'
        self.Dependencies = ['com.rs.fmt.workfmt']
        self.ReleaseDate = today.strftime("%Y-%m-%d")

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
            ver = super(DumpToolComponent, self).getExeVersion(releasedir)
            print(ver)
            return ver
        except:
            return "Unknown version"
