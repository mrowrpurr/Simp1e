target("Simp1e.ECS.Core")
    set_kind("headeronly")
    add_logging()
    add_deps("Simp1e.Core")
    add_packages("function_pointer", {public = true})
    add_includedirs("include", {public = true})
    add_headerfiles("include/(**.h)")
