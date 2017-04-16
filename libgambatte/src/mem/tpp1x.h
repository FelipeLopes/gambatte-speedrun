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

#ifndef TPP1X_H
#define TPP1X_H

#include <ctime>

namespace gambatte {

struct SaveState;

class Tpp1X {
public:
	Tpp1X();
	std::time_t baseTime() const { return baseTime_; }
	void setBaseTime(std::time_t baseTime) { baseTime_ = baseTime; }
	void latch();
	void settime();
	void resetOverflow() { overflow_ = false;}
	void halt();
	void resume();
	void rumble(unsigned char amount);
	void saveState(SaveState &state) const;
	void loadState(SaveState const &state);
	void set(bool enabled, unsigned char features) {
		enabled_ = enabled;
		features_ = features;
	}
	bool isTPP1() const { return enabled_; }
	void setRombank(unsigned short rombank) { rombank_ = rombank; }
	void setRambank(unsigned char rambank) { rambank_ = rambank; }
	void setMap(unsigned char map) { curmap_ = map; }
	unsigned char features() const { return features_; }
	unsigned char read(unsigned p) const;
	void write(unsigned p, unsigned data);

private:
	std::time_t baseTime_;
	std::time_t haltTime_;
	unsigned short rombank_;
	unsigned char rambank_;
	unsigned char dataW_;
	unsigned char dataH_;
	unsigned char dataM_;
	unsigned char dataS_;
	unsigned char rumble_;
	unsigned char curmap_;
	unsigned char features_;
	bool enabled_;
	bool running_;
	bool overflow_;
};

}

#endif
