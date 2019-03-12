if(UNIX)
	find_package(X11)
	find_path(IRRLICHT_INCLUDE_DIR irrlicht.h HINTS /usr/include /usr/include/irrlicht)
	file(GLOB_RECURSE IRRLICHT_LIBRARY_DIR /usr/libIrrlicht.so*)
	if (NOT IRRLICHT_LIBRARY_DIR OR NOT IRRLICHT_INCLUDE_DIR)
		message("Cannot find Irrlicht")
		include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)
		ExternalProject_add( Irrlicht
			DOWNLOAD_COMMAND sudo dnf install irrlicht && sudo dnf install irrlicht-devel
 			CONFIGURE_COMMAND ""
      		BUILD_COMMAND ""
      		INSTALL_COMMAND ""
			)
		set(IRRLICHT_INCLUDE_DIR /usr/include/irrlicht)
		file(GLOB_RECURSE IRRLICHT_LIBRARY_DIR /usr/libIrrlicht.so*)
	endif()
endif()

if(WIN32) #MVSC
	message("getIrrlicht WIN32")
  	set(IRRLICHT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/irrlicht-1.8.4/include)
  	set(IRRLICHT_LIBRARY_DIR "C:\\Program Files\\Irrlicht.lib")
endif()