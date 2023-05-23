function lib(library_name)
    target(library_name)
        set_kind("headeronly")
        add_includedirs("include", {public = true})
        add_headerfiles("include/(**.h)")
end
