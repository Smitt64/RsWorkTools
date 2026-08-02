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
        # Скрипт компонента (регистрация ассоциации .md/.markdown), лежит в meta
        self.Script = 'installscript.qs'

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

        # Скрипт компонента (регистрация ассоциации файлов) кладётся в meta,
        # package.xml ссылается на него через элемент Script
        src_script = os.path.join(os.path.dirname(os.path.realpath(__file__)),
                                  'markdownviewer_installscript.qs')
        copyfile(src_script, os.path.join(self.MetaPath, 'installscript.qs'))

        # Файловая иконка для ассоциации .md/.markdown (используется в installscript.qs),
        # устанавливается рядом с exe
        src_fileicon = os.path.join(project_root, 'MarkdownViewer', 'res', 'markdown-file-icon.ico')
        if os.path.exists(src_fileicon):
            copyfile(src_fileicon, os.path.join(self.DataPath, 'markdown-file-icon.ico'))
        else:
            print('Warning: markdown-file-icon.ico not found: ' + src_fileicon)

    def getVersion(self):
        try:
            project_root = os.path.dirname(os.path.dirname(os.path.dirname(os.path.realpath(__file__))))
            releasedir = os.path.join(project_root, self.__filesToCopy[0].format(ConfigObj.inst().getBinaryType()))
            ver = super(MarkdownViewerPackage, self).getExeVersion(releasedir)
            print(ver)
            return ver
        except:
            return "Unknown version"
