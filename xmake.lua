add_languages("c++17")
local depends = {
    "eigen", "stb", "openmp", "tinyobjloader"
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

target("Test")
    add_includedirs("includes")
    add_files("sources/*.cpp")
    add_packages(depends, {public = true})
    set_targetdir(".")
    add_files("test.cpp")
    set_kind("binary")

target("LightFormerFrontEnd")
    add_includedirs("includes")
    add_files("sources/*.cpp")
    add_packages(depends, {public = true})
    set_targetdir(".")
    add_files("lightformer.cpp")
    set_kind("binary")