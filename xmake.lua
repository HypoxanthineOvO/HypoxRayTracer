add_languages("c++17")
local depends = {
    "eigen", "stb", "openmp"
}

add_requires(depends)

target("HypoxRayTracer")
    add_includedirs("includes")
    add_files("sources/*.cpp")
    add_packages(depends, {public = true})
    set_targetdir(".")
    add_files("main.cpp")
    -- Add macro "DEBUG"
    add_defines("DEBUG")
    set_kind("binary")
