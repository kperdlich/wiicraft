/***
 *
 * Copyright (C) 2016 DaeFennek
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
***/

#pragma once

#include <wiiuse/wpad.h>

namespace core {
class WiiPad
{
public:
    WiiPad(int8_t chanID, uint32_t resX, uint32_t resY);
	WiiPad(const WiiPad&) = delete;
	WiiPad(WiiPad&&) = delete;
    ~WiiPad() = default;
	void operator=(const WiiPad&) = delete;
    void operator=(WiiPad&&) = delete;

	void Update();
    float GetNunchukAngleY() const;
    float GetNunchukAngleX() const;

    inline float GetX() const;
    inline float GetY() const;
    inline uint32_t ButtonsDown() const;
    inline uint32_t ButtonsHeld() const;
    inline uint32_t ButtonsUp() const;
    inline const WPADData* GetData() const;

private:
    static bool sPadsInitialised;
    WPADData* mData;
    uint32_t mButtonDown;
    uint32_t mButtonHeld;
    uint32_t mButtonUp;
    int8_t mChanID;
};

inline float WiiPad::GetX() const
{
    return mData->ir.x;
}

inline float WiiPad::GetY() const
{
    return mData->ir.y;
}

inline uint32_t WiiPad::ButtonsDown() const
{
    return mButtonDown;
}

inline uint32_t WiiPad::ButtonsHeld() const
{
    return mButtonHeld;
}

inline uint32_t WiiPad::ButtonsUp() const
{
    return mButtonUp;
}

inline const WPADData *WiiPad::GetData() const
{
    return mData;
}

}
