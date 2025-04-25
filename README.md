# 🔢 Jacobi Method Solver

Implementation of **single-threaded** and **multi-threaded** Jacobi method in C++.  
Includes performance analysis and visualizations 📈

---

## 📌 Features

- ✅ Classic **Jacobi method** (single-threaded)
- ⚡ **Multithreaded** Jacobi using `OpenMP`
- 📄 **DOT file** generation for graph data
- 📊 **Gnuplot** integration: build `.png` charts from results
- 🌐 HTML table report generation from results

---

## 📄 Documentation

This project uses **Doxygen** to generate C++ API documentation.  

📚 View the full documentation here:  
👉 [**Jacobi Method Docs on GitHub Pages**](https://ngnlzer0.github.io/Jacobi_method/)

To generate locally:

```bash

---

doxygen Doxyfile

## 🛠️ Build Instructions

```bash
git clone https://github.com/your_username/Jacobi_method.git
cd Jacobi_method
mkdir build
cd build
cmake ..
make
