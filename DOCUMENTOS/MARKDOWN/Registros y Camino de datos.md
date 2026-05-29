
## Slide 1

Chapter 7 - Part 2    1
Register Transfer Operations
Register Transfer Operations  – The movement and processing of data stored in registers
Three basic components:
  - set of registers
  - operations
  - control of operations
Elementary Operations -- load, count, shift, add, bitwise "OR", etc.
  - Elementary operations called microoperations


## Slide 2

Chapter 7 - Part 2    2
Register Notation
Letters and numbers  – denotes a register (ex.  R2, PC, IR)
Parentheses ( ) – denotes a range of register bits (ex. R1(1), PC(7:0), PC(L))
Arrow () – denotes data transfer (ex. R1  R2, PC(L)  R0)
Comma – separates parallel operations
Brackets [ ] – Specifies a memory address (ex. R0   M[AR],   R3  M[PC]  )


## Slide 3

Chapter 7 - Part 2    3
Conditional Transfer
If (K1 =1) then (R2  R1) is shortened to
K1: (R2  R1)
where K1 is a control variable specifying a conditional executionof the microoperation.


## Slide 4

Chapter 7 - Part 2    4
Microoperations
Logical Groupings:
  - Transfer - move data from one register to another
  - Arithmetic - perform arithmetic on data in registers
  - Logic - manipulate data or use bitwise logical operations
  - Shift - shift data in registers
Arithmetic operations+ Addition– Subtraction*  Multiplication/  Division
Logical operations         Logical OR          Logical AND        Logical Exclusive OR          Not


## Slide 5

Chapter 7 - Part 2    5
Example Microoperations
Add the content of R1 to the content of R2 and place the result in R1.
R1 R1 + R2
Multiply the content of R1 by the content of R6 and place the result in PC.
PC  R1 * R6
Exclusive OR the content of R1 with the content of R2 and place the result in R1.
R1  R1  R2


## Slide 6

Chapter 7 - Part 2    6
Example Microoperations (Continued)
Take the 1's Complement of the contents of R2 and place it in the PC.
PC  R2
On condition K1 OR K2,  the content of R1 is Logic bitwise Ored with the content of R3 and the result placed in R1.
(K1 + K2):  R1  R1  R3
NOTE:  "+" (as in K1 + K2) and means “OR.” In R1  R1 + R3, + means “plus.”


## Slide 7

Chapter 7 - Part 2    7
Control Expressions
The control expression for an operation appears to the left of the operation  and is separated from it by a colon
Control expressions specify the logical condition for the operation to occur
Control expression values of:
  - Logic "1" -- the operation occurs.
  - Logic "0" -- the operation is does not occur.
Example:X K1 :  R1  R1 + R2X K1 :  R1  R1 + R2 + 1
Variable K1 enables the add or subtract operation.
If X =0, then X =1 so                X K1 = 1, activating the addition of R1 and R2.
If X = 1, then X K1 = 1, activating the addition of R1 and the two's complement of R2 (subtract).


## Slide 8

