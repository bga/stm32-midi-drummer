#pragma once

template<class ConfigArg>
struct Self {
	typedef ConfigArg Config;
	
	Config m_config;
	
	enum {
		slotsCount = Config::slotsCount,
	};
	
	struct Slot {
		TickCount delay;
		TickCount endTick;
		Bool isPlaying;
		
		void startPlay(TickCount tickCount) {
			this->isPlaying = true;
			this->endTick = tickCount + this->delay;
		}
		void stopPlay(TickCount tickCount) {
			this->isPlaying = false;
			this->endTick = 0;
		}
	};
	
	struct Store {
		Slot slots[slotsCount];
	};
	
	Slot slots[slotsCount];
	
	Store* storePtr;
	
	virtual void onTick(TickCount tickCount) override {
		forInc(UInt, i, 0, slotsCount) {
			if(this->slots[i].isPlaying == false) {
				continue;
			}
			if(tickCount <= this->slots[i].endTick) {
				this->slots[i].stopSlot(i);
			}
		}
	}
};
