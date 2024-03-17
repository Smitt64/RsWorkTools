from installer.installer import InstallCreator
from worktoolsinstallers import workfmtmainpackage
from worktoolsinstallers import rstoolspackage
from worktoolsinstallers import qtpackage
from worktoolsinstallers import dbfiletoolpackage
from worktoolsinstallers import rsddriverpackage
from worktoolsinstallers import dumptoolpackage
from worktoolsinstallers import worklbrmainpackage
from worktoolsinstallers import changelogpackage
from worktoolsinstallers import rstoolsruntime

class WorkToolsInstaller(InstallCreator):
    def __init__(self):
        super(WorkToolsInstaller, self).__init__(name = 'RsWorkTools', 
                                                 version='1.0.0',
                                                 title='RsWorkTools',
                                                 publisher='Serpkov Nikita')
        
        self.setOutputName('RsWorkToolsInstaller.exe')
        self.MaintenanceToolName = 'RsWorkMaintenanceTool'
        self.MaintenanceToolIniFile = 'RsWorkMaintenanceTool.ini'
        self.WizardDefaultWidth = 1000

        self.__WorkFmtPackage = workfmtmainpackage.WorkFmtMainPackage()
        self.__RsToolsBinPackage = rstoolspackage.RsToolsPackage()
        self.__QtBinPackage = qtpackage.QtPackage()
        self.__DBFileToolPackage = dbfiletoolpackage.DBFileToolPackage()
        self.__RsdPackage = rsddriverpackage.RsdDriverPackage()
        self.__DumpToolPackage = dumptoolpackage.DumpToolComponent()
        self.__WorkLbrPackage = worklbrmainpackage.WorkLbrMainPackage()
        self.__ChangeLogPackage = changelogpackage.ChangeLogPackage()
        self.__ToolsRuntimePackage = rstoolsruntime.RsToolsRuntimePackage()

        self.addPackage(self.__WorkFmtPackage)
        self.addPackage(self.__RsToolsBinPackage)
        self.addPackage(self.__QtBinPackage)
        self.addPackage(self.__DBFileToolPackage)
        self.addPackage(self.__RsdPackage)
        self.addPackage(self.__DumpToolPackage)
        self.addPackage(self.__WorkLbrPackage)
        self.addPackage(self.__ChangeLogPackage)
        self.addPackage(self.__ToolsRuntimePackage)

