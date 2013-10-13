/*
 * Tests for mesh loading for vlc-warp additions.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../../../libvlc/test.h"
#include "../../../../modules/video_output/opengl.h"

#define MESH_DIR "modules/video_output/warp/test_meshes/"

/**
 * Helpers
 **/
static gl_vout_mesh default_mesh;

/* Compare floats with epsilon. */
static bool equals(float a, float b) {
  return fabs(a-b) < MESH_EP;
}

static void print_mesh(gl_vout_mesh* mesh) {
  printf("-----------------\n");
  printf("num_triangles %d\n", mesh->num_triangles);
  printf("cached_aspect %f\n", mesh->cached_aspect);
  printf("cached_top %f\n", mesh->cached_top);
  printf("cached_left %f\n", mesh->cached_left);
  printf("cached_right %f\n", mesh->cached_right);
  printf("cached_bottom %f\n", mesh->cached_bottom);
  int index = 0;
  printf("%4s %15s %15s %10s %15s %15s\n", "#", "triangles", "transformed", "uv", "uv_trans", "intensity");
  for (int tri = 0; tri < mesh->num_triangles; ++tri) {
    for (int i = 0; i < 6; ++i) {
        index = 6*tri+i;
        printf("%4d", index);
        printf("%15f", mesh->triangles[index]);
        printf("%15f", mesh->transformed[index]);
        printf("%15f", mesh->uv[index]);
        if (index % 2 == 0) {
          printf("%15f", mesh->uv_transformed[index]);
          printf("%15f\n", mesh->intensity[index/2]);
        } else {
          printf("%15f\n", mesh->uv_transformed[index]);
        }
    }
  }
  printf("-----------------\n");
}

/*
 * Mesh comparison.
 */
static bool compare_meshes(gl_vout_mesh* a, gl_vout_mesh* b) {
    /*
   * Compare the contents of the mesh structures
   */
  if (!(a->num_triangles == b->num_triangles)) {
    printf("a %d, b %d -> ", a->num_triangles, b->num_triangles);
    printf("num_triangles\n");
    return false;
  } 
  if (!equals(a->cached_aspect, b->cached_aspect)) {
    printf("a %f, b %f -> ", a->cached_aspect, b->cached_aspect);
    printf("cached_aspect\n");
    return false;
  }
  if (!equals(a->cached_top, b->cached_top)) {
    printf("a %f, b %f -> ", a->cached_top, b->cached_top);
    printf("cached_top\n");
    return false;
  }
  if (!equals(a->cached_left, b->cached_left)) {
    printf("a %f, b %f -> ", a->cached_left, b->cached_left);
    printf("cached_left\n");
    return false;
  }
  if (!equals(a->cached_right, b->cached_right)) {
    printf("a %f, b %f -> ", a->cached_right, b->cached_right);
    printf("cached_right\n");
    return false;
  }
  if (!equals(a->cached_bottom, b->cached_bottom)) {
    printf("a %f, b %f -> ", a->cached_bottom, b->cached_bottom);
    printf("cached_bottom\n");
    return false;
  }
  /* Doesn't matter which mesh we get num_triangles from at this point because
   * we already asserted that they both have the same number of triangles.
   */
  int index = 0;
  for (int tri = 0; tri < a->num_triangles; ++tri) {
    for (int i = 0; i < 6; ++i) {
      index = 6*tri+i;
      if (!equals(a->triangles[index], b->triangles[index])) {
        printf("a %f, b %f -> ", a->triangles[index], b->triangles[index]);
        printf("triangles %d\n", index);
        return false;
      }
      if (!equals(a->transformed[index], b->transformed[index])) {
        printf("a %f, b %f -> ", a->transformed[index], b->transformed[index]);
        printf("transformed %d\n", index);
        return false;
      }
      if (!equals(a->uv[index], b->uv[index])) {
        printf("a %f, b %f -> ", a->uv[index], b->uv[index]);
        printf("uv %d\n", index);
        return false;
      }
      if (!equals(a->uv_transformed[index], b->uv_transformed[index])) {
        printf("a %f, b %f -> ", a->uv_transformed[index], b->uv_transformed[index]);
        printf("uv_transformed %d\n", index);
        return false;
      }
      if (!equals(a->intensity[index/2], b->intensity[index/2])) {
        printf("a %f, b %f -> ", a->intensity[index/2], b->intensity[index/2]);
        printf("intensity %d\n", index/2);
        return false;
      }
    }
  }
  return true;
}

