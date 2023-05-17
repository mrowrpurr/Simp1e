add_rules("mode.debug", "mode.release")

set_languages("cxx20")
add_toolchains("msvc")

add_repositories("MrowrLib https://github.com/MrowrLib/Packages.git")
add_requires("string_format", "_Log_", "spdlog")

-- -- Include the Gooey library for UI
-- local gooey_path = os.getenv("GOOEY_PATH")
-- if gooey_path then
--     includes(path.join(gooey_path, "Gooey"))
--     includes(path.join(gooey_path, "Gooey.FLTK"))
-- else
--     print("GOOEY_PATH not set")
--     return
-- end

includes("**/xmake.lua")
