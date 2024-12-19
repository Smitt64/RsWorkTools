import os
from config.configobj import ConfigObj
from worktoolsinstallers import worktoolsinstaller
import socket

fqdn = socket.getfqdn()
OutputPath = os.path.dirname(os.path.realpath(__file__))
RepoPath = os.path.join(OutputPath, 'repository')
OutputPath = os.path.join(OutputPath, 'bin')

print(fqdn)

ConfigObj.inst().setInstallerPath(OutputPath)

if (fqdn == 'PC-32099.softlab.ru'):
      ConfigObj.inst().setQtComplect('d:\\Qt5\\5.15.2\msvc2019\\')
      ConfigObj.inst().setQtInstallerFramework('d:\\Qt5\\Tools\\QtInstallerFramework\\4.6\\')
else:
      ConfigObj.inst().setQtComplect('d:\\Qt\\5.15.2\\msvc2019\\')
      ConfigObj.inst().setQtInstallerFramework('d:\\Qt\\Tools\\QtInstallerFramework\\4.1')
      
ConfigObj.inst().setWorkFmtSourceDir('D:\\Work\\WorkFMT')
ConfigObj.inst().setWorkLbrSourceDir('D:\\Work\\ResEditor')
ConfigObj.inst().setDatFilesPath('D:\\svn\\UranRSBankV6\\Main\\Distrib\\DBFile\\Data')

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
InstallerObj.addRepository('file:///{}'.format(RepoPath), 'Local')
InstallerObj.addRepository('file://PC-32099.softlab.ru/WorkToolsRepo', 'Remote')
InstallerObj.make(ConfigObj.inst().getInstallerPath())