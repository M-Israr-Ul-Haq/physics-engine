# 🐧 Linux Setup Guide

This guide will help you set up and build the Physics Engine on Linux/WSL Ubuntu.

---

## 📋 Prerequisites

- Ubuntu 20.04 or later (or WSL2 with Ubuntu)
- Basic familiarity with terminal commands
- Git installed

---

## 🚀 Installation Steps

### 1. Update System and Install Build Tools
```bash
sudo apt update && sudo apt upgrade -y
sudo apt install -y build-essential cmake git
```

### 2. Install SFML 3.0 Dependencies
```bash
sudo apt install -y \
    libx11-dev \
    libxrandr-dev \
    libxcursor-dev \
    libxi-dev \
    libudev-dev \
    libgl1-mesa-dev \
    libfreetype6-dev \
    libopenal-dev \
    libflac-dev \
    libvorbis-dev
```

### 3. Build and Install SFML 3.0

Since SFML 3.0 is relatively new, we need to build it from source:
```bash
# Clone SFML 3.0
cd ~
git clone -b 3.0.0 https://github.com/SFML/SFML.git SFML-3.0
cd SFML-3.0

# Build SFML with shared libraries
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON
make -j$(nproc)
sudo make install

# Update library cache
sudo ldconfig
```

This will take a few minutes. The `-j$(nproc)` flag uses all CPU cores to speed up compilation.

### 4. Clone the Physics Engine Repository
```bash
cd ~  # or navigate to your preferred projects directory
git clone https://github.com/M-Israr-Ul-Haq/physics-engine.git
cd physics-engine
```

### 5. Configure Git (for contributing)
```bash
git config --global user.name "Your Name"
git config --global user.email "your-email@example.com"
```

For pushing changes, you'll need a GitHub Personal Access Token:
- Go to GitHub → Settings → Developer settings → Personal access tokens → Tokens (classic)
- Generate new token with `repo` permissions
- Use the token as your password when pushing

### 6. Build the Project
```bash
# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build the project
make -j$(nproc)
```

### 7. Run the Physics Engine
```bash
./physics_engine
```

You'll see a menu:
```
=== 2D Physics Engine ===
1. Atomic Chaos (Particle Collision)
2. Orbital Chaos (Gravity Simulation)
3. Pendulum Chaos (Double Pendulum)
4. Exit
```

---

## 🖥️ Graphics Display (WSL Users)

If you're using WSL and don't see graphics windows:

### Option 1: WSLg (Windows 11)
- WSLg comes pre-installed on Windows 11
- Graphics should work automatically

### Option 2: X Server (Windows 10)
1. Install an X server on Windows:
   - [VcXsrv](https://sourceforge.net/projects/vcxsrv/)
   - [X410](https://x410.dev/) (paid)
   
2. Launch the X server with "Disable access control" enabled

3. In WSL, set the DISPLAY variable:
```bash
   export DISPLAY=$(cat /etc/resolv.conf | grep nameserver | awk '{print $2}'):0
```
   
4. Add to `~/.bashrc` to make it permanent:
```bash
   echo 'export DISPLAY=$(cat /etc/resolv.conf | grep nameserver | awk '{print $2}):0' >> ~/.bashrc
```

---

## 🔧 Development Workflow

### Making Changes
```bash
# Create a new branch
git checkout -b feature-name

# Make your changes to the code...

# Rebuild after changes
cd build
make -j$(nproc)

# Test your changes
./physics_engine
```

### Committing and Pushing
```bash
# Stage your changes
git add .

# Commit with a descriptive message
git commit -m "Add feature: description"

# Push to GitHub
git push origin feature-name
```

### Rebuilding After Code Changes

You typically only need to run `make` again:
```bash
cd build
make -j$(nproc)
```

Only re-run `cmake ..` if you:
- Modified `CMakeLists.txt`
- Added/removed source files
- Changed project structure

---

## 🐛 Common Issues

### Issue: "Setting vertical sync not supported"
**Solution:** This is a warning, not an error. The simulation will still run. It occurs because WSL has limited GPU access.

### Issue: Cannot find SFML libraries
**Solution:** Make sure you ran `sudo ldconfig` after installing SFML.

### Issue: Graphics window doesn't open (WSL)
**Solution:** See the "Graphics Display (WSL Users)" section above.

### Issue: Permission denied when pushing to GitHub
**Solution:** Use a Personal Access Token instead of your password when prompted.

---

## 📁 Project Structure
```
physics-engine/
├── build/                        # Build directory (generated)
│   └── physics_engine           # Executable
├── dependencies/                 # Windows SFML binaries (not used on Linux)
├── physics_engine/              # Source code
│   └── src/
│       ├── Atomic_Chaos/       # Particle collision simulation
│       ├── Orbital_Chaos/      # Gravity simulation
│       └── Pendulum_Chaos/     # Double pendulum simulation
├── CMakeLists.txt              # Build configuration
└── README.md                   # Main documentation
```

---

## ✅ Verification

To verify everything is working:
```bash
cd ~/physics-engine/build
./physics_engine
```

Try each simulation option (1, 2, 3) to ensure they all work.

---

## 🆘 Getting Help

If you encounter issues:
1. Check the error message carefully
2. Verify all dependencies are installed
3. Make sure SFML 3.0 installed successfully
4. Open an issue on GitHub with details about your error

---

**Happy Coding! 🚀**
