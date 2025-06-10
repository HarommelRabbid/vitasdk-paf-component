cmake_minimum_required(VERSION 3.0)

project(libpaf)

set(LIB "ScePaf_stub_weak")
set(LIB_FILE "lib${LIB}.a")
set(LIB_YAML "${CMAKE_CURRENT_SOURCE_DIR}/ScePaf.yml")
set(LIB_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/stubs")

set(REPO_URL "https://github.com/GrapheneCt/ScePaf-RE")
# set(REPO_URL "https://github.com/Princess-of-Sleeping/ScePaf-RE")

add_custom_command(OUTPUT paf_repo
  COMMAND if [ ! -d "${CMAKE_CURRENT_BINARY_DIR}/ScePaf-RE" ] \; then git clone ${REPO_URL} \; find ${CMAKE_CURRENT_BINARY_DIR}/ScePaf-RE -type f | xargs dos2unix \; fi \;
  DEPENDS
)

add_custom_command(OUTPUT paf_yml_build
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/build_emd2yml/emd2yml > ScePaf.yml
  DEPENDS paf_repo
)

add_custom_target("${LIB}.target"
  DEPENDS "${LIB_FILE}"
)

add_custom_command(
  OUTPUT "${LIB_FILE}"
  DEPENDS paf_yml_build
  # COMMAND vita-libs-gen "${LIB_YAML}" "${LIB_BUILD_DIR}"
  # COMMAND vita-libs-gen-2 -yml=${LIB_YAML} -output=${LIB_BUILD_DIR}
  # COMMAND cd stubs && make
  # COMMAND make -C "${LIB_BUILD_DIR}"
  COMMAND vita-libs-gen-2 -yml=./ScePaf.yml -output=${LIB_BUILD_DIR} -cmake=true
  COMMAND cd stubs && cmake . && make -j$(nproc)
  COMMAND cp ${LIB_BUILD_DIR}/libScePaf_stub.a ./libScePaf_stub.a
  COMMAND cp ${LIB_BUILD_DIR}/libScePaf_stub_weak.a ./libScePaf_stub_weak.a
  COMMAND cp ${LIB_BUILD_DIR}/libSceAppSettings_stub.a ./libSceAppSettings_stub.a
  COMMAND cp ${LIB_BUILD_DIR}/libSceAppSettings_stub_weak.a ./libSceAppSettings_stub_weak.a
  COMMAND cp ${LIB_BUILD_DIR}/libSceCommonGuiDialog_stub.a ./libSceCommonGuiDialog_stub.a
  COMMAND cp ${LIB_BUILD_DIR}/libSceCommonGuiDialog_stub_weak.a ./libSceCommonGuiDialog_stub_weak.a
  COMMAND cp ${LIB_BUILD_DIR}/libScePafWebMapView_stub.a ./libScePafWebMapView_stub.a
  COMMAND cp ${LIB_BUILD_DIR}/libScePafWebMapView_stub_weak.a ./libScePafWebMapView_stub_weak.a
  COMMAND cp ${LIB_BUILD_DIR}/libSceWebUIPlugin_stub.a ./libSceWebUIPlugin_stub.a
  COMMAND cp ${LIB_BUILD_DIR}/libSceWebUIPlugin_stub_weak.a ./libSceWebUIPlugin_stub_weak.a
  COMMENT "Building stubs"
)

add_library("${LIB}" STATIC IMPORTED GLOBAL)

add_dependencies("${LIB}" "${LIB}.target")

set_target_properties("${LIB}" PROPERTIES
  IMPORTED_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/${LIB_FILE}"
)

add_custom_target(development
  DEPENDS ${LIB_FILE}
)
