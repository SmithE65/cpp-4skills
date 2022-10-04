#pragma once

#include <string>
#include <queue>
#include <stdexcept>
#include <map>
#include <ostream>

namespace Hospital
{
	enum class PatientStatus { Normal, Urgent };

	struct Patient
	{
		std::string Name{};
		PatientStatus Status{ PatientStatus::Normal };
	};

	class PatientQueue
	{
	public:
		void AddPatient(std::string name, PatientStatus status);

		Patient NextPatient();

		size_t NumPatients() const;

		void Write(std::ostream &stream) const;

	private:
		std::deque<Patient> _patientQueue{};
		int _specializationId{ -1 };
	};

	class HospitalReception
	{
	public:
		void AddPatient(int specId, std::string name, PatientStatus status);

		Patient GetNextPatient(int specId);

		bool HasPatients(int specId);

		void Write(std::ostream &stream) const;

	private:
		std::map<int, PatientQueue> _specializationQueues{};
	};
}

