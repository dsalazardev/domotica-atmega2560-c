
## Slide 1



## Slide 2

Chapter 9  Part 2   2
Overview
Part 1 – Datapaths
Part 2 – A Simple Computer
  - Instruction Set Architecture (ISA)
  - Single-Cycle Hardwired Control
    - PC Function
    - Instruction Decoder
    - Example Instruction Execution
Part 3 – Multiple Cycle Hardwired Control


## Slide 3

Chapter 9  Part 2   3
Instruction Set Architecture (ISA) for Simple Computer (SC)
A programmable system uses a sequence of instructions to control its operation
A typical instruction specifies:
  - Operation to be performed
  - Operands to use, and
  - Where to place the result, or
  - Which instruction to execute next
Instructions are stored in RAM or ROM as a program
The addresses for instructions in a computer are provided by a program counter (PC) that can
  - Count up
  - Load a new address based on an instruction and, optionally, status information


## Slide 4

Chapter 9  Part 2   4
Instruction Set Architecture (ISA) (continued)
The PC and associated control logic are part of the Control Unit
Executing an instruction - activating the necessary sequence of operations specified by the instruction
Execution is controlled by the control unit and performed:
  - In the datapath
  - In the control unit
  - In external hardware such as memory or input/output


## Slide 5

Chapter 9  Part 2   5
ISA: Storage Resources
The storage resources are "visible" to the programmer at the lowest software level (typically, machine or assembly language)
Storage resourcesfor the SC =>
Separate instruction anddata memories imply"Harvard architecture"
Done to permit use ofsingle clock cycle perinstruction implementation
Due to use of "cache" in modern computerarchitectures, is a fairlyrealistic model
Instruction
memory
2
15
x
16
Data
memory
2
15
x
16
Register file
8
x
16
Program counter
(PC)


## Slide 6

Chapter 9  Part 2   6
ISA: Instruction Format
A instruction consists of a bit vector
The fields of an instruction are subvectors representing specific functions and having specific binary codes defined
The format of an instruction defines the subvectors and their function
An ISA usually contains multiple formats
The SC ISA contains the three formats presented on the next slide


## Slide 7

Chapter 9  Part 2   7
ISA: Instruction Format
The three formats are: Register, Immediate, and Jump and Branch
All formats contain an Opcode field in bits 9 through 15.
The Opcode specifies the operation to be performed
More details on each format are provided on the next three slides


## Slide 8

Chapter 9  Part 2   8
ISA: Instruction Format (continued)
This format supports instructions represented by:
  - R1 ← R2 + R3
  - R1 ← sl R2
There are three 3-bit register fields:
  - DR - specifies destination register (R1 in the examples)
  - SA - specifies the A source register (R2 in the first example)
  - SB - specifies the B source register (R3 in the first example and R2 in the second example)
Why is R2 in the second example SB instead of SA?
  - The source for the shifter in our datapath to be used in implementation is Bus B rather than Bus A


## Slide 9

Chapter 9  Part 2   9
ISA: Instruction Format (continued)
This format supports instructions described by:
  - R1 ← R2 + 3
The B Source Register field is replaced by an Operand field OP which specifies a constant.
The Operand:
  - 3-bit constant
  - Values from 0 to 7
The constant:
  - Zero-fill (on the left of) the Operand to form 16-bit constant
  - 16-bit representation for values 0 through 7


## Slide 10

Chapter 9  Part 2   10
ISA: Instruction Format (continued)
This instruction supports changes in the sequence of instruction execution by adding an extended, 6-bit, signed 2s-complement address offset to the PC value
The 6-bit Address (AD) field replaces the DR and SB fields
  - Example: Suppose that a jump is specified by the Opcode and the PC contains 45 (0…0101101) and Address contains – 12 (110100). Then the new PC value will be:0…0101101 + (1…110100) = 0…0100001 (45 + (– 12) = 33)
The SA field is retained to permit jumps and branches on N or Z based on the contents of Source register A


## Slide 11

Chapter 9  Part 2   11
ISA: Instruction Specifications
The specifications provide:
  - The name of the instruction
  - The instruction's opcode
  - A shorthand name for the opcode called a mnemonic
  - A specification for the instruction format
  - A register transfer description of the instruction, and
  - A listing of the status bits that are meaningful  during an instruction's execution (not used in the architectures defined in this chapter)


## Slide 12

Chapter 9  Part 2   12
ISA: Instruction Specifications (continued)


## Slide 13

Chapter 9  Part 2   13
ISA:Example Instructions and Data in Memory


## Slide 14

Chapter 9  Part 2   14
Single-Cycle Hardwired Control
Based on the ISA defined, design a computer architecture to support the ISA
The architecture is to fetch and execute each instruction in a single clock cycle
The datapath from Figure 10-11 will be used
The control unit will be defined as a part of the design
The block diagram is shown on the next slide


## Slide 15

Chapter 9  Part 2   15


## Slide 16

