#include "Anim_Base.h"
#include "SpriteSheet.h"

////////////////////////////////////////////////////////////
Anim_Base::Anim_Base() :
	m_frameCurrent{ 0 },
	m_frameStart{ 0 },
	m_frameEnd{ 0 },
	m_frameRow{ 0 },
	m_frameTime{ 0 },
	m_elapsedTime{ 0 },
	m_frameActionStart{ -1 },
	m_frameActionEnd{ -1 },
	m_loop{ false },
	m_playing{ false },
	m_hasMoved{ false }{}


////////////////////////////////////////////////////////////
Anim_Base:: ~Anim_Base() {}

////////////////////////////////////////////////////////////
void Anim_Base::setSpriteSheet(SpriteSheet* t_sheet) {
	m_spriteSheet = t_sheet;
}


////////////////////////////////////////////////////////////
bool Anim_Base::setFrame(Frame t_frame) {
	if ((t_frame >= m_frameStart && t_frame <= m_frameEnd) ||
		(t_frame >= m_frameEnd && t_frame <= m_frameStart))
	{
		m_frameCurrent = t_frame;
		m_hasMoved = true;
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////
void Anim_Base::setStartFrame(Frame t_frame) {
	m_frameStart = t_frame;
}
////////////////////////////////////////////////////////////
void Anim_Base::setEndFrame(Frame t_frame) {
	m_frameEnd = t_frame;
}

////////////////////////////////////////////////////////////
void Anim_Base::setFrameRow(unsigned t_row) {
	m_frameRow = t_row;
}

////////////////////////////////////////////////////////////
void Anim_Base::setActionStart(Frame t_frame) {
	m_frameActionStart = t_frame;
}

////////////////////////////////////////////////////////////
void Anim_Base::setActionEnd(Frame t_frame) {
	m_frameActionEnd = t_frame;
}

////////////////////////////////////////////////////////////
void Anim_Base::setFrameTime(float t_time) {
	m_frameTime = t_time;
}

////////////////////////////////////////////////////////////
void Anim_Base::setLooping(bool t_loop) {
	m_loop = t_loop;
}

////////////////////////////////////////////////////////////
void Anim_Base::setName(const std::string& t_name) {
	m_name = t_name;
}

////////////////////////////////////////////////////////////
SpriteSheet* Anim_Base::getSpriteSheet() { return m_spriteSheet; }

////////////////////////////////////////////////////////////
Frame Anim_Base::getFrame()const { return m_frameCurrent; }

////////////////////////////////////////////////////////////
Frame Anim_Base::getStartFrame()const { return m_frameStart; }

////////////////////////////////////////////////////////////
Frame Anim_Base::getEndFrame()const { return m_frameEnd; }

////////////////////////////////////////////////////////////
unsigned Anim_Base::getFrameRow()const { return m_frameRow; }

////////////////////////////////////////////////////////////
int Anim_Base::getActionStart()const { return m_frameActionStart; }

////////////////////////////////////////////////////////////
int Anim_Base::getActionEnd()const { return m_frameActionEnd; }

////////////////////////////////////////////////////////////
float Anim_Base::getFrameTime()const { return m_frameTime; }

////////////////////////////////////////////////////////////
float Anim_Base::getElapsedTime()const { return m_elapsedTime; }

////////////////////////////////////////////////////////////
std::string Anim_Base::getName()const { return m_name; }

////////////////////////////////////////////////////////////
bool Anim_Base::isLooping()const { return m_loop; }

////////////////////////////////////////////////////////////
bool Anim_Base::isPlaying()const { return m_playing; }

////////////////////////////////////////////////////////////
bool Anim_Base::isInAction()const {
	if (m_frameActionStart == -1 || m_frameActionEnd == -1) { return true; }

	return (m_frameCurrent >= static_cast<Frame>(m_frameActionStart) && m_frameCurrent <= static_cast<Frame>(m_frameActionEnd));
}

////////////////////////////////////////////////////////////
bool Anim_Base::checkMoved() {
	bool result{ m_hasMoved };
	m_hasMoved = false;
	return result;
}

////////////////////////////////////////////////////////////
void Anim_Base::play() { m_playing = true; }

////////////////////////////////////////////////////////////
void Anim_Base::pause() { m_playing = false; }

////////////////////////////////////////////////////////////
void Anim_Base::stop() { m_playing = false; reset(); }

////////////////////////////////////////////////////////////
void Anim_Base::reset() {
	m_frameCurrent = m_frameStart;
	m_elapsedTime = 0.f;
	cropSprite();
}

////////////////////////////////////////////////////////////
void Anim_Base::update(float t_dt) {
	if (!m_playing) { return; }
	m_elapsedTime += t_dt;
	if (m_elapsedTime < m_frameTime) { return; }
	frameStep();
	cropSprite();
	m_elapsedTime = 0;
}
