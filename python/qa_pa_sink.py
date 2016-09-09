#!/usr/bin/env python
# 
# Copyright 2013 Phil Frost.
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

from gnuradio import gr, gr_unittest, analog
import pulseaudio_swig as pulseaudio

class qa_pa_sink (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        left_src = analog.sig_source_f(96000, analog.GR_COS_WAVE, 1000, .2, 0)
        right_src = analog.sig_source_f(96000, analog.GR_COS_WAVE, 440, .2, 0)
        sink = pulseaudio.pa_sink(
            samp_rate=96000,
            nchannels=2,
            application_name='GNU Radio test',
            device=None,
            stream_name='test',
            channel_map=None)
        self.tb.connect((left_src, 0), (sink, 0))
        self.tb.connect((right_src, 0), (sink, 1))
        self.tb.run ()


if __name__ == '__main__':
    gr_unittest.run(qa_pa_sink, "qa_pa_sink.xml")
