#include "emscripten.h"
#include "../include/pksav/gen1/save.h"

EMSCRIPTEN_KEEPALIVE
enum pksav_error gen1_get_file_save_type(
    const char* file_path,
    enum pksav_gen1_save_type* save_type_out
) {
    return pksav_gen1_get_file_save_type(file_path, save_type_out);
}
