#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define MAX_PATIENTS     100
#define MAX_DOCTORS      50
#define MAX_NURSES       50
#define MAX_NAME_LEN     49
#define MAX_PROBLEM_LEN  49
#define MAX_SPEC_LEN     49
#define MAX_DEPT_LEN     49
#define MAX_SHIFT_LEN    19
#define DATA_FILE        "hospital_data.txt"
typedef struct {
    char name[MAX_NAME_LEN + 1];
    int  age;
    char problem[MAX_PROBLEM_LEN + 1];
} Patient;

typedef struct {
    char name[MAX_NAME_LEN + 1];
    char specialization[MAX_SPEC_LEN + 1];
    int  experience;              
} Doctor;

typedef struct {
    char name[MAX_NAME_LEN + 1];
    char department[MAX_DEPT_LEN + 1];
    char shift[MAX_SHIFT_LEN + 1]; 
} Nurse;

Patient patients[MAX_PATIENTS];
int totalPatients = 0;

Doctor doctors[MAX_DOCTORS];
int totalDoctors = 0;

Nurse nurses[MAX_NURSES];
int totalNurses = 0;

/* ---------- Utility input helpers ---------- */

void trimNewline(char *s) {
    size_t len = strlen(s);
    if (len > 0 && s[len - 1] == '\n') {
        s[len - 1] = '\0';
    }
}

