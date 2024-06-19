add_executable(bfck)

target_compile_features(bfck
    PRIVATE
        cxx_std_23
)

target_include_directories(bfck
    PRIVATE
        "Include"
)

target_sources(bfck
    PRIVATE
        "Source/Utils.cpp"
        "Source/Interpreter.cpp"
        "Source/Machine.cpp"
        "bfck.cpp"
)
