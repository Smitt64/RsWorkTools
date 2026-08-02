import os
from shutil import copyfile
from datetime import date
from config.configobj import ConfigObj
from installer.installer import InstallerPackageInfoBase

class WorkLbrMainPackage(InstallerPackageInfoBase):
    def __init__(self):
        self.__filesToCopy = [
            'RsResEditor/{}/WorkRes.exe',
            'rsrescore/{}/rsrescore.dll',
            'rsreslbrfn/{}/rsreslbrfn.dll'
            ]

        super(WorkLbrMainPackage, self).__init__()
        
        today = date.today()
        self.DisplayName = 'Work Lbr'
        self.Description = 'WorkLbR – утилита, предоставляющая интерфейсный создания ресурсов программы'
        self.Name = 'com.rs.lbr.worklbr'
        self.ReleaseDate = today.strftime("%Y-%m-%d")
        # Скрипт компонента (регистрация ассоциации *.lbr), лежит в meta
        self.Script = 'installscript.qs'

        self.Dependencies.append('com.rs.tools.runtime')
        self.Dependencies.append('com.rs.qt.runtime')
        self.Dependencies.append('com.rs.worktools.runtime')

    def makeData(self, datadir):
        fmtdir = ConfigObj.inst().getWorkLbrSourceDir()
        for cpfiletemplate in self.__filesToCopy:
            filetocopy = cpfiletemplate.format(ConfigObj.inst().getBinaryType())
            srcexefile = os.path.join(fmtdir, filetocopy)
            dstexefile = os.path.join(self.DataPath, os.path.basename(filetocopy))
            copyfile(srcexefile, dstexefile)

        src_icons = os.path.join(fmtdir, 'RsResEditor', 'res', 'icons')
        dst_icons = os.path.join(self.DataPath, 'resources', 'icons')
        self.copyOverwrite(src_icons, dst_icons)

        # Скрипт компонента (регистрация ассоциации *.lbr) кладётся в meta,
        # package.xml ссылается на него через элемент Script
        src_script = os.path.join(os.path.dirname(os.path.realpath(__file__)),
                                  'worklbr_installscript.qs')
        copyfile(src_script, os.path.join(self.MetaPath, 'installscript.qs'))

        # Файловая иконка для ассоциации *.lbr (используется в installscript.qs),
        # устанавливается рядом с exe
        src_fileicon = os.path.join(fmtdir, 'RsResEditor', 'res', 'lbr-file-icon.ico')
        if os.path.exists(src_fileicon):
            copyfile(src_fileicon, os.path.join(self.DataPath, 'lbr-file-icon.ico'))
        else:
            print('Warning: lbr-file-icon.ico not found: ' + src_fileicon)

    def getVersion(self):
        try:
            releasedir = os.path.join(ConfigObj.inst().getWorkLbrSourceDir(), self.__filesToCopy[0].format(ConfigObj.inst().getBinaryType()))
            ver = super(WorkLbrMainPackage, self).getExeVersion(releasedir)
            print(ver)
            return ver
        except:
            return "Unknown version"
