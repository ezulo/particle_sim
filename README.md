# Particle Sim

An elastic particle simulation, with fun graphics! In progress

## Dependencies

-   SFML v3.0.2-1 or newer
-   clang 20.1.8 or newer

## Building

For the serial profile:

`make`

For the multithreaded profile:

`make openmp`

## Running

`./run` or `./run-openmp`

## Cleaning

`make clean`

## Configuration

All config variables are located in `src/config.h`. Project must be rebuilt entirely on changing.
 
## Developer Notes

-   OpenMP does not seem to net any performance gains (quite the contrary; the overhead is massive)

