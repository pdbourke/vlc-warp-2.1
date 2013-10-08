/*
 * Tests for mesh loading for vlc-warp additions.
 */
#include "./meshes.h"

/**
 * Helpers
 **/

struct gl_vout_mesh default_mesh;

/* Compare floats with epsilon. */
static bool equals(float a, float b) {
  return fabs(a-b) < EP;
}

/*
 * Mesh comparison.
 */
static bool compare_meshes(gl_vout_mesh* a, gl_vout_mesh* b) {
    /*
   * Compare the contents of the mesh structures
   */
  assert(a->num_triangles == b->num_triangles);
  assert(a->num_planes == b->num_planes);
  assert(equals(a->cached_aspect, b->cached_aspect));
  assert(equals(a->cached_top, b->cached_top));
  assert(equals(a->cached_left, b->cached_left));
  assert(equals(a->cached_right, b->cached_right));
  assert(equals(a->cached_bottom, b->cached_bottom));
  /* Doesn't matter which mesh we get num_triangles from at this point because 
   * we already asserted that they both have the same number of triangles.
   */ 
  for (int tri = 0; tri < a->num_triangles; ++tri) {
    for (int i = 0; i < 6; ++i) {
      assert(equals(a->triangles[6*tri+i], b->triangles[6*tri+i]));
      assert(equals(a->transformed[6*tri+i], b->transformed[6*tri+i]));
      assert(equals(a->uv[6*tri+i], b->uv[6*tri+i]));
      assert(equals(a->uv_transformed[6*tri+i], b->uv_transformed[6*tri+i]));
      assert(equals(a->intensity[3*tri+i%2], b->intensity[6*tri+i%2]));
    }
  }
  return true;
}

/* 
 * Check if a mesh is the default mesh 
 */ 
static bool is_default_mesh(gl_vout_mesh* mesh) {
  if (compare_meshes(default_mesh, mesh)) {
    return true;
  }
  return false;
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
void test_correct_mesh(void) {
  const char* error_msg = NULL;
  const char* filename = "mesh file here";
  gl_vout_mesh* mesh = vout_display_opengl_ReadMesh(filename, &error_msg);
  
  /**
   * I will be doing this tomorrow night starting around 6pm until I sleep ~12pm.
   **/
  free(mesh);  
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
void test_negative_mesh(void) {
  const char* error_msg_neg = NULL;
  const char* error_msg_pos = NULL;
  const char* neg_file = MESH_DIR"negative_uv.mesh";
  const char* pos_file = MESH_DIR"positive_uv.mesh";
  gl_vout_mesh* mesh_neg = vout_display_opengl_ReadMesh(neg_file, &error_msg_neg);
  gl_vout_mesh* mesh_pos = vout_display_opengl_ReadMesh(pos_file, &error_msg_pos);
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
void test_errored_mesh(void) {
  const char* error_msg = NULL;
  const char* filename = "thisdoesnotexist.data";
  gl_vout_mesh* mesh = vout_display_opengl_ReadMesh(filename, &error_msg); 
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
void test_no_name_mesh(void) {
  const char* error_msg = NULL;
  const char* filename = "";
  gl_vout_mesh* mesh = vout_display_opengl_ReadMesh(filename, &error_msg);
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
void test_bad_format_mesh(void) {
  const char* error_msg = NULL;
  const char* filename = MESH_DIR"malformatted_mesh.mesh";
  gl_vout_mesh* mesh = vout_display_opengl_ReadMesh(filename, &error_msg);
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
 * This is wrong, and I'm aware of it. will fix tomorrow as well.
 *
 */
void test_intensity(void) {
  const char* error_msg_miss = NULL;
  const char* error_msg_neg = NULL;
  const char* neg_file = MESH_DIR"neg_intensity.mesh";
  const char* miss_file = MESH_DIR"miss_intensity.mesh";
  gl_vout_mesh* neg_mesh = vout_display_opengl_ReadMesh(neg_file, &error_msg_neg);
  gl_vout_mesh* miss_mesh = vout_display_opengl_ReadMesh(miss_file, &error_msg_miss);
  assert(compare_meshes(neg_mesh, miss_mesh));
  free(neg_mesh); 
  free(miss_mesh);
}

int main(void) {
  /*
   * Initialise the default mesh.
   */

  // TODO my c is terrible should be -> or . ?
  // I can't really check which works without the struct being imported correctly.
  default_mesh.num_triangles = 2;
  default_mesh.num_planes = 2;
  default_mesh.triangles = {-1,-1,1,-1,1,1,-1,-1,1,1,-1,1};
  default_mesh.transformed = {}; 
  default_mesh.uv = {0,0,1,0,1,1,0,0,1,1,0,1};
  default_mesh.uv_transformed = {}; /* Irrelevant for default mesh */
  default_mesh.intensity = {1,1,1,1,1,1}; 
  default_mesh.cached_aspect = -1;
  default_mesh.cached_left = -1;
  default_mesh.cached_top = -1; 
  default_mesh.cached_right = -1;
  default_mesh.cached_bottom = -1;

  test_intensity();
  test_bad_format_mesh();
  test_no_name_mesh();
  test_errored_mesh();
  test_negative_mesh();
  test_correct_mesh();

	return 0;
}