Chapter 9  Part 2   16
The Control Unit
The Data Memory has been attached to the Address Out and Data Out and Data In lines of the Datapath.
The MW input to the Data Memory is the Memory Write signal from the Control Unit.
For convenience, the Instruction Memory, which is not usually a part of the Control Unit is shown within it.
The Instruction Memory address input is provided by the PC and its instruction output feeds the Instruction Decoder.
Zero-filled IR(2:0) becomes Constant In
Extended IR(8:6) || IR(2:0) and Bus A are address inputs to the PC.
The PC is controlled by Branch Control logic


## Slide 17

Chapter 9  Part 2   17
PC Function
PC function is based on instruction specifications involving jumps and branches taken from Slide 13:
In addition to the above register transfers, the PC must also implement:  PC ←  PC + 1
The first two transfers above require addition to the PC of:  Address Offset = Extended IR(8:6) || IR(2:0)
The third transfer requires that the PC be loaded with:  Jump Address = Bus A = R[SA]
The counting function of the PC requires addition to the PC of  1
Branch
on
Zero
BRZ
if (R[
S
A] =
0) PC
←
PC
+ s
e
A
D
Branch
on
Negative
BRN
if (R[
S
A] <
0) PC
PC
+ s
e
A
D
J
u
mp
JMP
P
C
R[SA
]
←
←


## Slide 18

Chapter 9  Part 2   18
PC Function (continued)
Branch Control determines the PC transfers based on five of its inputs defined as follows:
  - N,Z – negative and zero status bits
  - PL – load enable for the PC
  - JB – Jump/Branch select: If JB = 1, Jump, else Branch
  - BC  –  Branch Condition select: If BC = 1, branch for N = 1, else branch for Z = 1.
The above is summarize by the following table:
Sufficient information is provided here to design the PC
| PC Operation | PL | JB | BC |
| --- | --- | --- | --- |
| Count Up | 0 | X | X |
| Jump | 1 | 1 | X |
| Branch on Negative (else Count Up) | 1 | 0 | 1 |
| Branch on Zero (else Count Up) | 1 | 0 | 0 |


## Slide 19

Chapter 9  Part 2   19
Instruction Decoder
The combinational instruction decoder converts the instruction into the signals necessary to control all parts of the computer during the single cycle execution
The input is the 16-bit Instruction
The outputs are control signals:
  - Register file addresses DA, AA, and BA,
  - Function Unit Select FS
  - Multiplexer Select Controls MB and MD,
  - Register file and Data Memory Write Controls RW and MW, and
  - PC Controls PL, JB, and BC
The register file outputs are simply pass-through signals:     DA = DR, AA = SA, and BA = SBDetermination of the remaining signals is more complex.


## Slide 20

Chapter 9  Part 2   20
Instruction Decoder (continued)
The remaining control signals do not depend on the addresses, so must be a function of IR(13:9)
Formulation requires examining relationships between the outputs and the opcodes given in Slides 12 and 13.
Observe that for other than branches and jumps, FS = IR(12:9)
This implies that the other control signals should depend as much as possible on IR(15:13) (which actually were assigned with decoding in mind!)
To make some sense of this, we divide instructions into types as shown in the table on the next page


## Slide 21

Chapter 9  Part 2   21
Instruction Decoder (continued)


## Slide 22

Chapter 9  Part 2   22
Instruction Decoder (continued)
The types are based on the blocks controlled and the seven signals to be generated; types can be divided into two groups:
  - Datapath and Memory Control (First 4 types)
  - PC Control (Last 3 types)
In Datapath and Memory Control blocks controlled are considered:
  - Mux B (1st and 4th types)
  - Memory and Mux D (2nd and 3rd types)
  - By assigning codes with no or only one 1 for these, implementation of MB, MD, RW and MW are simplified.
In Control Unit more of a bit setting approach was used:
  - Bit 15 = Bit 14 = 1 were assigned to generate PL
  - Bit 13 values were assigned to generate JB.
  - Bit 9 was use as BC which contradicts FS = 0000 needed for branches.  To force FS(6) to 0 for branches, Bit 9 into FS(6) is disabled by PL.
Also, useful bit correlations between values in the two groups were exploited in assigning the codes.


## Slide 23

Chapter 9  Part 2   23
Instruction Decoder (continued)
The end result by use of the types, careful assignment of codes, and use of don't cares, yields very simple logic:
This completes thedesign of most of the essential parts ofthe single-cycle simple computer


## Slide 24

Chapter 9  Part 2   24
Example Instruction Execution
Decoding, control inputs and paths shown for ADI, RD and BRZ  on next 6 slides


## Slide 25

Chapter 9  Part 2   25
Decoding for ADI


## Slide 26

Chapter 9  Part 2   26


## Slide 27

Chapter 9  Part 2   27
Decoding for LD


## Slide 28

Chapter 9  Part 2   28


## Slide 29

Chapter 9  Part 2   29
Decoding for BRZ


## Slide 30

Chapter 9  Part 2   30


## Slide 31

Chapter 9  Part 2   31
Terms of Use
All (or portions) of this material © 2008 by Pearson Education, Inc.
Permission is given to  incorporate this material or adaptations thereof into classroom presentations and handouts to instructors in courses adopting the latest edition of Logic and Computer Design Fundamentals as the course textbook.
These materials or adaptations thereof are not to be sold or otherwise offered for consideration.
This Terms of Use slide or page is to be included within the original materials or any adaptations thereof.