/*
 * Check if a mesh is the default mesh
 */
static bool is_default_mesh(gl_vout_mesh* mesh) {
  return compare_meshes(&default_mesh, mesh);
}

/**
 * Tests
 **/
/*
 * UT001
 *
 * Test reading a correct mesh file with no negative UV coordinates or intensities.
 *
 */
static void test_correct_mesh(bool print) {
  const char* error_msg = NULL;
  const char* file_169 = MESH_DIR"16x9mesh.mesh";
  gl_vout_mesh* mesh_169 = vout_display_opengl_ReadMesh(file_169, &error_msg);

  /* Define what it should be */
  gl_vout_mesh mesh_expected;
  
  GLfloat triangles[] = {
    0.466732, -0.942460, 0.547456, -0.942125, 0.870373, -0.940743, 
    0.466732, -0.942460, 0.870373, -0.940743, 0.789642, -0.941094, 
    0.547456, -0.942125, 0.628185, -0.941784, 0.951104, -0.940389, 
    0.547456, -0.942125, 0.951104, -0.940389, 0.870373, -0.940743, 
    0.628185, -0.941784, 0.708911, -0.941442, 1.031830, -0.940031, 
    0.628185, -0.941784, 1.031830, -0.940031, 0.951104, -0.940389, 
    0.789642, -0.941094, 0.870373, -0.940743, 1.193320, -0.939301, 
    0.789642, -0.941094, 1.193320, -0.939301, 1.112580, -0.939666, 
    0.870373, -0.940743, 0.951104, -0.940389, 1.274050, -0.938930, 
    0.870373, -0.940743, 1.274050, -0.938930, 1.193320, -0.939301, 
    0.951104, -0.940389, 1.031830, -0.940031, 1.354780, -0.938555, 
    0.951104, -0.940389, 1.354780, -0.938555, 1.274050, -0.938930
  };

  GLfloat transformed[] = {
    0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f, 0.f, 0.f
  };

  GLfloat uv[] = {
    0.128205, 0.965517, 0.153846, 0.965517, 0.256410, 0.965517, 
    0.128205, 0.965517, 0.256410, 0.965517, 0.230769, 0.965517, 
    0.153846, 0.965517, 0.179487, 0.965517, 0.282051, 0.965517, 
    0.153846, 0.965517, 0.282051, 0.965517, 0.256410, 0.965517, 
    0.179487, 0.965517, 0.205128, 0.965517, 0.307692, 0.965517, 
    0.179487, 0.965517, 0.307692, 0.965517, 0.282051, 0.965517, 
    0.230769, 0.965517, 0.256410, 0.965517, 0.358974, 0.965517, 
    0.230769, 0.965517, 0.358974, 0.965517, 0.333333, 0.965517, 
    0.256410, 0.965517, 0.282051, 0.965517, 0.384615, 0.965517, 
    0.256410, 0.965517, 0.384615, 0.965517, 0.358974, 0.965517, 
    0.282051, 0.965517, 0.307692, 0.965517, 0.410256, 0.965517, 
    0.282051, 0.965517, 0.410256, 0.965517, 0.384615, 0.965517
  };

  GLfloat intensity[] = {
    1.f, 1.f, 1.f, 
    1.f, 1.f, 1.f, 
    1.f, 1.f, 1.f, 
    1.f, 1.f, 1.f, 
    1.f, 1.f, 1.f, 
    1.f, 1.f, 1.f, 
    1.f, 1.f, 1.f, 
    1.f, 1.f, 1.f, 
    1.f, 1.f, 1.f, 
    1.f, 1.f, 1.f, 
    1.f, 1.f, 1.f, 
    1.f, 1.f, 1.f
  };

  mesh_expected.num_triangles = 12;
  mesh_expected.cached_aspect = -1.f;
  mesh_expected.cached_top = -1.f;
  mesh_expected.cached_left = -1.f;
  mesh_expected.cached_right = -1.f;
  mesh_expected.cached_bottom = -1.f;
  mesh_expected.triangles = triangles;
  mesh_expected.transformed = transformed;
  mesh_expected.uv = uv;
  mesh_expected.uv_transformed = transformed;
  mesh_expected.intensity = intensity;
  /* end define */
  if (error_msg != NULL) {
    printf("%s\n", error_msg);
    // repeat check so it is obvious what is failing in test output.
    assert(error_msg != NULL);
  }
  if (print) {
    print_mesh(mesh_169);
    print_mesh(&mesh_expected);
  }
  assert(compare_meshes(mesh_169, &mesh_expected));
  free(mesh_169);
}

