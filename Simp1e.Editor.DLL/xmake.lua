target("Simp1e.Editor.DLL")
    set_kind("shared")
    set_basename("Simp1e.Editor")
    add_logging_runtime()
    add_string_formatting()
    add_deps("Simp1e.ServiceHost", "Simp1e.ECS.Services")
    add_files("Simp1e.Editor.DLL.cpp")
