/*
 * File: main.c
 * Creator: Sandesh Paudel, built on top of George Ferguson's code
 */
#include <stdio.h>
#include <stdlib.h>
#include "Circuit.h"


static char* b2s(bool b) {
	return b ? "T" : "F";
}


/** The following is letter (a) from the project. Please refer to the diagram. It is equivalent to (x AND ~y) OR (y AND Z)
**/
static Circuit* Circuit_a() {
    Value* in0 = new_Value(false); //x
    Value* in1 = new_Value(false); //y
    Value* in2 = new_Value(false); //z
    Gate* not1 = new_Inverter(in1);  //~y
    Gate* and0 = new_AndGate(in0, Gate_getOutput(not1));  //x AND ~y
    Gate* and1 = new_AndGate(in1, in2);  // y and z
    Gate* or0 = new_OrGate(Gate_getOutput(and0), Gate_getOutput(and1));  //(x AND ~y) OR (y AND z)


    Value** inputs = new_Value_array(3);
    inputs[0] = in0;
    inputs[1] = in1;
    inputs[2] = in2;
    Value** outputs = new_Value_array(1);
    outputs[0] = Gate_getOutput(or0);

    Gate** gates = new_Gate_array(4);
    gates[0] = not1;
    gates[1] = and0;
    gates[2] = and1;
    gates[3] = or0;
    return new_Circuit(3, inputs, 1, outputs, 4, gates);
}


static void testCircuita(Circuit* circuit, bool in0, bool in1, bool in2) {
    Circuit_setInput(circuit, 0, in0);
    Circuit_setInput(circuit, 1, in1);
    Circuit_setInput(circuit, 2, in2);
    //Circuit_dump(circuit);
    Circuit_update(circuit);
    bool out0 = Circuit_getOutput(circuit, 0);
    printf("%s %s %s -> %s\n", b2s(in0), b2s(in1), b2s(in2), b2s(out0));
}




/** The following is letter (b) from the project. Please refer to the diagram. It is equivalent to ~(~(x AND ~y) OR ~(y AND z))
**/
static Circuit* Circuit_b() {
    Value* in0 = new_Value(false); //x
    Value* in1 = new_Value(false); //y
    Value* in2 = new_Value(false); //z
    Gate* not0 = new_Inverter(in1);  //~y
    Gate* and0 = new_AndGate(in0, Gate_getOutput(not0));  //x AND ~y
    Gate* not1 = new_Inverter(Gate_getOutput(and0)); //~(x AND ~y)
    Gate* and1 = new_AndGate(in1, in2);  // y and z
    Gate* not2 = new_Inverter(Gate_getOutput(and1));  // ~(y and z)
    Gate* or0 = new_OrGate(Gate_getOutput(not1), Gate_getOutput(not2));  //~(x AND ~y) OR ~(y and z)
    Gate* not3 = new_Inverter(Gate_getOutput(or0));  //~(~(x AND ~y) OR ~(y and z))


    Value** inputs = new_Value_array(3);
    inputs[0] = in0;
    inputs[1] = in1;
    inputs[2] = in2;
    Value** outputs = new_Value_array(1);
    outputs[0] = Gate_getOutput(not3);

    Gate** gates = new_Gate_array(7);
    gates[0] = not0;
    gates[1] = and0;
    gates[2] = not1;
    gates[3] = and1;
    gates[4] = not2;
    gates[5] = or0;
    gates[6] = not3;

    return new_Circuit(3, inputs, 1, outputs, 7, gates);
}


static void testCircuitb(Circuit* circuit, bool in0, bool in1, bool in2) {
    Circuit_setInput(circuit, 0, in0);
    Circuit_setInput(circuit, 1, in1);
    Circuit_setInput(circuit, 2, in2);
    //Circuit_dump(circuit);
    Circuit_update(circuit);
    bool out0 = Circuit_getOutput(circuit, 0);
    printf("%s %s %s -> %s\n", b2s(in0), b2s(in1), b2s(in2), b2s(out0));
}



