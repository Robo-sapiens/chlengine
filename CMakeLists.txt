cmake_minimum_required(VERSION 3.13)
project(chlengine)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

add_compile_options(
        $<$<COMPILE_LANGUAGE:C,CXX>:-Wall>
        $<$<COMPILE_LANGUAGE:C,CXX>:-Wextra>
        $<$<COMPILE_LANGUAGE:C,CXX>:-Wpedantic>
        $<$<COMPILE_LANGUAGE:C,CXX>:-pedantic-errors>
        $<$<COMPILE_LANGUAGE:C,CXX>:-Wunused-const-variable>
        $<$<COMPILE_LANGUAGE:C,CXX>:-Wfloat-conversion>
        $<$<COMPILE_LANGUAGE:CXX>:-Wctor-dtor-privacy>
        $<$<AND:$<CXX_COMPILER_ID:GNU>,$<COMPILE_LANGUAGE:C,CXX>>:-Wduplicated-cond>
        $<$<AND:$<CXX_COMPILER_ID:Clang>,$<COMPILE_LANGUAGE:C,CXX>>:-Wextra-semi>
        $<$<COMPILE_LANGUAGE:CXX>:-Wnon-virtual-dtor>
        $<$<COMPILE_LANGUAGE:CXX>:-Woverloaded-virtual>
        $<$<COMPILE_LANGUAGE:C,CXX>:-Wredundant-decls>
        $<$<COMPILE_LANGUAGE:CXX>:-Wsign-promo>
#        $<$<COMPILE_LANGUAGE:CXX>:-Wzero-as-null-pointer-constant>
        $<$<AND:$<CXX_COMPILER_ID:GNU>,$<COMPILE_LANGUAGE:CXX>>:-Wsuggest-override>
        $<$<AND:$<CXX_COMPILER_ID:Clang>,$<COMPILE_LANGUAGE:CXX>>:-Winconsistent-missing-override>
        $<$<COMPILE_LANGUAGE:C,CXX>:-Wshadow>
        $<$<COMPILE_LANGUAGE:C,CXX>:-Wfloat-equal>
        $<$<COMPILE_LANGUAGE:C,CXX>:-Wcast-qual>
        $<$<AND:$<CXX_COMPILER_ID:GNU>,$<COMPILE_LANGUAGE:CXX>>:-Wlogical-op>
        $<$<COMPILE_LANGUAGE:C,CXX>:-Werror>
        $<$<COMPILE_LANGUAGE:C,CXX>:-Wcast-align>
)

if (CMAKE_COMPILER_IS_GNUCXX)
    if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 7.1.0)
        add_compile_options($<$<COMPILE_LANGUAGE:C,CXX>:-Wduplicated-branches>)
    endif ()
    if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 8.1.0)
        add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-Wextra-semi>)
    endif ()
endif ()

add_subdirectory(chlengine)
add_subdirectory(examples)

if (BUILD_TESTING)
    add_subdirectory(test)
endif (BUILD_TESTING)
