function add_requires_for_logging()
    add_requires("_Log_", "spdlog")
end

function add_logging()
    add_packages("_Log_", {public = true})
end

function add_logging_support_for_binaries()
    add_packages("spdlog")
end
