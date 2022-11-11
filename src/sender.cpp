#include <gst/gst.h>
#include <glib-unix.h>

#define HOST "127.0.0.1"

gboolean signal_handler (gpointer user_data)
{
  GMainLoop *loop = (GMainLoop *) user_data;

  g_print("closing the main loop");
  g_main_loop_quit(loop);

  return TRUE;
}

int main (gint argc, gchar *argv[])
{
  GstElement *pipeline;
  GstElement *src;
  GstElement *videoconvert;
  GstElement *videoscale;
  GstElement *encoder;
  GstElement *capsfilter;
  GstElement *muxer;
  GstElement *sink;
  GstCaps *caps;
  GMainLoop *loop;
  gint ret = -1;

  gst_init(NULL, NULL);

  pipeline = gst_pipeline_new("pipeline");

  src = gst_element_factory_make("autovideosrc", "autovideosrc");

  if (!src)
  {
    g_printerr("Failed to create src\n");

    goto no_src;
  }

  videoconvert = gst_element_factory_make("videoconvert", "videoconvert");

  if (!videoconvert)
  {
    g_printerr("Failed to create videoconvert\n");

    goto no_videoconvert;
  }

  videoscale = gst_element_factory_make("videoscale", "videoscale");

  if (!videoscale)
  {
    g_printerr("Failed to create videoscale\n");

    goto no_videoscale;
  }

  capsfilter = gst_element_factory_make("capsfilter", "capsfilter");

  if (!capsfilter)
  {
    g_printerr("Failed to create capsfilter\n");

    goto no_caps;
  }

  caps = gst_caps_from_string("video/x-raw,width=640,height=480");
  g_object_set(capsfilter, "caps", caps, NULL);
  gst_caps_unref(caps);

  encoder = gst_element_factory_make("theoraenc", "theoraenc");

  if (!encoder)
  {
    g_printerr("Failed to create encoder\n");

    goto no_encoder;
  }

  muxer = gst_element_factory_make("oggmux", "oggmux");

  if (!muxer)
  {
    g_printerr("Failed to create muxer\n");

    goto no_muxer;
  }

  sink = gst_element_factory_make("tcpserversink", "tcpserversink");

  if (!sink)
  {
    g_printerr("Failed to create sink\n");

    goto no_sink;
  }

  g_object_set(sink, "host", HOST, NULL);

  gst_bin_add_many(GST_BIN(pipeline), src, videoconvert, videoscale, capsfilter, encoder, muxer, sink, NULL);
  gst_element_link_many(src, videoconvert, videoscale, capsfilter, encoder, muxer, sink, NULL);

  gst_element_set_state(pipeline, GST_STATE_PLAYING);
  g_print("Pipeline playing\n");

  loop = g_main_loop_new(NULL, TRUE);
  g_unix_signal_add(SIGINT, signal_handler, loop);
  g_main_loop_run(loop);
  g_main_loop_unref(loop);

  gst_element_set_state(pipeline, GST_STATE_NULL);
  g_print("Closed succesfully\n");

  ret = 0;
  goto no_src;

no_sink:
  gst_object_unref(sink);

no_muxer:
  gst_object_unref(muxer);

no_encoder:
  gst_object_unref(encoder);

no_caps:
  gst_object_unref(capsfilter);

no_videoscale:
  gst_object_unref(videoscale);

no_videoconvert:
  gst_object_unref(videoconvert);

no_src:
  gst_object_unref(pipeline);
  gst_deinit();

  return ret;
}
