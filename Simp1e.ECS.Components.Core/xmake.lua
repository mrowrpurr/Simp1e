target("Simp1e.ECS.Components.Core")
    set_kind("headeronly")
    add_deps("Simp1e.Core", "Simp1e.ECS.Core")
    add_includedirs("include", {public = true})
    add_headerfiles("include/(**.h)")
