import os

class QtDestribElement:
    def __init__(self, path : str):
        self.__path = str(path)
        self.__version, self.__release_date = self.__read_qt_version()
        self.__complect = self.__read_complect()

    def __read_complect(self):
        return str(os.path.basename(os.path.normpath(self.__path)))

    def __read_qt_version(self):
        mkspecs = os.path.join(self.__path, 'mkspecs\\qconfig.pri')
        version = ''
        release_date = ''

        try:
            f = open(mkspecs, 'r', encoding='utf8')
            lines = f.readlines()
            for line in lines:
                if line.startswith('QT_VERSION'):
                    values = line.split('=')
                    version = values[1].strip()

                if line.startswith('QT_RELEASE_DATE'):
                    values = line.split('=')
                    release_date = values[1].strip()
            f.close()
        except:
            raise Exception('Can\'t read file ' + mkspecs)
        return str(version), str(release_date)

    def getVersion(self):
        return self.__version

    def getReleaseDate(self):
        return self.__release_date

    def getPath(self):
        return self.__path

    def getComplect(self):
        return self.__complect

class ConfigObj:
    __instance = None

    def __init__(self):
        self.__qtComplect = None
        self.__qtInstallerFramework = None

        self.__installerPath = os.path.dirname(os.path.realpath(__file__))
        os.path.join(self.__installerPath, 'bin')

        self.__workfmtsourcedir = None#'D:\\Work\\WorkFMT'
        self.__worklbrsourcedir = None
        self.__msvcvars = 'D:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\Professional\\VC\\Auxiliary\\Build\\vcvars32.bat'
        #os.path.dirname(__file__)

    @staticmethod
    def inst():
        if ConfigObj.__instance is None:
            ConfigObj.__instance = ConfigObj()

        return ConfigObj.__instance

    def getMsvcBat(self):
        return self.__msvcvars

    def setQtComplect(self, complect):
        self.__qtComplect = QtDestribElement(complect)
    
    def getQtComplect(self):
        return self.__qtComplect

    def setQtInstallerFramework(self, complect : str) -> None:
        self.__qtInstallerFramework = complect

    def getInstallerPath(self) -> str:
        return self.__installerPath
    
    def setInstallerPath(self, value : str) -> None:
        self.__installerPath = value

    def getQtInstallerFramework(self) -> str:
        return self.__qtInstallerFramework

    def getWorkFmtSourceDir(self) -> str:
        return self.__workfmtsourcedir

    def getBinaryType(self) -> str:
        return 'release'#'debug'
    
    def setWorkFmtSourceDir(self, path : str) -> None:
        self.__workfmtsourcedir = path

    def getWorkFmtSourceDir(self) -> str:
        return self.__workfmtsourcedir

    def getWorkLbrSourceDir(self) -> str:
        return self.__worklbrsourcedir

    def setWorkLbrSourceDir(self, path : str) -> None:
        self.__worklbrsourcedir = path