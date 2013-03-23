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
    pa_source::make()
    {
      return gnuradio::get_initial_sptr (new pa_source_impl());
    }

    /*
     * The private constructor
     */
    pa_source_impl::pa_source_impl()
      : gr_sync_block("pa_source",
		      gr_make_io_signature(0, 0, 0),
		      gr_make_io_signature(2, 2, sizeof (float)))
    {}

    /*
     * Our virtual destructor.
     */
    pa_source_impl::~pa_source_impl()
    {
    }

    int
    pa_source_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const float *in = (const float *) input_items[0];
        float *out = (float *) output_items[0];

        // Do <+signal processing+>

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace pulseaudio */
} /* namespace gr */

