add_rules("mode.debug", "mode.release")

set_languages("cxx20")
add_toolchains("msvc")

local gooey_path = os.getenv("GOOEY_PATH")
if gooey_path then
    includes(path.join(gooey_path, "Gooey"))
    includes(path.join(gooey_path, "Gooey.wxWidgets"))
else
    print("GOOEY_PATH not set")
    return
end

includes("**/xmake.lua")
