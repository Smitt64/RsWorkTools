import os, glob
from shutil import copyfile
from datetime import date
from config.configobj import ConfigObj
from installer.installer import InstallerPackageInfoBase

class RsToolsRuntimePackage(InstallerPackageInfoBase):
    def __init__(self):
        self.__filesToCopy = ['ToolsRuntimeProj/ToolsRuntime/{}/ToolsRuntime.dll',
                              'ToolsRuntimeProj/ToolsRuntime/hunspell/lib/{}/libhunspell.dll']
        
        self.__styles = ['ToolsRuntimeProj/WindowsModernStyle/{}/WindowsModernStyle.dll']
        self.__syntaxhighlighter = []
        #['RsWorkTools/ToolsRuntime/syntaxhighlighter/Default.json',
        #    'ToolsRuntimeProj/ToolsRuntime/syntaxhighlighter/Visual Studio (Dark).json',
        #    'ToolsRuntimeProj/ToolsRuntime/syntaxhighlighter/Visual Studio (Light).json']
        
        super(RsToolsRuntimePackage, self).__init__()
        
        today = date.today()
        self.DisplayName = 'Work Tools Runtime'
        self.Description = 'Библиотеки, содержащие необходимый общий функционал'
        self.Name = 'com.rs.worktools.runtime'
        self.ReleaseDate = today.strftime("%Y-%m-%d")
        self.ForcedInstallation = True

        self.Dependencies.append('com.rs.qt.runtime')

    def makeData(self, datadir):
        fmtdir = ConfigObj.inst().getWorkLbrSourceDir()
        syntaxhighlighterdir = os.path.join(self.DataPath, 'syntaxhighlighter')
        stylesdir = os.path.join(self.DataPath, 'styles')
        dictionariesdir = os.path.join(self.DataPath, 'dictionaries')

        for cpfiletemplate in self.__filesToCopy:
            filetocopy = cpfiletemplate.format(ConfigObj.inst().getBinaryType())
            srcexefile = os.path.join(fmtdir, filetocopy)
            dstexefile = os.path.join(self.DataPath, os.path.basename(filetocopy))
            copyfile(srcexefile, dstexefile)

        try:
            os.mkdir(syntaxhighlighterdir)
        except OSError as exc:
            pass

        try:
            os.mkdir(stylesdir)
        except OSError as exc:
            pass

        try:
            os.mkdir(dictionariesdir)
        except OSError as exc:
            pass

        for style in self.__styles:
            filetocopy = style.format(ConfigObj.inst().getBinaryType())
            srcexefile = os.path.join(fmtdir, filetocopy)
            dstexefile = os.path.join(self.DataPath, 'styles/' + os.path.basename(filetocopy))
            copyfile(srcexefile, dstexefile)

        for syntaxhighlighter in self.__syntaxhighlighter:
            srcfile = os.path.join(fmtdir, syntaxhighlighter)
            dstfile = os.path.join(syntaxhighlighterdir, os.path.basename(syntaxhighlighter))
            copyfile(srcfile, dstfile)

        src_dictionaries = os.path.join(fmtdir, 'ToolsRuntimeProj/ToolsRuntime/hunspell/dictionaries/*.*')
        for spell in glob.glob(src_dictionaries):
            srcfile = os.path.join(fmtdir, spell)
            dstfile = os.path.join(dictionariesdir, os.path.basename(spell))
            copyfile(srcfile, dstfile)

    def getVersion(self):
        try:
            releasedir = os.path.join(ConfigObj.inst().getWorkLbrSourceDir(), self.__filesToCopy[0].format(ConfigObj.inst().getBinaryType()))
            ver = super(RsToolsRuntimePackage, self).getExeVersion(releasedir)
            print(ver)
            return ver
        except:
            return "Unknown version"
