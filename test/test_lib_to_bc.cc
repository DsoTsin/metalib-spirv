#include "metalib2spirv.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char *argv[]) {
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
      for (int i = 0; i < metallib_function_count(mtlib); i++) {
        auto entry = metallib_function(mtlib, i);
        printf("name : %s, type: %d\n", entry->name.buffer, entry->type);
      }

      metallib_free(mtlib);
    }

    free(bytes);
  }
  return 0;
}