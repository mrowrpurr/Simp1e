target("Simp1e.ECS.Systems.Common")
    set_kind("headeronly")
    add_deps("Simp1e.ECS.Core", "Simp1e.ECS.Events.Common")
    add_includedirs("include", {public = true})
    add_headerfiles("include/(**.h)")
