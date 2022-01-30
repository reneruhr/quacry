# Quasycrystal Cut-and-Project Visualization

## Features:

Visualization of point sets and possible edges between them (defined from an LLL basis)

Implemented examples:
- Ammann-Beenker QC with vertex star patterns. [Runs on GPU] (See Baake-Grimm "Aperiodic Order" p275)
- Penrose Pointset. [Runs on CPU] (See Marlof-Strömbergsson https://arxiv.org/pdf/1304.2044.pdf p9)

## Demo




https://user-images.githubusercontent.com/18703843/145476568-a29ea56b-4161-4550-a6f4-6a5492883caa.mov



https://user-images.githubusercontent.com/18703843/145476580-eb4183ed-d2ed-4622-815e-6dd0f16d09ac.mov




https://user-images.githubusercontent.com/18703843/145476589-92f754e3-7461-494b-b27a-0ab46bdb03ab.mov


![Penrose Pointset ](demo_shots/penrose_points_small.jpg?raw=true)
![Penrose Edges ](demo_shots/penrose_edges_small.jpg?raw=true)
![Penrose Pointset Sheared](demo_shots/penrose_sheared_small.jpg?raw=true)




## Instructions for building from Source for Linux, Mac, Windows:

Building uses cmake and git to receive the source. Dependencies (via kipod) are GLM, GLFW and GLEW, and are installed via a package manager (on Mac/Linux) or can be cloned as submodules ( git clone https://github.com/reneruhr/quacry.git --recursive )

### Ubuntu

#### Step 1:
Install necessary packages:

```console
~$ sudo apt install build-essential libgl1-mesa-dev cmake git 
~$ sudo apt install libglfw3-dev glew-utils libglm-dev libglew-dev
```

#### Step 2: 
Clone and Building Instruction:

```console
~$ git clone https://github.com/reneruhr/quacry.git
~$ cd quacry
~$ git clone https://github.com/reneruhr/kipod.git
~$ git clone https://github.com/akuukka/quickhull.git external_libs/quickhull/
~$ mkdir build
~$ cd build
~$ cmake ..
~$ make
~$ ./quacry
```

(If you use 'git clone --recursive https://github.com/reneruhr/quacry.git', then cloning kipod and quickhull is not necessary).

### MacOS (Intel/ARM)

#### Step 1
Install homebrew (https://brew.sh/) and xcode command line developer tools:

```console
~$ xcode-select --install
~$ /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```
#### Step 2
Then install cmake, git and the necessary libraries glew, glfw3, glm:

```console
~$ brew install git cmake glew glfw3 glm
```

#### Step 3: 
See Step 2 Ubuntu.


### Windows (Visual Studio 2019)

#### Step 1
Get git e.g. via https://gitforwindows.org/.
The required libraries are included as git submodules. Use the tag --recursive to add these when cloning:

```console
~$ git clone --recursive https://github.com/reneruhr/quacry.git
```

#### Step 2
It is convenient to use the Visual Studio 2019 "C++ CMake tools for Windows" component. It is already installed with the 
"Desktop development with C++" and "Linux Development with C++" workload.
Opening the folder will detect the cmake file automatically.

