#!/bin/sh
set -eu

# Run from this script's directory so source files and vbuddy.cfg are found.
cd "$(dirname "$0")"

# Translate the counter and its C++ testbench, including waveform support.
verilator -Wall --cc --trace counter.sv --exe counter_tb.cpp

# Build the simulation executable. vbuddy.cpp is included by counter_tb.cpp.
make -j -C obj_dir -f Vcounter.mk Vcounter

# Run the simulation with the connected Vbuddy device.
./obj_dir/Vcounter
