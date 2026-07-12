TEMPLATE = subdirs

SUBDIRS += \
    CircularMenuTest \
    MarkdownViewer \
    RibbonMacroTest \
	RslTest \
    RslToolRuntimeStatic \
    ToolsRuntime \
    WindowsModernStyle


MarkdownViewer.depends = ToolsRuntime
RibbonMacroTest.depends = ToolsRuntime
RslTest.depends = ToolsRuntime
ToolsRuntime.depends = RslToolRuntimeStatic
