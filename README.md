# Multithreaded management for matrix multiplication


### **Description**

This project implements matrix multiplication using parallel programming in C++.
Each element `C[i][j]` of the resulting matrix `C` is calculated by a separate thread.

---

### **Directory Structure**

```
/src/          → Source code (main.cpp, Makefile, bootstrap.sh)
/matrices/     → Matrix input and output files (A, B, C)
.gitignore     → Files/Settings ignored by Git
```

---

### **Input File Format**

Each line represents a row of the matrix, with elements separated by spaces.

**Example - A:**

```
1 4
2 5
3 6
```

**Example - B:**

```
8 7 6
5 4 3
```

---

**Example - C (output)**

```
28 23 18 
41 34 27 
54 45 36 
```

### **Compilation**

From the `/src` directory:

```bash
make
```

---

### **Execution**

From the project root directory:

```bash
chmod +x bootstrap.sh
./bootstrap.sh
```

The **`bootstrap.sh`** script:

* Defines the paths to files `A` and `B` in the `/matrices` folder.
* Compiles the program via `make`.
* Runs the binary `./program` passing the files as arguments.

---

### **Output**

* Resulting matrix printed to the terminal.
* Matrix result written to output file.
* Success message after freeing resources.

---

### **Dependencies**

* C++11 or later compiler.
* `make`.
* `bash`.

---

### **Notes**

* Matrix A must have dimensions M x K.
* Matrix B must have dimensions K x N.
* Result: matrix C with dimensions M x N.
* A **thread** is created for each element of `C`.

---

### **Quick Commands**

```bash
cd src
make
./bootstrap.sh
```
