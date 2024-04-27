#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;

enum Specialization { COMPUTER_SCIENCE, INFORMATICS, MATH_ECONOMICS, PHYSICS_INFORMATICS, LABOR_TRAINING };

union Grade {
    int programming;
    int numerical_methods;
    int pedagogy;
};

struct Student {
    string surname;
    int course;
    Specialization specialization;
    int physics_grade;
    int math_grade;
    Grade additional_grade;
};

void CreateStudents(Student* students, int N);
void CalculateGrades(const Student* students, int N, int& count5, int& count4, int& count3);
double CalculatePercentage(const Student* students, int N);
void PrintStudents(const Student* students, int N);
void WriteStudents(const string& filename, const Student* students, int N);
void ReadStudents(const string& filename);

int main() {
    string filename;
    cout << "Enter filename for output: ";
    cin >> filename;
    
    int N;
    cout << "Enter the number of students: ";
    cin >> N;

    Student* students = new Student[N];

    CreateStudents(students, N);

    WriteStudents(filename, students, N);

    cout << "\nReading students' data from file:\n";
    ReadStudents(filename);

    int count5 = 0, count4 = 0, count3 = 0;
    CalculateGrades(students, N, count5, count4, count3);
    cout << "Count of '5' grades in programming: " << count5 << endl;
    cout << "Count of '4' grades in programming: " << count4 << endl;
    cout << "Count of '3' grades in programming: " << count3 << endl;

    double percentage = CalculatePercentage(students, N);
    cout << "Percentage of students with '4' or '5' in Physics and Math: " << percentage << "%" << endl;

    delete[] students;
    return 0;
}

void CreateStudents(Student* students, int N) {
    for (int i = 0; i < N; ++i) {
        cout << "Student #" << i + 1 << ":\n";
        cout << "Surname: ";
        cin >> students[i].surname;
        cout << "Course: ";
        cin >> students[i].course;
        cout << "Specialization (0 - Computer Science, 1 - Informatics, 2 - Math and Economics, 3 - Physics and Informatics, 4 - Labor Training): ";
        int spec;
        cin >> spec;
        students[i].specialization = static_cast<Specialization>(spec);
        cout << "Grade in Physics: ";
        cin >> students[i].physics_grade;
        cout << "Grade in Mathematics: ";
        cin >> students[i].math_grade;
        switch (students[i].specialization) {
        case COMPUTER_SCIENCE:
            cout << "Programming grade: ";
            cin >> students[i].additional_grade.programming;
            break;
        case INFORMATICS:
            cout << "Numerical methods grade: ";
            cin >> students[i].additional_grade.numerical_methods;
            break;
        default:
            cout << "Pedagogy grade: ";
            cin >> students[i].additional_grade.pedagogy;
            break;
        }
    }
}

void CalculateGrades(const Student* students, int N, int& count5, int& count4, int& count3) {
    count5 = count4 = count3 = 0;
    for (int i = 0; i < N; ++i) {
        if (students[i].specialization == COMPUTER_SCIENCE) {
            int grade = students[i].additional_grade.programming;
            if (grade == 5) count5++;
            else if (grade == 4) count4++;
            else if (grade == 3) count3++;
        }
    }
}

double CalculatePercentage(const Student* students, int N) {
    int count = 0;
    for (int i = 0; i < N; ++i) {
        if ((students[i].physics_grade == 4 || students[i].physics_grade == 5) &&
            (students[i].math_grade == 4 || students[i].math_grade == 5)) {
            count++;
        }
    }
    return static_cast<double>(count) / N * 100;
}

