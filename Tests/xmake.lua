target("Generate Tests")
    set_kind("phony")
    before_build(function(target)
        if os.isdir("Tests/TestFiles") then 
            os.rmdir("Tests/TestFiles")
        end
        for _, filepath in ipairs(os.files(path.join("Simp1e*", "**.Spec.h"))) do
            local after_include = string.gsub(filepath, ".*/include/", "")
            local only_alphachars = string.gsub(after_include, "[^%w]", "_")
            local unique_macro_variable = "__SpecsHack_TestFile_" .. only_alphachars
            local unique_macro_variable2 = "__SpecsHack_TestFile2_" .. only_alphachars
            local file_path = "Tests/TestFiles/" .. after_include .. ".cpp"
            local file_contents = "#define __SpecsHack_UniqueCppFileValue " .. unique_macro_variable .. "\n#define __SpecsHack_UniqueCppFileValue2 " .. unique_macro_variable2 .. "\n" .. "\n#include \"" .. after_include .. "\""
            io.writefile(file_path, file_contents)
        end
    end)

target("[tests] Simp1e Tests")
    set_filename("Simp1e Tests")
    add_specs()
    add_qt_libs()
    add_rules("qt.console")
    add_logging_support_for_binaries()
    add_string_formatting()
    set_kind("binary")
    -- ADD ALL THE THINGS!
    add_deps("Generate Tests", "Simp1e.Editor [includes]")
    add_files("TestRunner.cpp", "SpecHelperInternal.cpp", "TestFiles/**/*.cpp")
    add_includedirs(".")
    after_build(function(target)
        os.execv(target:targetfile())
    end)
    add_defines("SIMP1E_TESTING")