import os, glob, sqlite3, codecs
from shutil import copyfile
from datetime import date
from pathlib import Path
from config.configobj import ConfigObj
from installer.installer import InstallerPackageInfoBase

class DiffToScriptComponent(InstallerPackageInfoBase):
    def __init__(self):
        today = date.today()
        self.__filesToCopy = ['DiffToScript/{}/DiffToScript.exe']
        
        super(DiffToScriptComponent, self).__init__()

        self.DisplayName = 'Diff To Script'
        self.Description = 'Мастер создания скриптов вставки в бд по изменениям в dat файлах'
        self.Name = 'com.rs.fmt.workfmt.difftoscript'
        self.Dependencies = ['com.rs.fmt.workfmt']
        self.ReleaseDate = today.strftime("%Y-%m-%d")

    def readLines(self, lines):
        columns = []
        is_column = False
        for line in lines:
            try:
                if ((not is_column) and (line[0] == '(')):
                    is_column = True
                
                if (is_column):
                    col = line.replace('(', '').replace(',', '').rstrip()

                pos = col.find(' ')

                
                if (col[-1] == ')'):
                    col = col.replace(')', '')
                    is_column = False

                if pos != -1:
                    col = col[:pos]

                if (not is_column):
                    if col != '':
                        columns.append(col)
                    break
                else:
                    if col != '':
                        columns.append(col)
            except:
                pass
        
        return columns
    
    def makeDatInfoFile(self):
        datstruct_info = os.path.join(self.DataPath, 'datstruct.info')
        conn = sqlite3.connect(datstruct_info)

        sql_statements = [ 
        '''CREATE TABLE IF NOT EXISTS DAT_FIELDS (
                id INTEGER PRIMARY KEY, 
                NAME text NOT NULL, 
                column TEXT
        );''']

        cursor = conn.cursor()
        for statement in sql_statements:
            cursor.execute(statement)

        index = 1
        dat_path = ConfigObj.inst().getDatFilesPath()
        for name in glob.glob(dat_path + '//*.dat'): 
            with codecs.open(name, "r", "866") as f:
                lines = f.read().splitlines()
                cols = self.readLines(lines)

                if len(cols) != 0:
                    for col in cols:
                        cursor.execute('insert into DAT_FIELDS(id, NAME, column)values(?, ?, ?)', (index, Path(name).stem.upper(), col.upper()))
                        index = index + 1

                    print(Path(name).stem)
                    print(cols)
                    print('-------------------------------------------')

        conn.commit()
        cursor.close()
        conn.close()

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

        self.makeDatInfoFile()

    def getVersion(self):
        releasedir = os.path.join(ConfigObj.inst().getWorkFmtSourceDir(), self.__filesToCopy[0].format(ConfigObj.inst().getBinaryType()))
        try:
            ver = super(DiffToScriptComponent, self).getExeVersion(releasedir)
            print(ver)
            return ver
        except:
            return "Unknown version"
