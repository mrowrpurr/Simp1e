target("Simp1e.ServiceHost")
    set_kind("headeronly")
    add_logging()
    add_includedirs("include", {public = true})
    add_headerfiles("include/(**.h)")
