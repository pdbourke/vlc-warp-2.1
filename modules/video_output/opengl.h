/*****************************************************************************
 * opengl.h: OpenGL vout_display helpers
 *****************************************************************************
 * Copyright (C) 2004-2013 VLC authors and VideoLAN
 * Copyright (C) 2009 Laurent Aimar
 *
 * Authors: Laurent Aimar <fenrir _AT_ videolan _DOT_ org>
 *          Rémi Denis-Courmont
 *          Jean-Baptiste Kempf <jb@videolan.org>
 *          Ilkka Ollakka <ileoo@videolan.org>
 *          Felix Paul Kühne <fkuehne at videolan dot org>
 *          Rafaël Carré <funman@videolanorg>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#include <vlc_common.h>
#include <vlc_picture_pool.h>
#include <vlc_opengl.h>

/* Change USE_OPENGL_ES value to set the OpenGL ES version (1, 2) you want to use
 * A value of 0 will activate normal OpenGL */
#ifdef __APPLE__
# include <TargetConditionals.h>
# if !TARGET_OS_IPHONE
#  define USE_OPENGL_ES 0
#  define MACOS_OPENGL
#  include <OpenGL/gl.h>
# else /* Force ESv2 on iOS */
#  define USE_OPENGL_ES 2
#  include <OpenGLES/ES1/gl.h>
#  include <OpenGLES/ES2/gl.h>
#  include <OpenGLES/ES2/glext.h>
# endif
#else /* !defined (__APPLE__) */
# ifndef USE_OPENGL_ES
#  define USE_OPENGL_ES 0
# endif
# if USE_OPENGL_ES == 2
#  include <GLES2/gl2.h>
# elif USE_OPENGL_ES == 1
#  include <GLES/gl.h>
# else
#  ifdef _WIN32
#   include <GL/glew.h>
#   undef glClientActiveTexture
#   undef glActiveTexture
    PFNGLACTIVETEXTUREPROC glActiveTexture;
    PFNGLCLIENTACTIVETEXTUREPROC glClientActiveTexture;
#  endif
#  include <GL/gl.h>
# endif
#endif

static inline bool HasExtension(const char *apis, const char *api)
{
    size_t apilen = strlen(api);
    while (apis) {
        while (*apis == ' ')
            apis++;
        if (!strncmp(apis, api, apilen) && memchr(" ", apis[apilen], 2))
            return true;
        apis = strchr(apis, ' ');
    }
    return false;
}

typedef struct vout_display_opengl_t vout_display_opengl_t;

/* Will output debug fps information */
#define OUTPUT_DEBUG_FPS
#define DEBUG_FPS_BLOCK_SIZE (100)

/* Comment out to enable fps debug output */
#undef OUTPUT_DEBUG_FPS

/* Visible for testing */
typedef struct
{
    int num_triangles;
    int num_planes; /* The number of choma planes. */
    GLfloat *triangles; /* A list of coordinates to form triangles. */
    GLfloat *transformed; /* A transformed version of triangles, based on the current aspect ratio */
    GLfloat *uv; /* UV coordinates for each coordinate in triangles. */
    /* These store the linearly interpolated UV coordinates
     * based on the rectangle VLC gives us identifying the subregion
     * of the texture to draw. We assume that we will never want
     * differently transformed coordinates for different chroma planes. */
    GLfloat *uv_transformed;
    GLfloat *intensity; /* Intensity values for each coordinate in triangles. */

    /* If the current aspect ratio isn't the same as this, we need
     * to recalculate our transformed coordinates for rendering. */
    float cached_aspect;

    /* If the current left, top, right, bottom values differ from these,
     * we need to recalculate uv_transformed */
    float cached_left, cached_top, cached_right, cached_bottom;

#ifdef OUTPUT_DEBUG_FPS
    long long last_frame_millis;
    long long last_frame_seconds;
    long long last_block_millis;
    long long last_block_seconds;
    long long max_millis;
    long long min_millis;
    int frame_count;
#endif

} gl_vout_mesh;

vout_display_opengl_t *vout_display_opengl_New(video_format_t *fmt,
                                               const vlc_fourcc_t **subpicture_chromas,
                                               vlc_gl_t *gl);

/* Visible for testing */
gl_vout_mesh* vout_display_opengl_ReadMesh(const char *filename, const char** error_msg);

void vout_display_opengl_LoadMesh(vout_display_opengl_t* vgl, const char* filename, vlc_object_t* obj);

void vout_display_opengl_Delete(vout_display_opengl_t *vgl);

picture_pool_t *vout_display_opengl_GetPool(vout_display_opengl_t *vgl, unsigned);

int vout_display_opengl_Prepare(vout_display_opengl_t *vgl,
                                picture_t *picture, subpicture_t *subpicture);
int vout_display_opengl_Display(vout_display_opengl_t *vgl,
                                const video_format_t *source);
