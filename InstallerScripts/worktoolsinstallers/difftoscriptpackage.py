import os, glob
from shutil import copyfile
from datetime import date
from config.configobj import ConfigObj
from installer.installer import InstallerPackageInfoBase

class DiffToScriptComponent(InstallerPackageInfoBase):
    def __init__(self):
        today = date.today()
        self.__filesToCopy = ['DiffToScript/{}/DiffToScript.exe']

        '''self.__syntaxhighlighter = ['DiffToScript/relations/Default.json',
            'RsWorkTools/ToolsRuntime/syntaxhighlighter/Visual Studio (Dark).json',
            'RsWorkTools/ToolsRuntime/syntaxhighlighter/Visual Studio (Light).json']'''
        
        super(DiffToScriptComponent, self).__init__()

        self.DisplayName = 'Diff To Script'
        self.Description = 'Мастер создания скриптов вставки в бд по изменениям в dat файлах'
        self.Name = 'com.rs.fmt.workfmt.difftoscript'
        self.Dependencies = ['com.rs.fmt.workfmt']
        self.ReleaseDate = today.strftime("%Y-%m-%d")

    def makeData(self, datadir):
        fmtdir = ConfigObj.inst().getWorkFmtSourceDir()
        relationsdir = os.path.join(self.DataPath, 'relations')

        for cpfiletemplate in self.__filesToCopy:
            filetocopy = cpfiletemplate.format(ConfigObj.inst().getBinaryType())
            srcexefile = os.path.join(fmtdir, filetocopy)
            dstexefile = os.path.join(self.DataPath, os.path.basename(filetocopy))
            copyfile(srcexefile, dstexefile)

        try:
            os.mkdir(relationsdir)
        except OSError as exc:
            pass

        relationssrc = os.path.join(fmtdir, 'DiffToScript\\relations')
        relationsmask = os.path.join(relationssrc, '*.json')
        for file in glob.glob(relationsmask):
            dstexefile = os.path.join(relationsdir, os.path.basename(file))
            copyfile(file, dstexefile)

    def getVersion(self):
        releasedir = os.path.join(ConfigObj.inst().getWorkFmtSourceDir(), self.__filesToCopy[0].format(ConfigObj.inst().getBinaryType()))
        try:
            ver = super(DiffToScriptComponent, self).getExeVersion(releasedir)
            print(ver)
            return ver
        except:
            return "Unknown version"
