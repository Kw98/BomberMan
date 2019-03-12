set(GLOOP_SRCS
	GLoop.cpp
)

set(GLOOP_HEADER
	GLoop.hpp
)

add_library(libGLoop
	${GLOOP_SRCS}
	${GLOOP_HEADER}
)

target_include_directories(libGLoop PUBLIC tools)
target_include_directories(libGLoop PUBLIC ../GEcm)
