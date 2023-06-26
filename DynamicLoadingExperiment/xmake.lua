target("ExperimentalLoadingApp")
    set_kind("binary")
    add_files("App.cpp")
    add_logging_support()

target("ExperimentalLoadingLibrary")
    set_kind("shared")
    add_files("Library.cpp")
    add_logging_support()
