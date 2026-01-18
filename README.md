[![Open Github CodeSpace] (https://codespaces.new/eb546/Mini_Assignment_1)]

# 🧮 C Programming Utility Menu

A menu-driven C program that demonstrates core programming concepts through multiple interactive utilities. This project allows users to select from several mathematical and logical operations, all organized using functions, loops, and control structures.

Designed as a foundational C programming exercise, the program emphasizes **modular design**, **user input handling**, and **control flow management**.

---

## 📌 Project Overview

The **C Programming Utility Menu** is a console-based application written in C that presents users with a main menu of options. Each menu item executes a specific program module, allowing users to perform different calculations and checks without restarting the application.

This project is ideal for showcasing:

* Function-based program structure
* Menu-driven interfaces
* Loop control and condition handling
* Basic mathematical logic and character evaluation

---

## ✨ Features

### 🔁 Menu-Driven Interface

* Continuous loop until the user chooses to exit
* Clear numbered options for easy navigation
* Switch-case logic for organized control flow

### 🟢 Circle Circumference Calculator

* Computes the circumference using the formula:
  **2 × π × radius**
* Accepts floating-point radius values
* Allows repeated calculations until the user exits

### 📅 Leap Year Checker

* Determines whether a given year is a leap year
* Uses standard leap year rules:

  * Divisible by 400 → Leap year
  * Divisible by 100 → Not a leap year
  * Divisible by 4 → Leap year
* Supports continuous input until exit

### ➕ Sum of Natural Numbers

* Calculates the sum from 1 to a user-provided number
* Uses a `for` loop for iteration
* Demonstrates accumulation logic

### 🔤 Vowel Checker

* Identifies whether an input character is a vowel
* Supports both uppercase and lowercase letters
* Uses logical operators for efficient comparison
* Loop continues until a sentinel character is entered

---

## 🛠 Technologies Used

* **C Programming Language**
* **Standard Input/Output Library (`stdio.h`)**
* Compilable with **GCC** or any standard C compiler

---

## ▶️ How to Compile and Run

### 1️⃣ Compile the Program

```bash
gcc menu_program.c -o menu_program
```

### 2️⃣ Run the Executable

```bash
./menu_program
```

---

## 🧠 Concepts Demonstrated

* Functions and function prototypes
* `do-while`, `for`, and conditional loops
* `switch-case` statements
* User input via `scanf`
* Floating-point and integer calculations
* Character handling and logical operators
* Program flow control and modular design

---

## 📂 Program Structure

```
menu_program.c
├── main()
│   └── Menu selection logic
├── Circle_Circumference()
├── Leap_Year()
├── Sum_of_Natural_Numbers()
└── Vowel_Checker()
```

---

## 🎯 Intended Use

* Academic projects and programming exercises
* Practice with procedural programming in C
* Demonstration of foundational coding skills
* Entry-level portfolio projects

---

## 🚀 Possible Enhancements

* Input validation for invalid characters or numbers
* Additional mathematical utilities
* Improved output formatting
* Refactoring repetitive loops into reusable helpers

---

## 📄 License

This project is intended for educational purposes and personal learning. Free to modify and extend.

---

**Built to strengthen C fundamentals, one menu option at a time.**

