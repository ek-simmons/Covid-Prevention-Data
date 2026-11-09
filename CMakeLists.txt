cmake_minimum_required(VERSION 3.20)
project(Covid_Prevention_Data)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Covid_Data
        src/main.cpp
        src/Trie.cpp
        src/BTree.cpp
)