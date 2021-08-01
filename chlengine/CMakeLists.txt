add_library(${PROJECT_NAME}
        STATIC
        src/awaitable.cpp
        src/executor.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC include)
