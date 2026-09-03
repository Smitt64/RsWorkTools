# Добавление нового модуля (пакета) в инсталлятор RsWorkTools

Инструкция для агента: как добавить новый компонент (утилиту, библиотеку, плагин)
в инсталлятор, собираемый скриптами из `InstallerScripts`.

## Общая архитектура

Сборка инсталлятора построена на **Qt Installer Framework** (`binarycreator`/`repogen`):

- `main.py` — точка входа. Настраивает `ConfigObj` (пути к Qt, исходникам, QtIFW),
  создаёт `WorkToolsInstaller` и вызывает `make()`.
- `worktoolsinstallers/worktoolsinstaller.py` — класс `WorkToolsInstaller`
  (наследник `InstallCreator`). Здесь **регистрируются все пакеты**: импорт модуля,
  создание экземпляра, `addPackage(...)`.
- `worktoolsinstallers/<name>package.py` — по одному файлу на пакет. Класс-наследник
  `InstallerPackageInfoBase` (из `installer/instpackageinfo.py`).
- `installer/installer.py` + `installer/instpackageinfo.py` — движок: генерирует
  `config.xml`, `packages/<Name>/package.xml`, вызывает `makeData()` каждого пакета,
  затем `binarycreator` и `repogen`.

Каждый пакет при сборке получает три пути (заполняет движок):

- `self.VendorPath` = `packages/<Name>`
- `self.DataPath` = `packages/<Name>/data` — сюда `makeData()` кладёт файлы;
  содержимое `data` устанавливается в `@TargetDir@` с сохранением структуры папок.
- `self.MetaPath` = `packages/<Name>/meta` — сюда кладутся installscript.qs,
  страницы UI, переводы.

## Шаги добавления нового пакета

### 1. Подготовить версионный ресурс (если модуль — exe/dll)

Версия пакета по умолчанию берётся из VERSIONINFO бинарника через
`InstallerPackageInfoBase.getExeVersion()` (win32api). Поэтому у модуля должен
быть `.rc` файл с `VS_VERSION_INFO`, подключённый в `.pro`:

```qmake
win32:RC_FILE += res/mymodule.rc
```

Если версия берётся иначе (из файла, константа) — переопределите `getVersion()`.

### 2. Создать файл пакета `worktoolsinstallers/<name>package.py`

Минимальный шаблон (см. `dumptoolpackage.py`, `markdownviewerpackage.py`):

```python
import os
from shutil import copyfile
from datetime import date
from config.configobj import ConfigObj
from installer.installer import InstallerPackageInfoBase

class MyModulePackage(InstallerPackageInfoBase):
    def __init__(self):
        # Пути относительно корня исходников; {} подставляется
        # ConfigObj.inst().getBinaryType() ('release'/'debug')
        self.__filesToCopy = ['MyModule/{}/MyModule.exe']

        super(MyModulePackage, self).__init__()

        today = date.today()
        self.DisplayName = 'My Module'              # обязательно
        self.Description = 'Что делает модуль'
        self.Name = 'com.rs.myarea.mymodule'        # обязательно, уникальный id
        self.ReleaseDate = today.strftime("%Y-%m-%d")

        # Зависимости — Name других пакетов:
        self.Dependencies.append('com.rs.qt.runtime')        # Qt runtime
        self.Dependencies.append('com.rs.worktools.runtime') # ToolsRuntime.dll
        # Для модулей WorkFMT: 'com.rs.fmt.workfmt' и т.п.

    def makeData(self, datadir):
        srcdir = ConfigObj.inst().getWorkFmtSourceDir()   # или getWorkLbrSourceDir()
        for cpfiletemplate in self.__filesToCopy:
            filetocopy = cpfiletemplate.format(ConfigObj.inst().getBinaryType())
            copyfile(os.path.join(srcdir, filetocopy),
                     os.path.join(self.DataPath, os.path.basename(filetocopy)))

    def getVersion(self):
        try:
            releasedir = os.path.join(ConfigObj.inst().getWorkFmtSourceDir(),
                self.__filesToCopy[0].format(ConfigObj.inst().getBinaryType()))
            return super(MyModulePackage, self).getExeVersion(releasedir)
        except:
            return "Unknown version"
```

