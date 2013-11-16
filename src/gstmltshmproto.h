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

#ifndef __GST_MLTSHMPROTO_H__
#define __GST_MLTSHMPROTO_H__

#include <gst/gst.h>

G_BEGIN_DECLS

/* #defines don't like whitespacey bits */
#define GST_TYPE_MLTSHMPROTO \
  (gst_mltshmproto_get_type())
#define GST_MLTSHMPROTO(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_MLTSHMPROTO,Gstmltshmproto))
#define GST_MLTSHMPROTO_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_MLTSHMPROTO,GstmltshmprotoClass))
#define GST_IS_MLTSHMPROTO(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_MLTSHMPROTO))
#define GST_IS_MLTSHMPROTO_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_MLTSHMPROTO))

typedef struct _Gstmltshmproto      Gstmltshmproto;
typedef struct _GstmltshmprotoClass GstmltshmprotoClass;

struct _Gstmltshmproto
{
  GstElement element;

  GstPad *sinkpad, *videosrcpad, *audiosrcpad;

  gboolean silent;
  gboolean caps_set;
};

struct _GstmltshmprotoClass
{
  GstElementClass parent_class;
};

GType gst_mltshmproto_get_type (void);

G_END_DECLS

#endif /* __GST_MLTSHMPROTO_H__ */
