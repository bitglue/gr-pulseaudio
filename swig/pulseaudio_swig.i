/* -*- c++ -*- */

#define PULSEAUDIO_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "pulseaudio_swig_doc.i"

%{
#include "gr-pulseaudio/pa_sink.h"
#include "gr-pulseaudio/pa_source.h"
%}


%include "gr-pulseaudio/pa_sink.h"
GR_SWIG_BLOCK_MAGIC2(pulseaudio, pa_sink);
%include "gr-pulseaudio/pa_source.h"
GR_SWIG_BLOCK_MAGIC2(pulseaudio, pa_source);
