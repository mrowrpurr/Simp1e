target("Simp1e.Qt")
    set_kind("headeronly")
    add_qt()
    add_logging()
    add_packages("function_pointer", {public = true})
    add_includedirs("include", {public = true})
    add_headerfiles("include/(**.h)")
