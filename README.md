# C-Major-Project
A C program for Hospital Management System
# 🏥 Hospital Management System in C

A console-based application to manage patient, doctor, and nurse records efficiently.  
This project demonstrates structured programming in C, file persistence, and modular design.

---

## 📖 Overview
The Hospital Management System provides a menu-driven interface for handling hospital records.  
It supports adding, viewing, editing, searching, and deleting patient records, as well as managing doctors and nurses.  
Data is stored persistently in a text file (`hospital_data.txt`).

---

## ⚙️ Features

### Patient Operations
- Add new patient records
- View all patients
- View patient by number
- Search patient by name (case-insensitive)
- Edit patient details
- Delete patient records
- Clear all patient records
- Generate age group reports (Children, Adults, Seniors)

### Doctor Operations
- Add new doctor records
- View all doctors

### Nurse Operations
- Add new nurse records
- View all nurses

---

## 🛠️ Technical Details
- **Language**: C
- **Data Structures**: `struct` for Patient, Doctor, Nurse
- **Storage**: Fixed-size arrays with file persistence (`hospital_data.txt`)
- **Input Validation**: Functions like `readIntInRange` and `readYesNo` ensure safe input
- **Design**: Modular functions for each operation
- **Interface**: Menu-driven console application

---

## ✅ Advantages
- Data persistence between sessions
- Structured design with clear modularity
- Simple and intuitive interface

---

## ⚠️ Limitations
- Fixed-size arrays limit scalability
- Reporting limited to age groups
- No dynamic memory allocation

---

## 🚀 Future Enhancements
- Replace arrays with dynamic memory (linked lists)
- Advanced reporting (disease statistics, staff workload)
- Graphical User Interface (GUI)
- Database integration (e.g., MySQL)

---

## 📂 How to Run
1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/hospital-management-system.git
