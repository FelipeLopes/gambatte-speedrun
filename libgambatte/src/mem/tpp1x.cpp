//
//   Copyright (C) 2017 by TPPDevs <tppdevs.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License version 2 as
//   published by the Free Software Foundation.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License version 2 for more details.
//
//   You should have received a copy of the GNU General Public License
//   version 2 along with this program; if not, write to the
//   Free Software Foundation, Inc.,
//   51 Franklin St, Fifth Floor, Boston, MA  02110-1301, USA.
//

#include "tpp1x.h"
#include "../savestate.h"

namespace gambatte {

Tpp1X::Tpp1X()
: baseTime_(0)
, haltTime_(0)
, rombank_(1)
, rambank_(0)
, dataW_(0)
, dataH_(0)
, dataM_(0)
, dataS_(0)
, rumble_(0)
, curmap_(0)
, features_(0)
, enabled_(false)
, running_(false)
, overflow_(false)
{
}

void Tpp1X::latch() {
	std::time_t tmp = (running_ ? std::time(0) : haltTime_) - baseTime_;

	while (tmp >= 154828800) {
		baseTime_ += 154828800;
		tmp -= 154828800;
		overflow_ = true;
	}

	dataW_ = tmp / 604800;
	tmp %= 604800;

	dataH_ = (tmp / 86400) << 5;
	tmp %= 86400;
	dataH_ |= tmp / 3600;
	tmp %= 3600;

	dataM_ = tmp / 60;
	tmp %= 60;

	dataS_ = tmp;
}

void Tpp1X::settime() {
	baseTime_ = running_ ? std::time(0) : haltTime_;
	baseTime_ -= dataS_ + (dataM_ * 60) + ((dataH_ & 0x1F) * 3600) + (((dataH_ & 0xE0) >> 5) * 86400) + (dataW_ * 604800);
}

void Tpp1X::halt() {
	if (running_) {
		haltTime_ = std::time(0);
		running_ = false;
	}
}

void Tpp1X::resume() {
	if (!running_) {
		baseTime_ += std::time(0) - haltTime_;
		running_ = true;
	}
}

void Tpp1X::rumble(unsigned char amount) {
	if (features_ & 1)
		rumble_ = features_ & 2 ? amount : amount > 0;
}

void Tpp1X::saveState(SaveState &state) const {
	state.rtc.baseTime = baseTime_;
	state.rtc.haltTime = haltTime_;
	state.rtc.dataDh = (curmap_ & 0x0F) | (rumble_ << 4);
	state.rtc.dataDl = dataW_;
	state.rtc.dataH = dataH_;
	state.rtc.dataM = dataM_;
	state.rtc.dataS = dataS_;
	state.rtc.lastLatchData = (running_ << 1) | overflow_;
}

void Tpp1X::loadState(SaveState const &state) {
	baseTime_ = state.rtc.baseTime;
	haltTime_ = state.rtc.haltTime;
	curmap_ = state.rtc.dataDh & 0x0F;
	rumble_ = (state.rtc.dataDh & 0xF0) >> 4;
	dataW_ = state.rtc.dataDl;
	dataH_ = state.rtc.dataH;
	dataM_ = state.rtc.dataM;
	dataS_ = state.rtc.dataS;
	running_ = state.rtc.lastLatchData & 2;
	overflow_ = state.rtc.lastLatchData & 1;
}

unsigned char Tpp1X::read(unsigned p) const {
	switch (curmap_) {
	case 0:
		switch (p & 3) {
		case 0: return rombank_ & 0x00FF;
		case 1: return (rombank_ & 0xFF00) >> 8;
		case 2: return rambank_;
		case 3: return (rumble_ & 3) | (running_ << 2) | (overflow_ << 3) | 0xF0;
		}
	case 3:
		switch (p & 3) {
		case 0: return dataW_;
		case 1: return dataH_;
		case 2: return dataM_;
		case 3: return dataS_;
		}
	}
	return 0xFF;
}

void Tpp1X::write(unsigned p, unsigned data) {
	if (curmap_ == 3) { // only special map that can be written
		switch (p & 3) {
		case 0: dataW_ = data; break;
		case 1: dataH_ = data; break;
		case 2: dataM_ = data; break;
		case 3: dataS_ = data; break;
		}
	}
}

}
