import os
from shutil import copyfile
from datetime import date
from config.configobj import ConfigObj
from installer.installer import InstallerPackageInfoBase

class RegparmToolComponent(InstallerPackageInfoBase):
    def __init__(self):
        today = date.today()
        self.__filesToCopy = ['RegparmTool/{}/RegparmTool.exe']

        super(RegparmToolComponent, self).__init__()

        self.DisplayName = 'Regparm Tool'
        self.Description = 'Мастер работы с реестром параметров: просмотр dat файлов реестра, ' + \
            'добавление настроек из Word документа, перенос настроек из подключенной БД в dat файлы репозитория'
        self.Name = 'com.rs.fmt.workfmt.regparmtool'
        self.Dependencies = ['com.rs.fmt.workfmt',
            'com.rs.fmt.workfmt.difftoscript',
            'com.rs.fmt.workfmt.rsddriver',
            'com.rs.worktools.runtime']
        self.ReleaseDate = today.strftime("%Y-%m-%d")

    def makeData(self, datadir):
        fmtdir = ConfigObj.inst().getWorkFmtSourceDir()

        for cpfiletemplate in self.__filesToCopy:
            filetocopy = cpfiletemplate.format(ConfigObj.inst().getBinaryType())
            srcexefile = os.path.join(fmtdir, filetocopy)
            dstexefile = os.path.join(self.DataPath, os.path.basename(filetocopy))
            copyfile(srcexefile, dstexefile)

        # rsl макросы: мастер ищет их в ".\mac\regparmtool" относительно exe
        macdir = os.path.join(datadir, 'mac', 'regparmtool')
        try:
            os.makedirs(macdir)
        except:
            pass

        srcmac = os.path.join(fmtdir, 'RegparmTool/mac')
        self.copyOverwrite(srcmac, macdir)

    def getVersion(self):
        releasedir = os.path.join(ConfigObj.inst().getWorkFmtSourceDir(), self.__filesToCopy[0].format(ConfigObj.inst().getBinaryType()))
        try:
            ver = super(RegparmToolComponent, self).getExeVersion(releasedir)
            print(ver)
            return ver
        except:
            return "Unknown version"
