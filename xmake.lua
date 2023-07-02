includes("xmake/*.lua")

add_rules("mode.debug", "mode.release")

set_languages("c++20")

add_requires("nlohmann_json", "dependency_injection", "void_pointer", "function_pointer")
add_requires_for_logging()
add_requires_for_string_formatting()

includes("Simp1e*/xmake.lua")
includes("Experimentation/xmake.lua")
