target("DLL-LoadMe-SystemAndComponent")
    set_kind("shared")
    add_files("LoadMeSystemAndComponent.cpp")
    add_logging_runtime()
    add_string_formatting()
