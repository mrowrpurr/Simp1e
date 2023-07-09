target("Asteroids")
    set_kind_qt_console_application()
    add_deps(
        "Simp1e.ECS.Core",
        "Simp1e.ECS.Local",
        "Simp1e.ECS.Components.Core",
         "Simp1e.ECS.Components.Common",
         "Simp1e.ECS.Components.GUI",
         "Simp1e.Qt",
         "Simp1e.Qt.ECS"
    )
    add_logging_runtime()
    add_string_formatting()
    add_files("Asteroids.cpp", "Asteroids.qrc")
    add_includedirs("include")
