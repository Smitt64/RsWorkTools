import os
from config.configobj import ConfigObj
from installer.installer import InstallerPackageInfoBase
from datetime import date
import configparser

class RsComponentBase(InstallerPackageInfoBase):
    def __init__(self):
        self.FmtDir = ConfigObj.inst().getWorkFmtSourceDir()
        self.__VersionFile = os.path.join(self.FmtDir, 'qrsd\\toolsversion.txt')
        self.VersionConfig = configparser.ConfigParser()
        self.__readVersionFile()

        super(RsComponentBase, self).__init__()

        today = date.today()
        self.ReleaseDate = today.strftime("%Y-%m-%d")

    def __readVersionFile(self):
        self.VersionConfig.read(self.__VersionFile)

    def getVersion(self):
        try:
            ver = self.VersionConfig['Version']['Rsd'] + \
                '.' + self.VersionConfig['Version']['QRsdAddVersion']
            return ver
        except:
            return "1.0.0.0"

    def copyArray(self, rsdDllDir, srcDir):
        for dll in rsdDllDir:
            dllpath = os.path.join(srcDir, dll)
            dlldstpath = os.path.join(self.DataPath, dll)
            self.copyFileToDestrib(dllpath, dlldstpath)
