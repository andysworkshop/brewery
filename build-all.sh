#!/usr/bin/env bash

(cd bridging/sendcommand && scons install)
(cd firmware/switching && scons mains=50 install)
