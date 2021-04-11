#include "Sequencer.hpp"

#include "CoreTypes.hpp"

// =========================================================
// Sequencer
// =========================================================

Nova::Sequencer::Sequencer() = default;

void Nova::Sequencer::Start() {
}

void Nova::Sequencer::Update(float newTime) {
}

void Nova::Sequencer::Stop() {
}

auto Nova::Sequencer::ScheduleEvent(float time) -> SequencerEvent& {
	throw NotImplementedException{};
}

auto Nova::Sequencer::ScheduleRange(float startTime, float stopTime) -> SequencerRange& {
	throw NotImplementedException{};
}