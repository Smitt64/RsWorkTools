TEMPLATE = subdirs

SUBDIRS += \
    CircularMenuTest \
	RslTest \
    RslToolRuntimeStatic \
    ToolsRuntime \
    WindowsModernStyle


RslTest.depends = ToolsRuntime
ToolsRuntime.depends = RslToolRuntimeStatic
