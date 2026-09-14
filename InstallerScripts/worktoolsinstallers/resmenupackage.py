import os
from shutil import copyfile
from datetime import date
from config.configobj import ConfigObj
from installer.installer import InstallerPackageInfoBase

class ResMenuPackage(InstallerPackageInfoBase):
    def __init__(self):
        self.__filesToCopy = ['ResMenu/{}/ResMenuPlugin.dll']

        super(ResMenuPackage, self).__init__()

        today = date.today()
        self.DisplayName = 'ResMenu Plugin'
        self.Description = 'ResMenu – плагин редактора ресурсов меню (Menu) для WorkLbR'
        self.Name = 'com.rs.lbr.resmenu'
        self.ReleaseDate = today.strftime("%Y-%m-%d")

        # Плагин загружается WorkRes.exe через rsrescore, поэтому зависит от
        # основного пакета Work Lbr; Qt/ToolsRuntime подтягиваются транзитивно,
        # но указываем явно на случай установки без пакета Work Lbr
        self.Dependencies.append('com.rs.lbr.worklbr')
        self.Dependencies.append('com.rs.qt.runtime')
        self.Dependencies.append('com.rs.worktools.runtime')

    def makeData(self, datadir):
        fmtdir = ConfigObj.inst().getWorkLbrSourceDir()

        # rsrescore ищет плагины в <appdir>/reseditor
        # (см. RsResCore::getPluginSearchPaths)
        dstplugindir = os.path.join(self.DataPath, 'reseditor')
        os.makedirs(dstplugindir, exist_ok=True)

        for cpfiletemplate in self.__filesToCopy:
            filetocopy = cpfiletemplate.format(ConfigObj.inst().getBinaryType())
            srcexefile = os.path.join(fmtdir, filetocopy)
            dstexefile = os.path.join(dstplugindir, os.path.basename(filetocopy))
            copyfile(srcexefile, dstexefile)

        # RsBankLibRes.dll ставится в корень каталога установки
        # (рядом с WorkRes.exe), плагин загружает её динамически
        src_installerfiles = os.path.join(fmtdir, 'ResMenu', 'installerfiles')
        if os.path.isdir(src_installerfiles):
            for fname in os.listdir(src_installerfiles):
                srcfile = os.path.join(src_installerfiles, fname)
                if os.path.isfile(srcfile):
                    copyfile(srcfile, os.path.join(self.DataPath, fname))

        # Иконки плагина: при сборке копируются в bin/resources/icons,
        # в инсталлятор кладём из исходников в общую папку темы
        src_icons = os.path.join(fmtdir, 'ResMenu', 'icons')
        dst_icons = os.path.join(self.DataPath, 'resources', 'icons')
        if os.path.exists(src_icons):
            self.copyOverwrite(src_icons, dst_icons)

        # Шаблоны меню с примерами: rsrescore ищет их в
        # <appdir>/reseditor/templates/menu
        # (ResTemplateRegistry::programTemplatesDir)
        src_templates = os.path.join(fmtdir, 'ResMenu', 'templates', 'menu')
        dst_templates = os.path.join(self.DataPath, 'reseditor', 'templates', 'menu')
        if os.path.exists(src_templates):
            self.copyOverwrite(src_templates, dst_templates)

        # Пользовательские элементы ToolBox с примерами:
        # <appdir>/reseditor/toolbox/menu (ResTemplateRegistry::programToolBoxDir)
        src_toolbox = os.path.join(fmtdir, 'ResMenu', 'toolbox', 'menu')
        dst_toolbox = os.path.join(self.DataPath, 'reseditor', 'toolbox', 'menu')
        if os.path.exists(src_toolbox):
            self.copyOverwrite(src_toolbox, dst_toolbox)

    def getVersion(self):
        try:
            releasedir = os.path.join(ConfigObj.inst().getWorkLbrSourceDir(), self.__filesToCopy[0].format(ConfigObj.inst().getBinaryType()))
            ver = super(ResMenuPackage, self).getExeVersion(releasedir)
            print(ver)
            return ver
        except:
            return "Unknown version"
