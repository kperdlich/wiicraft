#include <cmath>
#include "wii_pad.h"

bool core::WiiPad::sPadsInitialised = false;

core::WiiPad::WiiPad(int8_t chanID, uint32_t resX, uint32_t resY)
    : mChanID(chanID)
{
    if (!sPadsInitialised)
    {
        WPAD_Init();
        sPadsInitialised = true;
    }

    WPAD_SetVRes(chanID, resX, resY);
	WPAD_SetDataFormat(chanID, WPAD_FMT_BTNS_ACC_IR);
    Update();
}

void core::WiiPad::Update()
{    
    WPAD_ScanPads();
    mData = WPAD_Data(mChanID);
    mButtonDown = WPAD_ButtonsDown(mChanID);
    mButtonHeld = WPAD_ButtonsHeld(mChanID);
    mButtonUp = WPAD_ButtonsUp(mChanID);
}

float core::WiiPad::GetNunchukAngleY() const
{
	static float driftY = 0;

    float y = mData->exp.nunchuk.js.pos.y - mData->exp.nunchuk.js.min.y;
    y -= (mData->exp.nunchuk.js.max.y - mData->exp.nunchuk.js.min.y) * 0.5f;

    if (std::abs(y) - 100.0F > driftY)
	{
		if (y < 0)
		{
			driftY = -(y + 100.0F);
		}
		else
		{
			driftY = 100.0F - y;
		}
	}

    if (std::abs(y + driftY) > 100.0F)
	{
		driftY = 0;
	}

	y += driftY;
	y *= 0.01F;

    if (std::abs(y) < 0.32F)
	{
		y = 0;
	}

	return y;
}

float core::WiiPad::GetNunchukAngleX() const
{
	static float driftX = 0;

    float x = mData->exp.nunchuk.js.pos.x - mData->exp.nunchuk.js.min.x;
    x -= (mData->exp.nunchuk.js.max.x - mData->exp.nunchuk.js.min.x) * 0.5F;

    if (std::abs(x) - 100.0F > driftX)
	{
		if (x < 0)
		{
			driftX = -(x + 100.0F);
		}
		else
		{
			driftX = 100.0F - x;
		}
	}

    if (std::abs(x + driftX) > 100.0F)
	{
		driftX = 0;
	}

	x += driftX;
	x *= 0.01F;

    if (std::abs(x) < 0.32F)
	{
		x = 0;
	}

	return x;
}
