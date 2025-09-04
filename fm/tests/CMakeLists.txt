set (TESTS align_test compat_test)
set (PROGS format_test self_format_test scale_test)

foreach (TEST ${TESTS} )
    ADD_EXECUTABLE(${TEST} ${TEST}.c test_funcs.c)
    TARGET_LINK_LIBRARIES(${TEST} ffs )
    ADD_TEST(NAME ${TEST} COMMAND ${TEST})
endforeach()
ADD_EXECUTABLE(simple simple.c)
ADD_TEST(NAME simple_test COMMAND simple)

if(WIN32)
    # set (PATH_ADDITION "")
    # if (FFS_USE_ATL)
    #    string (APPEND PATH_ADDITION "D:\\a\\ffs\\ffs\\atl\\install\\lib;D:\\a\\ffs\\ffs\\atl\\install\\bin;") #"$<SHELL_PATH:$<TARGET_FILE_DIR:atl::atl>>$<SEMICOLON>")
    # endif()
    # if (DILL_FOUND)
    #    string (APPEND PATH_ADDITION "D:\\a\\ffs\\ffs\\dill\\install\\lib;D:\\a\\ffs\\ffs\\dill\\install\\bin;") # $<SHELL_PATH:$<TARGET_FILE_DIR:dill::dill>>$<SEMICOLON>")
    # endif()
    # # Ensure the required DLLs can be found at runtime
    # message(STATUS "PATH ADDITION is ${PATH_ADDITION}")
    # message(STATUS "TESTS is ${TESTS}")
    # ADD_TEST(NAME WIP COMMAND "C:/Program Files/Microsoft Visual Studio/2022/Enterprise/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/dumpbin.exe" /dependents "D:/a/ffs/ffs/build/bin/Release/align_test.exe")
    ADD_TEST(NAME WIP3 COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/ls.bat")
    LIST (APPEND TESTS WIP WIP3)
    # set_tests_properties(
    #         ${TESTS}
    #         PROPERTIES ENVIRONMENT "PATH=${PATH_ADDITION}$ENV{PATH}"
    # )
endif()

foreach (PROG ${PROGS} )
    ADD_EXECUTABLE(${PROG} ${PROG}.c test_funcs.c)
    TARGET_LINK_LIBRARIES(${PROG} ffs )
endforeach()

install(TARGETS ${PROGS}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT ${FFS_RUNTIME_COMPONENT}
)
