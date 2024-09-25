import os, shutil
from shutil import copyfile
from datetime import date
from config.configobj import ConfigObj
from installer.installer import InstallerPackageInfoBase

class WorkFmtMainPackage(InstallerPackageInfoBase):
    def __init__(self):
        self.__filesToCopy = ['WorkFMT/{}/WorkFMT.exe',
            'FmtLib/{}/FmtLib.dll',
            'FmtDbgHelp/{}/FmtDbgHelp.dll',
            'FmtLib/res/CapitalizeField.exe',
            'FmtRslModule/{}/FmtRslModule.dll']

        super(WorkFmtMainPackage, self).__init__()
        
        today = date.today()
        self.DisplayName = 'Work Fmt'
        self.Description = 'WorkFMT – утилита, предоставляющая интерфейсный режим работы с FMT словарем непосредственно в схеме в БД. ' + \
            'Программное обеспечение позволяет управлять уже существующими данными в словаре, а так же создавать новые записи.'
        self.Name = 'com.rs.fmt.workfmt'
        self.ReleaseDate = today.strftime("%Y-%m-%d")

        self.Dependencies.append('com.rs.tools.runtime')
        self.Dependencies.append('com.rs.qt.runtime')

    def copyOverwrite(self, from_path, to_path):
        shutil.copytree(from_path, to_path, dirs_exist_ok=True)

    def makeData(self, datadir):
        fmtdir = ConfigObj.inst().getWorkFmtSourceDir()
        macdir = os.path.join(datadir, 'mac')

        for cpfiletemplate in self.__filesToCopy:
            filetocopy = cpfiletemplate.format(ConfigObj.inst().getBinaryType())
            srcexefile = os.path.join(fmtdir, filetocopy)
            dstexefile = os.path.join(self.DataPath, os.path.basename(filetocopy))
            copyfile(srcexefile, dstexefile)

        try:
            os.makedirs(macdir)
        except:
            pass

        srcmac = os.path.join(fmtdir, 'FmtLib/mac')
        self.copyOverwrite(srcmac, macdir)

    def getVersion(self):
        try:
            releasedir = os.path.join(ConfigObj.inst().getWorkFmtSourceDir(), self.__filesToCopy[0].format(ConfigObj.inst().getBinaryType()))
            ver = super(WorkFmtMainPackage, self).getExeVersion(releasedir)
            print(ver)
            return ver
        except:
            return "Unknown version"
