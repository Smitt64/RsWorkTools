TEMPLATE = subdirs

SUBDIRS += \
	RslTest \
    RslToolRuntimeStatic \
    ToolsRuntime \
    WindowsModernStyle \
    loki \
    rsldbg


RslTest.depends = ToolsRuntime
ToolsRuntime.depends = RslToolRuntimeStatic
