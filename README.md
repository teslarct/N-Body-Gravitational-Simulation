# Raylib N-Body Gravity Simulator

A simple single-file **N-body gravitational physics simulation** built in **C++** using **raylib**.

This project simulates multiple bodies attracting each other through Newtonian gravity and renders the motion visually in real time.

## Project Status

This is an experimental learning project.
The main goal is to understand:

* N-body simulation
* gravitational force calculation
* velocity and position integration
* real-time rendering with raylib
* basic physics engine structure

## Tech Stack

* C++
* raylib
* Single-file structure: `main.cpp`

## Features

* Real-time 2D gravitational simulation
* Multiple interacting bodies
* Visual rendering using raylib
* Basic Newtonian attraction between objects
* Simple project structure for learning and experimentation

## File Structure

```text
raylib-nbody-gravity/
├── main.cpp
└── README.md
```

## Physics Idea

Each body attracts every other body using Newton's law of universal gravitation:

```text
F = G * (m1 * m2) / r²
```

The simulation updates acceleration, velocity, and position over time to produce orbital-like motion.

## How to Build

Make sure raylib is installed on your system.

### macOS

If raylib is installed with Homebrew:

```bash
brew install raylib
```

Then compile:

```bash
clang++ main.cpp -std=c++20 $(pkg-config --cflags --libs raylib) -o nbody_gravity
```

Run:

```bash
./nbody_gravity
```

## How to Run in CLion

1. Open the project folder in CLion.
2. Make sure raylib is installed.
3. Configure CMake or use your existing raylib setup.
4. Build and run `main.cpp`.

## Goal of This Project

This project is not just a visual demo.
It is a foundation for learning how a physics engine works internally.

Future improvements may include:

* collision detection
* better numerical integration
* trails/orbit paths
* Barnes-Hut optimization
* configurable bodies
* UI controls
* energy and momentum tracking

## Author

Built by *teslarct* as part of a physics engine learning journey.
