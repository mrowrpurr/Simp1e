function add_requires_for_specs()
    -- Specs.cpp is not yet released as a package, so you have to require it locally
    local specs_path = os.getenv("SPECS_CPP")
    if specs_path then
        includes(path.join(specs_path, "Specs/xmake.lua"))
    else
        print("SPECS_CPP not set")
        return
    end

    add_requires("vcpkg::snowhouse")
end

function add_specs()
    add_packages("vcpkg::snowhouse")
    add_deps("Specs")
end
