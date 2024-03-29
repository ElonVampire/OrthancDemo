macro(ADD_VISUAL_STUDIO_PRECOMPILED_HEADERS PrecompiledHeaders PrecompiledSource Sources Target)
  get_filename_component(PrecompiledBasename ${PrecompiledHeaders} NAME_WE)
  message("PrecompiledBasename: ${PrecompiledBasename}")
  set(PrecompiledBinary "${PrecompiledBasename}_${CMAKE_BUILD_TYPE}_${CMAKE_GENERATOR_PLATFORM}.pch")

  set_source_files_properties(${PrecompiledSource}
    PROPERTIES COMPILE_FLAGS "/Yc\"${PrecompiledHeaders}\" /Fp\"${PrecompiledBinary}\""
    OBJECT_OUTPUTS "${PrecompiledBinary}")
  message("PrecompiledSource: ${PrecompiledSource}")

  set_source_files_properties(${${Sources}}
    PROPERTIES COMPILE_FLAGS "/Yu\"${PrecompiledHeaders}\" /FI\"${PrecompiledHeaders}\" /Fp\"${PrecompiledBinary}\""
    OBJECT_DEPENDS "${PrecompiledBinary}")
  message("Sources: ${Sources}")    
  
  set(${Target} ${PrecompiledSource})
endmacro()