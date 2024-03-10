import os
from datetime import date
from shutil import copyfile
from config.configobj import ConfigObj
from installer.installer import InstallerPackageInfoBase

class DBFileToolPackage(InstallerPackageInfoBase):
    def __init__(self):
        self.__fmtdir = ConfigObj.inst().getWorkFmtSourceDir()
        self.__releasfile = os.path.join(self.__fmtdir, 'DBFileTool/{}/DBFileTool.exe'.format(ConfigObj.inst().getBinaryType()))
        super(DBFileToolPackage, self).__init__()

        today = date.today()
        self.DisplayName = 'DB file tool'
        self.Description = 'Утилита для работы с *.dat файлами'
        self.Name = 'com.rs.fmt.workfmt.dbfiletool'
        self.ReleaseDate = today.strftime("%Y-%m-%d")

        self.Dependencies.append('com.rs.fmt.workfmt')
        self.Dependencies.append('com.rs.qt.runtime')

    def getVersion(self):
        try:
            ver = super(DBFileToolPackage, self).getExeVersion(self.__releasfile)
            print(ver)
            return ver
        except:
            return "1.0.0.0"

    def makeData(self, datadir):
        srcexefile = self.__releasfile
        basedbfiletoolname = os.path.basename(self.__releasfile)
        dstexefile = os.path.join(self.DataPath, os.path.basename(basedbfiletoolname))
        copyfile(srcexefile, dstexefile)