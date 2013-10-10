/*
 * Tests for mesh loading for vlc-warp additions.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../../../libvlc/test.h"
#include "../../../../modules/video_output/opengl.h"

#define MESH_DIR "modules/video_output/warp/test_meshes/"
/* Generous machine epsilon */
#define EP 1e-3

/**
 * Helpers
 **/

static gl_vout_mesh default_mesh;

/* Compare floats with epsilon. */
static bool equals(float a, float b) {
  return fabs(a-b) < EP;
}

static void print_mesh(gl_vout_mesh* mesh) {
  printf("-----------------\n");
  printf("num_triangles %d\n", mesh->num_triangles);
  printf("num_planes %d\n", mesh->num_planes);
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
    printf("a %d, b %d", a->num_triangles, b->num_triangles);
    printf("num_triangles\n");
    return false;
  } 
  if (!(a->num_planes == b->num_planes)) {
    printf("a %d, b %d", a->num_planes, b->num_planes);
    printf("num_planes\n");
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
 * Test reading a correct mesh file with no negative coordinates.
 *
 */
static void test_correct_mesh(bool print) {
  const char* error_169 = NULL;
  const char* file_169 = MESH_DIR"16x9mesh.mesh";
  gl_vout_mesh* mesh_169 = vout_display_opengl_ReadMesh(file_169, &error_169);

  /* Define what it should be */
  gl_vout_mesh mesh_actual;
  
  GLfloat triangles[] = {
    0.064473, -1.011250, 0.145160, -1.010950, 0.467920, -1.009740, 
    0.064473, -1.011250, 0.467920, -1.009740, 0.387227, -1.010050, 
    0.145160, -1.010950, 0.225849, -1.010660, 0.548613, -1.009420, 
    0.145160, -1.010950, 0.548613, -1.009420, 0.467920, -1.009740, 
    0.225849, -1.010660, 0.306538, -1.010360, 0.629306, -1.009120, 
    0.225849, -1.010660, 0.629306, -1.009120, 0.548613, -1.009420, 
    0.387227, -1.010050, 0.467920, -1.009740, 0.790699, -1.008470, 
    0.387227, -1.010050, 0.790699, -1.008470, 0.710002, -1.008790, 
    0.467920, -1.009740, 0.548613, -1.009420, 0.871397, -1.008140, 
    0.467920, -1.009740, 0.871397, -1.008140, 0.790699, -1.008470, 
    0.548613, -1.009420, 0.629306, -1.009120, 0.952095, -1.007810, 
    0.548613, -1.009420, 0.952095, -1.007810, 0.871397, -1.008140
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
    0.f, 1.f, 0.025641, 1.f, 0.128205, 1.f, 
    0.f, 1.f, 0.128205, 1.f, 0.102564, 1.f, 
    0.025641, 1.f, 0.051282, 1.f, 0.153846, 1.f, 
    0.025641, 1.f, 0.153846, 1.f, 0.128205, 1.f, 
    0.051282, 1.f, 0.076923, 1.f, 0.179487, 1.f, 
    0.051282, 1.f, 0.179487, 1.f, 0.153846, 1.f, 
    0.102564, 1.f, 0.128205, 1.f, 0.230769, 1.f, 
    0.102564, 1.f, 0.230769, 1.f, 0.205128, 1.f, 
    0.128205, 1.f, 0.153846, 1.f, 0.256410, 1.f, 
    0.128205, 1.f, 0.256410, 1.f, 0.230769, 1.f, 
    0.153846, 1.f, 0.179487, 1.f, 0.282051, 1.f, 
    0.153846, 1.f, 0.282051, 1.f, 0.256410, 1.f
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

  mesh_actual.num_triangles = 12;
  mesh_actual.num_planes = 0;
  mesh_actual.cached_aspect = -1.f;
  mesh_actual.cached_top = -1.f;
  mesh_actual.cached_left = -1.f;
  mesh_actual.cached_right = -1.f;
  mesh_actual.cached_bottom = -1.f;
  mesh_actual.triangles = triangles;
  mesh_actual.transformed = transformed;
  mesh_actual.uv = uv;
  mesh_actual.uv_transformed = transformed;
  mesh_actual.intensity = intensity;
  /* end define */

  if (print) {
    print_mesh(mesh_169);
    print_mesh(&mesh_actual);
  }
  assert(compare_meshes(mesh_169, &mesh_actual));
  free(mesh_169);
}

/*
 * UT002
 *
 * Test reading a mesh file that has negative UV coordinates.ls
 * Make sure that the coordinates are clamped to 0 if negative or 1 if larger than 1.
 *
 * Where negative mesh has negatives, positive mesh has 0 and where negative mesh has a value larger
 * than 1 positive mesh has 1.
 *
 * By specification positive mesh and negative mesh should output the same mesh structure.
 */
static void test_negative_mesh(bool print) {
  const char* error_msg_neg = NULL;
  const char* error_msg_pos = NULL;
  const char* neg_file = MESH_DIR"negative_uv.mesh";
  const char* pos_file = MESH_DIR"positive_uv.mesh";
  gl_vout_mesh* mesh_neg = vout_display_opengl_ReadMesh(neg_file, &error_msg_neg);
  gl_vout_mesh* mesh_pos = vout_display_opengl_ReadMesh(pos_file, &error_msg_pos);
  if (print) {
    printf("%s\n", pos_file);
    print_mesh(mesh_pos);
    printf("%s\n", neg_file);
    print_mesh(mesh_neg);
  }
  assert(compare_meshes(mesh_pos, mesh_neg));
  free(mesh_pos);
  free(mesh_neg);
}

/*
 * UT003
 *
 * Test reading a file that does not exist.
 *
 */
static void test_errored_mesh(bool print) {
  const char* error_msg = NULL;
  const char* filename = "thisdoesnotexist.data";
  gl_vout_mesh* mesh = vout_display_opengl_ReadMesh(filename, &error_msg);
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
 * No file name.
 *
 * This should return the default mesh.
 *
 */
static void test_no_name_mesh(bool print) {
  const char* error_msg = NULL;
  const char* filename = "";
  gl_vout_mesh* mesh = vout_display_opengl_ReadMesh(filename, &error_msg);
    if (print) {
    printf("\"\"\n");
    print_mesh(mesh);
  }
  assert(is_default_mesh(mesh));
  free(mesh);
}

/*
 * UT005
 *
 * Reading an incorrectly formatted mesh file.
 *
 * This should return the default mesh.
 */
static void test_bad_format_mesh(bool print) {
  const char* error_msg = NULL;
  const char* filename = MESH_DIR"malformatted_mesh.mesh";
  gl_vout_mesh* mesh = vout_display_opengl_ReadMesh(filename, &error_msg);
    if (print) {
    printf("%s\n", filename);
    print_mesh(mesh);
  }
  assert(is_default_mesh(mesh));
  free(mesh);
}

/*
 * UT006
 *
 * Test that coordinates with non-positive intensity are not drawn.
 *
 * This is the same as the point not existing in the first place so testing by
 * having two meshes, one with the point missing and one with negative or zero intensity
 * values for those points.
 *
 */
static void test_intensity(bool print) {
  const char* error_msg_miss = NULL;
  const char* error_msg_neg = NULL;
  const char* neg_file = MESH_DIR"neg_intensity.mesh";
  gl_vout_mesh* neg_mesh = vout_display_opengl_ReadMesh(neg_file, &error_msg_neg);
  
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
    1.f, 0.f, 1.f,
    1.f, 0.f, 1.f,
    1.f, 1.f, 0.f
  };

  mesh_miss.num_triangles = 4;
  mesh_miss.num_planes = 0;
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

  if (print) {
    printf("%s\n", neg_file);
    print_mesh(neg_mesh);
    printf("Should be... \n");
    print_mesh(&mesh_miss);
  }
  assert(compare_meshes(neg_mesh, &mesh_miss));
  free(neg_mesh);
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
  default_mesh.num_planes = 0;
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

  // TODO remove this
  //  char** test = NULL;
  // print_mesh(vout_display_opengl_ReadMesh(NULL, &test));

  test_init();
  /*
   * true as arg will make it print the meshes it is using to test.
   */
  test_intensity(false); // PASSES
  test_bad_format_mesh(false); // PASSES
  test_no_name_mesh(false); // PASSES
  test_errored_mesh(false); // PASSES
  test_negative_mesh(true); // FAILS -> Seems like UV not clamping correctly
  test_correct_mesh(false); // PASSES

	return 0;
}
