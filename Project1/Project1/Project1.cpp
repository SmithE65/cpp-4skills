// Project1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

import PatientQueue;

// forward declarations
void print_menu();
void add_patient(Hospital::HospitalReception &hospital);
void print_patients(Hospital::HospitalReception &hospital);
void get_next_patient(Hospital::HospitalReception &hospital);

int main()
{
    Hospital::HospitalReception hospital{};

    while (true)
    {
        print_menu();
        int input{ 0 };
        std::cin >> input;
        if (input == 4) break;

        switch (input)
        {
        case 1:
            add_patient(hospital);
            break;
        case 2:
            print_patients(hospital);
            break;
        case 3:
            get_next_patient(hospital);
            break;
        default:
            std::cout << "Invalid input." << std::endl;
            break;
        }
    }
}

void print_menu()
{
    std::cout << "Enter your choice:" << std::endl
        << "1) Add new patient" << std::endl
        << "2) Print all patients" << std::endl
        << "3) Get next patient" << std::endl
        << "4) Exit" << std::endl;
}

void add_patient(Hospital::HospitalReception &hospital)
{
    // make it look like the PDF
    std::cout << "Enter specialization number, name, and status (0/1): ";
    int spec{ 0 };
    std::string name;
    int status{ 0 };
    std::cin >> spec >> name >> status;

    hospital.AddPatient(spec, name, static_cast<Hospital::PatientStatus>(status));
}

void print_patients(Hospital::HospitalReception& hospital)
{
    hospital.Write(std::cout);
}

void get_next_patient(Hospital::HospitalReception& hospital)
{
    std::cout << "Enter specialization: ";
    int spec{ 0 };
    std::cin >> spec;

    if (!hospital.HasPatients(spec))
    {
        std::cout << "No patients at the moment. Have a rest, Dr." << std::endl;
        return;
    }

    auto p = hospital.GetNextPatient(spec);
    std::cout << p.Name << " please go with the Dr." << std::endl;
}
