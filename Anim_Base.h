#ifndef ANIM_BASE_H
#define ANIM_BASE_H

#include <string>

using Frame = unsigned int;

class SpriteSheet;
class Anim_Base {
	friend class SpriteSheet;
protected:
	Frame m_frameCurrent;
	Frame m_frameStart;
	Frame m_frameEnd;
	Frame m_frameRow;
	int m_frameActionStart;
	int m_frameActionEnd;
	float m_frameTime;
	float m_elapsedTime;
	bool m_loop;
	bool m_playing;
	bool m_hasMoved;

	std::string m_name;

	SpriteSheet* m_spriteSheet;

public:
	Anim_Base();
	virtual ~Anim_Base();

	void setSpriteSheet(SpriteSheet* t_sheet);

	bool setFrame(Frame t_frame);
	void setStartFrame(Frame t_frame);
	void setEndFrame(Frame t_frame);
	void setFrameRow(unsigned t_row);
	void setActionStart(Frame t_frame);
	void setActionEnd(Frame t_frame);
	void setFrameTime(float t_time);
	void setLooping(bool t_loop);
	void setName(const std::string& t_name);

	SpriteSheet* getSpriteSheet();

	Frame getFrame()const;
	Frame getStartFrame()const;
	Frame getEndFrame()const;
	unsigned getFrameRow()const;
	int getActionStart()const;
	int getActionEnd()const;
	float getFrameTime()const;
	float getElapsedTime()const;
	bool isLooping()const;
	bool isPlaying()const;
	bool isInAction()const;
	bool checkMoved();
	std::string getName()const;

	void play();
	void pause();
	void stop();
	void reset();

	virtual void update(float t_dt);

	////////////////////////////////////////////////////////////
	friend std::stringstream& operator >> (std::stringstream& t_stream, Anim_Base& a) {
		a.readIn(t_stream);
		return t_stream;
	}

protected:
	virtual void frameStep() = 0;
	virtual void cropSprite() = 0;
	virtual void readIn(std::stringstream& t_stream) = 0;
};

#endif // !ANIM_BASE_H