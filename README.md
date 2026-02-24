# L-Systems, Turtle Graphics & Image Processing (C)

A modular C application that combines:

- L-System generation
- Turtle graphics rendering
- Custom image processing
- Font parsing and text rendering
- Undo / Redo state management

Built as a systems programming project focused on low-level memory management and algorithm implementation.

---

## Architecture

The core of the application is a global structure called `all_states`.

It manages:

- The history of application states
- The total number of stored states
- A redo stack

Whenever a new command is executed:

1. The previous state is copied
2. Changes are applied to the new state
3. The command is stored (used for UNDO/REDO)

### Undo / Redo

- `UNDO` removes the current state and moves its command to the redo stack.
- `REDO` re-executes the last stored command through the CLI handler.

This creates a lightweight state versioning system inside the application.

---

## L-System Engine

An L-System is defined by:

- An **axiom**
- A set of **production rules**

The `derive()` function:

- Applies rules iteratively
- Dynamically builds the resulting string
- Manages memory allocation manually

The resulting string can be printed or used by the turtle renderer.

---

## Turtle Graphics Renderer

The derived string is interpreted using turtle graphics commands:

| Symbol | Action                 |
| ------ | ---------------------- |
| `F`    | Move forward and draw  |
| `+`    | Rotate right           |
| `-`    | Rotate left            |
| `[`    | Save current state     |
| `]`    | Restore previous state |

A custom stack structure stores position and orientation.

Lines are drawn using a manually implemented **Bresenham line algorithm**.

---

## Font Rendering

- A BDF font file is parsed
- Characters are loaded into memory
- The `TYPE` command renders text onto an image
- Rendering is done at pixel level

---

## Engraving Validation

The program analyzes the binary representation of image pixels.

If the bit sequences:

- `1101`
- `0010`

are detected within a pixel’s bit pattern, an error message is displayed indicating which color component might be incorrect.

---

## Technical Highlights

- Dynamic memory management (`malloc`, `free`)
- Pointer manipulation
- Custom stack implementation
- String processing
- Bit-level operations
- Raster graphics rendering
- Modular C project organization
