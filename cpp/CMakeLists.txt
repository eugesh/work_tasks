#include_directories(
#  ${CMAKE_CURRENT_SOURCE_DIR}
#  ${CMAKE_CURRENT_BINARY_DIR}
#)

include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/cpp")

#set (HEADERS
#	 main_window.h)

#QT4_WRAP_UI(UISrcs ${CMAKE_CURRENT_SOURCE_DIR}/../form_ui/mainwindow.ui)

# QT4_WRAP_CPP(MOCSrcs HEADERS)
# QT4_WRAP_CPP(MOCSrcs main_window.h)


add_executable (plate_extractor
	            main.cpp)
#	            ${MOCSrcs} ${UISrcs})

target_link_libraries (plate_extractor ${ADDON_LIBRARIES} ${OpenCV_LIBS})

