# sdl-basic

## A basic SDL based game demo

### Development Setup

```
# Raspberry Pi 4 (Raspbian Buster)

# update and upgrade system packages
sudo apt update -y && sudo apt upgrade -y

# install text editor or use nano
sudo apt install emacs25-nox

# install dependency for SDL2
sudo apt install libxext-dev

# download, untar, build and install

# SDL2
wget https://libsdl.org/release/SDL2-2.0.12.tar.gz
tar -xzf SDL2-2.0.12.tar.gz SDL2-2.0.12/
cd SDL2-2.0.12/
mkdir build
cd build
../configure 
make
sudo make install

# SDL2_image
wget https://www.libsdl.org/projects/SDL_image/release/SDL2_image-2.0.5.tar.gz
tar -xzf SDL2_image-2.0.5.tar.gz
cd SDL2_image-2.0.5
./configure
make
make install
```

### Development and Build

```
cd sdl-basic/
mkdir bin
emacs basic.cpp
make
make run
```
