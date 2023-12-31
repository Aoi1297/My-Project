#include "field.h"

#include "../UnitManager/Unit/Player/player.h"

const int CField::m_pixel_width						= 100;
const int CField::m_param							= 8;
const float CField::m_mini_boss_spawn_time			= 70;
const float CField::m_back_ground_speed_normal		= 5;
const float CField::m_back_ground_speed_laser		= 10;
const float CField::m_back_ground_speed_headmode	= 10;

CField::CField(IGameObject* parent)
	// parent(CGameMain)
	: IGameObject(parent, "Field")
	, m_BackGroundSpeed(0)
{
}

void CField::Initialize(void)
{
	// プレイヤークラスポインタを取得
	m_Player_P = (CPlayer*)aqua::FindGameObject("Player");

	// 動かない背景画像
	m_BackGroundSprite.Create("data/texture/others/back_ground.png");

	// 動く背景画像１枚目
	m_BackGroundMoveSprite.Create("data/texture/others/back_ground.png");
	m_BackGroundMoveSprite.ApplyGaussFilter(m_param, m_pixel_width);

	// 動く背景画像２枚目
	m_BackGroundMoveNextSprite.Create("data/texture/others/back_ground.png");
	m_BackGroundMoveNextSprite.ApplyGaussFilter(m_param, m_pixel_width);

	// ２枚目の背景画像を右端にセット
	m_BackGroundMoveNextSprite.position.x = (float)aqua::GetWindowWidth();

	// 中ボス出現時間設定
	m_MiniBossSpawnTimer.Setup(m_mini_boss_spawn_time);
}

void CField::Update(void)
{
	// タイマー更新処理
	TimerUpdate();

	// 背景移動計算処理
	MoveCalculation();

	// 背景移動処理
	MoveBackGround();
}

void CField::Draw(void)
{
	m_BackGroundSprite.Draw();				// !< 背景の描画
	m_BackGroundMoveSprite.Draw();			// !< 背景の１枚目の描画
	m_BackGroundMoveNextSprite.Draw();		// !< 背景の２枚目の描画
}

void CField::Finalize(void)
{
	// 画像データをメモリの解放
	m_BackGroundMoveNextSprite.Delete();	// !< 背景２枚目の解放
	m_BackGroundMoveSprite.Delete();		// !< 背景１枚目の解放
	m_BackGroundSprite.Delete();			// !< 背景の解放
}

void CField::TimerUpdate(void)
{
	m_MiniBossSpawnTimer.Update();
}

void CField::MoveCalculation(void)
{
	m_BackGroundSpeed = m_back_ground_speed_normal;

	if (m_MiniBossSpawnTimer.Finished())
	{
		m_BackGroundSpeed += m_back_ground_speed_headmode;

		m_MiniBossSpawnTimer.SetTime(m_MiniBossSpawnTimer.GetLimit());
	}

	if (m_Player_P->GetLaserFlag())
	{
		m_BackGroundSpeed += m_back_ground_speed_laser;
	}
}

void CField::MoveBackGround(void)
{
	// 画像を移動させる
	m_BackGroundMoveSprite.position.x -= m_BackGroundSpeed;
	m_BackGroundMoveNextSprite.position.x -= m_BackGroundSpeed;

	// １枚目の画像移動処理
	{
		// １枚目の画像が画面から見えなくなったら
		if (m_BackGroundMoveSprite.position.x < -(float)aqua::GetWindowWidth())
			// 画像右端へ移動させる
			m_BackGroundMoveSprite.position.x = (float)aqua::GetWindowWidth();
	}

	// ２枚目の画像移動処理
	{
		// ２枚目の画像が画面から見えなくなったら
		if (m_BackGroundMoveNextSprite.position.x < -(float)aqua::GetWindowWidth())
			// 画面右端へ移動させる
			m_BackGroundMoveNextSprite.position.x = (float)aqua::GetWindowWidth();
	}
}