/*
 * UT002
 *
 * Test that coordinates with non-positive intensity are not drawn.
 *
 */
static void test_intensity(bool print) {
  const char* error_msg = NULL;
  const char* neg_file = MESH_DIR"neg_intensity.mesh";
  gl_vout_mesh* neg_mesh = vout_display_opengl_ReadMesh(neg_file, &error_msg);

  /* Define what the output should be. */
  gl_vout_mesh mesh_miss;

  GLfloat triangles[] = {
    0.225849, -1.010660, 0.306538, -1.010360, 0.629306, -1.009120, 
    0.225849, -1.010660, 0.629306, -1.009120, 0.548613, -1.009420, 
    0.548613, -1.009420, 0.629306, -1.009120, 0.952095, -1.007810, 
    0.548613, -1.009420, 0.952095, -1.007810, 0.871397, -1.008140
  };

  GLfloat transformed[] = {
    0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f, 0.f, 0.f
  };

  GLfloat uv[] = {
    0.f, 1.f, 1.f, 1.f, 0.179487, 1.f, 
    0.f, 1.f, 0.179487, 1.f, 1.f, 1.f, 
    1.f, 1.f, 0.179487, 1.f, 0.282051, 1.f, 
    1.f, 1.f, 0.282051, 1.f, 0.256410, 0.f
  };

  GLfloat intensity[] = {
    1.f, 0.f, 0.f, 
    1.f, 0.f, 0.500000, 
    0.500000, 0.f, 1.f, 
    0.500000, 1.f, 1.f
  };

  mesh_miss.num_triangles = 4;
  mesh_miss.cached_aspect = -1.f;
  mesh_miss.cached_top = -1.f;
  mesh_miss.cached_left = -1.f;
  mesh_miss.cached_right = -1.f;
  mesh_miss.cached_bottom = -1.f;
  mesh_miss.triangles = triangles;
  mesh_miss.transformed = transformed;
  mesh_miss.uv = uv;
  mesh_miss.uv_transformed = transformed;
  mesh_miss.intensity = intensity;
  /* end define */
  if (error_msg != NULL) {
    printf("%s\n", error_msg);
    // repeat check so it is obvious what is failing in test output.
    assert(error_msg != NULL);
  }
  if (print) {
    printf("%s\n", neg_file);
    print_mesh(neg_mesh);
    printf("Should be... \n");
    print_mesh(&mesh_miss);
  }
  assert(compare_meshes(neg_mesh, &mesh_miss));
  free(neg_mesh);
}

/*
 * UT003 - 01
 *
 * Reading incorrectly formatted mesh files.
 *
 * Wrong number of numbers in a line.
 *
 * This should return the default mesh.
 */
static void test_bad_format_mesh_01(bool print) {
  const char* error_msg = NULL;
  const char* filename = MESH_DIR"malformed/wrong_num_num.mesh";
  gl_vout_mesh* mesh = vout_display_opengl_ReadMesh(filename, &error_msg);
  if (strcmp(error_msg, MAL_MESH_ERR) != 0) {
    printf("%s\n", error_msg);
    // repeat check so it is obvious what is failing in test output.
    assert(strcmp(error_msg, MAL_MESH_ERR) == 0);
  }
  if (print) {
    printf("%s\n", filename);
    print_mesh(mesh);
  }
  assert(is_default_mesh(mesh));
  free(mesh);
}

/*
 * UT003 - 02
 *
 * Reading incorrectly formatted mesh files.
 *
 * No values in the mesh file.
 *
 * This should return the default mesh.
 */
static void test_bad_format_mesh_02(bool print) {
  const char* error_msg = NULL;
  const char* filename = MESH_DIR"malformed/no_val.mesh";
  gl_vout_mesh* mesh = vout_display_opengl_ReadMesh(filename, &error_msg);
  if (strcmp(error_msg, MAL_MESH_ERR) != 0) {
    printf("%s\n", error_msg);
    // repeat check so it is obvious what is failing in test output.
    assert(strcmp(error_msg, MAL_MESH_ERR) == 0);
  }
  if (print) {
    printf("%s\n", filename);
    print_mesh(mesh);
  }
  assert(is_default_mesh(mesh));
  free(mesh);
}

/*
 * UT003 - 03
 *
 * Reading incorrectly formatted mesh files.
 *
 * Wrong number of lines in the mesh file.
 *
 * This should return the default mesh.
 */
