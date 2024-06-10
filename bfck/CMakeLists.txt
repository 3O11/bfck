add_executable(bfck)

target_compile_features(bfck
    PRIVATE
        cxx_std_23
)

target_sources(bfck
    PRIVATE
        "Source/Main.cpp"
)
