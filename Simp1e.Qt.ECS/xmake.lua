target("Simp1e.Qt.ECS")
    set_kind("headeronly")
    add_logging()
    add_deps("Simp1e.Qt", "Simp1e.ECS.Core")
    add_includedirs("include", {public = true})
    add_headerfiles("include/(**.h)")