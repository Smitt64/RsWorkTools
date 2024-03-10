import os, glob, time
from shutil import copyfile
from datetime import date
from config.configobj import ConfigObj
from worktoolsinstallers.rscomponentbase import RsComponentBase

class ChangeLogPackage(RsComponentBase):
    def __init__(self):
        super(ChangeLogPackage, self).__init__()

        today = date.today()
        self.DisplayName = 'ChangeLog'
        self.Description = 'История изменений'
        self.Name = 'com.rs.changelog'
        self.ForcedUpdate = True
        self.ForcedInstallation = True
        self.Default = None
        self.Virtual = True
        self.Essential = True
        self.ReleaseDate = today.strftime("%Y-%m-%d")

    def __makeDataFromPath(self, path):
        mask = os.path.join(path, '**/com.rs.*.xml')
        
        for filename in glob.glob(mask, recursive=True):
            basedbfiletoolname = os.path.basename(filename)
            dstexefile = os.path.join(self.DataPath, 'changelog/' + os.path.basename(basedbfiletoolname))

            try:
                driverdir = os.path.join(self.DataPath, 'changelog')
                os.mkdir(driverdir)
                copyfile(filename, dstexefile)
            except FileExistsError:
                copyfile(filename, dstexefile)
            except Exception as e:
                print('Fail: ' + str(e))

    def getVersion(self):
        return str(int(time.time()))

    def makeData(self, datadir):
        fmtdir = ConfigObj.inst().getWorkFmtSourceDir()
        lbrdir = ConfigObj.inst().getWorkLbrSourceDir()
        
        self.__makeDataFromPath(fmtdir)
        self.__makeDataFromPath(lbrdir)