Ключевые моменты:

- **`Name`** — уникальный dotted-id (`com.rs....`). По нему же инсталлятор
  определяет обновления, его указывают в `Dependencies` других пакетов.
- **Откуда брать исходники**: `ConfigObj` знает два корня —
  `getWorkFmtSourceDir()` (D:\Work\WorkFMT) и `getWorkLbrSourceDir()`
  (D:\Work\ResEditor). Если нужен новый корень — добавьте пару
  set/get в `config/configobj.py` и установку значения в `main.py`.
- **Куда класть файлы в `data/`**: структура папок под `DataPath` повторяется
  в целевой директории установки. Примеры:
  - exe/dll приложения → прямо в `DataPath`;
  - плагины ResEditor → `DataPath/reseditor/` (их ищет
    `RsResCore::getPluginSearchPaths` в `<appdir>/reseditor`);
  - иконки тем → `DataPath/resources/icons/`;
  - файлы истории версий → `DataPath/changelog/` (см. ниже).
- **Вспомогательные методы базового класса**: `copyOverwrite(src, dst)` —
  рекурсивное копирование каталога; `copyFileToDestrib(from, to)` — копирование
  файла с логом.
- **Скрипт установки** (опционально): если пакету нужны действия при установке
  (регистрация ассоциаций, ярлыки), задайте `self.Script = 'installscript.qs'`
  и скопируйте qs-файл в `self.MetaPath` внутри `makeData()`
  (пример — `worklbrmainpackage.py`).

### 3. Зарегистрировать пакет в `worktoolsinstaller.py`

Три правки в классе `WorkToolsInstaller`:

```python
from worktoolsinstallers import mymodulepackage        # 1. импорт
...
self.__MyModulePackage = mymodulepackage.MyModulePackage()  # 2. создание
...
self.addPackage(self.__MyModulePackage)               # 3. регистрация
```

Порядок `addPackage` определяет `SortingPriority` (порядок в дереве компонентов).

### 4. История версий (changelog)

Если модуль имеет историю версий, положите в корень его исходников XML
вида `com.rs.*.xml`:

```xml
<?xml version = "1.0" encoding = "UTF-8"?>
<changelog project="My Module">
    <version ver="1.0.0.0">
        <add>Добавлено ...;</add>
        <fix>Исправлено ...;</fix>
    </version>
</changelog>
```

Пакет `com.rs.changelog` (`changelogpackage.py`) сам соберёт все файлы по маске
`**/com.rs.*.xml` из каталогов WorkFMT и WorkLbr — отдельная регистрация
не нужна. Важно: имя файла должно начинаться с `com.rs.`.

### 5. Проверка

```bash
cd InstallerScripts
python main.py
```

Скрипт соберёт `bin/RsWorkToolsInstaller.exe` и локальный репозиторий в
`repository/`. Перед сборкой убедитесь, что модуль собран в конфигурации
`release` (именно её подставляет `getBinaryType()`).

## Существующие пакеты (справочно)

| Name | Назначение |
|------|-----------|
| `com.rs.fmt.workfmt` | WorkFMT — основная утилита |
| `com.rs.fmt.workfmt.dumptool` | DumpTool |
| `com.rs.lbr.worklbr` | WorkLbR / ResEditor (WorkRes.exe, rsrescore, rsreslbrfn) |
| `com.rs.lbr.resmenu` | Плагин редактора меню для WorkLbR |
| `com.rs.worktools.runtime` | ToolsRuntime.dll и зависимости |
| `com.rs.worktools.markdownviewer` | MarkdownViewer |
| `com.rs.qt.runtime` | Qt runtime |
| `com.rs.changelog` | Виртуальный пакет историй версий |
