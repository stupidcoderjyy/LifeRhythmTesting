cmake_minimum_required(VERSION 3.27)
project(LifeRhythmTesting)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "C:/Program Files/Qt/5.15.2/mingw81_64/lib/cmake")
set(LR_PROJECT_PATH "C:/Users/JYY/CLionProjects/LifeRhythm")
set(LR_PROJECT_INCLUDES "${LR_PROJECT_PATH}/include")
set(LR_CORE_DLL_DEBUG "${LR_PROJECT_PATH}/cmake-build-makedll-debug")
set(LR_CORE_DLL_RELEASE "${LR_PROJECT_PATH}/cmake-build-makedll-release")

set(TEST_ENTRYPOINT widgets/testCalendar.cpp)

set(INCLUDES
        widgets/button
        widgets/arrowButton
        widgets/calendar
        widgets/dropDown
        widgets/focusContainer
#        src/input_text_label
#        src/jbTextButton
        widgets/label
        widgets/listWidget
        widgets/loadingIcon
        widgets/menu
        widgets/miniCalendar
        widgets/slotsPainter
        widgets/slotsWidget
        widgets/tiledListWidget
#        src/xyplot

        util/async
        util/models
)

set(SRC_FILES
        widgets/arrowButton/ArrowButton.cpp
        widgets/arrowButton/ArrowButton.h
        widgets/button/Button.cpp
        widgets/button/Button.h

        widgets/calendar/Calendar.cpp
        widgets/calendar/Calendar.h
        widgets/calendar/Common.h
        widgets/calendar/DropDownRange.cpp
        widgets/calendar/DropDownRange.h
        widgets/calendar/MiniCalendarDropDown.cpp
        widgets/calendar/MiniCalendarDropDown.h
        widgets/calendar/ButtonSwitchView.cpp
        widgets/calendar/ButtonSwitchView.h
        widgets/calendar/SlotsWidgetCalendar.cpp
        widgets/calendar/SlotsWidgetCalendar.h
        widgets/calendar/SlotsPainterWeekdays.cpp
        widgets/calendar/SlotsPainterWeekdays.h

        widgets/dropDown/DropDown.h
        widgets/dropDown/DropDown.cpp
        widgets/focusContainer/FocusContainer.h
        widgets/focusContainer/FocusContainer.cpp
#        src/input_text_label/InputTextLabel.cpp
#        src/input_text_label/InputTextLabel.h
#        src/jbTextButton/JbTextButton.cpp
#        src/jbTextButton/JbTextButton.h
        widgets/label/Label.h
        widgets/label/Label.cpp
        widgets/listWidget/ListWidget.h
        widgets/listWidget/ListWidget.cpp
        widgets/loadingIcon/LoadingIcon.h
        widgets/loadingIcon/LoadingIcon.cpp
        widgets/menu/Menu.h
        widgets/menu/Menu.cpp
        widgets/miniCalendar/MiniCalendar.cpp
        widgets/miniCalendar/MiniCalendar.h
        widgets/slotsPainter/SlotsPainter.cpp
        widgets/slotsPainter/SlotsPainter.h
        widgets/tiledListWidget/TiledListWidget.h
        widgets/tiledListWidget/TiledListWidget.cpp
        widgets/slotsWidget/SlotsWidget.cpp
        widgets/slotsWidget/SlotsWidget.h

        util/async/AsyncTask.h
        util/async/AsyncTask.cpp
        util/models/ListData.h
        util/models/ListData.cpp
)

#把LR自带的资源
file(COPY "${LR_PROJECT_PATH}/assets/lr" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/assets")
#本模块资源
file(COPY "assets" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "assets" DESTINATION "${LR_PROJECT_PATH}")


find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    set(CUR_CORE_DLL_PATH ${LR_CORE_DLL_DEBUG})
elseif(CMAKE_BUILD_TYPE MATCHES "Release")
    set(CUR_CORE_DLL_PATH ${LR_CORE_DLL_RELEASE})
endif ()

if (NOT EXISTS "${CUR_CORE_DLL_PATH}/LifeRhythm.dll")
    message(FATAL_ERROR "缺少DLL文件")
endif ()

#复制LR Core DLL
file(COPY "${CUR_CORE_DLL_PATH}/LifeRhythm.dll" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY "${CUR_CORE_DLL_PATH}/LifeRhythm.dll" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/lib)

link_directories(${CMAKE_CURRENT_BINARY_DIR}/lib)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    add_executable(${PROJECT_NAME} test/${TEST_ENTRYPOINT} ${SRC_FILES} ${INCLUDE_FILES})
endif ()

target_link_libraries(${PROJECT_NAME}
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        LifeRhythm
)

file(GLOB_RECURSE FILES LIST_DIRECTORIES true "${LR_PROJECT_INCLUDES}/*")

set(CORE_INCLUDES "")
foreach (file ${FILES})
    if (IS_DIRECTORY ${file})
        list(APPEND CORE_INCLUDES ${file})
    endif ()
endforeach ()

list(APPEND CORE_INCLUDES ${LR_PROJECT_INCLUDES})

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDES} ${CORE_INCLUDES})

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt5${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
