function add_requires_for_string_formatting()
    add_requires("string_format")
    -- check if toolchain is msvc

    if not is_host("windows") then
        add_requires("fmt") -- MSVC doesn't need this (supports std::format)
    end
end

function add_string_formatting()
    if not is_host("windows") then
        add_packages("fmt", {public = true}) -- MSVC doesn't need this (supports std::format)
        add_defines("STRING_FORMAT_USE_FMT") -- Mac Clang needs this (at the moment)
    end
end
