
#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

int main(int argc, char **argv, char **env) {
    int i;
    int clk;
    int paused_cycles = 0;
    bool paused_at_nine = false;

    Verilated::commandArgs(argc, argv);

    // init top verilog instance
    Vcounter* top = new Vcounter;

    // init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("counter.vcd");

    // initialize simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->en = 0;

    // run simulation for many clock cycles
    for (i = 0; i < 300; i++) {
        top->rst = (i < 2);
        top->en = (i > 4);

        if (top->rst) {
            paused_cycles = 0;
            paused_at_nine = false;
        } else {
            // Pause once per visit to 0x9, for three rising clock edges.
            if (top->count != 0x9)
                paused_at_nine = false;
            if (top->count == 0x9 && !paused_at_nine) {
                paused_cycles = 3;
                paused_at_nine = true;
            }
            if (paused_cycles > 0) {
                top->en = 0;
                --paused_cycles;
            }
        }

        // Update and evaluate signals before recording this timestamp.
        for (clk = 0; clk < 2; clk++) {
            top->clk = !top->clk;
            top->eval();
            tfp->dump(2*i+clk);  // unit is in ps!!!
        }

//I put dump after eval() because I want to record the value of the signals after they have been 
//evaluated for this time step. If I put dump before eval(), 
//it would record the values of the signals before they have been updated, which is not what I want.   
//By putting dump after eval(), I ensure that I am capturing the correct state of the simulation at each 
//time step.


        if (Verilated::gotFinish()) exit(0);
    }

    tfp->close();
    exit(0);
}
