TEMPLATE = subdirs

SUBDIRS += \
	RslTest \
	ToolsRuntime

RslTest.depends = ToolsRuntime
