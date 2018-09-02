link_libraries(tinycompiler)
set(source_list
    test_main.cpp
    test_scanner.cpp
    test_parser.cpp
    )
add_executable(unittest ${source_list})
