target("Simp1e.Qt.ECS")
    set_kind("headeronly")
    add_logging()
    add_deps("Simp1e.Qt", "Simp1e.ECS.Core", "Simp1e.ECS.Components.Common", "Simp1e.ECS.Components.GUI")
    add_includedirs("include", {public = true})
    add_headerfiles("include/(**.h)")