Chapter 7 - Part 2    8
Arithmetic Microoperations
FromTable7-3:
Note that any register may be specified for source 1, source 2, or destination.
These simple microoperations operate on the whole word
Symbolic Designation
Description
R0
¬
R1 + R2
Addition
R0
¬
R1
Ones Complement
R0
¬
R1 + 1
Two's Complement
R0
¬
R2 +
R1 + 1
R2 minus R1 (2's Comp)
R1
¬
R1 + 1
Increment (count up)
R1
¬
R1
–
1
Decrement (count down)


## Slide 9

Chapter 7 - Part 2    9
Logical Microoperations
From Table 7-4:
Symbolic
Designation
Description
R0
¬
R1
Bitwise NOT
R0
¬
R1
Ú
R2
Bitwise OR (sets bits)
R0
¬
R1
Ù
R2
Bitwise AND (clears bits)
R0
¬
R1
Å
R2
Bitwise EXOR (complements bits)


## Slide 10

Chapter 7 - Part 2    10
Logical Microoperations (continued)
Let  R1  = 10101010, and  R2  = 11110000
Then after the operation, R0 becomes:


## Slide 11

Chapter 7 - Part 2    11
Shift Microoperations
From Table 7-5:
Let R2  = 11001001
Then after the operation, R1 becomes:
Note:  These shifts "zero fill".   Sometimes a separate flip-flop is used to provide the data shifted in, or to “catch” the data shifted out.
Other shifts are possible (rotates, arithmetic) (see Chapter 10).


## Slide 12

Chapter 7 - Part 2    12
Register Transfer Structures
Multiplexer-Based Transfers - Multiple inputs are selected by a multiplexer dedicated to the register
Bus-Based Transfers - Multiple inputs are selected by a shared multiplexer driving a bus that feeds inputs to multiple registers
Three-State Bus  - Multiple inputs are selected by3-state drivers with outputs connected to a bus that feeds multiple registers
Other Transfer Structures -  Use multiple multiplexers, multiple buses, and combinations of all the above


## Slide 13

Chapter 7 - Part 2    13
Multiplexer-Based Transfers
Multiplexers connected to register inputs produce flexible transfer structures (Note: Clocks are omitted for clarity)
The transfers are:       K1: R0  R1                                      K2×K1: R0  R2


## Slide 14

Chapter 7 - Part 2    14
Multiplexer and Bus-Based Transfers for Multiple Registers
Multiplexer dedicated to each register
Shared transfer paths for registers
  - A shared transfer object is a called a bus (Plural: buses)
Bus implementation using:
  - multiplexers
  - three-state nodes and drivers
In most cases, the number of bits is the length of the receiving register


## Slide 15

Chapter 7 - Part 2    15
Dedicated MUX-Based Transfers
Multiplexer connected to each register input produces a very flexible transfer structure =>
Characterize the simultaneous transfers possible with this structure.


## Slide 16

Chapter 7 - Part 2    16
Multiplexer Bus
A single bus driven by a multiplexer lowers cost, but limits the available transfers =>
Characterize the simultaneous transfers possible with this structure.
Characterize the cost savings compared to dedicated multiplexers
L0


## Slide 17

Chapter 7 - Part 2    17
Three-State Bus
The 3-input MUX can be replaced by a 3-state node (bus) and 3-state buffers.
Cost is further reduced, but transfers are limited
Characterize the simultaneous transfers possible with this structure.
Characterize the cost savings and compare
Other advantages?


## Slide 18

Chapter 7 - Part 2    18
Datapaths
Guiding principles for basic datapaths:
  - The set of registers
    - Collection of individual registers
    - A set of registers with common access resources called a register file
    - A combination of the above
  - Microoperation implementation
    - One or more shared resources for implementing microoperations
    - Buses - shared transfer paths
    - Arithmetic-Logic Unit (ALU) -  shared resource for implementing arithmetic and logic microoperations
    - Shifter -  shared resource for implementing shift microoperations


## Slide 19

Chapter 7 - Part 2    19
Four parallel-loadregisters
Two mux-based register selectors
Register destination decoder
Mux  B for external constant input
Buses A and B with externaladdress and data outputs
ALU and Shifter withMux F for output select
Mux D for external data input
Logic for generating status bitsV, C, N, Z
Datapath Example


## Slide 20

Chapter 7 - Part 2    20
Microoperation: R0 ← R1 + R2
Datapath Example: Performing a Microoperation


## Slide 21

Chapter 7 - Part 2    21
Datapath Example: Key Control Actions for Microoperation Alternatives
Perform a shift microoperation – apply 1 to MF select
Use a constant in a micro- operation using Bus B – apply 1 to MB select
Provide an address and data for a memory or output  write microoperation – apply 0 to Load enable to prevent register loading
Provide an address and obtain data for a memory or output read microoperation – apply 1 to MD select
For some of the above, other control signals become don't cares


## Slide 22

Chapter 7 - Part 2    22
Arithmetic Logic Unit (ALU)
In this and the next section, we deal with detailed design of  typical ALUs and shifters
Decompose the ALU into:
  - An arithmetic circuit
  - A logic circuit
  - A selector to pick between the two circuits
Arithmetic circuit design
  - Decompose the arithmetic circuit into:
    - An n-bit parallel adder
    - A block of logic that selects four choices for the B input to the adder
    - See next slide for diagram


## Slide 23

Chapter 7 - Part 2    23
There are only four functions of B to select as Y in G = A + Y:
  - 0
  - B
  - B
  - 1
What functions are implemented with carry-in to the adder = 0?  =1?
Arithmetic Circuit Design (continued)
Cin = 0
Cin = 1
G = A
G = A + 1
G = A – 1
G = A + B
G = A
G = A + B + 1


## Slide 24

Chapter 7 - Part 2    24
Arithmetic Circuit Design (continued)
Adding selection codes to the functions of B:
The useful arithmetic functions are labeled in the table
Note that all four functions of B produce at least one useful function
+
+
+
+
+
+


## Slide 25

Chapter 7 - Part 2    25
Logic Circuit
The text gives a circuit implemented using a multiplexer plus gates implementing: AND, OR, XOR and NOT
Here we custom design a circuit for bit Gi by beginning with a truth table organized as a K-map and assigning (S1, S0) codes to AND, OR, etc.
Gi = S0 Ai Bi  + S1 Ai Bi       +    S0 Ai Bi  + S1 S0 Ai
Gate input count forMUX solution > 29
Gate input count forabove circuit < 20
Custom design better
| S1S0 | AND | OR | XOR | NOT |
| --- | --- | --- | --- | --- |
| AiBi | 0 0 | 0 1 | 1 1 | 1 0 |
| 0 0 | 0 | 0 | 0 | 1 |
| 0 1 | 0 | 1 | 1 | 1 |
| 1 1 | 1 | 1 | 0 | 0 |
| 1 0 | 0 | 1 | 1 | 0 |


## Slide 26

Chapter 7 - Part 2    26
Arithmetic Logic Unit (ALU) (continued)
The next most significant select signals, S0 for the arithmetic circuit and S1 for the logic circuit, are wired together, completing the two select signals for the logic circuit.
The remaining S2 completes the three select signals for the arithmetic circuit.


## Slide 27

Chapter 7 - Part 2    27
Direction: Left, Right
Number of positions with examples:
  - Single bit:
    - 1 position
    - 0 and 1 positions
  - Multiple bit:
    - 1 to n – 1 positions
    - 0 to n – 1 positions
Filling of vacant positions
  - Many options depending on instruction set
  - Here, will provide input lines or zero fill
Combinational Shifter Parameters


## Slide 28

Chapter 7 - Part 2    28
4-Bit Basic Left/Right Shifter
Serial Inputs:
  - IR for right shift
  - IL for left shift
Serial Outputs
  - R for right shift (Same as MSB input)
  - L for left shift (Same as LSB input)
Shift Functions:(S1, S0) = 00  Pass B unchanged                 01  Right shift                 10  Left shift                 11  Unused


## Slide 29

Chapter 7 - Part 2    29
Barrel Shifter
A rotate is a shift in which the bits shifted out are inserted into the positions vacated
The circuit rotates its contents left from 0 to 3 positions depending on S:S = 00 position unchanged                  S = 10 rotate left by 2 positionsS = 01 rotate left by 1 positions          S = 11 rotate left by 3 positions
See Table 10-3 in text for details


## Slide 30

Chapter 7 - Part 2    30
Barrel Shifter (continued)
Large barrel shifters can be constructed by using:
  - Layers of multiplexers - Example 64-bit:
    - Layer 1 shifts by 0, 16, 32, 48
    - Layer 2 shifts by 0, 4, 8, 12
    - Layer 3 shifts by 0, 1, 2, 3
    - See example in section 12-2 of the text
  - 2 - dimensional array circuits designed at the electronic level


## Slide 31

Chapter 7 - Part 2    31
Datapath Representation
Have looked at detailed design ofALU and shifter in the datapath in slide 8
Here we move up one level in the hierarchy from that datapath
The registers, and the multiplexer, decoder, and enable hardware for accessing them become a register file
The ALU, shifter, Mux F and status hardware become a function unit
The remaining muxes and buses which handle data transfers are at the new level of the hierarchy


## Slide 32

Chapter 7 - Part 2    32
Datapath Representation (continued)
In the register file:
  - Multiplexer select inputs become A address and B address
  - Decoder input becomes D address
  - Multiplexer outputs become A data and B data
  - Input data to the registers becomes D data
  - Load enable becomes write
The register file now appears like a memory based on clocked flip-flops (the clock is not shown)
The function unit labeling is quite straightforward except for FS


## Slide 33

Chapter 7 - Part 2    33
The Control Word
The datapath has many control inputs
The signals driving these inputs can be defined and organized into a control word
To execute a microinstruction, we apply  control word values for a clock cycle. For most microoperations, the positive edge of the clock cycle is needed to perform the register load
The datapath control word format and the field definitions are shown on the next slide


## Slide 34

Chapter 7 - Part 2    34
The Control Word Fields
Fields
  - DA – D Address
  - AA – A Address
  - BA – B Address
  - MB – Mux B
  - FS – Function Select
  - MD – Mux D
  - RW – Register Write
The connections to datapath are shown in the next slide


## Slide 35

Chapter 7 - Part 2    35
Control Word Block Diagram


## Slide 36

Chapter 7 - Part 2    36
F      A
Encoding of Control
W
D
A
,
AA,
B
A
MB
FS
MD
R
W
Function
Code
Function
Code
Function
Code
Function
Code
Function
Code
R
0
000
Register
0
0000
Function
0
No write
0
R
1
001
Constant
1
0001
Data In
1
Write
1
R
2
010
0010
R
3
011
0011
R
4
100
0100
R
5
101
0101
R
6
110
0110
R
7
111
0111
1000
1001
1010
1011
1100
1101
1110
F
A
¬
F      A
1
+
¬
B
¬
F      A
B
1
¬
F      A
B
¬
F      A
B
1
¬
F      A
1
-
¬
F      A
¬
F      A
B
Ù
¬
F      A
B
Ú
¬
F      A
B
¬
F
A
¬
F      B
¬
F
sr
B
¬
F
sl
B
¬
+
+
+
+
+
+
Å
Control Word Encoding


## Slide 37

Chapter 7 - Part 2    37
Microoperations for the Datapath - Symbolic Representation
Micr
o-
op
eratio
n
D
A
A
A
B
A
M
B
F
S
M
D
R
W
R
1
R
2
R
3
R
e
g
ister
F
unction
Write
R
4
—
R
6
R
e
g
ister
F
unction
Write
R
7
R
7
—
Re
gister
Function
Write
R
1
R
0
—
Con
s
tant
Func
tio
n
Write
——
R
3
R
eg
i
s
t
e
r
—
—
N
o Wr
it
e
R
4
——
—
—
Data in
Write
R
5
R
0
R
0
R
e
g
ister
F
unction
Write
R
1
R
2
R
3
–
¬
F     A
B
1
+    +
=
R
4
s
l R6
¬
F
sl
B
=
R
7
R
7    1
+
¬
F     A
1
+
=
R
1
R
0    2
+
¬
F     A
B
+
=
Data out
R
3
¬
R
4
D
ata in
¬
R
5     0
¬
F     A
B
Å
=


## Slide 38

Chapter 7 - Part 2    38
m
Microoperations from T
a
Binary C
o
o
Results of simulation of the above on the next slide
Microoperations for the Datapath - Binary Representation


## Slide 39

Chapter 7 - Part 2    39
Datapath Simulation


## Slide 40

Chapter 7 - Part 2    40
Terms of Use
All (or portions) of this material © 2008 by Pearson Education, Inc.
Permission is given to  incorporate this material or adaptations thereof into classroom presentations and handouts to instructors in courses adopting the latest edition of Logic and Computer Design Fundamentals as the course textbook.
These materials or adaptations thereof are not to be sold or otherwise offered for consideration.
This Terms of Use slide or page is to be included within the original materials or any adaptations thereof.
