TEMPLATE = subdirs

SUBDIRS += \
    CodeEditorStatic \
    RslTest \
    RslToolRuntimeStatic \
    ToolsRuntime \
    WindowsModernStyle \
    loki \
    rsldbg \
    rsldbgclnt

rsldbg.depends = CodeEditorStatic loki
RslTest.depends = CodeEditorStatic
RslTest.depends = ToolsRuntime
ToolsRuntime.depends = CodeEditorStatic RslToolRuntimeStatic
