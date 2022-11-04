# Taiko Arcade Loader

This is a loader for Taiko no Tatsujin Nijiiro ver 08.18

## How to build

### For Ubuntu

- install prerequisites

```
sudo apt install gcc-mingw-w64-x86-64 g++-mingw-w64-x86-64 build-essential clang
curl https://sh.rustup.rs -sSf | sh
cargo install cross
```

- checkout code
```
git clone --recurse-submodules -j8 git@github.com:phstudy/TaikoArcadeLoader.git
```

- build
```
source "$HOME/.cargo/env"

cd TaikoArcadeLoader

make clean

make

make plugins 
```

- distribution
```
make dist
```
