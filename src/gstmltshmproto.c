/*
 * GstMltSHMProto
 * A GStreamer element to demux audio and video from the gstshm MLT consumer.
 *
 * Copyright © <2013> Instituto Nacional de Asociativismo y Economia Social.
 * Copyright © <2013> Cooperativa de Trabajo OpCode Limitada <info@opcode.coop>.
 * Copyright © <2013> Adrián Pardini     <adrian@opcode.coop>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * SECTION:element-mltshmdemux
 *
 * Demuxer for the shm protocol used by the gstshm MLT consumer.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch-1.0 shmsrc socket-path=/dev/shm/mlt.shm ! mltshmdemux name=demux  ! video/x-raw ! queue2 ! autovideosink  demux. ! audio/x-raw ! queue2 ! autoaudiosink
 * ]|
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gst/gst.h>
#include <glib.h>
#include <glib/gprintf.h>

#include "mltcommon.h"
#include "gstmltshmproto.h"

GST_DEBUG_CATEGORY_STATIC (gst_mltshmproto_debug);
#define GST_CAT_DEFAULT gst_mltshmproto_debug

/* Filter signals and args */
enum
{
  /* FILL ME */
  LAST_SIGNAL
};

enum
{
  PROP_0,
  PROP_SILENT
};

/* the capabilities of the inputs and outputs.
 *
 * describe the real formats here.
 */
static GstStaticPadTemplate sink_factory = GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("ANY")
    );

static GstStaticPadTemplate audiosrc_factory = GST_STATIC_PAD_TEMPLATE ("audiosrc",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("audio/x-raw")
    );

static GstStaticPadTemplate videosrc_factory = GST_STATIC_PAD_TEMPLATE ("videosrc",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("video/x-raw")
    );

#define gst_mltshmproto_parent_class parent_class
G_DEFINE_TYPE (Gstmltshmproto, gst_mltshmproto, GST_TYPE_ELEMENT);

static void gst_mltshmproto_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec);
static void gst_mltshmproto_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec);

static GstFlowReturn gst_mltshmproto_chain (GstPad * pad, GstObject * parent, GstBuffer * buf);

/* GObject vmethod implementations */

/* initialize the mltshmproto's class */
static void
gst_mltshmproto_class_init (GstmltshmprotoClass * klass)
{
  GObjectClass *gobject_class;
  GstElementClass *gstelement_class;

  gobject_class = (GObjectClass *) klass;
  gstelement_class = (GstElementClass *) klass;

  gobject_class->set_property = gst_mltshmproto_set_property;
  gobject_class->get_property = gst_mltshmproto_get_property;

  g_object_class_install_property (gobject_class, PROP_SILENT,
      g_param_spec_boolean ("silent", "Silent", "Produce verbose output ?",
          TRUE, G_PARAM_READWRITE));

  gst_element_class_set_static_metadata(gstelement_class,
    "MLT Framework shm protocol demuxer",
    "Demuxer",
    "Reads buffers from the gstshm MLT consumer and splits them in audio and video streams",
    "Adrián Pardini <adrian@opcode.coop>, "
    "Cooperativa de Trabajo OpCode Limitada <info@opcode.coop>"
    );

  gst_element_class_add_pad_template (gstelement_class,
      gst_static_pad_template_get (&audiosrc_factory));
  gst_element_class_add_pad_template (gstelement_class,
      gst_static_pad_template_get (&videosrc_factory));
  gst_element_class_add_pad_template (gstelement_class,
      gst_static_pad_template_get (&sink_factory));
}

/* initialize the new element
 * instantiate pads and add them to element
 * set pad calback functions
 * initialize instance structure
 */
static void
gst_mltshmproto_init (Gstmltshmproto * self)
{
  self->sinkpad = gst_pad_new_from_static_template (&sink_factory, "sink");

  gst_pad_set_chain_function (self->sinkpad,
                              GST_DEBUG_FUNCPTR(gst_mltshmproto_chain));
  GST_PAD_SET_PROXY_CAPS (self->sinkpad);
  gst_element_add_pad (GST_ELEMENT (self), self->sinkpad);

  self->audiosrcpad = gst_pad_new_from_static_template (&audiosrc_factory, "audiosrc");
  GST_PAD_SET_PROXY_CAPS (self->audiosrcpad);
  gst_element_add_pad (GST_ELEMENT (self), self->audiosrcpad);

  self->videosrcpad = gst_pad_new_from_static_template (&videosrc_factory, "videosrc");
  GST_PAD_SET_PROXY_CAPS (self->videosrcpad);
  gst_element_add_pad (GST_ELEMENT (self), self->videosrcpad);

  self->silent = TRUE;
  self->caps_set = FALSE;
}

