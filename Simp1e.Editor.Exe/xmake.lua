target("Simp1e.Editor.Exe")
    set_kind_qt_console_application()
    set_basename("Simp1e")
    add_includedirs("include")
    add_logging_runtime()
    add_string_formatting()
    add_deps("Simp1e.LibraryLoader", "Simp1e.ECS.NativeAPI.Host")
    add_files("Simp1e.Editor.Exe.cpp", "../Resources/Editor.qrc")
    add_defines("_CRT_SECURE_NO_WARNINGS")
    if is_host("windows") then
        add_files("Simp1e.Editor.Exe.rc")
    end
