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

#ifndef INCLUDED_PULSEAUDIO_PA_SIMPLE_CONNECTION_H
#define INCLUDED_PULSEAUDIO_PA_SIMPLE_CONNECTION_H

#include <pulse/simple.h>
#include <gr_io_signature.h>

namespace gr {
  namespace pulseaudio {
    class pa_simple_connection {
      private:
        pa_sample_spec sample_spec;
        pa_channel_map channel_map;
        pa_simple *pa_connection;
        int nchannels;
        int buffer_size;
        float *audio_buffer;

      public:
        pa_simple_connection(
            int samp_rate,
            int nchannels,
            const char *application_name,
            pa_stream_direction_t direction,
            const char *stream_name,
            const char *channel_map);
        ~pa_simple_connection();

        int read(gr_vector_void_star &output_items, size_t noutput_items);
        int write(gr_vector_const_void_star &input_items, size_t noutput_items);
    };
  } // namespace pulseaudio
} // namespace gr

#endif
