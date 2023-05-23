includes("xmake/*.lua")
add_rules("mode.debug", "mode.release")
set_languages("c++20")
add_requires("nlohmann_json")
add_requires_for_logging()
add_requires_for_string_formatting()
add_requires_for_specs()
includes("Libraries/**/xmake.lua")
includes("Tests/xmake.lua")
