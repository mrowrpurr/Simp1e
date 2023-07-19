function add_requires_for_logging()
    add_requires("mrowr_log", "spdlog")
end

function add_logging()
    add_packages("mrowr_log", {public = true})
end

function add_logging_runtime()
    add_packages("mrowr_log", "spdlog")
end
