includes("xmake/*.lua")

add_rules("mode.debug", "mode.release")

set_languages("c++20")

add_requires("nlohmann_json", "clip", "dependency_injection")
add_requires_for_logging()
add_requires_for_string_formatting()

includes("Simp1e*/xmake.lua")

----------------------------
-- TODO : bring back testing
-- add_requires_for_specs()
-- includes("Tests/xmake.lua")
-- target("Specs and Editor")
--     set_kind("phony")
--     add_deps("Simp1e Tests", "Simp1e Editor")
