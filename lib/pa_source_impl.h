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

#ifndef INCLUDED_PULSEAUDIO_PA_SOURCE_IMPL_H
#define INCLUDED_PULSEAUDIO_PA_SOURCE_IMPL_H

#include <pulseaudio/pa_source.h>

#include <pulse/simple.h>

namespace gr {
  namespace pulseaudio {

    class pa_source_impl : public pa_source
    {
    private:
      pa_sample_spec sample_spec;
      pa_simple *pa_connection;
      int nchannels;
      int buffer_size;
      float *audio_buffer;

    public:
      pa_source_impl(int samp_rate, int nchannels);
      ~pa_source_impl();

      // Where all the action really happens
      int work(int noutput_items,
	       gr_vector_const_void_star &input_items,
	       gr_vector_void_star &output_items);
    };

  } // namespace pulseaudio
} // namespace gr

#endif /* INCLUDED_PULSEAUDIO_PA_SOURCE_IMPL_H */

