import os
from datetime import date
from shutil import copyfile
from config.configobj import ConfigObj
from worktoolsinstallers.rscomponentbase import RsComponentBase

class RsdDriverPackage(RsComponentBase):
    def __init__(self):
        super(RsdDriverPackage, self).__init__()
        self.__RsdDllDir = os.path.join(
            self.FmtDir, 'qrsd/rsd/lib/{}'.format(ConfigObj.inst().getBinaryType()))
        self.__releasfile = os.path.join(
            self.FmtDir, 'qrsd/{}/qrsd.dll'.format(ConfigObj.inst().getBinaryType()))

        self.__RsdToCopy = [
            'rddrvo.dll',
            'rddrvou.dll',
            'rdrset.dll',
            'RsBtr.dll',
            'rsdc.dll',
            'dbinit.exe', 
            'fmtxml.exe',
            'fmtxml.xsd'
        ]

        self.DisplayName = 'Rsd Connector For Qt'
        self.Description = 'Драйвер подключения к БД через rsd'
        self.Name = 'com.rs.fmt.workfmt.rsddriver'

        self.Dependencies.append('com.rs.qt.runtime')

    def getVersion(self):
        try:
            ver = self.VersionConfig['Version']['Rsd'] + \
                '.' + self.VersionConfig['Version']['QRsdAddVersion']
            return ver
        except:
            return "1.0.0.0"

    def makeData(self, datadir):
        srcexefile = self.__releasfile
        basedbfiletoolname = os.path.basename(self.__releasfile)
        dstexefile = os.path.join(self.DataPath, 'sqldrivers/' + os.path.basename(basedbfiletoolname))
        
        try:
            driverdir = os.path.join(self.DataPath, 'sqldrivers')
            os.mkdir(driverdir)
            copyfile(srcexefile, dstexefile)
        except FileExistsError:
            copyfile(srcexefile, dstexefile)
        except Exception as e:
            print('Fail: ' + str(e))

        self.copyArray(self.__RsdToCopy, self.__RsdDllDir)