static void test_bad_format_mesh_03(bool print) {
  const char* error_msg = NULL;
  const char* filename = MESH_DIR"malformed/wrong_num_lines.mesh";
  gl_vout_mesh* mesh = vout_display_opengl_ReadMesh(filename, &error_msg);
  if (strcmp(error_msg, MAL_MESH_ERR) != 0) {
    printf("%s\n", error_msg);
    // repeat check so it is obvious what is failing in test output.
    assert(strcmp(error_msg, MAL_MESH_ERR) == 0);
  }
  if (print) {
    printf("%s\n", filename);
    print_mesh(mesh);
  }
  assert(is_default_mesh(mesh));
  free(mesh);
}

/*
 * UT003 - 04
 *
 * Reading incorrectly formatted mesh files.
 *
 * Characters in the mesh file instead of numbers.
 *
 * This should return the default mesh.
 */
static void test_bad_format_mesh_04(bool print) {
  const char* error_msg = NULL;
  const char* filename = MESH_DIR"malformed/char_mesh.mesh";
  gl_vout_mesh* mesh = vout_display_opengl_ReadMesh(filename, &error_msg);
  if (strcmp(error_msg, MAL_MESH_ERR) != 0) {
    printf("%s\n", error_msg);
    // repeat check so it is obvious what is failing in test output.
    assert(strcmp(error_msg, MAL_MESH_ERR) == 0);
  }
  if (print) {
    printf("%s\n", filename);
    print_mesh(mesh);
  }
  assert(is_default_mesh(mesh));
  free(mesh);
}

/*
 * UT004
 *
 * Test reading a file that does not exist.
 *
 */
static void test_errored_mesh(bool print) {
  const char* error_msg = NULL;
  const char* filename = "thisdoesnotexist.data";
  gl_vout_mesh* mesh = vout_display_opengl_ReadMesh(filename, &error_msg);
  if (strcmp(error_msg, UNDEF_FILE_ERR) != 0) {
    printf("%s\n", error_msg);
    // repeat check so it is obvious what is failing in test output.
    assert(strcmp(error_msg, UNDEF_FILE_ERR) != 0);
  } 
  if (print) {
    printf("%s\n", filename);
    print_mesh(mesh);
  }
  assert(is_default_mesh(mesh));
  free(mesh);
}

/*
 * UT005
 *
 * No file name.
 *
 * This should return the default mesh.
 *
 */
static void test_no_name_mesh(bool print) {
  const char* error_msg = NULL;
  const char* filename = "";
  gl_vout_mesh* mesh = vout_display_opengl_ReadMesh(filename, &error_msg);
  if (strcmp(error_msg, NO_MESH_ERR) != 0) {
    printf("%s\n", error_msg);
    // repeat check so it is obvious what is failing in test output.
    assert(strcmp(error_msg, NO_MESH_ERR) != 0);
  }
  if (print) {
    printf("\"\"\n");
    print_mesh(mesh);
  }
  assert(is_default_mesh(mesh));
  free(mesh);
}

int main(void) {
  /*
   * Initialise the default mesh.
   */
  GLfloat default_triangles[] = {-1.f, -1.f, 1.f, -1.f, 1.f, 1.f, -1.f, -1.f, 1.f, 1.f, -1.f, 1.f};
  GLfloat default_uv[] = {0.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f};
  GLfloat default_transformed[] = {0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f};
  GLfloat default_intensity[] = {1.f, 1.f, 1.f, 1.f, 1.f, 1.f};

  default_mesh.num_triangles = 2;
  default_mesh.triangles = default_triangles;
  default_mesh.transformed = default_transformed;
  default_mesh.uv = default_uv;
  default_mesh.uv_transformed = default_transformed;
  default_mesh.intensity = default_intensity;
  default_mesh.cached_aspect = -1.f;
  default_mesh.cached_left = -1.f;
  default_mesh.cached_top = -1.f;
  default_mesh.cached_right = -1.f;
  default_mesh.cached_bottom = -1.f;

  test_init();
  /*
   * true as arg will make it print the meshes it is using to test.
   */

  test_correct_mesh(false); // PASSES
  test_intensity(false); // PASSES
  test_bad_format_mesh_01(false); // PASSES
  test_bad_format_mesh_02(false); // PASSES
  test_bad_format_mesh_03(false); // PASSES
  test_bad_format_mesh_04(false); // PASSES
  test_no_name_mesh(false); // PASSES
  test_errored_mesh(false); // PASSES

  return 0;
}
