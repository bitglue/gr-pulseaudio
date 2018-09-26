# gr-pulseaudio

Audio source and sink blocks for GNU Radio using PulseAudio.

Current status: works, but needs review from experienced GNU Radio and
PulseAudio developers.

[GQRX also appears to have PulseAudio sources and sinks](https://github.com/csete/gqrx/tree/master/pulseaudio),
though they do need seem to be structured with the intent of for standalone use.

## Dependencies

* GNU Radio (of course)
* libpulse-dev (if you don't have it yet, run `sudo apt-get install libpulse-dev -y`)

## Installation

* `git clone https://github.com/bitglue/gr-pulseaudio`
* `cd gr-pulseaudio`
* `mkdir build`
* `cd build`
* `cmake ../ -DCMAKE_INSTALL_PREFIX=/usr`
      or
  `cmake ../ -DCMAKE_INSTALL_PREFIX=/usr/local`
* `make`
* `sudo make install`

