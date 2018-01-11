cmake_minimum_required(VERSION 2.6)

set (INIT_VER "0.0.2")
set (INIT_URL "https://github.com/swanson-os/init/archive/v${INIT_VERSION}.tar.gz")
set (INIT_OUT "${CMAKE_CURRENT_BINARY_DIR}/init-${INIT_VERSION}.tar.gz")
file (DOWNLOAD ${INIT_URL} ${INIT_OUT})

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/init-${INIT_VER}"
	COMMAND "${CMAKE_COMMAND}" -E tar xvf "${INIT_OUT}")
