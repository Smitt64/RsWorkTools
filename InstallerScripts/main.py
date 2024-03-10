import os
from config.configobj import ConfigObj
from worktoolsinstallers import worktoolsinstaller

OutputPath = os.path.dirname(os.path.realpath(__file__))
RepoPath = os.path.join(OutputPath, 'repository')
OutputPath = os.path.join(OutputPath, 'bin')

ConfigObj.inst().setInstallerPath(OutputPath)
ConfigObj.inst().setQtComplect('d:\\Qt\\5.15.2\msvc2019\\')
ConfigObj.inst().setQtInstallerFramework('d:\\Qt\\Tools\\QtInstallerFramework\\4.1')
ConfigObj.inst().setWorkFmtSourceDir('D:\\Work\\WorkFMT')
ConfigObj.inst().setWorkLbrSourceDir('D:\\Work\\ResEditor')

print('Installer output path:')
print(ConfigObj.inst().getInstallerPath())

print()
print('Qt installer framework:')
print(ConfigObj.inst().getQtInstallerFramework())

print()
print('Qt version:')
print(ConfigObj.inst().getQtComplect().getVersion() + 
      ' ({0})'.format(ConfigObj.inst().getQtComplect().getComplect()))
print(ConfigObj.inst().getQtComplect().getReleaseDate())

InstallerObj = worktoolsinstaller.WorkToolsInstaller()
InstallerObj.addRepository('file:///{}'.format(RepoPath), 'Test Repo')
InstallerObj.make(ConfigObj.inst().getInstallerPath())