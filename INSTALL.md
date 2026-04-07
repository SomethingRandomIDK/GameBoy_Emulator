# Build From Source

- [Linux](INSTALL#Linux)
    - [Docker](INSTALL#Docker)
- [Windows](INSTALL#Windows)
    - [MSVC](INSTALL#MSVC)
    - [MinGW](INSTALL#MinGW)
- [MacOS](INSTALL#MacOS)

## Linux

1) Install Dependencies and Build Tools

Debian/Ubuntu:

```sh
sudo apt install build-essential cmake libsdl2-dev git
```

Arch:

```sh
sudo pacman -S base-devel cmake sdl2 git
```

2) Clone and build the repository

```sh
git clone https://github.com/SomethingRandomIDK/GameBoy_Emulator.git gbemu
cd gbemu
mkdir build
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

The binary will be available in the build folder.  You can then run `make
install` from the build folder to install it in the /usr/local/bin/ folder.

### Docker

The Docker image will build the program using Ubuntu 20.04.  You can use the
following command to build the image:

```sh
docker build -t gbemu-build .
```

Run the following command to then run the image which will compile the binary
and put it in the build folder:

```sh
docker run --rm -v $(pwd):/app gbemu-build
```

## Windows

### MSVC

### MinGW

## MacOS


