target("Simp1e.ECS.DLL")
    set_kind("shared")
    set_basename("Simp1e.ECS")
    add_logging_runtime()
    add_string_formatting()
    add_deps("Simp1e.ServiceHost", "Simp1e.ECS.Services")
    add_files("Simp1e.ECS.DLL.cpp")
