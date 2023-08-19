include Sources/UsrProg/FreeRTOSAssistant/Makefile.mk
include Sources/UsrProg/libSupport/Makefile.mk

### C flags
CFLAGS += -ISources/UsrProg

### CPP flags
CPPFLAGS += -ISources/UsrProg

### Sources
CPP_SOURCES += Sources/UsrProg/app.cpp
CPP_SOURCES += Sources/UsrProg/task_list.cpp
CPP_SOURCES += Sources/UsrProg/task_system.cpp
CPP_SOURCES += Sources/UsrProg/task_console.cpp
CPP_SOURCES += Sources/UsrProg/task_devManager.cpp

CPP_SOURCES += Sources/UsrProg/CsHandle.cpp
