/* -*- c++ -*- */
/*
 * Copyright 2013 Phil Frost.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <iostream>

#include <gnuradio/io_signature.h>
#include "pa_simple_connection.h"

#include <pulse/channelmap.h>
#include <pulse/error.h>

namespace gr {
  namespace pulseaudio {
    pa_simple_connection::pa_simple_connection(
        int samp_rate,
        int nchannels,
        const char *application_name,
        pa_stream_direction_t direction,
        const char *device,
        const char *stream_name,
        const char *channel_map,
        float latency)
    {
      int error;

      this->nchannels = nchannels;
      this->buffer_size = samp_rate / 4;        // enough working space for 250ms of audio
      this->audio_buffer = new float[buffer_size*nchannels];

      this->sample_spec.format = PA_SAMPLE_FLOAT32;
      this->sample_spec.channels = nchannels;
      this->sample_spec.rate = samp_rate;

      pa_channel_map *use_channel_map = NULL;
      if (channel_map != NULL)
      {
        pa_channel_map_parse(&this->channel_map, channel_map);
        use_channel_map = &this->channel_map;
      }

      buffer_attr.maxlength = -1;
      buffer_attr.tlength = -1;
      buffer_attr.prebuf = -1;
      buffer_attr.minreq = -1;
      buffer_attr.fragsize = -1;
      if (latency) {
        size_t bytes = pa_usec_to_bytes((pa_usec_t)(latency*1000), &sample_spec);
        if (direction = PA_STREAM_PLAYBACK) {
          buffer_attr.tlength = bytes;
        } else {
          buffer_attr.fragsize = bytes;
        }
      }

      pa_connection = pa_simple_new(
          NULL,                 // server
          application_name,     // application name
          direction,            // direction
          device,               // device
          stream_name,          // stream name
          &this->sample_spec,   // sample format
          use_channel_map,      // channel map
          &buffer_attr,         // buffering
          &error);              // error code

      if (pa_connection == NULL) {
        std::cerr << pa_strerror(error);
      }
    }

    pa_simple_connection::~pa_simple_connection()
    {
      pa_simple_free(this->pa_connection);
      delete[] this->audio_buffer;
    }

    int pa_simple_connection::read(gr_vector_void_star &output_items, size_t noutput_items)
    {
      int channel;
      float *float_outputs[this->nchannels];

      for (channel = 0; channel < this->nchannels; channel += 1)
      {
        float_outputs[channel] = (float *) output_items[channel];
      }

      if (noutput_items > this->buffer_size)
      {
        noutput_items = this->buffer_size;
      }

      pa_simple_read(
          this->pa_connection,
          this->audio_buffer,
          this->nchannels*noutput_items*sizeof(float),
          NULL);

      float *buffer_pos = this->audio_buffer;
      for (int i = noutput_items; i; i -= 1)
      {
        for (channel = 0; channel < this->nchannels; channel += 1)
        {
          *(float_outputs[channel])++ = *buffer_pos++;
        }
      }

      return noutput_items;
    }

    int pa_simple_connection::write(gr_vector_const_void_star &input_items, size_t noutput_items)
    {
      int channel;
      const float *float_inputs[this->nchannels];

      for (channel = 0; channel < this->nchannels; channel += 1)
      {
        float_inputs[channel] = (const float *) input_items[channel];
      }

      if (noutput_items > this->buffer_size)
      {
        noutput_items = this->buffer_size;
      }

      float *buffer_pos = this->audio_buffer;
      for (int i = noutput_items; i; i -= 1)
      {
        for (channel = 0; channel < this->nchannels; channel += 1)
        {
          *buffer_pos++ = *(float_inputs[channel])++;
        }
      }

      pa_simple_write(
          this->pa_connection,
          this->audio_buffer,
          this->nchannels*noutput_items*sizeof(float),
          NULL);

      return noutput_items;
    }

  } /* namespace pulseaudio */
} /* namespace gr */
