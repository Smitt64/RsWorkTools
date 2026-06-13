TEMPLATE = subdirs

SUBDIRS += \
    CircularMenuTest \
    RibbonMacroTest \
	RslTest \
    RslToolRuntimeStatic \
    ToolsRuntime \
    WindowsModernStyle


RibbonMacroTest.depends = ToolsRuntime
RslTest.depends = ToolsRuntime
ToolsRuntime.depends = RslToolRuntimeStatic
