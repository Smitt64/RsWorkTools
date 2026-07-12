# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

RsWorkTools is a Qt5 C++ shared library and plugin framework for Windows that provides:
- RSL scripting language integration (binding C++ QObjects to an external RSL runtime)
- A code editor widget with pluggable syntax highlighters (C++, RSL, SQL, XML)
- Custom widgets, ribbon UI (SARibbon), options dialogs, spell checking, and archive handling

The codebase is Russian-language (comments and some identifiers). Every source file starts with a PVS-Studio suppression comment.

## Build System

The project has **dual build systems**. qmake is the primary day-to-day build; CMake exists but is secondary and may lag behind.

### qmake (primary)

- Qt 5.15.2, MSVC2019, 32-bit (`win32-msvc`)
- Qt installed at `D:\Qt5\5.15.2\msvc2019`
- Top-level `.pro` file: `ToolsRuntimeProj.pro`
- Build the entire project:
  ```bash
  qmake ToolsRuntimeProj.pro -spec win32-msvc
  nmake
  ```
- Or use Qt Creator with `ToolsRuntimeProj.pro`

### CMake (secondary)

- Hardcodes Qt path: `d:/Qt/5.15.2/msvc2019/lib/cmake`
- Top-level `CMakeLists.txt` builds subdirectories: `RslTest`, `RslToolRuntimeStatic`, `ToolsRuntime`, `WindowsModernStyle`
- Note: `CircularMenuTest` is **not** included in CMake

### Subproject dependency chain

```
RslToolRuntimeStatic  ←  ToolsRuntime  ←  RslTest
                                    ←  CircularMenuTest
WindowsModernStyle (independent Qt style plugin)
```

## Architecture

### Module Structure

The repository is a `subdirs` project with these targets:

| Target | Type | Depends on | Purpose |
|--------|------|-----------|---------|
| `RslToolRuntimeStatic` | static lib + Qt plugin | — | Registers all RSL static modules via `RslStaticModuleInterface` |
| `ToolsRuntime` | shared lib (DLL) | RslToolRuntimeStatic | Main library: RSL bindings, editor, widgets, utilities |
| `RslTest` | executable | ToolsRuntime | Development test app for RSL/script features |
| `CircularMenuTest` | executable | ToolsRuntime | Test app for the circular menu widget |
| `WindowsModernStyle` | shared plugin | Qt private headers | Modern Windows Qt style plugin |

### RSL Script Binding System

This is the most complex cross-file subsystem. RSL is an external scripting language (closed-source runtime: `RSScript.dll`, `rsrtlwm.dll`).

**Three-layer architecture:**

1. **Runtime layer** (`rsscript/`): Low-level binding infrastructure
   - `RegisterInfoBase` — base class that introspects a `QObject`'s `QMetaObject` and exposes it to RSL via callback functions (`ObjFindMember`, `ObjEnumProps`, etc.)
   - `RslExecutor` — executes RSL scripts/play reports; owns the RSL instance
   - `addtemplproc.hpp` — `rslAddTemplateProc(name, func)` macro to register free C++ functions as RSL procedures using variadic templates

2. **Module layer** (`rslmodule/`): RSL modules that group related functionality
   - Each module inherits `RslStaticModule` (Init/Proc/Close lifecycle)
   - Submodules: `io/`, `sql/`, `richtext/`, `regexp/`, `types/`, `dialogs/`, `stddialogs.cpp`, `uistaticmodule.cpp`
   - `ToolsRuntimeModule` (in `toolsruntimemodule.cpp`) is the root module that registers all submodules
   - `toolsruntinersl.h/cpp` — `ToolsRuntime` QObject with `Q_INVOKABLE` methods exposed to RSL

3. **Plugin layer** (`RslToolRuntimeStatic/`):
   - `RslToolsRuntimeModule` implements `RslStaticModuleInterface` (Qt plugin interface)
   - This is a static plugin loaded at link time; its `registerStaticModules()` calls into `ToolsRuntimeModule`
   - Declared in `RslToolsRuntimeModule.json`

**Key pattern for adding RSL-callable C++:**
- For QObject classes: inherit `QObject`, use `Q_INVOKABLE` / `Q_PROPERTY`, then register via `RegisterInfoBase`
- For free functions: use `rslAddTemplateProc("FuncName", MyFunc)` from `addtemplproc.hpp`
- The RSL value ↔ QVariant conversion helpers are in `rslexecutor.h` (`GetFuncParam`, `SetReturnVal`, `SetValueFromVariant`, etc.)

### Code Editor

- `CodeEditor` (`codeeditor/codeeditor.h`) — `QPlainTextEdit` subclass with line numbers, search widget, and pluggable highlighter
- `CodeHighlighter` — abstract base; concrete highlighters: `CppCodeHighlighter`, `RslCodeHighlighter`, `SqlCodeHighlighter`, `XmlCodeHighlighter`
- Styles are loaded from JSON files in `codeeditor/Default.json` and `syntaxhighlighter/`

### Options Dialog Framework

- `OptionsDlg` — modal dialog that hosts multiple `OptionsPage` tabs
- `OptionsPage` — base class for settings pages; implements `save()` and `restore()`
- Pages are added to the dialog programmatically; existing pages: code editor, commands, log, RSL, style, update
- Settings backend uses `jsonsettings.cpp` (JSON-based `QSettings` replacement)

### SARibbon Integration

- `SARibbon.h` / `SARibbon.cpp` are vendored as single-file headers (see `SARibbon.pri`)
- `ApplicationWidgetBase` (`ribbon/`) is a `SARibbonApplicationWidget` subclass for tabbed application options UI

## Code Conventions

- **PIMPL pattern**: Most classes use `d_ptr` + `Q_DECLARE_PRIVATE(ClassName)`
- **Export macro**: `TOOLSRUNTIME_EXPORT` from `ToolsRuntime_global.h` — uses `Q_DECL_EXPORT` when `TOOLSRUNTIME_LIBRARY` is defined, else `Q_DECL_IMPORT`
- **Logging**: Uses Qt logging categories declared in `toolsruntime.h` (`logCommon`, `logRsl`, `logSql`, etc.)
- **Debug builds**: `API_LOCALHOST` is defined in debug, which may affect API URL behavior

## External Dependencies

- **Qt 5.15.2** modules: Core, Widgets, Gui, WebEngine, WebEngineWidgets, Xml, UiTools, Sql, TestLib, XmlPatterns, Svg
- **RSScript / rsrtlwm / rsldlmms** — closed-source RSL runtime libraries (DLLs in `RslTest/`)
- **rstools** — headers in `ToolsRuntime/rstools/include/`, libs in `ToolsRuntime/rstools/lib/` (debug/release)
- **libhunspell** — headers in `ToolsRuntime/hunspell/include/`, libs in `ToolsRuntime/hunspell/lib/`
- **Windows SDK libs**: Version, Gdi32, User32, Advapi32, Ole32, uxtheme

## Important Notes

- The project is **Windows-only** (`Q_WS_WIN` is defined globally)
- `WindowsModernStyle` requires **Qt private headers** (`Qt::CorePrivate`, `Qt::WidgetsPrivate`, `Qt::GuiPrivate`)
- The CMake build is missing `CircularMenuTest`; if adding new source files, update both `.pro` and `CMakeLists.txt`
- `configtools.pri` is referenced in `.pro` files but lives outside this repo (`../../configtools.pri`)
