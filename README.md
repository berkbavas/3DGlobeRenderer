# 3D Globe Renderer

**3D Globe Renderer** is a C++ application for rendering interactive 3D globes, built with OpenGL and Qt.

## Project Structure

```text
3DGlobeRenderer/
├── Source/
│   ├── Main.cpp
│   ├── Core/         # Application core logic (window, controller, events)
│   ├── Node/         # Scene graph nodes (Globe, Space, Sphere, etc.)
│   ├── Renderer/     # Rendering engine, shaders, textures
│   ├── Structs/      # Data structures (Mouse, Texture, etc.)
│   └── Util/         # Utilities (Logger, Math, etc.)
├── Resources/        # Shaders, textures, heightmaps
├── Libs/             # External libraries (e.g., qtimgui)
├── build/            # Build output (ignored in VCS)
├── CMakeLists.txt    # CMake build configuration
└── README.md         # Project documentation
```

## Prerequisites

- **CMake** >= 3.25.1
- **Visual Studio 2022** with MSVC C++ v143 Compiler
- **Qt 6.7.3** (MSVC2022 64bit)

## Setup Instructions

1. Install the required tools and libraries listed above.
2. Set the environment variable `Qt6_DIR` to your Qt installation path, e.g.:

	```powershell
	$env:Qt6_DIR = "C:\Qt\6.7.3\msvc2022_64"
	```

3. Clone the repository:

	```sh
	git clone https://github.com/berkbavas/3DGlobeRenderer.git
	```

4. Create and enter a build directory:

	```sh
	mkdir Build
	cd Build
	```

5. Generate project files with CMake:

	```sh
	cmake ..
	```

6. Open `3DGlobeRenderer.sln` in Visual Studio 2022.
7. Build and run the project in **Release** configuration.

## Demo

Watch a demo video:  
[3D Globe Renderer Demo](https://github.com/user-attachments/assets/4df2122d-be62-48bf-8dea-97887d2e4332)

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
