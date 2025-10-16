# Unreal Engine 5.6 Multiplayer Demo

## Example Project for IADE - University of Creative Arts

This project is developed as part of the **Games and Apps Development** course at IADE - University of Creative Arts, Portugal, specifically for the **Emergent Game Technologies** and **Distributed Programming** classes.

### 📋 Project Overview

ExampleProject is a multiplayer Unreal Engine 5.6 demonstration showcasing:
- **Network Programming**: Client-server architecture implementation
- **Replication Systems**: Player state synchronization across network
- **Gameplay Mechanics**: Interactive collectible system with real-time updates

### 🎮 Features

#### Core Gameplay
- **Third-Person Character Controller**: Multiple character variants (Combat, Platforming, Side-scrolling)
- **Collectible System**: Interactive coins with network replication
- **Player State Management**: Synchronized coin collection across all clients
- **Visual Effects**: Particle systems for collectible interactions

#### Network Architecture
- **Server Authority**: Server controls game state and validates actions
- **Client Prediction**: Local feedback for responsive gameplay
- **State Replication**: Automatic synchronization of player statistics
- **Role-based Debugging**: Comprehensive logging system for network troubleshooting

### 🛠️ Technical Implementation

#### Network Components
- `AMyPlayerState`: Replicated player state with coin collection tracking
- `ACoinActor`: Networked collectible with collision detection
- `AExampleProjectCharacter`: Base character class with networking support
- `AGameManager`: Game state management and coordination

#### Key Classes
```
Source/ExampleProject/
├── Core/                       # Core game systems
│   ├── ExampleProjectCharacter.h/cpp    # Base character controller
│   ├── ExampleProjectGameMode.h/cpp     # Game mode implementation
│   ├── ExampleProjectPlayerController.h/cpp  # Player controller
│   ├── MyPlayerState.h/cpp     # Player state with replication
│   └── CoinsGameStateBase.h/cpp # Game state management
├── Collectibles/               # Collectible system
│   └── CoinActor.h/cpp         # Collectible implementation
├── Gameplay/                   # General gameplay mechanics
│   ├── EnemyCharacter.h/cpp    # Enemy character implementation
│   └── LevelCompleteArea.h/cpp # Level completion detection
├── Variant_Combat/             # Combat-focused variant
│   ├── CombatCharacter.h/cpp   # Combat character
│   ├── CombatGameMode.h/cpp    # Combat game mode
│   ├── AI/                     # AI systems
│   ├── Animation/              # Animation notifies
│   ├── Gameplay/               # Combat gameplay elements
│   ├── Interfaces/             # Combat interfaces
│   └── UI/                     # Combat UI elements
├── Variant_Platforming/        # Platforming variant
│   ├── PlatformingCharacter.h/cpp  # Platforming character
│   ├── PlatformingGameMode.h/cpp   # Platforming game mode
│   └── Animation/              # Platforming animations
└── Variant_SideScrolling/      # Side-scrolling variant
    ├── SideScrollingCharacter.h/cpp  # Side-scrolling character
    ├── SideScrollingGameMode.h/cpp   # Side-scrolling game mode
    ├── AI/                     # Side-scrolling AI
    ├── Gameplay/               # Side-scrolling mechanics
    ├── Interfaces/             # Side-scrolling interfaces
    └── UI/                     # Side-scrolling UI
```

### 🚀 Getting Started

#### Prerequisites
- **Unreal Engine 5.6** or compatible version
- **Visual Studio 2022** (for Windows development)
- **Git** for version control

#### Installation
1. Clone the repository:
   ```bash
   git clone [repository-url]
   cd ExampleProject
   ```

2. Open `ExampleProject.uproject` with Unreal Engine 5.6

3. Allow the engine to rebuild the project files

4. Compile the project in your IDE

#### Running the Project
1. **Single Player**: Launch directly from the editor
2. **Multiplayer Testing**: 
   - Use "Play in Editor" with "Number of Players" > 1
   - Or package the project and run multiple instances

### 🎯 Learning Objectives

This project demonstrates:
- **Client-Server Architecture**: Understanding authority and replication
- **Network Programming**: Implementing multiplayer game mechanics
- **State Synchronization**: Keeping game state consistent across clients
- **Debug Techniques**: Network troubleshooting and logging
- **Unreal Engine Networking**: Using UE5's replication system

### 📁 Project Structure

```
ExampleProject/
├── Content/                    # Game assets and blueprints
│   ├── Characters/            # Character models and animations
│   ├── ThirdPerson/           # Third-person game variant
│   ├── Variant_Combat/        # Combat-focused variant
│   ├── Variant_Platforming/   # Platforming variant
│   └── Variant_SideScrolling/ # Side-scrolling variant
├── Source/ExampleProject/      # C++ source code (organized)
│   ├── Core/                  # Core game systems
│   ├── Collectibles/          # Collectible mechanics
│   ├── Gameplay/              # General gameplay elements
│   └── Variant_*/             # Variant-specific implementations
├── Config/                    # Engine configuration files
└── Plugins/                   # Unreal Engine plugins
```


### 📄 License

**Educational Use License**

This project is created for educational purposes as part of the Emergent Game Technologies and Distributed Programming courses at IADE - University of Creative Arts.

**Copyright Notice:**
- **Educational Institution**: IADE - University of Creative Arts, Portugal
- **Course**: Games and Apps Development
- **Classes**: Emergent Game Technologies, Distributed Programming
- **Purpose**: Academic learning and demonstration
- **Usage**: This project is intended solely for educational purposes within the context of the specified classes

**Restrictions:**
- This code may not be used for commercial purposes
- Distribution is limited to educational contexts
- Modifications should maintain educational value and proper attribution

**Attribution:**
When using or referencing this project, please include:
- Course: Games and Apps Development
- Institution: IADE - University of Creative Arts
- Purpose: Educational demonstration


---

*This project demonstrates practical implementation of multiplayer game development concepts using Unreal Engine 5.6, focusing on network programming and distributed systems in game development.*
