macro (libhandler_lcm)
  libhandler_find_library (lcm "did you build lcm in third-party?" ${ARGN})
  if (LCM_FOUND)
    set (IRPLIB_LCM ${LCM_LIBRARIES})
  endif ()
endmacro ()
