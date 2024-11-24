# Police vs Thief Simulation

This project is a simulation game where a thief tries to escape while detectives chase him across a city map. 
The simulation involves creating a city map, setting up agents (thief and detectives), and executing their strategies.

## Features

- **City Map Modeling**:
  - Bidirectional roads connecting cities.
  - Named cities with customizable road lengths.
- **Agent Behavior**:
  - Thief and detectives move across the map using strategies like:
    - Random movement.
    - Cheapest-Least-Visited (CLV) strategy.
    - Depth-First-Search (DFS) strategy.
  - Agents have stamina and starting positions.
- **Game Logic**:
  - Run simulations to determine if the thief escapes or gets caught.
  - Display game states, including agent locations and stamina.

## Getting Started

### Prerequisites

- GCC or any compatible C compiler installed on your system.

### Compilation

Compile the project using the provided `Makefile`:
```bash
make
```

### Running the Simulation

Run the game with the following command:
```bash
./game <city_data_file> <agent_data_file> <cycles> [seed]
```

Example:
```bash
./game data/citiesSmall.data data/agentsS0.data 10
```

### Game Commands

- **`display`**: Show the current state of the game.
- **`run`**: Run the simulation until the maximum number of cycles is reached.
- **`step`**: Step through one cycle of the simulation.
- **`stats`**: Display detailed stats for each agent.
- **`map`**: Display the current map.

## Files and Structure

### Source Files

- **`Agent.c` / `Agent.h`**: Defines agent behavior and movement strategies.
- **`Map.c` / `Map.h`**: Defines the city map and road relationships.
- **`game.c`**: Implements the main game logic and simulation.
- **`Makefile`**: Automates the compilation process.

### Data Files

The `data` directory contains predefined datasets for running the simulation:

- **Agent Data**:
  - `agentsS0.data`, `agentsS1.data`, `agentsS2.data`, `agentsS3.data`: Define the attributes of agents (detectives and thief) for different scenarios, including their initial positions, stamina, and strategies.
- **City Data**:
  - `citiesSmall.data`, `citiesMedium.data`: Define maps with small and medium city sizes for testing.
  - `citiesInformants.data`: Includes additional informant data in the city for enhanced gameplay.

To use these files, provide the file paths as arguments when running the simulation.

## How to Contribute

Contributions are welcome. Feel free to fork the repository, create issues, and submit pull requests.

## License

This project is licensed under the MIT License.
