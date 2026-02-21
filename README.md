
# LinuxLearner v0.1.2

Currently in its pre-alpha stage, Linux Learner is a software package that creates an environment with folders and files on your Linux machine to practice the commands covered in the CompTIA Linux+ V8 XK0-006 exam.

This version of Linux Learner is missing a heavy amount of features. Linux Learner Alpha (0.2.0) will bring a near-permanent basis for all future versions.

Moreover, the following versions will scrap the current GUI for a better one. Stay tuned.


## Installation
LinuxLearner v0.1.2 is built from source. Follow the instructions based on your architecture.

### Prerequisites
- C compiler (gcc)
- Make utility (make)
- ncurses libraries
```bash
  sudo apt update
  sudo apt install build-essential libncurses-dev
```

### Building Binaries
- Using x86_64 architecture:
```bash
  make all
```

### Usage
```bash
./LinuxLearner

