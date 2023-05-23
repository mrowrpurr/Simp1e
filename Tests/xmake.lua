
target("Simp1e Tests")
    add_specs()
    add_logging_support_for_binaries()
    add_string_formatting()
    set_kind("binary")
    add_deps("Simp1e.DI")
    add_files("*.cpp")
    add_includedirs(".")
    before_build(function(target)
        local lines = {}
        for _, filepath in ipairs(os.files(path.join("Libraries", "**.Spec.h"))) do
            table.insert(lines, "#include \"" .. string.gsub(filepath, ".*/include/", "") .. "\"")
        end
        io.writefile("Tests/__SpecHeaderIncludes.h", table.concat(lines, "\n"))
    end)