/** The following is letter (c) from the project. Please refer to the diagram. It is equivalent to (x AND y) OR (~x and ~y) = z
**/
static Circuit* Circuit_c() {
    Value* in0 = new_Value(false); //x
    Value* in1 = new_Value(false); //y


    Gate* and0 = new_AndGate(in0, in1);  //x AND y
    Gate* not0 = new_Inverter(in0);  //~x
    Gate* not1 = new_Inverter(in1);  //~y
    Gate* and1 = new_AndGate(Gate_getOutput(not0), Gate_getOutput(not1));  // (~x AND ~y)
    Gate* or0 = new_OrGate(Gate_getOutput(and0), Gate_getOutput(and1));  // (x AND y) or (~x AND ~y)


    Value** inputs = new_Value_array(3);
    inputs[0] = in0;
    inputs[1] = in1;

    Value** outputs = new_Value_array(1);
    outputs[0] = Gate_getOutput(or0); //z

    Gate** gates = new_Gate_array(5);
    gates[0] = and0;
    gates[1] = not0;
    gates[2] = not1;
    gates[3] = and1;
    gates[4] = or0;

    return new_Circuit(2, inputs, 1, outputs, 5, gates);
}


static void testCircuitc(Circuit* circuit, bool in0, bool in1) {
    Circuit_setInput(circuit, 0, in0);
    Circuit_setInput(circuit, 1, in1);
    //Circuit_dump(circuit);
    Circuit_update(circuit);
    bool out0 = Circuit_getOutput(circuit, 0);

    printf("%s %s -> %s \n", b2s(in0), b2s(in1), b2s(out0));
}



//Extra Credit Effort:  One Bit adder
static Circuit* Circuit_ex() {
    //Inputs
    Value* x = new_Value(false); //x
    Value* y = new_Value(false); //y
    Value* c = new_Value(false); //c

    Gate* notx = new_Inverter(x);  //~x
    Gate* noty = new_Inverter(y);  //~y
    Gate* notc = new_Inverter(c);  //c
    Gate* and1 = new_And3Gate(Gate_getOutput(notx), Gate_getOutput(noty), c); //(~x~yc)GATE1
    Gate* and2 = new_And3Gate(Gate_getOutput(notx), y, Gate_getOutput(notc)); //(~xy~c)GATE2
    Gate* and3 = new_And3Gate(Gate_getOutput(notx), y, c);                    //(~xyc)GATE3
    Gate* and4 = new_And3Gate(x, Gate_getOutput(noty), Gate_getOutput(notc)); //(x~y~c)GATE4
    Gate* and5 = new_And3Gate(x, Gate_getOutput(noty), c);                    //(x~yc)GATE5
    Gate* and6 = new_And3Gate(x, y, Gate_getOutput(notc));                    //(xy~c)GATE6
    Gate* and7 = new_And3Gate(x, y, c);                                       //(xyc)GATE7
    //Outputs
    Gate* orz = new_Or4Gate(Gate_getOutput(and1),Gate_getOutput(and2),Gate_getOutput(and4),Gate_getOutput(and7)); // or output z
    Gate* ord = new_Or4Gate(Gate_getOutput(and3),Gate_getOutput(and5),Gate_getOutput(and6),Gate_getOutput(and7)); // or output d



    Value** inputs = new_Value_array(3);
    inputs[0] = x;
    inputs[1] = y;
    inputs[2] = c;

    Value** outputs = new_Value_array(2);
    outputs[0] = Gate_getOutput(orz); //z
    outputs[1] = Gate_getOutput(ord); //d

    Gate** gates = new_Gate_array(12);
    gates[0] = notx;
    gates[1] = noty;
    gates[2] = notc;
    gates[3] = and1;
    gates[4] = and2;
    gates[5] = and3;
    gates[6] = and4;
    gates[7] = and5;
    gates[8] = and6;
    gates[9] = and7;
    gates[10] = orz;
    gates[11] = ord;


    return new_Circuit(3, inputs, 2, outputs, 12, gates);
}





static void testCircuit_ex(Circuit* circuit, bool in0, bool in1, bool in2) {
    Circuit_setInput(circuit, 0, in0);
    Circuit_setInput(circuit, 1, in1);
    Circuit_setInput(circuit, 2, in2);

    //Circuit_dump(circuit);
    Circuit_update(circuit);
    bool out0 = Circuit_getOutput(circuit, 0);
    bool out1 = Circuit_getOutput(circuit, 1);
    printf("%s %s %s -> %s %s\n", b2s(in0), b2s(in1), b2s(in2), b2s(out0), b2s(out1));
}



