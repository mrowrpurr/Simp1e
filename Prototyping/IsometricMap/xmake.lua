target("Prototype - IsometricMap")
    add_files("IsometricMap.cpp")
    set_kind("binary")
    add_qt()
    add_includedirs("include")
    add_string_formatting()
    add_logging_support_for_binaries()