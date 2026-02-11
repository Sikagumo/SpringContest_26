#include "StatusPlayer.h"

StatusPlayer::StatusPlayer(void):
	startName_(""), handlePath_(""),
	scale_(0.0f), radius_(0.0f),
	hp_(0), power_(0), luck_(0), weaponId_(0),
	speed_(0.0f), speedAcc_(0.0f), dashMult_(1.0f),
	timeEvasion_(0.0f), timeInv_(0.0f), comboMag_(0.0f),
	animSpeedIdle_(0.0f), animSpeedWalk_(0.0f), animSpeedDash_(0.0f)
{
	motion_.clear();
}

StatusPlayer::Motion::Motion(void) :
	name(""),
	timeActive(0.0f), timeEnd(0.0f), timeAtkTiming(0.0f),
	animSpeed(0.0f),
	radius(0.0f)
{
}
