-- requires add_qt_libs
function add_3d_qt_libs()
    local qt_root = os.getenv("QTROOT")

    if is_host("windows") then
        local qt_platform_folder = path.join(qt_root, "msvc2019_64")
        local qt_lib = path.join(qt_platform_folder, "lib")
        local qt_include = path.join(qt_platform_folder, "include")

        add_includedirs(path.join(qt_include, "Qt3DCore"))
        add_includedirs(path.join(qt_include, "Qt3DRender"))
        add_includedirs(path.join(qt_include, "Qt3DExtras"))

        add_links("Qt63DCore", "Qt63DRender", "Qt63DExtras")
    end
end

function add_qt()
    -- Path to your Qt installation folder, including the version
    -- e.g. C:\Qt\5.12.0\msvc2017_64 or /usr/local/Qt-5.12.0
    local qt_root = os.getenv("QTROOT")

    if is_host("windows") then

        local qt_platform_folder = path.join(qt_root, "msvc2019_64")
        local qt_lib = path.join(qt_platform_folder, "lib")
        local qt_include = path.join(qt_platform_folder, "include")

        -- include
        add_includedirs(path.join(qt_include, "QtCore"))
        add_includedirs(path.join(qt_include, "QtGui"))
        add_includedirs(path.join(qt_include, "QtWidgets"))
        add_includedirs(path.join(qt_include, "QtSvgWidgets"))
        add_includedirs(path.join(qt_include, "QtSensors"))

        -- link
        add_linkdirs(qt_lib)
        add_links("Qt6Core", "Qt6Gui", "Qt6Widgets", "Qt6Svg", "Qt6SvgWidgets", "Qt6Sensors") -- , "Qt63DCore", "Qt63DRender", "Qt63DExtras")

        -- compiler flag
        add_cxflags("/Zc:__cplusplus")

    elseif is_host("macosx") then

        local qt_platform_folder = path.join(qt_root, "macos")
        local qt_lib = path.join(qt_platform_folder, "lib")
        local qt_include = path.join(qt_platform_folder, "include")

        -- include
        add_includedirs(qt_include)
        add_includedirs(path.join(qt_lib, "QtCore.framework", "Headers"))
        add_includedirs(path.join(qt_lib, "QtGui.framework", "Headers"))
        add_includedirs(path.join(qt_lib, "QtWidgets.framework", "Headers"))

        -- link
        add_linkdirs(path.join(qt_lib, "QtCore.framework"))
        add_linkdirs(path.join(qt_lib, "QtGui.framework"))
        add_linkdirs(path.join(qt_lib, "QtWidgets.framework"))

        -- frameworks
        add_ldflags("-F" .. qt_lib)
        add_frameworks("QtCore", "QtGui", "QtWidgets")

    elseif is_host("linux") then

        local qt_platform_folder = path.join(qt_root, "gcc_64")
        local qt_lib = path.join(qt_platform_folder, "lib")
        local qt_include = path.join(qt_platform_folder, "include")

        -- include
        add_includedirs(path.join(qt_include, "QtCore"))
        add_includedirs(path.join(qt_include, "QtGui"))
        add_includedirs(path.join(qt_include, "QtWidgets"))

        -- link
        add_linkdirs(qt_lib)
        add_links("Qt6Core", "Qt6Gui", "Qt6Widgets")

    else
        print("Unsupported host platform")
    end
end

-- Call this in an existing target to configure it as a Qt application.
function set_kind_qt_console_application()
    set_kind("binary")
    add_rules("qt.console")
    add_rules("qt.qrc")
    add_rules("qt.moc")
    add_qt()
end

function set_kind_qt_application()
    set_kind("binary")
    add_rules("qt.application")
    add_rules("qt.qrc")
    add_rules("qt.moc")
    add_qt()
end

function set_kind_qt_shared_library()
    set_kind("shared")
    add_rules("qt.shared")
    add_rules("qt.qrc")
    add_rules("qt.moc")
    add_qt()
end