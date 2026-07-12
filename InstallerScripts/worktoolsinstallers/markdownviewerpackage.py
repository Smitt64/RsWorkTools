import os
from shutil import copyfile
from datetime import date
from config.configobj import ConfigObj
from installer.installer import InstallerPackageInfoBase

class MarkdownViewerPackage(InstallerPackageInfoBase):
    def __init__(self):
        self.__filesToCopy = ['MarkdownViewer/{}/MarkdownViewer.exe']

        super(MarkdownViewerPackage, self).__init__()

        today = date.today()
        self.DisplayName = 'Markdown Viewer'
        self.Description = 'Markdown Viewer — утилита для просмотра Markdown-документов с интерфейсом Ribbon, MDI-вкладками и рендерингом через Qt WebEngine.'
        self.Name = 'com.rs.worktools.markdownviewer'
        self.ReleaseDate = today.strftime("%Y-%m-%d")

        self.Dependencies.append('com.rs.worktools.runtime')
        self.Dependencies.append('com.rs.qt.runtime')

    def makeData(self, datadir):
        project_root = os.path.dirname(os.path.dirname(os.path.dirname(os.path.realpath(__file__))))

        for cpfiletemplate in self.__filesToCopy:
            filetocopy = cpfiletemplate.format(ConfigObj.inst().getBinaryType())
            srcexefile = os.path.join(project_root, filetocopy)
            dstexefile = os.path.join(self.DataPath, os.path.basename(filetocopy))
            copyfile(srcexefile, dstexefile)

        src_changelog = os.path.join(project_root, 'MarkdownViewer', 'changelog')
        dst_changelog = os.path.join(self.DataPath, 'changelog')
        if os.path.exists(src_changelog):
            self.copyOverwrite(src_changelog, dst_changelog)

    def getVersion(self):
        try:
            project_root = os.path.dirname(os.path.dirname(os.path.dirname(os.path.realpath(__file__))))
            releasedir = os.path.join(project_root, self.__filesToCopy[0].format(ConfigObj.inst().getBinaryType()))
            ver = super(MarkdownViewerPackage, self).getExeVersion(releasedir)
            print(ver)
            return ver
        except:
            return "Unknown version"