int main(int argc, char **argv) {

    printf("\nWelcome to circuit simulation! \n\n");
    Circuit* a = Circuit_a();
    printf("Circuit (a) simulation (Case 1):\n");   //expected: true
    testCircuita(a, true, true , true);
    printf("Circuit (a) simulation (Case 2):\n");   //expected: true
    testCircuita(a, false, true , true);
    printf("Circuit (a) simulation (Case 3):\n");   //expected: true
    testCircuita(a, true, false , true);
    printf("Circuit (a) simulation (Case 4):\n");   //expected: false
    testCircuita(a, true, true , false);
    printf("Circuit (a) simulation (Case 5):\n");   //expected: false
    testCircuita(a, false, false , true);
    printf("Circuit (a) simulation (Case 6):\n");   //expected: true
    testCircuita(a, true, false , false);
    printf("Circuit (a) simulation (Case 7):\n");   //expected: false
    testCircuita(a, false, true , false);
    printf("Circuit (a) simulation (Case 8):\n");   //expected: false
    testCircuita(a, false, false , false);


    printf("----------------------------------------------------\n");
    Circuit* b = Circuit_b();

    printf("Circuit (b) simulation (Case 1):\n");   //expected: false
    testCircuitb(b, true, true , true);
    printf("Circuit (b) simulation (Case 2):\n");   //expected: false
    testCircuitb(b, false, true , true);
    printf("Circuit (b) simulation (Case 3):\n");   //expected: false
    testCircuitb(b, true, false , true);
    printf("Circuit (b) simulation (Case 4):\n");   //expected: false
    testCircuitb(b, true, true , false);
    printf("Circuit (b) simulation (Case 5):\n");   //expected: false
    testCircuitb(b, false, false , true);
    printf("Circuit (b) simulation (Case 6):\n");   //expected: false
    testCircuitb(b, true, false , false);
    printf("Circuit (b) simulation (Case 7):\n");   //expected: false
    testCircuitb(b, false, true , false);
    printf("Circuit (b) simulation (Case 8):\n");   //expected: false
    testCircuitb(b, false, false , false);


    printf("----------------------------------------------------\n");

    Circuit* ca = Circuit_c();
    printf("Circuit (c) simulation (Case 1):\n");   //expected: true
    testCircuitc(ca, true, true);
    printf("Circuit (c) simulation (Case 2):\n");   //expected: false
    testCircuitc(ca, true, false);
    printf("Circuit (c) simulation (Case 3):\n");   //expected: false
    testCircuitc(ca, false, true);
    printf("Circuit (c) simulation (Case 4):\n");   //expected: true
    testCircuitc(ca, false, false);


    //----------------------------ONE BIT ADDER FOR EXTRA CREDIT--------

    printf("----------------------------------------------------\n");
    Circuit* one_bit = Circuit_ex();

    printf("Circuit (one-bit adder) simulation (Case 1):\n");   //expected: true, true*
    testCircuit_ex(one_bit, true, true , true);
    printf("Circuit (one-bit adder) simulation (Case 2):\n");   //expected: false, true*
    testCircuit_ex(one_bit, false, true , true);
    printf("Circuit (one-bit adder) simulation (Case 3):\n");   //expected: false, true*
    testCircuit_ex(one_bit, true, false , true);
    printf("Circuit (one-bit adder) simulation (Case 4):\n");   //expected: false, true*
    testCircuit_ex(one_bit, true, true , false);
    printf("Circuit (one-bit adder) simulation (Case 5):\n");   //expected: true, false*
    testCircuit_ex(one_bit, false, false , true);
    printf("Circuit (one-bit adder) simulation (Case 6):\n");   //expected: true, false*
    testCircuit_ex(one_bit, true, false , false);
    printf("Circuit (one-bit adder) simulation (Case 7):\n");   //expected: true, false*
    testCircuit_ex(one_bit, false, true , false);
    printf("Circuit (one-bit adder) simulation (Case 8):\n");   //expected: false, false*
    testCircuit_ex(one_bit, false, false , false);

}
