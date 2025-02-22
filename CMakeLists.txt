cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(SPIDERMAN C CXX)
include("${VITASDK}/share/vita.cmake" REQUIRED)
set(VITA_APP_NAME "Ultimate Spider-Man: Total Mayhem")
set(VITA_TITLEID  "SPIDERMAN")
set(VITA_VERSION  "01.00")
set(VITA_MKSFOEX_FLAGS "-d ATTRIBUTE2=12")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wl,-q,--wrap,memcpy,--wrap,memmove,--wrap,memset -D_GNU_SOURCE -Wall -O3 -mfloat-abi=softfp")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11 -Wno-write-strings")

add_executable(SPIDERMAN
  loader/main.c
  loader/dialog.c
  loader/so_util.c
  loader/video_player.c
  loader/audio_player.cpp
)

target_link_libraries(SPIDERMAN
  -Wl,--whole-archive pthread -Wl,--no-whole-archive
  soloud_static
  SDL2_mixer_ext
  SDL2
  vorbisfile
  vorbis
  ogg
  mpg123
  opusfile
  opus
  modplug
  mikmod
  FLAC
  stdc++
  openal
  vitaGL
  vitashark
  SceShaccCgExt
  m
  mathneon
  taihen_stub
  kubridge_stub
  SceHid_stub
  SceMotion_stub
  SceIme_stub
  SceAvPlayer_stub
  SceAppMgr_stub
  SceAudio_stub
  SceAudioIn_stub
  SceCtrl_stub
  SceCommonDialog_stub
  SceDisplay_stub
  SceKernelDmacMgr_stub
  SceGxm_stub
  SceShaccCg_stub
  SceSysmodule_stub
  ScePower_stub
  SceTouch_stub
  SceVshBridge_stub
  SceAppUtil_stub
)

vita_create_self(eboot.bin SPIDERMAN UNSAFE)
vita_create_vpk(spiderman.vpk ${VITA_TITLEID} eboot.bin
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
  FILE ${CMAKE_SOURCE_DIR}/livearea/icon0.png sce_sys/icon0.png
       ${CMAKE_SOURCE_DIR}/livearea/bg0.png sce_sys/livearea/contents/bg0.png
       ${CMAKE_SOURCE_DIR}/livearea/startup.png sce_sys/livearea/contents/startup.png
       ${CMAKE_SOURCE_DIR}/livearea/template.xml sce_sys/livearea/contents/template.xml
)

add_custom_target(copy
  COMMAND cp eboot.bin D:/app/${VITA_TITLEID}/eboot.bin
  DEPENDS eboot.bin
)
