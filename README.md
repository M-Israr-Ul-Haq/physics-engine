# ⚛️ Physics Engine  
*A lightweight 2D physics simulation engine built with C++ and SFML 3.0.*
---

## 🧩 Overview
The **Physics Engine** is a real-time 2D simulation framework that models physical interactions ranging from **microscopic particle collisions** to **macroscopic orbital mechanics**.  
It’s designed for **learning, experimentation, and visualization** — perfect for anyone curious about how physics works under the hood of games and simulations.

---

## 🚀 Features

### 🔹 Particle Collision System
- Physically accurate **particle-particle** and **particle-wall** collisions with **momentum conservation**  
- **Spin dynamics** and **angular momentum transfer**  
- **Continuous Collision Detection (CCD)** — prevents tunneling for fast-moving particles  
- **Damping** for realistic energy dissipation  
- Robust **boundary collision handling**, including screen corners
- --
 ![Atomic Chaos Demo](physics_engine/assets/atomic_chaos_demo.gif)


### 🌍 Planetary Motion (2D → 3D Migration)
* N-body gravitational simulation with elliptical Keplerian orbits
* Vis-viva equation implementation for realistic orbital dynamics
* Real-time visualization with orbital trails and diagnostic output
* Eight planets with configurable masses, velocities, and eccentricities
* Physics substep integration (40 steps/frame) for accuracy
* Currently 2D (SFML) - transitioning to 3D simulation next
 ![Orbital Chaos Demo](physics_engine/assets/orbital_chaos_demo.gif)
---

## 🧰 Getting Started

### Requirements
- **C++17** (or later) compiler  
- **SFML 3.0** (included in project dependencies)

### Run Instructions
All required SFML dependencies are included in the repository.  
Clone and run the project directly:

```bash
git clone https://github.com/M-Israr-Ul-Haq/physics-engine.git
cd physics-engine
# Open in your IDE and run
```
# 🎮 When you run the program, choose from the menu:
Press 1: Atomic Chaos - Particle collision and dynamics simulation 

Press 2: Orbital Chaos - Planetary motion and gravitational interactions

Press 3: Exit

---
## 🧭 Project Sturcture
```bash
physics-engine/
├── External Dependencies/        # SFML 3.0 libraries and DLLs
├── assets/                       # Demo files and media (e.g., GIFs)
├── src/                          # Core engine source code
│   ├── Atomic_Chaos/          
│   ├── Orbital_Chaos/             
│── main.cpp                      # Entry point for the simulation
└── README.md                     # Project documentation
```
---
## ⚖️ License
This is a personal learning project created for experimentation and exploration. 
Contributions and suggestions are welcome! :)
