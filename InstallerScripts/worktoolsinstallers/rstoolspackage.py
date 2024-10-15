import os
from shutil import copyfile 
from config.configobj import ConfigObj
from installer.installer import InstallerPackageInfoBase
from worktoolsinstallers.rscomponentbase import RsComponentBase

class RsToolsPackage(RsComponentBase):
    def __init__(self):
        super(RsToolsPackage, self).__init__()
        self.__RsDllDir = os.path.join(
            self.FmtDir, 'qrsd/tools/lib/release'.format(ConfigObj.inst().getBinaryType()))

        self.__ToolsToCopy = [
            'rsrtlwm.dll',
            'RSScript.dll'
        ]

        self.__toolsruntimeproj = os.path.join(self.FmtDir, 'ToolsRuntimeProj/ToolsRuntime/installerfiles')
        self.__filesToCopy = ['fs32cm.dll',
                              'rsldbg.dll']

        self.__macToCopy = ['dynamicobj.d32']
        
        self.DisplayName = 'Rs Tools Binaries'
        self.Description = 'Динамический инструментальный Runtime'
        self.Name = 'com.rs.tools.runtime'
        self.AutoDependOn.append('com.rs.fmt.workfmt')
        #self.ForcedInstallation = True

    def makeData(self, datadir):
        macdir = os.path.join(datadir, 'mac')
        
        self.copyArray(self.__ToolsToCopy, self.__RsDllDir)
        self.copyArray(self.__filesToCopy, self.__toolsruntimeproj)

        try:
            os.makedirs(macdir)
        except:
            pass

        for mac in self.__macToCopy:
            srecfile = os.path.join(self.__toolsruntimeproj, 'mac/' + mac)
            copyfile(srecfile, os.path.join(macdir, mac))