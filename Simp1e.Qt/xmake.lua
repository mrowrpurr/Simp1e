target("Simp1e.QT")
    set_kind("headeronly")
    add_includedirs("include", {public = true})
    add_headerfiles("include/(**.h)")
