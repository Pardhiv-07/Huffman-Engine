# Huffman Compression Engine 🗜️
🔴 **Live Demo:** [Click here to use the application](https://huffman-engine.onrender.com) *(Note: As this is hosted on a free tier, it may take 50 seconds to wake up on the first click).*
A high-performance file compression web application that bridges a C++ backend with a modern Python/Flask frontend.

## 🚀 Features
* **C++ Core Engine:** Implements the Huffman Coding algorithm using a custom Min-Heap and chronological tie-breaking for stable, $O(N \log N)$ tree generation.
* **Hardware-Level Packing:** Manipulates memory via bitwise operations to pack binary codes efficiently into 8-bit chunks.
* **Python/Flask Bridge:** Uses `subprocess` routing to asynchronously execute compiled C++ binaries from HTTP requests.
* **Cross-Platform:** Automated build pipeline utilizing CMake and Docker.
* **Modern UI:** Responsive, dark-themed dashboard built with HTML, CSS, and vanilla JavaScript.

## 🛠️ Tech Stack
* **Backend Engine:** C++17
* **Web Server:** Python 3, Flask, Gunicorn
* **Frontend:** HTML5, CSS3, JavaScript
* **Build Tools:** CMake, MinGW / GCC, Docker

## 🧠 Why I Built This
In modern software engineering, there is often a tradeoff between high-level web accessibility and low-level execution speed. I built this project to bridge that gap. By engineering a custom C++ backend for memory-safe, bit-level data compression and connecting it to a responsive Python/Flask frontend, this application demonstrates how to successfully decouple computationally expensive tasks into dedicated, high-performance microservices.
