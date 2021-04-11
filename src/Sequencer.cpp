#include "Sequencer.hpp"

#include <stdexcept>

#include "CoreTypes.hpp"

// =========================================================
// Sequencer Event
// =========================================================

Nova::SequencerEvent::SequencerEvent(float triggerTime)
	: triggerTime{triggerTime} {
}

auto Nova::SequencerEvent::BindAt(const FuncSignature& callback) -> SequencerEvent& {
	this->callbackAt = callback;
	return *this;
}

void Nova::SequencerEvent::ResetAll() {
	this->callbackAt.Reset();
}

// =========================================================
// Sequencer Range
// =========================================================

Nova::SequencerRange::SequencerRange(float startTime, float stopTime)
	: startTime{startTime}, stopTime{stopTime} {
}

auto Nova::SequencerRange::BindBegin(const FuncSignature& callback) -> SequencerRange& {
	this->callbackBegin = callback;
	return *this;
}

auto Nova::SequencerRange::BindDuring(const FuncSignature& callback) -> SequencerRange& {
	this->callbackDuring = callback;
	return *this;
}

auto Nova::SequencerRange::BindEnd(const FuncSignature& callback) -> SequencerRange& {
	this->callbackEnd = callback;
	return *this;
}

void Nova::SequencerRange::ResetAll() {
	this->callbackBegin.Reset();
	this->callbackEnd.Reset();
}

// =========================================================
// Sequencer
// =========================================================

Nova::Sequencer::Sequencer() = default;

void Nova::Sequencer::Start() {
	this->currentTime = 0.0f;

	for (auto& item : this->events) {
		item.ResetAll();
	}
	for (auto& item : this->ranges) {
		item.ResetAll();
	}
}

void Nova::Sequencer::Update(float deltaTime) {
	if (deltaTime < 0) {
		throw std::out_of_range{"Delta time must be non-negative."};
	}

	this->currentTime += deltaTime;

	for (auto& item : this->events) {
		if (this->currentTime >= item.triggerTime) {
			item.callbackAt.Call(this->currentTime);
		}
	}

	for (auto& item : this->ranges) {
		const bool afterBegin = this->currentTime >= item.startTime;
		const bool afterEnd = this->currentTime >= item.stopTime;
		if (afterBegin) {
			item.callbackBegin.Call(this->currentTime);
		}
		if (afterBegin && !afterEnd) {
			item.callbackDuring(this->currentTime);
		}
		if (afterEnd) {
			item.callbackEnd.Call(this->currentTime);
		}
	}
}

void Nova::Sequencer::Stop() {
	for (auto& item : this->ranges) {
		if (item.callbackBegin.IsConsumed()) {
			item.callbackEnd.Call(this->currentTime);
		}
	}
}

auto Nova::Sequencer::ScheduleEvent(float triggerTime) -> SequencerEvent& {
	if (triggerTime < 0) {
		throw std::out_of_range{"[Nova::Sequencer] Trigger time must be non-negative."};
	}
	return this->events.emplace_back(triggerTime);
}

auto Nova::Sequencer::ScheduleRangeWithBounds(float startTime, float stopTime) -> SequencerRange& {
	if (startTime < 0 || stopTime < 0) {
		throw std::out_of_range{"[Nova::Sequencer] Bounds must be non-negative."};
	}
	if (stopTime <= startTime) {
		throw std::out_of_range{"[Nova::Sequencer] Start time must less than stop time."};
	}
	return this->ranges.emplace_back(startTime, stopTime);
}

auto Nova::Sequencer::ScheduleRangeWithDuration(float startTime, float duration) -> SequencerRange& {
	if (startTime < 0) {
		throw std::out_of_range{"[Nova::Sequencer] Start time must be non-negative."};
	}
	if (duration <= 0) {
		throw std::out_of_range{"[Nova::Sequencer] Duration must be positive."};
	}
	return this->ranges.emplace_back(startTime, startTime + duration);
}

auto Nova::Sequencer::ScheduleRepeating(float initialDelay, float repeatDelay) -> SequencerEvent& {
	throw NotImplementedException{};
}