option (BUILD_LCM "Build and install third-party LCM?" ON)
if (BUILD_LCM)
  # EXTERNAL DEPS
  libhandler_glib ()
  libhandler_java ()
  libhandler_python ()

  # TARGETS
  #set (LCM_SRC "${THIRD_PARTY_DIR}/lcm")
  #set (LCM_DIR "lcm")
  set (LCM_SRC "${THIRD_PARTY_DIR}/lcm-1.3.1.zip")
  set (LCM_DIR "lcm-1.3.1")

  add_custom_target (lcm
    COMMAND rm -f ${LCM_DIR}/BUILT_FLAG
    COMMAND cmake ..
    COMMAND make lcm-target
    )

  if (NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/${LCM_DIR}/BUILT_FLAG)
    add_custom_target (lcm-target
      # COMMAND svn export --force ${LCM_SRC} ${LCM_DIR}
      # COMMAND cd ${LCM_DIR} && ./bootstrap.sh && ./configure
      # COMMAND make -C ${LCM_DIR} -j
      # COMMAND sudo make -C ${LCM_DIR} install
      # COMMAND sudo ldconfig
      # COMMAND touch ${LCM_DIR}/BUILT_FLAG
      # COMMAND cmake ..

      COMMAND mkdir -p ${LCM_DIR}
      COMMAND unzip ${LCM_SRC}
      #COMMAND tar zxvfp  ${LCM_SRC} -C ${LCM_DIR} --strip 1
      COMMAND cd ${LCM_DIR} && pwd && ./configure
      COMMAND make -C ${LCM_DIR} -j
      COMMAND sudo make -C ${LCM_DIR} install
      COMMAND sudo ldconfig
      COMMAND touch ${LCM_DIR}/BUILT_FLAG
      COMMAND cmake ..
      )
    add_dependencies (third-party lcm-target)
  else ()
    message (STATUS "Skipping built target lcm")
  endif ()

  add_custom_target (uninstall-lcm
    COMMAND cd ${LCM_DIR} && sudo make uninstall
    COMMAND sudo ldconfig
    COMMAND rm -f ${LCM_DIR}/BUILT_FLAG
    COMMAND cmake ..
    )
  add_dependencies (uninstall uninstall-lcm)

  add_custom_target (clean-lcm
    COMMAND cd ${LCM_DIR} && make clean
    COMMAND rm -f ${LCM_DIR}/BUILT_FLAG
    COMMAND cmake ..
    )
  add_dependencies (clean clean-lcm)
endif (BUILD_LCM)
