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

#include <gnuradio/io_signature.h>
#include "pa_sink_impl.h"

namespace gr {
  namespace pulseaudio {

    pa_sink::sptr
    pa_sink::make(
          int samp_rate,
          int nchannels,
          const char *application_name,
          const char *device,
          const char *stream_name,
          const char *channel_map,
          float latency)
    {
      return gnuradio::get_initial_sptr (new pa_sink_impl(
            samp_rate,
            nchannels,
            application_name,
            device,
            stream_name,
            channel_map,
            latency));
    }

    pa_sink_impl::pa_sink_impl(
          int samp_rate,
          int nchannels,
          const char *application_name,
          const char *device,
          const char *stream_name,
          const char *channel_map,
          float latency)
      : gr::sync_block("pa_sink",
		      gr::io_signature::make(nchannels, nchannels, sizeof (float)),
		      gr::io_signature::make(0, 0, 0)),
      pa_connection(samp_rate, nchannels, application_name, PA_STREAM_PLAYBACK, device, stream_name, channel_map, latency)
    {
      /* empty */
    }

    pa_sink_impl::~pa_sink_impl()
    {
      /* empty */
    }

    int
    pa_sink_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        return this->pa_connection.write(input_items, noutput_items);
    }

  } /* namespace pulseaudio */
} /* namespace gr */

