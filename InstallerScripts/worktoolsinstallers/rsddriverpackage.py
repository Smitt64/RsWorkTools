import os, glob, shutil
from datetime import date
from shutil import copyfile
from config.configobj import ConfigObj
from worktoolsinstallers.rscomponentbase import RsComponentBase

class RsdDriverPackage(RsComponentBase):
    def __init__(self):
        super(RsdDriverPackage, self).__init__()
        self.__RsdDllDir = os.path.join(self.FmtDir, 'qrsd/rsd/lib/release')
        self.__SqlConverterDir = os.path.join(self.FmtDir, 'qrsd/PgConvWrapper/Release')
        self.__releasfile = os.path.join(
            self.FmtDir, 'qrsd/{}/qrsd.dll'.format(ConfigObj.inst().getBinaryType()))
        
        self.__installerfiles = os.path.join(self.FmtDir, 'qrsd/installerfiles/')

        self.__RsdToCopy = [
            'boost_filesystem-vc142-mt-x32-1_72.dll', 
            'boost_iostreams-vc142-mt-x32-1_72.dll',
            'boost_locale-vc142-mt-x32-1_72.dll',
            'boost_regex-vc142-mt-x32-1_72.dll',
            'boost_serialization-vc142-mt-x32-1_72.dll',
            'boost_thread-vc142-mt-x32-1_72.dll',
            'icudt57.dll', 
            'icuin57.dll',
            'icuuc57.dll',
            'rddrvo.dll', 
            'rddrvou.dll', 
            'rdrset.dll', 
            'RsBtr.dll', 
            'rsdc.dll', 
            'SqlConverter.dll', 
            'SqlConverterDebugConfig.json', 
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

        src_conf_file = self.__SqlConverterDir
        base_conf_filename = os.path.basename(self.__SqlConverterDir)
        dst_conf_filename = os.path.join(self.DataPath, base_conf_filename)

        try:
            copyfile(src_conf_file, dst_conf_filename)
        except:
            pass
        
        shutil.copytree(self.__installerfiles, datadir, dirs_exist_ok=True)

        try:
            driverdir = os.path.join(self.DataPath, 'sqldrivers')
            os.mkdir(driverdir)
            copyfile(srcexefile, dstexefile)
        except FileExistsError:
            copyfile(srcexefile, dstexefile)
        except Exception as e:
            print('Fail: ' + str(e))

        self.copyArray(self.__RsdToCopy, self.__RsdDllDir)