static void
gst_mltshmproto_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
  Gstmltshmproto *self = GST_MLTSHMPROTO (object);

  switch (prop_id) {
    case PROP_SILENT:
      self->silent = g_value_get_boolean (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
gst_mltshmproto_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec)
{
  Gstmltshmproto *self = GST_MLTSHMPROTO (object);

  switch (prop_id) {
    case PROP_SILENT:
      g_value_set_boolean (value, self->silent);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

/* GstElement vmethod implementations */

/* chain function
 * this function does the actual processing
 */
static GstFlowReturn
gst_mltshmproto_chain (GstPad * pad, GstObject * parent, GstBuffer * buf)
{
  Gstmltshmproto *self;

  self = GST_MLTSHMPROTO (parent);

  GstMemory *mem = gst_buffer_get_all_memory(buf);
  GstMapInfo minfo;

  if (gst_memory_map(mem, &minfo, GST_MAP_READ)) {
    struct posix_shm_header *header = (struct posix_shm_header *)minfo.data;
    if (!self->silent) {
      g_printf("Got buffer of size %i, width: %i height: %i format: %i frame is %i\n", minfo.size, header->width, header->height, header->image_format, header->frame);
    }

    if (!self->caps_set) {
      self->caps_set = TRUE;
      GstEvent *event = NULL;
      char     *layout = NULL;
      char     *fmt = mlt_image_format_to_gst(header->image_format);
      GstCaps *caps = gst_caps_new_simple("video/x-raw",
                                          "width",     G_TYPE_INT,        header->width,
                                          "height",    G_TYPE_INT,        header->height,
                                          "format",    G_TYPE_STRING,     fmt,
                                          "framerate", GST_TYPE_FRACTION, header->frame_rate_num, header->frame_rate_den,
                                          NULL);
      gst_pad_set_caps(self->videosrcpad, caps);
      event = gst_event_new_caps(caps);
      gst_pad_push_event(self->videosrcpad, event);
      gst_event_unref(event);
      gst_caps_unref(caps);
      g_free(fmt);

      fmt = mlt_audio_format_to_gst(header->audio_format, &layout);
      caps = gst_caps_new_simple("audio/x-raw",
                                          "rate",     G_TYPE_INT,    header->frequency,
                                          "channels", G_TYPE_INT,    header->channels,
                                          "format",   G_TYPE_STRING, fmt,
                                          "layout",   G_TYPE_STRING, layout,
                                          NULL);
      gst_pad_set_caps(self->audiosrcpad, caps);
      event = gst_event_new_caps(caps);
      gst_pad_push_event(self->audiosrcpad, event);
      gst_event_unref(event);
      gst_caps_unref(caps);
      g_free(fmt);
      g_free(layout);
    }

    GstBuffer *newbuf = gst_buffer_new();
    if (newbuf) {
      GstMemory *vmem = gst_memory_copy(mem, sizeof(struct posix_shm_header), header->image_size);
      gst_buffer_replace_all_memory(newbuf, vmem);

      gst_pad_push (self->videosrcpad, newbuf);
    }

    newbuf = gst_buffer_new();
    if (newbuf) {
      GstMemory *amem = gst_memory_copy(mem, sizeof(struct posix_shm_header)+header->image_size, header->audio_size);
      gst_buffer_replace_all_memory(newbuf, amem);

      gst_pad_push (self->audiosrcpad, newbuf);
    }
    gst_memory_unmap(mem, &minfo);

  }
  gst_memory_unref(mem);
  gst_buffer_unref(buf);
  return GST_FLOW_OK;
}


/* entry point to initialize the plug-in
 * initialize the plug-in itself
 * register the element factories and other features
 */
static gboolean
mltshmproto_init (GstPlugin * mltshmproto)
{
  GST_DEBUG_CATEGORY_INIT (gst_mltshmproto_debug, "mltshmproto",
      0, "MLT");

  return gst_element_register (mltshmproto, "mltshmdemux", GST_RANK_NONE,
      GST_TYPE_MLTSHMPROTO);
}

/* PACKAGE: this is usually set by autotools depending on some _INIT macro
 * in configure.ac and then written into and defined in config.h, but we can
 * just set it ourselves here in case someone doesn't use autotools to
 * compile this code. GST_PLUGIN_DEFINE needs PACKAGE to be defined.
 */
#ifndef PACKAGE
#define PACKAGE "gstmltshmproto"
#endif

GST_PLUGIN_DEFINE (
    GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    mltshmproto,
    "Elements to handle the protocol used by the gstshm MLT producer and consumer.",
    mltshmproto_init,
    VERSION,
    "LGPL",
    "gstmltshmproto",
    "http://opcode.coop"
)
