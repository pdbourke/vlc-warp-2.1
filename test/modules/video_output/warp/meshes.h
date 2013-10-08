#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../../../libvlc/test.h"
#include "../../../../modules/video_output/opengl.h"

#define MESH_DIR ../test_meshes/ 
/* Generous machine epsilon */
#define EP 1e-3


equals(float a, float b);

compare_meshes(gl_vout_mesh* a, gl_vout_mesh* a);

is_default_mesh(gl_vout_mesh* mesh);

test_correct_mesh(void);

test_negative_mesh(void);

test_errored_mesh(void);

test_no_name_mesh(void);

test_bad_format_mesh(void);

test_intensity(void);