void readLine(const char *prompt, char *buf, size_t size) {
    printf("%s", prompt);
    if (fgets(buf, (int)size, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }
    trimNewline(buf);
}

int readIntInRange(const char *prompt, int min, int max) {
    char line[64];
    int value;
    while (1) {
        printf("%s", prompt);
        if (!fgets(line, sizeof(line), stdin)) {
            continue;
        }
        if (sscanf(line, "%d", &value) == 1 && value >= min && value <= max) {
            return value;
        }
        printf("Please enter a valid number between %d and %d.\n", min, max);
    }
}

char readYesNo(const char *prompt) {
    char line[16];
    while (1) {
        printf("%s", prompt);
        if (!fgets(line, sizeof(line), stdin)) {
            continue;
        }
        if (line[0] == 'y' || line[0] == 'Y' || line[0] == 'n' || line[0] == 'N') {
            return (char)tolower((unsigned char)line[0]);
        }
        printf("Please enter y or n.\n");
    }
}

/* ---------- File persistence ---------- */

void saveAllData(void) {
    FILE *fp = fopen(DATA_FILE, "w");
    if (!fp) {
        perror("Error opening data file for writing");
        return;
    }

    // Patients
    fprintf(fp, "PATIENTS %d\n", totalPatients);
    for (int i = 0; i < totalPatients; i++) {
        fprintf(fp, "%s|%d|%s\n",
                patients[i].name,
                patients[i].age,
                patients[i].problem);
    }

    // Doctors
    fprintf(fp, "DOCTORS %d\n", totalDoctors);
    for (int i = 0; i < totalDoctors; i++) {
        fprintf(fp, "%s|%s|%d\n",
                doctors[i].name,
                doctors[i].specialization,
                doctors[i].experience);
    }

    // Nurses
    fprintf(fp, "NURSES %d\n", totalNurses);
    for (int i = 0; i < totalNurses; i++) {
        fprintf(fp, "%s|%s|%s\n",
                nurses[i].name,
                nurses[i].department,
                nurses[i].shift);
    }

    fclose(fp);
}

void loadAllData(void) {
    FILE *fp = fopen(DATA_FILE, "r");
    if (!fp) {
        return; // no file yet
    }

    char section[32];
    int count;
    char line[256];

    totalPatients = totalDoctors = totalNurses = 0;

    while (fscanf(fp, "%31s %d\n", section, &count) == 2) {
        if (strcmp(section, "PATIENTS") == 0) {
            for (int i = 0; i < count && i < MAX_PATIENTS; i++) {
                if (!fgets(line, sizeof(line), fp)) break;
                trimNewline(line);
                char *name = strtok(line, "|");
                char *ageStr = strtok(NULL, "|");
                char *problem = strtok(NULL, "|");
                if (!name || !ageStr || !problem) continue;
                strncpy(patients[totalPatients].name, name, MAX_NAME_LEN);
                patients[totalPatients].name[MAX_NAME_LEN] = '\0';
                patients[totalPatients].age = atoi(ageStr);
                strncpy(patients[totalPatients].problem, problem, MAX_PROBLEM_LEN);
                patients[totalPatients].problem[MAX_PROBLEM_LEN] = '\0';
                totalPatients++;
            }
        } else if (strcmp(section, "DOCTORS") == 0) {
            for (int i = 0; i < count && i < MAX_DOCTORS; i++) {
                if (!fgets(line, sizeof(line), fp)) break;
                trimNewline(line);
                char *name = strtok(line, "|");
                char *spec = strtok(NULL, "|");
                char *expStr = strtok(NULL, "|");
                if (!name || !spec || !expStr) continue;
                strncpy(doctors[totalDoctors].name, name, MAX_NAME_LEN);
                doctors[totalDoctors].name[MAX_NAME_LEN] = '\0';
                strncpy(doctors[totalDoctors].specialization, spec, MAX_SPEC_LEN);
                doctors[totalDoctors].specialization[MAX_SPEC_LEN] = '\0';
                doctors[totalDoctors].experience = atoi(expStr);
                totalDoctors++;
            }
        } else if (strcmp(section, "NURSES") == 0) {
            for (int i = 0; i < count && i < MAX_NURSES; i++) {
                if (!fgets(line, sizeof(line), fp)) break;
                trimNewline(line);
                char *name = strtok(line, "|");
                char *dept = strtok(NULL, "|");
                char *shift = strtok(NULL, "|");
                if (!name || !dept || !shift) continue;
                strncpy(nurses[totalNurses].name, name, MAX_NAME_LEN);
                nurses[totalNurses].name[MAX_NAME_LEN] = '\0';
                strncpy(nurses[totalNurses].department, dept, MAX_DEPT_LEN);
                nurses[totalNurses].department[MAX_DEPT_LEN] = '\0';
                strncpy(nurses[totalNurses].shift, shift, MAX_SHIFT_LEN);
                nurses[totalNurses].shift[MAX_SHIFT_LEN] = '\0';
                totalNurses++;
            }
        } else {
            // Unknown section, skip its lines
            for (int i = 0; i < count; i++) {
                if (!fgets(line, sizeof(line), fp)) break;
            }
        }
    }

    fclose(fp);
}

/* ---------- Patient operations ---------- */

void addPatient(void) {
    if (totalPatients >= MAX_PATIENTS) {
        printf("\nHospital is full! Cannot add more patients.\n\n");
        return;
    }

    Patient p;
    readLine("\nEnter Patient Name: ", p.name, sizeof(p.name));
    if (p.name[0] == '\0') {
        strcpy(p.name, "Unknown");
    }
    p.age = readIntInRange("Enter Age (0-120): ", 0, 120);
    readLine("Enter Problem: ", p.problem, sizeof(p.problem));
    if (p.problem[0] == '\0') {
        strcpy(p.problem, "Not specified");
    }

    patients[totalPatients++] = p;
    saveAllData();
    printf("Patient added successfully!\n\n");
}

void showAllPatients(void) {
    if (totalPatients == 0) {
        printf("\nNo patients registered yet!\n\n");
        return;
    }

    printf("\n=== All Patients (%d total) ===\n", totalPatients);
    printf("%-4s %-22s %-6s %-25s\n", "No.", "Name", "Age", "Problem");
    printf("-------------------------------------------------------\n");
    for (int i = 0; i < totalPatients; i++) {
        printf("%-4d %-22s %-6d %-25s\n",
               i + 1, patients[i].name, patients[i].age, patients[i].problem);
    }
    printf("\n");
}

void viewPatientByNumber(void) {
    if (totalPatients == 0) {
        printf("\nNo patients to view!\n\n");
        return;
    }
    int no = readIntInRange("\nEnter Patient No: ", 1, totalPatients);
    int index = no - 1;
    printf("\n=== Patient Details ===\n");
    printf("No     : %d\n", no);
    printf("Name   : %s\n", patients[index].name);
    printf("Age    : %d years\n", patients[index].age);
    printf("Problem: %s\n\n", patients[index].problem);
}

void searchPatientByName(void) {
    if (totalPatients == 0) {
        printf("\nNo patients to search!\n\n");
        return;
    }
    char query[MAX_NAME_LEN + 1];
    readLine("\nEnter name (full or part, case-insensitive): ", query, sizeof(query));
    if (query[0] == '\0') {
        printf("Empty query.\n\n");
        return;
    }

    printf("\nMatches:\n");
    int found = 0;
    for (int i = 0; i < totalPatients; i++) {
        char nameLower[MAX_NAME_LEN + 1];
        char qLower[MAX_NAME_LEN + 1];

        strncpy(nameLower, patients[i].name, MAX_NAME_LEN);
        nameLower[MAX_NAME_LEN] = '\0';
        strncpy(qLower, query, MAX_NAME_LEN);
        qLower[MAX_NAME_LEN] = '\0';

        for (char *p = nameLower; *p; ++p) {
            *p = (char)tolower((unsigned char)*p);
        }
        for (char *p = qLower; *p; ++p) {
            *p = (char)tolower((unsigned char)*p);
        }

        if (strstr(nameLower, qLower) != NULL) {
            printf("%-4d %-22s %-6d %-25s\n",
                   i + 1, patients[i].name, patients[i].age, patients[i].problem);
            found = 1;
        }
    }
    if (!found) {
        printf("No patients found with that name.\n");
    }
    printf("\n");
}

void editPatient(void) {
    if (totalPatients == 0) {
        printf("\nNo patients to edit!\n\n");
        return;
    }
    int no = readIntInRange("\nEnter Patient No to edit: ", 1, totalPatients);
    int idx = no - 1;
    Patient *p = &patients[idx];

    printf("\nEditing patient %d (%s)\n", no, p->name);
    char buf[128];

    readLine("New Name (leave empty to keep same): ", buf, sizeof(buf));
    if (buf[0] != '\0') {
        strncpy(p->name, buf, MAX_NAME_LEN);
        p->name[MAX_NAME_LEN] = '\0';
    }

    readLine("New Age (leave empty to keep same): ", buf, sizeof(buf));
    if (buf[0] != '\0') {
        int newAge;
        if (sscanf(buf, "%d", &newAge) == 1 && newAge >= 0 && newAge <= 120) {
            p->age = newAge;
        } else {
            printf("Invalid age, keeping old value.\n");
        }
    }

    readLine("New Problem (leave empty to keep same): ", buf, sizeof(buf));
    if (buf[0] != '\0') {
        strncpy(p->problem, buf, MAX_PROBLEM_LEN);
        p->problem[MAX_PROBLEM_LEN] = '\0';
    }

    saveAllData();
    printf("Patient updated.\n\n");
}

void deletePatient(void) {
    if (totalPatients == 0) {
        printf("\nNo patients to delete!\n\n");
        return;
    }
    int no = readIntInRange("\nEnter Patient No to delete: ", 1, totalPatients);
    int idx = no - 1;

    printf("Deleting patient %d: %s, age %d, problem: %s\n",
           no, patients[idx].name, patients[idx].age, patients[idx].problem);
    char c = readYesNo("Are you sure? (y/n): ");
    if (c == 'y') {
        for (int i = idx; i < totalPatients - 1; i++) {
            patients[i] = patients[i + 1];
        }
        totalPatients--;
        saveAllData();
        printf("Patient deleted.\n\n");
    } else {
        printf("Operation cancelled.\n\n");
    }
}

void clearAllPatients(void) {
    if (totalPatients == 0) {
        printf("\nNo patients to clear!\n\n");
        return;
    }
    char c = readYesNo("\nClear ALL patients? (y/n): ");
    if (c == 'y') {
        totalPatients = 0;
        saveAllData();
        printf("All patient records cleared!\n\n");
    } else {
        printf("Operation cancelled.\n\n");
    }
}

/* ---------- Patient reports ---------- */

void reportByAgeGroups(void) {
    int child = 0, adult = 0, senior = 0;
    for (int i = 0; i < totalPatients; i++) {
        if (patients[i].age < 18) child++;
        else if (patients[i].age < 60) adult++;
        else senior++;
    }
    printf("\n=== Age Group Report ===\n");
    printf("Children (<18): %d\n", child);
    printf("Adults (18-59): %d\n", adult);
    printf("Seniors (60+):  %d\n\n", senior);
}

/* ---------- Doctor operations ---------- */

void addDoctor(void) {
    if (totalDoctors >= MAX_DOCTORS) {
        printf("\nDoctor list is full!\n\n");
        return;
    }

    Doctor d;
    readLine("\nEnter Doctor Name: ", d.name, sizeof(d.name));
    if (d.name[0] == '\0') {
        strcpy(d.name, "Unknown");
    }
    readLine("Enter Specialization: ", d.specialization, sizeof(d.specialization));
    if (d.specialization[0] == '\0') {
        strcpy(d.specialization, "General");
    }
    d.experience = readIntInRange("Enter Experience (years, 0-60): ", 0, 60);

    doctors[totalDoctors++] = d;
    saveAllData();
    printf("Doctor added successfully!\n\n");
}

void showAllDoctors(void) {
    if (totalDoctors == 0) {
        printf("\nNo doctors registered yet!\n\n");
        return;
    }
    printf("\n=== All Doctors (%d total) ===\n", totalDoctors);
    printf("%-4s %-22s %-20s %-10s\n", "No.", "Name", "Specialization", "Exp(yrs)");
    printf("------------------------------------------------------------\n");
    for (int i = 0; i < totalDoctors; i++) {
        printf("%-4d %-22s %-20s %-10d\n",
               i + 1, doctors[i].name, doctors[i].specialization, doctors[i].experience);
    }
    printf("\n");
}

/* ---------- Nurse operations ---------- */

void addNurse(void) {
    if (totalNurses >= MAX_NURSES) {
        printf("\nNurse list is full!\n\n");
        return;
    }

    Nurse n;
    readLine("\nEnter Nurse Name: ", n.name, sizeof(n.name));
    if (n.name[0] == '\0') {
        strcpy(n.name, "Unknown");
    }
    readLine("Enter Department/Ward: ", n.department, sizeof(n.department));
    if (n.department[0] == '\0') {
        strcpy(n.department, "General");
    }
    readLine("Enter Shift (Morning/Evening/Night): ", n.shift, sizeof(n.shift));
    if (n.shift[0] == '\0') {
        strcpy(n.shift, "Morning");
    }

    nurses[totalNurses++] = n;
    saveAllData();
    printf("Nurse added successfully!\n\n");
}

void showAllNurses(void) {
    if (totalNurses == 0) {
        printf("\nNo nurses registered yet!\n\n");
        return;
    }
    printf("\n=== All Nurses (%d total) ===\n", totalNurses);
    printf("%-4s %-22s %-20s %-15s\n", "No.", "Name", "Department", "Shift");
    printf("------------------------------------------------------------\n");
    for (int i = 0; i < totalNurses; i++) {
        printf("%-4d %-22s %-20s %-15s\n",
               i + 1, nurses[i].name, nurses[i].department, nurses[i].shift);
    }
    printf("\n");
}

/* ---------- Menu ---------- */

void showMenu(void) {
    printf("=================================================================\n");
    printf("  HOSPITAL MANAGEMENT SYSTEM  (Patients: %d, Doctors: %d, Nurses: %d)\n",
           totalPatients, totalDoctors, totalNurses);
    printf("=================================================================\n");
    printf("1.  Add Patient\n");
    printf("2.  Show All Patients\n");
    printf("3.  View Patient by Number\n");
    printf("4.  Search Patient by Name\n");
    printf("5.  Edit Patient\n");
    printf("6.  Delete Patient\n");
    printf("7.  Clear All Patients\n");
    printf("8.  Age Group Report\n");
    printf("9.  Add Doctor\n");
    printf("10. Show All Doctors\n");
    printf("11. Add Nurse\n");
    printf("12. Show All Nurses\n");
    printf("13. Exit\n");
}

/* ---------- main ---------- */

int main(void) {
    loadAllData();
    printf("Welcome to Enhanced Hospital Management System!\n\n");

    while (1) {
        showMenu();
        int choice = readIntInRange("Choice (1-13): ", 1, 13);

        switch (choice) {
            case 1:  addPatient(); break;
            case 2:  showAllPatients(); break;
            case 3:  viewPatientByNumber(); break;
            case 4:  searchPatientByName(); break;
            case 5:  editPatient(); break;
            case 6:  deletePatient(); break;
            case 7:  clearAllPatients(); break;
            case 8:  reportByAgeGroups(); break;
            case 9:  addDoctor(); break;
            case 10: showAllDoctors(); break;
            case 11: addNurse(); break;
            case 12: showAllNurses(); break;
            case 13:
                if (readYesNo("Are you sure you want to exit? (y/n): ") == 'y') {
                    printf("\nThank you for using Hospital Management System!\n");
                    return 0;
                }
                break;
        }
    }
}