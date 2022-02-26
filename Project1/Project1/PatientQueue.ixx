#include <string>
#include <queue>
#include <stdexcept>
#include <map>
#include <ostream>
export module PatientQueue;

//export class Hospital::HospitalReception;

namespace Hospital
{
	enum class PatientStatus { Normal, Urgent };
	export PatientStatus;

	struct Patient
	{
		std::string Name{};
		PatientStatus Status{ PatientStatus::Normal };
	};
	export Patient;

	class PatientQueue
	{
	public:
		void AddPatient(std::string name, PatientStatus status)
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
				throw std::exception{ "Invalid PatientStatus" };
				break;
			}
		}

		Patient NextPatient()
		{
			auto p = _patientQueue.at(0);
			_patientQueue.pop_front();
			return p;
		}

		size_t NumPatients() const
		{
			return _patientQueue.size();
		}

		void Write(std::ostream &stream) const
		{
			for (const auto& p : _patientQueue)
			{
				stream << p.Name << " " << (p.Status == PatientStatus::Urgent ? "urgent" : "regular") << std::endl;
			}
		}

	private:
		std::deque<Patient> _patientQueue{};
		int _specializationId{ -1 };
	};

	class HospitalReception
	{
	public:
		void AddPatient(int specId, std::string name, PatientStatus status)
		{
			if (!_specializationQueues.contains(specId))
			{
				_specializationQueues.insert({ specId, PatientQueue{} });
			}

			auto& queue = _specializationQueues.at(specId);
			queue.AddPatient(name, status);
		}

		Patient GetNextPatient(int specId)
		{
			return _specializationQueues.at(specId).NextPatient();
		}

		bool HasPatients(int specId)
		{
			if (!_specializationQueues.contains(specId)) return false;

			return _specializationQueues.at(specId).NumPatients() > 0;
		}

		void Write(std::ostream &stream) const
		{
			for (const auto& q : _specializationQueues)
			{
				stream << "There are " << q.second.NumPatients() << " in specialization " << q.first << std::endl;
				q.second.Write(stream);
			}
		}

	private:
		std::map<int, PatientQueue> _specializationQueues{};
	};

	export HospitalReception;
}