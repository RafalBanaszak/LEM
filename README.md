# LEM - Lightweight Event Machine

LEM (short for Lightweight Event Machine) is a high-performance event-driven framework designed for small microcontrollers. 
It's written in C and intended to replace traditional RTOSes when using one would be too resource-intensive. 

This framework allows you to write non-blocking, event-driven code that responds quickly to external stimuli, such as sensors or timers.
LEM is designed with efficiency and resource utilization in mind. It's ideal for devices with limited memory and processing power, but can also be used in larger systems where real-time performance is critical.

LEM features include:
- Non-blocking event-driven architecture
- Low memory footprint and efficient
- Lockless design (if a CAS instruction is available)
- Easy to use API
- Open-source and free to use

## Idea
- An event represents a job that needs to be done.
- The events are pre-allocated statically in a pool. ```em_alloc``` returns a free event from the pool. ```em_free``` returns the event to the pool.
- User configures the event. 

## Build and installation
1. Clone the repository:
    ```bash
    git clone https://github.com/RafalBanaszak/LEM.git
    cd LEM
    ```
1. (Optional) Clone the submodules if you want to run the unit tests:
    ```bash
    git submodule update --init --recursive
    ```
1. Configure project. Note the `ARCH` option. You should set it according to your target architecture. Currently supported architectures are:
   - `ARMV6M` - ARM Cortex-M0/M0+/M1
   - `ARMV7M` - ARM Cortex-M3/M4/M7
   - `ARMV8M` - ARM Cortex-M23/M33
   - `GENERIC` - Used to run unit tests on a host machine

    ```bash
    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release CMAKE_C_COMPILER=<C_compiler_path> -DARCH=ARMV6M ..
    ```
   > (Optional) If you want to build the unit tests set the UT_ENABLE option to ON. <br>
   > ```-DUT_ENABLE=ON``` 
1. Build the library
    ```bash
    cmake --build .
    ```
1. Add the library to your project.
    - Add liblem.a to your linker flags.
    - Add lib/inc to your include paths.

## Usage
TBD

