#add_library(test_common SHARED test_common.cc test_common.h)
#target_compile_definitions(test_common PRIVATE BUILD_LIB=1)
add_executable(test_bc test_lib_to_bc.cc)
target_link_libraries(test_bc metal_compiler metalib2spirv)
add_executable(test_spv test_bc_module_to_spirv.cc)
target_link_libraries(test_spv metal_compiler metalib2spirv)