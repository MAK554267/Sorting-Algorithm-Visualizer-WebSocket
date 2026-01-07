<div align="center">

# 🔢 Sorting Algorithm Visualizer (WebSocket Based)

**🎓 Developed for Advanced Data Structures & Algorithms Learning**

*Real-time visualization of sorting algorithms using C++ WebSocket server and web frontend*

[![University](https://img.shields.io/badge/Institution-University_of_Wah-blue.svg)](#)
[![Department](https://img.shields.io/badge/Department-Cybersecurity-purple.svg)](#)
[![Course](https://img.shields.io/badge/Course-Data_Structures_&_Algorithms-teal.svg)](#)
[![Semester](https://img.shields.io/badge/Semester-3rd-green.svg)](#)
[![Project](https://img.shields.io/badge/Type-Visualization_Project-orange.svg)](#)

---

**"Visualize. Understand. Master Sorting Algorithms."**

⭐ *Star this repository if it helped your DSA learning journey!*

</div>

---

## 📌 Project Overview

This project is a **real-time sorting algorithm visualizer** built using a **C++ WebSocket server (Boost.Beast)** and a **web-based frontend (HTML, CSS, JavaScript)**.

The system allows users to **visualize how sorting algorithms work step-by-step**, helping students better understand algorithm behavior and complexity.

---

## ⚙️ Technologies Used

### 🔹 Backend
- **C++**
- **Boost.Asio & Boost.Beast**
- **WebSocket Communication**
- **nlohmann/json**

### 🔹 Frontend
- **HTML5**
- **CSS3**
- **JavaScript**
- **Canvas API**

---

## 🔁 Implemented Sorting Algorithms

- ✅ Bubble Sort
- ✅ Quick Sort
- ✅ Insertion Sort
- ✅ Selection Sort
- ✅ Merge Sort

Each algorithm sends **real-time sorting steps** from the server to the browser.

---

## 🧠 Features

- Real-time sorting visualization
- WebSocket-based client-server communication
- Algorithm selection from UI
- Start / Pause / Reset controls
- Clean and responsive UI
- Educational & beginner-friendly

---


## ▶️ Step-by-Step Guide to Run the Project

This project consists of **two parts**:
1. **Backend (C++ WebSocket Server)**
2. **Frontend (Web-based Visualization)**

Follow the steps below carefully.

---

### 🔹 Step 1: Prerequisites

Make sure the following are installed on your system:

- ✅ **Windows OS**
- ✅ **Visual Studio 2022**
- ✅ **Git**
- ✅ **vcpkg (C++ package manager)**
- ✅ **Modern Browser (Chrome / Edge)**

---

### 🔹 Step 2: Install Required C++ Libraries (One Time Only)

Open **PowerShell as Administrator** and run:

```powershell
cd C:\vcpkg
.\vcpkg install boost
.\vcpkg install nlohmann-json
.\vcpkg integrate install
```
This installs:
 - **Boost.Asio**
 - **Boost.Beast**
 - **nlohmann/json**

### 🔹 Step 3: Run the Backend Server (C++)

1. **Open Visual Studio 2022**
2. **Open the project folder**
3. **Make sure the configuration is set to:**

   ```bash
   Debug | x64
   ```
4. Build the project:

   ```bash
   Build → Rebuild Solution
   ``` 
6. Run the server:
   ```bash
   ▶ Local Windows Debugger
   ```
If successful, you will see:
 ```bash
 WebSocket server is running on port 12345...
 ```

⚠️ **Keep the server window open**

---

### 🔹 Step 4: Run the Frontend (Browser)

1. Navigate to the frontend folder
2. Open `index.html` in **Chrome** or **Edge**
3. The UI will load automatically

---

### 🔹 Step 5: Use the Visualizer

1. Select a sorting algorithm:
   - Bubble Sort  
   - Quick Sort  
   - Insertion Sort  
   - Selection Sort  
   - Merge Sort  
2. Click **Start**
3. Watch the sorting process step-by-step
4. Use **Pause** or **Reset** if needed

---

### ✅ Output

- Sorting bars animate in real time  
- Server sends sorting steps via WebSocket  
- Visualization updates instantly in the browser  

---

<div align="center">

**🎓 Developed for Advanced Data Structures & Algorithms Learning**

*Understanding sorting algorithms through real-time visualization and hands-on implementation*

[![University](https://img.shields.io/badge/Institution-University_of_Wah-blue.svg)](#)
[![Department](https://img.shields.io/badge/Department-Cybersecurity-purple.svg)](#)
[![Course](https://img.shields.io/badge/Course-Data_Structures_&_Algorithms-teal.svg)](#)
[![Semester](https://img.shields.io/badge/Semester-3rd-green.svg)](#)
[![Project](https://img.shields.io/badge/Type-Visualization_Project-orange.svg)](#)

---

**"Visualize. Understand. Master Sorting Algorithms."**

**⭐ Star this repository if it helped your DSA learning journey!**

</div>
