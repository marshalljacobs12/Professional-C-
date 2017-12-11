# If you create new headers/cpp files, add them to these list!
set(HEADER_FILES
   Command.h
   Cursors.h
   EventID.h
   PaintApp.h
   PaintDrawPanel.h
   PaintFrame.h
   PaintModel.h
   Shape.h
   DrawCommand.h
   RectShape.h
   EllipseShape.h
   LineShape.h
   PencilShape.h
   SetPenCommand.h
   SetBrushCommand.h
   DeleteCommand.h
   MoveCommand.h
)

set(SOURCE_FILES
   Command.cpp
   Cursors.cpp
   PaintApp.cpp
   PaintDrawPanel.cpp
   PaintFrame.cpp
   PaintModel.cpp
   Shape.cpp
   DrawCommand.cpp
   RectShape.cpp
   EllipseShape.cpp
   LineShape.cpp
   PencilShape.cpp
   SetPenCommand.cpp
   SetBrushCommand.cpp
   DeleteCommand.cpp
   MoveCommand.cpp
)

# Don't change this
add_library(src ${SOURCE_FILES} ${HEADER_FILES})
