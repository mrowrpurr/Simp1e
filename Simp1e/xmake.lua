target("Simp1e")
    set_kind("headeronly")
    add_includedirs("include", {public = true})
    add_headerfiles("include/(**.h)")