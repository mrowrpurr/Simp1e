target("Game - Side Scroller")
    add_files("Game - Side Scroller.cpp")
    set_kind("binary")
    add_qt()
    add_includedirs(".", "include")
    add_string_formatting()
    add_logging_support_for_binaries()
    add_packages("nlohmann_json")
    add_deps("Simp1e.UI", "Simp1e.Qt.v1", "Simp1e.Maps", "Simp1e.UI.Maps", "Simp1e.Qt.Maps", "Animations")
    add_defines("_CRT_SECURE_NO_WARNINGS")