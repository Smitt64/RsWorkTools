import os
from shutil import copyfile
from datetime import date
from config.configobj import ConfigObj
from installer.installer import InstallerPackageInfoBase

class WorkLbrMainPackage(InstallerPackageInfoBase):
    def __init__(self):
        self.__filesToCopy = ['rsrescore/{}/rsrescore.dll',
            'rsreslbrfn/{}/rsreslbrfn.dll',
            'RsResEditor/{}/WorkRes.exe']

        super(WorkLbrMainPackage, self).__init__()
        
        today = date.today()
        self.DisplayName = 'Work LbR'
        self.Description = 'WorkLbR – утилита, предоставляющая интерфейсный создания ресурсов программы'
        self.Name = 'com.rs.lbr.worklbr'
        self.ReleaseDate = today.strftime("%Y-%m-%d")

        self.Dependencies.append('com.rs.tools.runtime')
        self.Dependencies.append('com.rs.qt.runtime')

    def makeData(self, datadir):
        fmtdir = ConfigObj.inst().getWorkLbrSourceDir()
        for cpfiletemplate in self.__filesToCopy:
            filetocopy = cpfiletemplate.format(ConfigObj.inst().getBinaryType())
            srcexefile = os.path.join(fmtdir, filetocopy)
            dstexefile = os.path.join(self.DataPath, os.path.basename(filetocopy))
            copyfile(srcexefile, dstexefile)

    def getVersion(self):
        try:
            releasedir = os.path.join(ConfigObj.inst().getWorkLbrSourceDir(), self.__filesToCopy[0].format(ConfigObj.inst().getBinaryType()))
            ver = super(WorkLbrMainPackage, self).getExeVersion(releasedir)
            print(ver)
            return ver
        except:
            return "Unknown version"
