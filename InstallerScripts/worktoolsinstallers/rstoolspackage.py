import os
from config.configobj import ConfigObj
from installer.installer import InstallerPackageInfoBase
from worktoolsinstallers.rscomponentbase import RsComponentBase

class RsToolsPackage(RsComponentBase):
    def __init__(self):
        super(RsToolsPackage, self).__init__()
        self.__RsDllDir = os.path.join(
            self.FmtDir, 'qrsd/tools/lib/{}'.format(ConfigObj.inst().getBinaryType()))

        self.__ToolsToCopy = [
            'rsrtlwm.dll',
            'RSScript.dll'
        ]

        self.__filesToCopy = ['ToolsRuntimeProj/ToolsRuntime/installerfiles/fs32cm.dll',
                              'ToolsRuntimeProj/ToolsRuntime/installerfiles/rsldbg.dll']

        self.DisplayName = 'Rs Tools Binaries'
        self.Description = 'Динамический инструментальный Runtime'
        self.Name = 'com.rs.tools.runtime'
        self.AutoDependOn.append('com.rs.fmt.workfmt')
        #self.ForcedInstallation = True

    def makeData(self, datadir):
        self.copyArray(self.__ToolsToCopy, self.__RsDllDir)
        self.copyArray(self.__filesToCopy, self.__RsDllDir)
