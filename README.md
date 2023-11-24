# remora - Implementing audio processing and control interfaces on the 21489 SHARC DSP

<img src="https://user-images.githubusercontent.com/15021145/285431726-ce0b137a-df77-49d8-b06e-e49332750b60.jpg" width="480">

### Requirements

- Hardware:
  - Evaluation Board(EVB)
  - Emulator
  - USB-UART Adapter

- Software:
  - CrossCore Embedded Studio(CCES)
  - Visual Studio
 
### Command Line Device Programmer(CLDP)

- Processor:
  - `-proc ADSP-21479`: Select ADSP-21479
  - `-proc ADSP-21489`: Select ADSP-21489
  - vice versa.

- Emulator Models and Configuration:
  - `-emu ice-1000`: AD-HP510ICE compatible.
  - `-emu kit`: Supports EZ-KIT Debug Agent.

- For serial booting, execute this command:
  ```bash
  cldp -proc ADSP-21479 -emu kit -driver "directory\to\driver\21479_m25p16_dpia.dxe" -cmd prog -erase all -format ascii -file "directory\to\program\in-application-programming.ldr"
  ```
- For parallel booting, use the following command:
  ```bash
  cldp -proc ADSP-21479 -emu kit -driver "directory\to\driver\21479_m29w320_dpia.dxe" -cmd prog -erase affected -format hex -file "directory\to\program\in-application-programming.ldr"
  ```

### Things I easily forget
    
- Boot Mode Selection(Compiled option in CCES):
  - `SPI Flash`: For serial booting.
  - `PROM`: For parallel booting.

- EZ-KIT Boot Encoding Options(Rotate the switch on EZ-KIT):
  - `0`: SPI Slave Boot.
  - `1`: Boot from SPI Flash Memory (SPI Master Boot).
  - `2`: Boot from 8-bit External Parallel Flash Memory (Default).

### Overview

This project utilizes an Evaluation Board(EVB) purchased from Alibaba at a cost of $60. The EVB is equipped with high-quality audio components, including a 21489 SHARC DSP, a flash, a SDRAM, a PSU, an ADC-PCM1804, and a DAC-PCM1798, ensuring high fidelity quality.

An emulator, crucial for programming the EVB, was also sourced from Alibaba. This un-authorized unit(AD-HP510ICE), priced at $200, provides a cost-effective alternative to the official emulator(ICE-1000/ICE-2000), which is priced around $2,000.

Additionally, a USB-UART adapter was adopted for serial communication purposes. Priced at approximately $5, this adapter underscores our commitment to cost-efficiency without compromising on performance.

Overall, this setup offers an affordable and robust solution for developing high-quality audio DSP solutions in 2021.
