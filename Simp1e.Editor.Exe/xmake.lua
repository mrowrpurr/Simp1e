target("Simp1e.Editor.Exe")
    set_kind("binary")
    set_basename("Simp1e")
    add_logging_runtime()
    add_string_formatting()
    add_deps("Simp1e.LibraryLoader")
    add_files("Simp1e.Editor.Exe.cpp")
