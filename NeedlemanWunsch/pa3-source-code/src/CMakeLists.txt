# If you create new headers/cpp files, add them to these list!
set(HEADER_FILES
	SrcMain.h
	FastaFile.h
	DNATranslate.h
	NeedlemanWunsch.h
)

set(SOURCE_FILES
	SrcMain.cpp
	FastaFile.cpp
	DNATranslate.cpp
	NeedlemanWunsch.cpp
)

# Don't change this
add_library(src ${SOURCE_FILES} ${HEADER_FILES})
