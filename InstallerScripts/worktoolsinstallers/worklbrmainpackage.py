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

        # Шаблоны панелей с примерами: rsrescore ищет их в
        # <appdir>/reseditor/templates/panels
        # (ResTemplateRegistry::programTemplatesDir)
        src_templates = os.path.join(fmtdir, 'rsbaseeditor', 'templates', 'panels')
        dst_templates = os.path.join(self.DataPath, 'reseditor', 'templates', 'panels')
        if os.path.exists(src_templates):
            self.copyOverwrite(src_templates, dst_templates)

        # Пользовательские элементы ToolBox с примерами:
        # <appdir>/reseditor/toolbox/panels (ResTemplateRegistry::programToolBoxDir)
        src_toolbox = os.path.join(fmtdir, 'rsbaseeditor', 'toolbox', 'panels')
        dst_toolbox = os.path.join(self.DataPath, 'reseditor', 'toolbox', 'panels')
        if os.path.exists(src_toolbox):
            self.copyOverwrite(src_toolbox, dst_toolbox)

    def getVersion(self):
        try:
            releasedir = os.path.join(ConfigObj.inst().getWorkLbrSourceDir(), self.__filesToCopy[0].format(ConfigObj.inst().getBinaryType()))
            ver = super(WorkLbrMainPackage, self).getExeVersion(releasedir)
            print(ver)
            return ver
        except:
            return "Unknown version"
