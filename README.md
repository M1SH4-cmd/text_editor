
# Text Editor Application

A simple text editor application built with Qt framework


## Features

- Open, edit and save text files
- Save files with different formats/locations (Save As)
- Font selection and size adjustment
- Basic file operations via menu and toolbar

## Code Structure

The main window class (`MainWindow`) contains:

### UI Elements
- `QTextEdit` - Main text editing area
- Toolbar with action buttons:
  - `openBtn` - Open file
  - `saveAsBtn` - Save file with new name/location
  - `saveFileBtn` - Save current file
- Font controls:
  - `fontComboBox` - Font family selection
  - `fontValue` - Font size adjustment
- Menu bar with File operations

### Core Functionality
- File operations:
  - `open()` - Open file dialog and load content
  - `saveAs()` - Save with new file name/location
  - `saveFile()` - Save to current file
- Font management through `QFontDatabase`

## Building the Project

### Prerequisites
- Qt 6
- C++ compiler with C++17 support (MinGW or MSVC)

### Build Instructions
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Usage
1. Open a text file using File → Open or toolbar button
2. Edit text content
3. Change font settings if needed
4. Save using File → Save or toolbar buttons