target("Animations")
    set_kind("headeronly")
    add_includedirs("include", {public = true})
    add_headerfiles("include/(**.h)")
    add_defines("_CRT_SECURE_NO_WARNINGS")