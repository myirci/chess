#pragma once

#include <chrono>
#include <string>

namespace chesslib::utility::time 
{
	using TimePoint		= std::chrono::time_point<std::chrono::steady_clock>;
	
	enum class CommandType { StartTimer, EndTimer, TagTimePoint };

	struct TimerCommand
	{
		TimerCommand(TimePoint tp, CommandType ctype, std::string_view log) :
			_tp{ tp }, _ctype{ ctype }, _log{ log } { }

		TimePoint   _tp;
		CommandType _ctype;
		std::string _log;
	};

	template<typename DurationType = std::chrono::seconds>
	class Timer
	{
	public:
		Timer()
		{
			if constexpr (std::is_same_v<DurationType, std::chrono::milliseconds>)
				_unit = "milliseconds";
			else if constexpr (std::is_same_v < DurationType, std::chrono::minutes>)
				_unit = "minutes";
			else if constexpr (std::is_same_v < DurationType, std::chrono::hours>)
				_unit = "hours";
			else
				_unit = "seconds";
		}

		std::string GetUnit() const { return _unit; }

		void Start(std::string_view log)	{ Command(CommandType::StartTimer, log); }
		void End(std::string_view log)		{ Command(CommandType::EndTimer, log); }
		void TagEvent(std::string_view log) { Command(CommandType::TagTimePoint, log); }
		
		void Reset() { _commands.clear(); }

		const std::vector<TimerCommand>& GetCommands() const { return _commands; }

	private:
		std::string _unit;
		std::vector<TimerCommand> _commands;

		void Command(CommandType ctype, std::string_view log = "")
		{
			// StartTimer command can be received once at the beginning.
			if (_commands.empty() && ctype != CommandType::StartTimer || 
				!_commands.empty() && ctype == CommandType::StartTimer)
				throw std::logic_error("Timer command error.");

			// EndTimer command can be received once at the end.
			if ((ctype == CommandType::EndTimer || ctype == CommandType::TagTimePoint) &&
				std::any_of(_commands.begin(), _commands.end(), [](const auto& tc) 
					{ return tc._ctype == CommandType::EndTimer; }))
				throw std::logic_error("Timer command error.");

			// Add the event
			_commands.emplace_back(std::chrono::steady_clock::now(), ctype, log);
		}
	};

	template <typename T>
	std::ostream& operator<< (std::ostream& os, const Timer<T>& timer)
	{
		const auto& commands = timer.GetCommands();
		auto unit = timer.GetUnit();

		auto it = commands.begin();
		auto start = it->_tp;
		bool start_log{ !it->_log.empty() };

		if(start_log)
			os << it->_log << std::endl;
		for (it++; it != commands.end(); it++) 
		{
			auto d = it->_tp - start;
			if (it->_ctype == CommandType::TagTimePoint && start_log) 
				os << "\t";
			os << it->_log << " " << std::chrono::duration_cast<T>(d) << " " << unit << ".\n";
		}
		return os;
	}
}