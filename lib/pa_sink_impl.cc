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

#include <gr_io_signature.h>
#include "pa_sink_impl.h"

namespace gr {
  namespace pulseaudio {

    pa_sink::sptr
    pa_sink::make(int samp_rate, int nchannels)
    {
      return gnuradio::get_initial_sptr (new pa_sink_impl(samp_rate, nchannels));
    }

    /*
     * The private constructor
     */
    pa_sink_impl::pa_sink_impl(int samp_rate, int nchannels)
      : gr_sync_block("pa_sink",
		      gr_make_io_signature(nchannels, nchannels, sizeof (float)),
		      gr_make_io_signature(0, 0, 0))
    {
      this->nchannels = nchannels;
      this->buffer_size = samp_rate / 4;        // enough working space for 250ms of audio
      this->audio_buffer = new float[buffer_size*nchannels];

      this->sample_spec.format = PA_SAMPLE_FLOAT32;
      this->sample_spec.channels = nchannels;
      this->sample_spec.rate = samp_rate;

      pa_connection = pa_simple_new(
          NULL,                 // server
          "GNU Radio",          // application name
          PA_STREAM_PLAYBACK,   // direction
          NULL,                 // device
          "Awesome",            // description
          &sample_spec,         // sample format
          NULL,                 // channel map
          NULL,                 // buffering
          NULL);                // error code
    }

    /*
     * Our virtual destructor.
     */
    pa_sink_impl::~pa_sink_impl()
    {
      pa_simple_free(this->pa_connection);
      delete[] this->audio_buffer;
    }

    int
    pa_sink_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
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

