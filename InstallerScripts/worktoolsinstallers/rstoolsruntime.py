import os
from shutil import copyfile
from datetime import date
from config.configobj import ConfigObj
from installer.installer import InstallerPackageInfoBase

class RsToolsRuntimePackage(InstallerPackageInfoBase):
    def __init__(self):
        self.__filesToCopy = ['RsWorkTools/ToolsRuntime/{}/ToolsRuntime.dll']

        super(RsToolsRuntimePackage, self).__init__()
        
        today = date.today()
        self.DisplayName = 'Work Tools Runtime'
        self.Description = 'Библиотеки, содержащие необходимый общий функционал'
        self.Name = 'com.rs.worktools.runtime'
        self.ReleaseDate = today.strftime("%Y-%m-%d")
        self.ForcedInstallation = True

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
            ver = super(RsToolsRuntimePackage, self).getExeVersion(releasedir)
            print(ver)
            return ver
        except:
            return "Unknown version"
