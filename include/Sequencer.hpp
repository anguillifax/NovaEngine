#pragma once

#include <functional>
#include <vector>

namespace Nova {

	template <class... Args>
	class OneTimeCallable {
	public:
		using FuncSignature = std::function<void(Args...)>;

		OneTimeCallable() = default;
		OneTimeCallable(const FuncSignature& func) // implicit
			: func{func} {
		}

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

		[[nodiscard]] bool IsConsumed() const noexcept {
			return state == State::Consumed;
		}

	private:
		enum class State : char {
			Ready = 0,
			Consumed,
		};

		FuncSignature func{};
		State state{};
	};

	class Sequencer;

	class SequencerEvent {
	public:
		using FuncSignature = std::function<void(float)>;
		SequencerEvent(float triggerTime);
		SequencerEvent& BindAt(const FuncSignature& callback);

	private:
		friend Sequencer;
		void ResetAll();
		float triggerTime{};
		OneTimeCallable<float> callbackAt{};
	};

	class SequencerRange {
	public:
		using FuncSignature = std::function<void(float)>;
		SequencerRange(float startTime, float stopTime);
		SequencerRange& BindBegin(const FuncSignature& callback);
		SequencerRange& BindDuring(const FuncSignature& callback);
		SequencerRange& BindEnd(const FuncSignature& callback);

	private:
		friend Sequencer;
		void ResetAll();
		float startTime{};
		float stopTime{};
		OneTimeCallable<float> callbackBegin{};
		std::function<void(float)> callbackDuring{};
		OneTimeCallable<float> callbackEnd{};
	};

	class Sequencer {
	public:
		explicit Sequencer();

		void Start();
		void Update(float deltaTime);
		/// Stop() ensures that ranges are closed before sequencer finishing.
		void Stop();

		[[nodiscard]] SequencerEvent& ScheduleEvent(float triggerTime);
		[[nodiscard]] SequencerRange& ScheduleRangeWithBounds(float startTime, float stopTime);
		[[nodiscard]] SequencerRange& ScheduleRangeWithDuration(float startTime, float duration);
		[[nodiscard]] SequencerEvent& ScheduleRepeating(float initialDelay, float repeatDelay);

	private:
		float currentTime{};
		std::vector<SequencerEvent> events{};
		std::vector<SequencerRange> ranges{};
	};

} // namespace Nova