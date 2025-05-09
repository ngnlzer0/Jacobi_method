# 🔢 Jacobi Method Solver

Implementation of **single-threaded** and **multi-threaded** Jacobi method in C++.  
Includes performance analysis and visualizations 📈

---

Description of the Jacobi algorithm on [**Wiki**](https://en.wikipedia.org/wiki/Jacobi_method)

---

## 📌 Features

- ✅ Classic **Jacobi method** (single-threaded)
- ⚡ **Multithreaded** Jacobi using `OpenMP`
- 📄 **DOT file** generation for graph data
- 📊 **Gnuplot** integration: build `.png` charts from results
- 🌐 HTML table report generation from results

---

There are small problems in the multi-threaded implementation, 
if the deviation E <= 0.0005 is too small, the program starts to work very slowly, 
especially on small matrices, with larger matrices there +- works fine, 
so it is recommended to use E >= 0.001 for multi-threaded implementation


the results of the methods in .dot formats, graphical diagrams, html tables can be seen in the 
[“RESULT”](https://github.com/ngnlzer0/Jacobi_method/tree/main/results) file
or results are drawn up in 1 file 
[**results_summary**](https://github.com/ngnlzer0/Jacobi_method/blob/main/results/results_summary.md)

---

## 📄 Documentation

This project uses **Doxygen** to generate C++ API documentation.  

📚 View the full documentation here:  
👉 [**Jacobi Method Docs on GitHub Pages**](https://ngnlzer0.github.io/Jacobi_method/)

---

To generate locally:

```bash

doxygen Doxyfile

## 🛠️ Build Instructions

```bash
git clone https://github.com/ngnlzer0/Jacobi_method.git
cd Jacobi_method
mkdir build
cd build
cmake ..
make
