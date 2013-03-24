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
#include "pa_source_impl.h"

namespace gr {
  namespace pulseaudio {

    pa_source::sptr
    pa_source::make(int samp_rate, int nchannels)
    {
      return gnuradio::get_initial_sptr (new pa_source_impl(samp_rate, nchannels));
    }

    /*
     * The private constructor
     */
    pa_source_impl::pa_source_impl(int samp_rate, int nchannels)
      : gr_sync_block("pa_source",
		      gr_make_io_signature(0, 0, 0),
		      gr_make_io_signature(nchannels, nchannels, sizeof (float)))
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
          PA_STREAM_RECORD,     // direction
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
    pa_source_impl::~pa_source_impl()
    {
      pa_simple_free(this->pa_connection);
      delete[] this->audio_buffer;
    }

    int
    pa_source_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
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

  } /* namespace pulseaudio */
} /* namespace gr */

