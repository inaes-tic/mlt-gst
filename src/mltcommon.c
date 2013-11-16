#include <mltcommon.h>
#include <glib.h>

char* mlt_image_format_to_gst(mlt_image_format format)
{
  switch (format) {
    case mlt_image_yuv422:
      return g_strdup("YUY2");
      break;

    case mlt_image_yuv420p:
      return g_strdup("YV12");
      break;

    case mlt_image_rgb24:
      return g_strdup("RGB");
      break;

    case mlt_image_rgb24a:
      return g_strdup("RGBA");
      break;

    case mlt_image_none:
    case mlt_image_opengl:
    case mlt_image_glsl:
    case mlt_image_glsl_texture:
    default:
      return NULL;
  }
}

char* mlt_audio_format_to_gst(mlt_image_format format, char **layout)
{
  switch (format) {
    case mlt_audio_s16:
      if (layout)
        *layout = g_strdup("interleaved");
      return g_strdup("S16LE");
      break;

    case mlt_audio_s32:
      if (layout)
        *layout = g_strdup("non-interleaved");
      return g_strdup("S32LE");
      break;

    case mlt_audio_float:
      if (layout)
        *layout = g_strdup("non-interleaved");
      return g_strdup("F32LE");
      break;

    case mlt_audio_s32le:
      if (layout)
        *layout = g_strdup("interleaved");
      return g_strdup("S32LE");
      break;

    case mlt_audio_f32le:
      if (layout)
        *layout = g_strdup("interleaved");
      return g_strdup("F32LE");
      break;

    case mlt_audio_u8:
      if (layout)
        *layout = g_strdup("interleaved");
      return g_strdup("U8");
      break;

    default:
      return NULL;
  }
}

