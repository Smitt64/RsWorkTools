TEMPLATE = subdirs

SUBDIRS += \
	RslTest \
    RslToolRuntimeStatic \
    ToolsRuntime \
    WindowsModernStyle


RslTest.depends = ToolsRuntime
ToolsRuntime.depends = RslToolRuntimeStatic
