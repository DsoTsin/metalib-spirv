cmake_minimum_required (VERSION 3.18)
project (MetalibSPIRV C CXX)
set_property(GLOBAL PROPERTY USE_FOLDERS on)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_PDB_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/pdb)

# -DCMAKE_PREFIX_PATH="C:\vcpkg\installed\x64-windows\share\llvm"
find_package(LLVM)

include_directories(
    include
    third_party/SPIRV-Headers/include
    ${LLVM_INCLUDE_DIRS}    
)

add_subdirectory(third_party/SPIRV-Headers)
add_subdirectory(third_party/SPIRV-Tools)

set_property(
    TARGET 
        acc_gen 
        core_tables 
        enum_string_mapping 
        extinst_tables 
        install-headers 
        intrinsics_gen 
        omp_gen 
        spirv-tools-pkg-config 
        spirv-tools-shared-pkg-config 
        SPIRV-Headers-example 
    PROPERTY 
        FOLDER "externals"
)

add_subdirectory(source)
add_subdirectory(test)