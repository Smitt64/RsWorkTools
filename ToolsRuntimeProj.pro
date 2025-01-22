TEMPLATE = subdirs

SUBDIRS += \
    CodeEditorStatic \
    FindDialog \
    RslTest \
    RslToolRuntimeStatic \
    ToolsRuntime \
    WindowsModernStyle \
    loki \
    rsldbg \
    rsldbgclnt

rsldbg.depends = CodeEditorStatic loki
rsldbgclnt.depends = CodeEditorStatic FindDialog
RslTest.depends = CodeEditorStatic
RslTest.depends = ToolsRuntime
ToolsRuntime.depends = CodeEditorStatic RslToolRuntimeStatic
