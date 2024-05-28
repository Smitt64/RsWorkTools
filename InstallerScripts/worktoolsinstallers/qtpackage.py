import os, ntpath, glob
from config.configobj import ConfigObj
from installer.installer import InstallerPackageInfoBase

class QtPackage(InstallerPackageInfoBase):
    def __init__(self):
        self.__qt_element = ConfigObj.inst().getQtComplect()
        self.__QtBinDir = os.path.join(self.__qt_element.getPath(), 'bin')
        self.__QtPluginsDir = os.path.join(self.__qt_element.getPath(), 'plugins')

        self.__QtDllsToCopy = [
            'Qt{}Core.dll', 
            'Qt{}Gui.dll', 
            'Qt{}Script.dll', 
            'Qt{}ScriptTools.dll', 
            'Qt{}Sql.dll', 
            'Qt{}Widgets.dll', 
            'Qt{}Xml.dll',
            'Qt{}Svg.dll',
            'Qt{}WebEngine.dll',
            'Qt{}WebEngineCore.dll',
            'Qt{}WebEngineWidgets.dll',
            'Qt{}Quick.dll',
            'Qt{}Qml.dll',
            'Qt{}PrintSupport.dll',
            'Qt{}Network.dll',
            'Qt{}QuickWidgets.dll',
            'Qt{}QmlModels.dll',
            'Qt{}Positioning.dll',
            'Qt{}WebChannel.dll',
            'Qt{}Test.dll',
            'libEGL.dll',
            'QtWebEngineProcess.exe'
        ]

        self.__VersionedDlls = [
            'd3dcompiler*.dll',
            'libgcc_s_dw*.dll',
            'libGLES*.dll',
            'libstdc++*.dll',
            'libwinpthread*.dll'
        ]

        self.__QtFoldersToCopy = [
            'platforms',
            'imageformats',
            'iconengines',
            'sqldrivers',
            'styles'
        ]

        self.__QtOtherFoldersToCopy = [
            'platforms',
            'imageformats',
            'iconengines',
            'sqldrivers',
            'styles'
        ]

        super(QtPackage, self).__init__()

        self.DisplayName = 'Qt Framework Binaries'
        self.Description = 'Библиотеки, необходимые для работы программы'
        self.Name = 'com.rs.qt.runtime'
        self.ReleaseDate = self.__qt_element.getReleaseDate()

        self.AutoDependOn.append('com.rs.fmt.workfmt')
        #self.ForcedInstallation = True

    def getVersion(self):
        return self.__qt_element.getVersion() + '.2'

    def __extract_major(self, version):
        dot = version.find('.')
        major = version
        if dot != -1:
            major = version[:dot]
        return major

    def __is_qt_debug_dll(self, path):
        is_not_qt_debug = False
        base_path = os.path.dirname(path)
        base = ntpath.basename(path)
        filename, file_extension = os.path.splitext(base)
        filename_tmp = os.path.join(base_path, filename[:-1] + file_extension)
        if (os.path.isfile(filename_tmp)):
            is_not_qt_debug = True
        return is_not_qt_debug

    def __copy_qt_files(self, src_path, dst_path, mask = '/*.dll'):
        files = glob.glob(src_path + mask)
        for file in files:
            if not self.__is_qt_debug_dll(file):
                dst_dll = os.path.join(dst_path, ntpath.basename(file))
                self.__copy_file_destrib(file, dst_dll)

    def __copy_file_destrib(self, fromf, tof):
        self.copyFileToDestrib(fromf, tof)

    def __check_dir(self, path):
        result = True
        try:
            if not os.path.isdir(path):
                print('\'',path , '\' not exists. Create: ', end='', sep = '')
                os.mkdir(path)
                print('Ok')
            else:
                print('\'',path , '\' exists', sep = '')
        except FileExistsError:
            print('Fail')
            result = False
        return result

    def __copy_qt_dir(self, qtpluginsdir, destribfolder, _dir, mask = '/*.dll'):
        print('Copy Qt ', _dir, ' : ', sep='')
        destrib_platforms = os.path.join(destribfolder, _dir)
        QtDir = os.path.join(qtpluginsdir, _dir)
        if self.__check_dir(destrib_platforms):
            self.__copy_qt_files(QtDir, destrib_platforms, mask)

    def makeData(self, datadir):
        major = self.__extract_major(self.__qt_element.getVersion())

        for dll in self.__QtDllsToCopy:
            qtdll = dll.format(major)
            qtdllpath = os.path.join(self.__QtBinDir, qtdll)
            qtdlldstpath = os.path.join(self.DataPath, qtdll)
            self.__copy_file_destrib(qtdllpath, qtdlldstpath)

        for dll in self.__VersionedDlls:
            qtdllpath = os.path.join(self.__QtBinDir, dll)
            for customdll in glob.glob(qtdllpath):
                qtdlldstpath = os.path.join(self.DataPath, os.path.basename(customdll))
                self.__copy_file_destrib(customdll, qtdlldstpath)

        for folder in self.__QtFoldersToCopy:
            print()
            self.__copy_qt_dir(self.__QtPluginsDir, self.DataPath, folder)

        print()
        
        self.__copy_qt_dir(self.__qt_element.getPath(), self.DataPath, 'translations', '/*_ru.qm')

        #qtwebengine_res = os.path.join(self.__qt_element.getPath(), 'resources')
        try:
            driverdir = os.path.join(self.DataPath, 'resources')
            os.mkdir(driverdir)
            self.__copy_qt_dir(self.__qt_element.getPath(), self.DataPath, 'resources', '/*.*')
        except FileExistsError:
            self.__copy_qt_dir(self.__qt_element.getPath(), self.DataPath, 'resources', '/*.*')
        except Exception as e:
            print('Fail: ' + str(e))