void PrintStudents(const Student* students, int N) {
    cout << "=======================================================================================================" << endl;
    cout << "| # | Surname       | Course | Specialization               | Physics | Mathematics | Additional Grade |" << endl;
    cout << "-------------------------------------------------------------------------------------------------------" << endl;
    for (int i = 0; i < N; ++i) {
        cout << "| " << setw(2) << right << i + 1 << " | ";
        cout << setw(13) << left << students[i].surname << " | ";
        cout << setw(6) << right << students[i].course << " | ";
        string specName;
        switch (students[i].specialization) {
        case COMPUTER_SCIENCE:
            specName = "Computer Science";
            cout << setw(28) << left << specName << " | ";
            cout << setw(7) << right << students[i].physics_grade << " | ";
            cout << setw(11) << right << students[i].math_grade << " | ";
            cout << setw(16) << right << students[i].additional_grade.programming << " |" << endl;
            break;
        case INFORMATICS:
            specName = "Informatics";
            cout << setw(28) << left << specName << " | ";
            cout << setw(7) << right << students[i].physics_grade << " | ";
            cout << setw(11) << right << students[i].math_grade << " | ";
            cout << setw(16) << right << students[i].additional_grade.numerical_methods << " |" << endl;
            break;
        default:
            specName = (students[i].specialization == MATH_ECONOMICS) ? "Math and Economics" :
                (students[i].specialization == PHYSICS_INFORMATICS) ? "Physics and Informatics" : "Labor Training";
            cout << setw(28) << left << specName << " | ";
            cout << setw(7) << right << students[i].physics_grade << " | ";
            cout << setw(11) << right << students[i].math_grade << " | ";
            cout << setw(16) << right << students[i].additional_grade.pedagogy << " |" << endl;
            break;
        }
    }
    cout << "=======================================================================================================" << endl;
}

void WriteStudents(const string& filename, const Student* students, int N) {
    ofstream file(filename, ios::binary | ios::out);
    if (!file) {
        cerr << "Error opening file for writing." << endl;
        return;
    }

    file.write(reinterpret_cast<const char*>(&N), sizeof(N));

    for (int i = 0; i < N; ++i) {
        size_t length = students[i].surname.length();
        file.write(reinterpret_cast<const char*>(&length), sizeof(length));
        file.write(students[i].surname.c_str(), length);

        file.write(reinterpret_cast<const char*>(&students[i].course), sizeof(students[i].course));
        file.write(reinterpret_cast<const char*>(&students[i].specialization), sizeof(students[i].specialization));
        file.write(reinterpret_cast<const char*>(&students[i].physics_grade), sizeof(students[i].physics_grade));
        file.write(reinterpret_cast<const char*>(&students[i].math_grade), sizeof(students[i].math_grade));

        switch (students[i].specialization) {
        case COMPUTER_SCIENCE:
            file.write(reinterpret_cast<const char*>(&students[i].additional_grade.programming), sizeof(students[i].additional_grade.programming));
            break;
        case INFORMATICS:
            file.write(reinterpret_cast<const char*>(&students[i].additional_grade.numerical_methods), sizeof(students[i].additional_grade.numerical_methods));
            break;
        default:
            file.write(reinterpret_cast<const char*>(&students[i].additional_grade.pedagogy), sizeof(students[i].additional_grade.pedagogy));
            break;
        }
    }

    file.close();
}

void ReadStudents(const string& filename) {
    ifstream file(filename, ios::binary | ios::in);
    if (!file) {
        cerr << "Error opening file for reading." << endl;
        return;
    }

    int N;
    file.read(reinterpret_cast<char*>(&N), sizeof(N));
    Student* students = new Student[N];

    for (int i = 0; i < N; ++i) {
        size_t length;
        file.read(reinterpret_cast<char*>(&length), sizeof(length));
        students[i].surname.resize(length);
        file.read(&students[i].surname[0], length);

        file.read(reinterpret_cast<char*>(&students[i].course), sizeof(students[i].course));
        file.read(reinterpret_cast<char*>(&students[i].specialization), sizeof(students[i].specialization));
        file.read(reinterpret_cast<char*>(&students[i].physics_grade), sizeof(students[i].physics_grade));
        file.read(reinterpret_cast<char*>(&students[i].math_grade), sizeof(students[i].math_grade));

        switch (students[i].specialization) {
        case COMPUTER_SCIENCE:
            file.read(reinterpret_cast<char*>(&students[i].additional_grade.programming), sizeof(students[i].additional_grade.programming));
            break;
        case INFORMATICS:
            file.read(reinterpret_cast<char*>(&students[i].additional_grade.numerical_methods), sizeof(students[i].additional_grade.numerical_methods));
            break;
        default:
            file.read(reinterpret_cast<char*>(&students[i].additional_grade.pedagogy), sizeof(students[i].additional_grade.pedagogy));
            break;
        }
    }

    PrintStudents(students, N);

    file.close();
}
