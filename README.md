# Andy's Workshop Brewery Process Controllers

This repo is a collection of firmware and applications that go together with the hardware projects comprising my brewery process controllers.

Full writeups to all the projects can be found at [my website](http://andybrown.me.uk)

## Quick installation

Don't want to build from source? That's OK, compiled firmware and executables are available in the `bin` directory right here.

The firmware files are `.hex` files ready to be flashed to the MCU on the matching controller board. You may also need to set MCU fuses so that the correct speed and clock options are set. See the appropriate writeup on the website for details.

All binary files are for 64-bit Linux. I compiled them on Ubuntu.

## Build from source

Wherever there's something to be built you'll find an accompanying `SConstruct` file. You need to be running Linux. You should have installed the `scons` package. You also need a recent `avr-gcc`. I use version 4.9.2 but I'm sure the more recent editions also work fine.

There's a `build-all.sh` script in the top-level directory here that you can run to build everything and install the results in the `bin` directory.

## Setting MCU fuses

The firmware `SConscript` files have a `fuse` target that will program the fuses on your MCU to the correct values if you have a "usbasp" programmer attached and `avrdude` installed.

## Project writeups on the website

Each of the projects has its own writeup on my website. This section contains links to each of them.

### Introduction to the whole concept

This small article introduces you to the whole concept of process control and why I'm doing it.

[Click here](http://andybrown.me.uk/2017/04/30/intro/)

### Relays and triacs

This article breaks down the relays and triacs project. Schematics, gerbers etc. are all available for download. Click the image to view it.

[![Click to view article](http://andybrown.me.uk/wp-content/images/brewery/system/relays/built.jpg)](http://andybrown.me.uk/2017/05/21/switchgear)
