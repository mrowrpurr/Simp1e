target("Simp1e.Editor")
    add_qt()
    set_kind("binary")
    add_files("Editor.cpp", "../Resources/Editor.qrc", "include/**/*_Q.h")
    add_includedirs("include")
    add_headerfiles("include/(**).h")
    add_deps("Simp1e.Data", "Simp1e.Data.Types", "Simp1e.Editor.Types")
    add_string_formatting()
    add_logging_support_for_binaries()
    if is_plat("windows") then
        add_files("Editor.rc")
    end

target("Simp1e.Editor [includes]")
    set_kind("headeronly")
    add_qt_libs()
    add_includedirs("include", {public = true})
    add_string_formatting()
    add_logging()
    add_deps("Simp1e.Data", "Simp1e.Data.Types", "Simp1e.Editor.Types")

-- mac_icon_for_target("SimP1e.Editor", "Resources/Logo/iconset.icns")
