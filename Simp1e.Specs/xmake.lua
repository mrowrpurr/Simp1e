add_requires("vcpkg::snowhouse")

-- Include the Specs.cpp library for tests
local specs_path = os.getenv("SPECS_CPP")
if specs_path then
    includes(specs_path)
else
    print("SPECS_CPP not set")
    return
end

target("Simp1e.Specs")
    set_kind("binary")
    add_packages("vcpkg::snowhouse")
    add_deps("Simp1e", "Specs")
    add_files("*.cpp")
    add_includedirs(".")
