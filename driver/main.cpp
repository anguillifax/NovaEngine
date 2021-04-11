#include <chrono>
#include <cmath>
#include <functional>
#include <iostream>
#include <thread>

#include "QuickFormat.hpp"

#include "Sequencer.hpp"

namespace DriverTesting {

	void SequencerThread(Nova::Sequencer sequencer, float maxDuration) {
		std::cout << "[ Begin sequencer thread for " << maxDuration << "s ]\n";

		sequencer.Start();

		float time{0};
		while (time < maxDuration) {
			sequencer.Update(0.1f);
			time += 0.1f;
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		sequencer.Stop();

		std::cout << "[ End sequencer thread for " << maxDuration << "s ]\n\n";
	}

	void Main() {
		Nova::Sequencer seq{};

		seq.ScheduleEvent(3.5)
			.BindAt([](float t) {
				Print("Triggered event at", t);
			});

		seq.ScheduleRangeWithDuration(1, 0.5)
			.BindBegin([](float t) {
				Print("Begin range", t);
			})
			.BindDuring([](float t) {
				Print("During range", t);
			})
			.BindEnd([](float t) {
				Print("End range", t);
			});

		std::thread{SequencerThread, seq, 5.0f}.join();
		std::thread{SequencerThread, seq, 1.2f}.join();
		std::thread{SequencerThread, seq, 0.3f}.join();
	}
} // namespace DriverTesting

int main(int argc, char** argv) {
	DriverTesting::Main();
}