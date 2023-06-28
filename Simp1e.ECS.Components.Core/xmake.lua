target("Simp1e.ECS.Components.Core")
    set_kind("headeronly")
    add_includedirs("include", {public = true})
    add_headerfiles("include/(**.h)")
