target("Simp1e.Qt.v1")
    set_kind("headeronly")
    add_includedirs("include", {public = true})
    add_headerfiles("include/(**.h)")
    add_packages("clip", {public = true}) -- make this optional! Windows only
