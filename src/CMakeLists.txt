# Copyright 2018-2020 Camilo Higuita <milo.h@aol.com>
# Copyright 2018-2020 Nitrux Latinoamericana S.C.
#
# SPDX-License-Identifier: GPL-3.0-or-later

qtquick_compiler_add_resources(hjyRec qml.qrc)

if(ANDROID)
    add_library(index SHARED
        main.cpp
        index.cpp
        controllers/filepreviewer.cpp
        controllers/compressedfile.cpp
        models/left_menu/leftmenudata.cpp
        qml.qrc
    )
else()
    add_executable(index
        main.cpp
        index.cpp
        controllers/filepreviewer.cpp
        controllers/compressedfile.cpp
        models/left_menu/leftmenudata.cpp
        models/ProcessModel.cpp
        ${hjyRec}
    )
endif()

if(EMBEDDED_TERMINAL)
    target_compile_definitions(index PUBLIC EMBEDDED_TERMINAL)
endif()

if (ANDROID)
    target_link_libraries(index Qt5::AndroidExtras Qt5::QuickControls2)
    kde_source_files_enable_exceptions(index index.cpp)
elseif(UNIX)
    target_link_libraries(index KF5::ConfigCore KF5::KIOCore KF5::KIOFileWidgets KF5::KIONTLM KF5::KIOWidgets)
endif()

target_compile_definitions(index
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)


find_package(KF5DBusAddons REQUIRED)
target_link_libraries(index MauiKit Qt5::Sql Qt5::Quick Qt5::Qml Qt5::Svg KF5::Archive KF5::CoreAddons KF5::I18n KF5::DBusAddons jappcoreqt)

#TODO: port to ecm_install_icons()
if(UNIX AND NOT APPLE AND NOT ANDROID)
    install(TARGETS index ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
    install(FILES org.kde.index.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})

    install(FILES assets/index_new.svg DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps)
endif()

