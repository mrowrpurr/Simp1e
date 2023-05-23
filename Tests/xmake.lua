target("Simp1e Tests")
    add_specs()
    add_logging_support_for_binaries()
    add_string_formatting()
    set_kind("binary")
    add_deps("Simp1e.DI")
    add_files("TestRunner.cpp", "SpecHelperInternal.cpp", "TestFiles/**/*.cpp")
    add_includedirs(".")
    before_build(function(target)
        if os.isdir("Tests/TestFiles") then 
            os.rmdir("Tests/TestFiles")
        end
        for _, filepath in ipairs(os.files(path.join("Libraries", "**.Spec.h"))) do
            local after_include = string.gsub(filepath, ".*/include/", "")
            local only_alphachars = string.gsub(after_include, "[^%w]", "_")
            local unique_macro_variable = "__SpecsHack_TestFile_" .. only_alphachars
            local file_path = "Tests/TestFiles/" .. after_include .. ".cpp"
            local file_contents = "#define __SpecsHack_UniqueCppFileValue " .. unique_macro_variable .. "\n#include \"" .. after_include .. "\""
            io.writefile(file_path, file_contents)
        end
    end)
