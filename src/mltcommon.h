#ifndef _POSIXSHM_COMMON_H_
#define _POSIXSHM_COMMON_H_

#include <inttypes.h>

typedef enum
{
        mlt_image_none = 0,/**< image not available */
        mlt_image_rgb24,   /**< 8-bit RGB */
        mlt_image_rgb24a,  /**< 8-bit RGB with alpha channel */
        mlt_image_yuv422,  /**< 8-bit YUV 4:2:2 packed */
        mlt_image_yuv420p, /**< 8-bit YUV 4:2:0 planar */
        mlt_image_opengl,  /**< (deprecated) suitable for OpenGL texture */
        mlt_image_glsl,    /**< for opengl module internal use only */
        mlt_image_glsl_texture /**< an OpenGL texture name */
}
mlt_image_format;

/** The set of supported audio formats */

typedef enum
{
        mlt_audio_none = 0,/**< audio not available */
        mlt_audio_pcm = 1, /**< \deprecated signed 16-bit interleaved PCM */
        mlt_audio_s16 = 1, /**< signed 16-bit interleaved PCM */
        mlt_audio_s32,     /**< signed 32-bit non-interleaved PCM */
        mlt_audio_float,   /**< 32-bit non-interleaved floating point */
        mlt_audio_s32le,   /**< signed 32-bit interleaved PCM */
        mlt_audio_f32le,   /**< 32-bit interleaved floating point */
        mlt_audio_u8       /**< unsigned 8-bit interleaved PCM */
}
mlt_audio_format;

struct posix_shm_header {
  uint32_t frame;
  uint32_t frame_rate_num;
  uint32_t frame_rate_den;
  uint32_t image_size;
  mlt_image_format image_format;
  uint32_t width;
  uint32_t height;
  uint32_t audio_size;
  mlt_audio_format audio_format;
  uint32_t frequency;
  uint32_t channels;
  uint32_t samples;
};

char* mlt_image_format_to_gst (mlt_image_format format);
char* mlt_audio_format_to_gst (mlt_image_format format, char **layout);


#endif

