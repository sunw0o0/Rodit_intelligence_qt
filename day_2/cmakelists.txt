cmake_minimum_required(VERSION 3.19)
project(miracle_project LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(miracle_project
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    bottlephysics.h
    bottlephysics.cpp
    pagemodeselect.h
    pagemodeselect.cpp
    pagegame.h
    pagegame.cpp
    pageaigame.h
    pageaigame.cpp
)

target_link_libraries(miracle_project
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS miracle_project
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET miracle_project
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
