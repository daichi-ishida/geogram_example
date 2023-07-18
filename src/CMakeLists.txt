#::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
# ADD ALL PROJECTS ::::::::::::::::::::::::::::::::::::::::::::::::::::::
#::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
function(ADD_PROJECT SUBDIR)
    message(STATUS "Target: ${SUBDIR}")
    add_subdirectory(${SUBDIR})
endfunction(ADD_PROJECT)

file(GLOB SUBDIR_LIST RELATIVE ${CMAKE_CURRENT_LIST_DIR} "*")
foreach(SUBDIR ${SUBDIR_LIST})
  if (IS_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/${SUBDIR}")
    ADD_PROJECT(${SUBDIR})
  endif()
endforeach()

#::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
# DEPENDENCIES ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
#::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
# add_dependencies(A B)

#::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
# POST PROCESS ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
#::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

# add_custom_command(TARGET <PROJECT> POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_OBJECTS:${A}> $<TARGET_FILE_DIR:PROJECT> COMMAND_EXPAND_LISTS)