#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Init()
{
}
void Player::Update()
{

}
void Player::Draw()
{
	// ƒvƒŒƒCƒ„[‚Ì•`‰æ
	DrawCapsule3D(VGet(0.0f,100.0f,0.0f),VGet(0.0f,180.0f,0.0f),40.0f,8,GetColor(255,255,255),GetColor(255,255,255),TRUE);
}

