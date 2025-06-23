# OpenGL Virtual Room Project

Step into a world of virtual wonders!
This project is developed using **OpenGL in C++** and showcases a captivating 3D virtual room featuring brick walls, a glass roof, mirrors, and interactive elements.

## Demo Video

[👉 Click here to watch the demo](https://www.youtube.com/watch?v=xpCOPLiZYlE)

## Installation Instructions

### 1. Install Code::Blocks

Download and install [Code::Blocks](http://www.codeblocks.org/downloads).

### 2. Setup OpenGL (GLUT)

To configure GLUT for your Code::Blocks environment:

* Copy `glut.dll` to:
  `C:\Windows\System32`

* Copy `glut.h` to:
  `CodeBlocks\MinGW\include\GL\`

* Copy `libglut32.a` to:
  `CodeBlocks\MinGW\lib\`

---

## Create a New Project in Code::Blocks

1. Open **Code::Blocks**.
2. Click **File → New → Project**.
3. Select **Empty Project** and click **Go**.
4. Click **Next >**.
5. Enter your **Project Title**, then click **Next >**.
6. Click **Finish**.

---

## Configure Project Build Settings

1. In the left **Management** panel, right-click your project.
2. Click **Build Options**.
3. Go to the **Linker Settings** tab.
4. Click **Add**, then add the following libraries one by one:

   * `opengl32`
   * `glut32`
   * `glu32`
5. Click **OK**.

---

## Run the Project

1. Open your project from:
   `Code/opengl_Assignment`
2. Press **Build and Run** (F9) to start the virtual room simulation.

---

## Requirements

* Code::Blocks with MinGW
* OpenGL libraries (GLUT)
* C++ compiler

---

