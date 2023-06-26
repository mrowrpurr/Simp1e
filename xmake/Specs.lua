function add_requires_for_specs()
    -- Specs.cpp is not yet released as a package, so you have to require it locally
    local specs_path = os.getenv("SPECS_CPP")
    if specs_path then
        includes(path.join(specs_path, "Specs/xmake.lua"))
    else
        print("SPECS_CPP not set")
        return
    end

    add_requires("vcpkg::snowhouse")
end

function add_specs()
    add_packages("vcpkg::snowhouse")
    add_deps("Specs")
end

function test_target(target_name)
    target(target_name)
        add_specs()
        add_qt_libs()
        add_rules("qt.console")
        add_logging_support_for_binaries()
        add_string_formatting()
        set_kind("binary")
        add_files(path.join(os.projectdir(), "Tests", "TestRunner.cpp"))
        add_files(path.join(os.projectdir(), "Tests", "SpecHelperInternal.cpp"))
        add_files("**/*.Spec.cpp")
        add_includedirs(path.join(os.projectdir(), "Tests"))
        after_build(function(target)
            print("Running '" .. target:name() .. "' tests... " .. target:targetfile())
            os.execv(target:targetfile())
        end)
        add_defines("SIMP1E_TESTING")
end

----------------------------
-- TODO : bring back testing
-- add_requires_for_specs()
-- includes("Tests/xmake.lua")
-- target("Specs and Editor")
--     set_kind("phony")
--     add_deps("Simp1e Tests", "Simp1e Editor")
