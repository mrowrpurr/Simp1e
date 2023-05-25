function prototype(target_name)
    target(target_name)
        add_files(target_name .. ".cpp")
        set_kind("binary")
        add_qt()
        add_string_formatting()
        add_logging_support_for_binaries()
        add_includedirs("include")

    test_target("[test] " .. target_name)
        add_includedirs("include")
        add_includedirs(".")
end

includes("**/xmake.lua")
