target("Simp1e.ECS.Local")
    set_kind("headeronly")
    add_logging()
    add_packages("void_pointer", {public = true})
    add_deps("Simp1e.ECS.Core")
    add_includedirs("include", {public = true})
    add_headerfiles("include/(**.h)")
