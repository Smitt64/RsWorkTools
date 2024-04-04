import os
from shutil import copyfile
from datetime import date
from config.configobj import ConfigObj
from installer.installer import InstallerPackageInfoBase

class WorkFmtMainPackage(InstallerPackageInfoBase):
    def __init__(self):
        self.__filesToCopy = ['WorkFMT/{}/WorkFMT.exe',
            'FmtLib/{}/FmtLib.dll',
            'FmtDbgHelp/{}/FmtDbgHelp.dll',
            'FmtLib/res/CapitalizeField.exe']

        super(WorkFmtMainPackage, self).__init__()
        
        today = date.today()
        self.DisplayName = 'Work Fmt'
        self.Description = 'WorkFMT – утилита, предоставляющая интерфейсный режим работы с FMT словарем непосредственно в схеме в БД. ' + \
            'Программное обеспечение позволяет управлять уже существующими данными в словаре, а так же создавать новые записи.'
        self.Name = 'com.rs.fmt.workfmt'
        self.ReleaseDate = today.strftime("%Y-%m-%d")

        self.Dependencies.append('com.rs.tools.runtime')
        self.Dependencies.append('com.rs.qt.runtime')

    def makeData(self, datadir):
        fmtdir = ConfigObj.inst().getWorkFmtSourceDir()
        for cpfiletemplate in self.__filesToCopy:
            filetocopy = cpfiletemplate.format(ConfigObj.inst().getBinaryType())
            srcexefile = os.path.join(fmtdir, filetocopy)
            dstexefile = os.path.join(self.DataPath, os.path.basename(filetocopy))
            copyfile(srcexefile, dstexefile)

    def getVersion(self):
        try:
            releasedir = os.path.join(ConfigObj.inst().getWorkFmtSourceDir(), self.__filesToCopy[0].format(ConfigObj.inst().getBinaryType()))
            ver = super(WorkFmtMainPackage, self).getExeVersion(releasedir)
            print(ver)
            return ver
        except:
            return "Unknown version"
