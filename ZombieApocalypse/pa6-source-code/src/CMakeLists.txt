# If you create new headers/cpp files, add them to these list!
set(HEADER_FILES
   Exceptions.h
	Machine.h
	Op.h
	Singleton.h
	Traits.h
	ZomApp.h
	ZomDrawPanel.h
	ZomFrame.h
	World.h
	Utils.h
)

set(SOURCE_FILES
   Op.cpp
	ZomApp.cpp
	ZomDrawPanel.cpp
	ZomFrame.cpp
)

# Don't change this
add_library(src ${SOURCE_FILES} ${HEADER_FILES})
