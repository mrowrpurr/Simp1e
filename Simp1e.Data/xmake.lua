-- Header-only library for working with Simp1e data files and data structures
target("Simp1e.Data")
    set_kind("headeronly")
    add_includedirs("include", {public = true})
    add_headerfiles("include/(**.h)")
    add_logging()
    add_packages("nlohmann_json", {public = true})
