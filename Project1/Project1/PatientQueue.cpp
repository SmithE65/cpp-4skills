#include <string>
#include <queue>
#include <stdexcept>
#include <map>
#include <ostream>

#include "PatientQueue.h"

void Hospital::PatientQueue::AddPatient(std::string name, PatientStatus status)
{
	switch (status)
	{
	case Hospital::PatientStatus::Normal:
		_patientQueue.push_back(Patient{ name, status });
		break;
	case Hospital::PatientStatus::Urgent:
		_patientQueue.push_front(Patient{ name, status });
		break;
	default:
		throw std::invalid_argument{ "Invalid PatientStatus" };
		break;
	}
}

Hospital::Patient Hospital::PatientQueue::NextPatient()
{
	auto p = _patientQueue.at(0);
	_patientQueue.pop_front();
	return p;
}

size_t Hospital::PatientQueue::NumPatients() const
{
	return _patientQueue.size();
}

void Hospital::PatientQueue::Write(std::ostream &stream) const
{
	for (const auto& p : _patientQueue)
	{
		stream << p.Name << " " << (p.Status == PatientStatus::Urgent ? "urgent" : "regular") << std::endl;
	}
}

void Hospital::HospitalReception::AddPatient(int specId, std::string name, Hospital::PatientStatus status)
{
	if (!_specializationQueues.contains(specId))
	{
		_specializationQueues.insert({ specId, PatientQueue{} });
	}

	auto& queue = _specializationQueues.at(specId);
	queue.AddPatient(name, status);
}

Hospital::Patient Hospital::HospitalReception::GetNextPatient(int specId)
{
	return _specializationQueues.at(specId).NextPatient();
}

bool Hospital::HospitalReception::HasPatients(int specId)
{
	if (!_specializationQueues.contains(specId)) return false;

	return _specializationQueues.at(specId).NumPatients() > 0;
}

void Hospital::HospitalReception::Write(std::ostream &stream) const
{
	for (const auto& q : _specializationQueues)
	{
		stream << "There are " << q.second.NumPatients() << " in specialization " << q.first << std::endl;
		q.second.Write(stream);
	}
}
