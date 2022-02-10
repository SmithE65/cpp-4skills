// Project1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

// definitions
constexpr auto NUM_SPECIALIZATIONS = 20;
constexpr auto MAX_QUEUE = 5;
constexpr auto MAX_PATIENTS = NUM_SPECIALIZATIONS * MAX_QUEUE;

// forward declarations
void print_menu();
void add_patient(std::string names[], int patient_status[], int queue_lengths[]);
void print_patients(std::string names[], int patient_status[], int queue_lengths[]);
void get_next_patient(std::string names[], int patient_status[], int queue_lengths[]);

int main()
{
    // only using what's been covered in the course
    std::string names[MAX_PATIENTS];
    int status[MAX_PATIENTS]{};
    int queue_lengths[NUM_SPECIALIZATIONS]{};

    while (true)
    {
        print_menu();
        int input{ 0 };
        std::cin >> input;
        if (input == 4) break;

        switch (input)
        {
        case 1:
            add_patient(names, status, queue_lengths);
            break;
        case 2:
            print_patients(names, status, queue_lengths);
            break;
        case 3:
            get_next_patient(names, status, queue_lengths);
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

bool check_specialization(int spec)
{
    return spec >= 1 && spec <= NUM_SPECIALIZATIONS;
}

bool check_status(int status)
{
    return status == 0 || status == 1;
}

bool check_queue(int spec, int queue_lengths[])
{
    return queue_lengths[spec-1] < MAX_QUEUE;
}

int decrement_queue(int spec, int queue_lengths[])
{
    auto idx = spec - 1;
    auto val = queue_lengths[idx];
    val--;
    if (val < 0) val = 0;
    queue_lengths[idx] = val;
    return val;
}

int get_queue(int spec, int queue_lengths[])
{
    return queue_lengths[spec - 1];
}

int increment_queue(int spec, int queue_lengths[])
{
    auto idx = spec - 1;
    auto val = queue_lengths[idx];
    val++;
    if (val >= MAX_QUEUE) val = MAX_QUEUE;
    queue_lengths[idx] = val;
    return val;
}

std::string get_name(int spec, int index, std::string names[])
{
    return names[MAX_QUEUE * (spec - 1) + index];
}

int get_status(int spec, int index, int patient_status[])
{
    return patient_status[MAX_QUEUE * (spec - 1) + index];
}

void set_name(std::string name, int spec, int index, std::string names[])
{
    names[MAX_QUEUE * (spec - 1) + index] = name;
}

void set_status(int status, int spec, int index, int patient_status[])
{
    patient_status[MAX_QUEUE * (spec - 1) + index] = status;;
}

void push(std::string name, int status, int spec, std::string names[], int patient_status[], int queue_lengths[])
{
    int index = increment_queue(spec, queue_lengths);
    set_name(name, spec, index - 1, names);
    set_status(status, spec, index - 1, patient_status);
}

void push_front(std::string name, int status, int spec, std::string names[], int patient_status[], int queue_lengths[])
{
    int index = increment_queue(spec, queue_lengths);

    if (index == 1)
    {
        set_name(name, spec, 0, names);
        set_status(status, spec, 0, patient_status);
        return;
    }

    for (int i = index - 1; i > 0; i--)
    {
        auto n = get_name(spec, i - 1, names);
        auto s = get_status(spec, i - 1, patient_status);
        set_name(n, spec, i, names);
        set_status(s, spec, i, patient_status);
    }

    set_name(name, spec, 0, names);
    set_status(status, spec, 0, patient_status);
}

std::string pop(int &status, int spec, std::string names[], int patient_status[], int queue_lengths[])
{
    auto queue = queue_lengths[spec - 1];

    if (queue == 0)
    {
        status = 0;
        return std::string();
    }

    auto first_name = get_name(spec, 0, names);
    status = get_status(spec, 0, patient_status);

    if (queue == 1)
    {
        set_name(std::string(), spec, 0, names);
        set_status(status, spec, 0, patient_status);
        decrement_queue(spec, queue_lengths);
        return first_name;
    }

    for (int i = 0; i < queue - 1; i++)
    {
        auto n = get_name(spec, i + 1, names);
        int s = get_status(spec, i + 1, patient_status);
        set_name(n, spec, i, names);
        set_status(s, spec, i, patient_status);
    }

    set_name(std::string(), spec, queue, names);
    set_status(0, spec, queue, patient_status);
    decrement_queue(spec, queue_lengths);
    return first_name;
}

void print_invalid_specialization(int spec)
{
    std::cout << "Invalid specialization '" << spec << "'. Must be [1-" << NUM_SPECIALIZATIONS << "]";
}

void add_patient(std::string names[], int patient_status[], int queue_lengths[])
{
    // make it look like the PDF
    std::cout << "Enter specialization number, name, and status (0/1): ";
    int spec{ 0 };
    std::string name;
    int status{ 0 };
    std::cin >> spec >> name >> status;

    if (!check_specialization(spec))
    {
        print_invalid_specialization(spec);
        return;
    }

    if (!check_status(status))
    {
        std::cout << "Status must be 0 (normal) or 1 (urgent)" << std::endl;
        return;
    }

    if (!check_queue(spec, queue_lengths))
    {
        std::cout << "Sorry, we can't add more patients for this specialization" << std::endl;
        return;
    }

    if (status == 1)
    {
        push_front(name, status, spec, names, patient_status, queue_lengths);
    }
    else
    {
        push(name, status, spec, names, patient_status, queue_lengths);
    }
}

void print_patients(std::string names[], int patient_status[], int queue_lengths[])
{
    for (int i = 0; i < NUM_SPECIALIZATIONS; i++)
    {
        auto num_patients = queue_lengths[i];

        if (num_patients == 0)
        {
            continue;
        }

        std::cout << "There are " << num_patients << " in specialization " << i + 1 << std::endl;

        for (int j = 0; j < num_patients; j++)
        {
            std::string patient = names[MAX_QUEUE * i + j];
            auto status = patient_status[MAX_QUEUE * i + j];
            std::cout << patient << " " << (status == 1 ? "urgent" : "regular") << std::endl;
        }

        std::cout << std::endl;
    }
}

void get_next_patient(std::string names[], int patient_status[], int queue_lengths[])
{
    std::cout << "Enter specialization: ";
    int spec{ 0 };
    std::cin >> spec;

    if (!check_specialization(spec))
    {
        print_invalid_specialization(spec);
    }

    if (get_queue(spec, queue_lengths) == 0)
    {
        std::cout << "No patients at the moment. Have a rest, Dr." << std::endl;
        return;
    }

    int status{ 0 };
    auto name = pop(status, spec, names, patient_status, queue_lengths);
    std::cout << name << " please go with the Dr." << std::endl;
}
