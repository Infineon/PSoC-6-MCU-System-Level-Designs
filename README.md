## Table of Contents

* [Introduction](#introduction)
* [Navigating the Repository](#navigating-the-repository)
* [Required Tools](#required-tools)
* [Code Examples List](#code-examples-list)
* [References](#references)

# Introduction
This repository contains examples and demos for PSoC 6 MCU family of devices, a single chip solution for the emerging IoT devices. PSoC 6 MCU bridges the gap between expensive, power hungry application processors and low‑performance microcontrollers (MCUs). The ultra‑low‑power, dual-core architecture of PSoC 6 MCU offers the processing performance needed by IoT devices, eliminating the tradeoffs between power and performance.

Cypress provides a wealth of data at [www.cypress.com](http://www.cypress.com/) to help you select the right PSoC device and effectively integrate it into your design. Visit our [PSoC 6 MCU](http://www.cypress.com/products/32-bit-arm-cortex-m4-psoc-6) webpage to explore more about PSoC 6 MCU family of device.
Feel free to explore through the code example source files and let us innovate together!

# Navigating the Repository

This repository contains reference projects for developing system-level design using PSoC 6 MCU such as handling faults in PSoC 6 MCU, tuning CapSense designs, using DMA in your design etc.

If you are new to developing projects with PSoC 6 MCU, we recommend you to refer the [PSoC 6 Getting Started GitHub](https://github.com/cypresssemiconductorco/PSoC-6-MCU-Getting-Started) page which can help you familiarize with device features and guides you to create a simple PSoC 6 design with PSoC Creator IDE. For other block specific design please visit the following GitHub Pages:
#### 1. [Analog Designs](https://github.com/cypresssemiconductorco/PSoC-6-MCU-Analog-Designs)
#### 2. [Digital Designs](https://github.com/cypresssemiconductorco/PSoC-6-MCU-Digital-Designs)
#### 3. [BLE Connectivity Designs](https://github.com/cypresssemiconductorco/PSoC-6-MCU-BLE-Connectivity-Designs)
#### 4. [Audio Designs](https://github.com/cypresssemiconductorco/PSoC-6-MCU-Audio-Designs)
#### 5. [Device Related Designs](https://github.com/cypresssemiconductorco/PSoC-6-MCU-Device-Related-Design)
#### 6. [PSoC 6 Pioneer Kit Designs](https://github.com/cypresssemiconductorco/PSoC-6-MCU-Pioneer-Kits)
#### 7. [PSoC 6 MCU based RTOS Designs](https://github.com/cypresssemiconductorco/PSoC-6-MCU-RTOS-Based-Design)

You can use these block level examples to guide you through the development of a system-level design using PSoC 6 MCU. All the code examples in this repository comes with well documented design guidelines to help you understand the design and how to develop it. The code examples and their associated documentation are in the Code Example folder in the repository.

# Required Tools

## Software
### Integrated Development Environment (IDE)
To use the code examples in this repository, please download and install
[PSoC Creator](http://www.cypress.com/products/psoc-creator)

## Hardware
### PSoC 6 MCU Development Kits
* [CY8CKIT-062-BLE PSoC 6 BLE Pioneer Kit](http://www.cypress.com/documentation/development-kitsboards/psoc-6-ble-pioneer-kit).

* [CY8CKIT-062-WiFi-BT PSoC 6 WiFi-BT Pioneer Kit](http://www.cypress.com/documentation/development-kitsboards/psoc-6-wifi-bt-pioneer-kit). 

**Note** Please refer to the code example documentation for selecting the appropriate kit for testing the project

## Code Examples List
#### 1. CE218541 - PSoC 6 MCU Fault-Handling Basics
This code example demonstrates how to find a fault location using the PDL SysLib and the Arm exception handler. The
example has three different faults: Arm Cortex M0+ Hard Fault, Cortex M4 Usage Fault, and a Cortex M4 (CM4) Bus Fault.
The example uses a UART to display information for debugging.
#### 2. CE218552 – PSoC 6 MCU: UART to Memory Buffer Using DMA
This example demonstrates how a PSoC 6 DMA channel transfers data received from the UART to a buffer in memory. When
the buffer is filled, a second DMA channel drains the buffer to the UART, to be transmitted.
#### 3. CE218553 – PSoC 6 MCU: PWM Triggering a DMA Channel
This code examples demonstrates how to route trigger signals on a PSoC 6 device. The trigger signal, in this code example, is
routed from the PWM to the DMA, using PSoC Creator's auto routing feature.
#### 4. CE219765 – PSoC 6 MCU Event Profiling
This example uses PSoC 6 MCU profiler block to observe the behavior of the SCB in UART mode. It demonstrates how to use
the profiler to see the impact that different programming techniques can have on performance. In this case, it shows the
difference between interrupt-driven code and code that polls the UART driver. 
#### 5. CE219940 – PSoC 6 MCU Multiple DMA Concatenation
This example demonstrates the use of multiple concatenated DMA channels to manipulate memory, with no CPU usage. The
incoming data from the UART is packed into 5-byte packets and stored in a memory array, along with a timestamp from the
RTC. When four packets of data are stored, they are echoed back to the UART using DMA
#### 6. CE220465 – PSoC 6 MCU Cryptography:AES Demonstration
This code example encrypts and decrypts user input data using the AES algorithm using a 128-bit long key. The encrypted
and decrypted data are displayed on a UART terminal emulator.
#### 7. CE220511 – PSoC 6 MCU Cryptography:SHA Demonstration
This code example shows how to generate a 20-byte hash value or message digest for an arbitrary user input message using
the SHA2 algorithm using the Cryptographic hardware block in PSoC 6 MCU. The example further shows that any change in
the message results in a unique hash value for the message. The hash value generated for the message is displayed on a
UART terminal emulator.
#### 8. CE221295 – PSoC 6 MCU Cryptography:True Random Number Generation
This code example demonstrates generating a One-Time Password (OTP) using the True Random Number generation feature
of PSoC 6 MCU cryptography block.
#### 9. CE222827 – PSoC 6 MCU: CapSense Tuner
This example demonstrates how to implement Tuner GUI interface for CapSense® design using UART and I2C interfaces in
PSoC 6 MCU devices.


## References
#### 1. PSoC 6 MCU
PSoC 6 bridges the gap between expensive, power hungry application processors and low‑performance microcontrollers (MCUs). The ultra‑low‑power PSoC 6 MCU architecture offers the processing performance needed by IoT devices, eliminating the tradeoffs between power and performance. The PSoC 6 MCU contains a dual‑core architecture, with both cores on a single chip. It has an Arm® Cortex®‑M4 for high‑performance tasks, and an Arm® Cortex®‑M0+ for low-power tasks, and with security built-in, your IoT system is protected.
To learn more on the device, please visit our [PSoC 6 MCU](http://www.cypress.com/products/32-bit-arm-cortex-m4-psoc-6) webpage.

####  2. PSoC 6 MCU Learning resource list
##### 2.1 PSoC 6 MCU Datasheets
Device datasheets list the features and electrical specifications of PSoC 6 families of devices: [PSoC 6 MCU Datasheets](http://www.cypress.com/search/all?f%5B0%5D=meta_type%3Atechnical_documents&f%5B1%5D=resource_meta_type%3A575&f%5B2%5D=field_related_products%3A114026)
##### 2.2 PSoC 6 MCU Application Notes
Application notes are available on the Cypress website to assist you with designing your PSoC application: [A list of PSoC 6 MCU ANs](http://www.cypress.com/psoc6an)
##### 2.3 PSoC 6 MCU Component Datasheets
PSoC Creator utilizes "components" as interfaces to functional Hardware (HW). Each component in PSoC Creator has an associated datasheet that describes the functionality, APIs, and electrical specifications for the HW. You can access component datasheets in PSoC Creator by right-clicking a component on the schematic page or by going through the component library listing. You can also access component datasheets from the Cypress website: [PSoC 6 Component Datasheets](http://www.cypress.com/documentation/component-datasheets)
##### 2.4 PSoC 6 MCU Technical Reference Manuals (TRM)
The TRM provides detailed descriptions of the internal architecture of PSoC 6 devices:[PSoC 6 MCU TRMs](http://www.cypress.com/psoc6trm)

## FAQ

### Technical Support
Need support for your design and development questions? Check out the [Cypress Developer Community 3.0](https://community.cypress.com/welcome).  

Interact with technical experts in the embedded design community and receive answers verified by Cypress' very best applications engineers. You'll also have access to robust technical documentation, active conversation threads, and rich multimedia content. 

You can also use the following support resources if you need quick assistance:
##### Self-help: [Technical Support](http://www.cypress.com/support)
##### Local Sales office locations: [Sales Office](http://www.cypress.com/about-us/sales-offices)
