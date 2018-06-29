This is an implementation of the Cooley-Tukey FFT algorithm for one of
Tensilica's Xtensa processor platforms. The Tensilica Instruction Extension
hardware-description DSL is utilized to customize the core Xtensa core
architecture by means of additional specialized registers and instructions
which make it possible to perform the specialized task of computing a DIT/DIF
FFT at significantly higher speeds than would ordinarily be possible with the
base ISA alone.

This project was completed as part of the _Hardware-Software Codesign_ Lecture
as given by the Vodafone Chair for _Mobile Communication Systems_ at TU Dresden
in the winter semester 17/18. If you are taking this course and have questions
regarding my solution, feel free to contact me on Github. A very detailed
lab report can also be found under `report/pdf/master.pdf`.

The algorithm itself is implemented in assembly in `project/source/asm/fft.S`,
the instruction set extensions are implemented in `project/source/tie/fft.tie`
in the aforementioned TIE language. In order to actually simulate this design
you would need a commercial licence for the Xtensa SDK.
