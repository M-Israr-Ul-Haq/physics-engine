# ⚛️ Physics Engine  
*A lightweight 2D physics simulation engine built with C++ and SFML 3.0.*

![Atomic Chaos Demo](assets/atomic_chaos_demo.gif)

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

### 🌍 Planetary Motion *(In Development)*
- Gravitational interactions between celestial bodies  
- Orbital mechanics simulation based on Newtonian gravity  
- Configurable body masses, velocities, and gravitational constants  

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
This is a personal project created for learning and experimentation. 
Feel free to contribute :)
