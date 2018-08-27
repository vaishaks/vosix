# Vosix
My attempt at writing a bare-metal Operating System for the Raspberry Pi 3 Model B.

## Useful Tutorials
- [s-matyukevich/raspberry-pi-os](https://github.com/s-matyukevich/raspberry-pi-os): This repository contains a step-by-step guide that teaches how to create a simple operating system (OS) kernel from scratch. 
- [bxtsrc/raspi3-tutorial](https://github.com/bztsrc/raspi3-tutorial): This tutorial series are made for those who would like to compile their own bare metal application for the Raspberry Pi.

These guys are awesome for creating these tutorials! I reccomend that you check them out. This repo is my experiments using what I learn from their tutorials.

## Development Environment
I'm developing this on Windows using the Windows subsystem for Linux because I had some trouble with running docker on Windows. If you're running Linux or if you can setup docker without any issues, that could serve as a good environment too.

If you're building this on Linux or on the Windows subsystem for Linux, then run the following commands to setup build dependencies for this repo:

```
sudo apt-get update

sudo apt-get install -y gcc-aarch64-linux-gnu build-essential
````

I'm using qemu-system-arm and the raspi machine to debug the code locally. As of writing this (08/26/18), qemu 2.5 is available for Ubuntu 16.04. The raspi machine is included from qemu 2.12. So I had to compile the latest qemu locally using the [following steps](https://raspberrypi.stackexchange.com/questions/34733/how-to-do-qemu-emulation-for-bare-metal-raspberry-pi-images/85135#85135).

````
# Install qemu build dependencies
sudo apt-get install git libglib2.0-dev libfdt-dev libpixman-1-dev zlib1g-dev

sudo apt-get install libnfs-dev libiscsi-dev

# Clone the qemu repository
git clone git://git.qemu.org/qemu.git

cd qemu

# Checkout a stable version
git checkout v2.12.0

# Configure and build
./configure --target-list=aarch64-softmmu  
make -j`nproc`

# Export to path or add path to .bashrc
export PATH="$(pwd)/aarch64-softmmu:${PATH}"
````

To run @bztsrc's tutorial with the installed compiler, make sure to run the following command in the project you want to build:

`find . -name Makefile | xargs sed -i 's/aarch64-elf-/aarch64-linux-gnu-/'`

## Building and Running

- Run `make` to build the source.
- Run `make debug` to debug the kernel locally

Follow instructions from [here](https://github.com/s-matyukevich/raspberry-pi-os/blob/master/docs/lesson01/rpi-os.md#raspberry-pi-config) to test the kernel on an actual Raspberry Pi 3 Model B.

