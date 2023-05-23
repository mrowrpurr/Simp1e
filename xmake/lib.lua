function lib(library_name)
    local all_headers = os.files("include/**/*.h")
    local non_spec_headers = {}
    for _, header in ipairs(all_headers) do
        if not string.match(header, "%.Spec.h$") then
            table.insert(non_spec_headers, header)
        end
    end

    target(library_name)
        set_kind("headeronly")
        add_includedirs("include", {public = true})
        for _, header in ipairs(non_spec_headers) do
            add_headerfiles(header)
        end
end
