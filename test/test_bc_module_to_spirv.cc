#include "metalib2spirv.h"
#include "metal_compiler.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char *argv[]) {
  mtl_process_t proc = nullptr;
  metal_run_process(1, "metal", "", &proc);
  metal_close_process(proc);
  metallib_t mtlib = nullptr;
  FILE *file =
      fopen("../../test/shaderlibs/modern_renderer_ios_debug.metallib", "rb");
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  fseek(file, 0, SEEK_SET);
  char *bytes = (char *)malloc(size);
  if (bytes) {
    auto read_len = fread(bytes, 1, size, file);
    if (read_len == size) {
      fclose(file);
      file = nullptr;
    }
    auto err = metallib_open(bytes, size, &mtlib);
    if (mtlib) {
      mtl2spv_options options;
      metallib2spv2(mtlib, &options, "depthPyramid");
    }
    free(bytes);
  }
  return 0;
}