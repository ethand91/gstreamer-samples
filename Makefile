# GStreamer samples Makefil
# Author: Ethan
# Version: 1.0

CC=gcc

GST_LIBS=`pkg-config gstreamer-1.0 --libs`
GST_CFLAGS=`pkg-config gstreamer-1.0 --cflags`
GST_BASE_LIBS=`pkg-config gstreamer-base-1.0 --libs`
GST_BASE_CFLAGS=`pkg-config gstreamer-base-1.0 --cflags`

CFLAGS=-ggdb -Iinclude -g

LDFLAGS=

EXE=sender

SRC_DIR=src

all: $(EXE)

$(EXE):
  $(CC) $+ $(CFLAGS) $(GST_CFLAGS) $(GST_LIBS) $(GST_BASE_CFLAGS) -o $@ src/sender.cpp -g $(GST_LIBS) $(GST_BASE_LIBS) -luuid

clean:
  rm -rf $(EXE)

rebuild:
  clean all
