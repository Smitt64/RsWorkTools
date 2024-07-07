TEMPLATE = subdirs

SUBDIRS += \
	RslTest \
        RslToolRuntimeStatic \
        ToolsRuntime


RslTest.depends = ToolsRuntime
ToolsRuntime.depends = RslToolRuntimeStatic
