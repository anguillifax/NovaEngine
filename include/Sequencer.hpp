#pragma once

#include <functional>
#include <vector>

namespace Nova {

	template <class... Args>
	class OneTimeCallable {
	public:
		using FuncSignature = std::function<void(Args...)>;

		explicit OneTimeCallable() = default;

		void Bind(const FuncSignature& function) {
			func = function;
		}

		void Reset() {
			state = State::Ready;
		}

		void Call(const Args&... args) {
			if (state == State::Ready) {
				if (func) {
					func(args...);
				}
				state = State::Consumed;
			}
		}

	private:
		enum class State : char {
			Ready = 0,
			Consumed,
		};

		FuncSignature func{};
		State state{};
	};

	class SequencerEvent {
	public:
	};

	class SequencerRange {
	public:
	};

	class Sequencer {
	public:
		explicit Sequencer();
		void Start();
		void Update();
		void Update(float newTime);
		void Stop();
		SequencerEvent& ScheduleEvent(float time);
		SequencerRange& ScheduleRange(float startTime, float stopTime);

	private:
		float currentTime{};
		std::vector<SequencerEvent> events{};
		std::vector<SequencerRange> ranges{};
	};

} // namespace Nova