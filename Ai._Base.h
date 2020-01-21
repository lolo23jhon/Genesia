#ifndef AI_BASE_H
#define AI_BASE_H

#include <functional>
#include <queue>
#include <string>
#include <unordered_map>
#include "Actor_Base.h"

// A task is any activity the actor might carry on.
// It can be atomic llw level action like move forward 5, or a sequence of smaller actions, or a goal based activity.
// THis means that tasks are not restricted to single engine ticks.
// The functionality is ultimately determined by the particular implementation, this is but a base.

class Actor_Base;

template <class TDerived>
using TaskFunctor = void(TDerived::*)(Actor_Base*, const float& );
using TaskCallback = std::function<void(Actor_Base *, const float&)>;
using TaskQueue = std::queue<std::string>;
using TaskMap = std::unordered_map<std::string, TaskCallback>;

class Ai_Base {
protected:
	TaskMap m_taskMap;					// Includes all the tasks or actions the actor is capable of doing
	TaskQueue m_taskQueue;				// Contains the currently queued tasks 
	const std::string* m_currentTask;	// Used for keeping track of tasks, considering these could take more than one tick
	float m_waitCoundown;				// seconds
	bool m_isDone;						// Whether or not the current action was completed

public:
	Ai_Base();
	////////////////////////////////////////////////////////////
	template <class TDerived> void registerTask(const std::string& t_taskId, TaskFunctor<TDerived> t_functor) {
		m_taskMap[t_taskId] = std::bind(t_functor, dynamic_cast<TDerived*>(this), std::placeholders::_1, std::placeholders::_2);
	}
	
	bool executeTask(const std::string& t_taskName, Actor_Base* t_owner, const float& t_elapsed);		// Return false if task does not exist
	void pollTasks();							// Checks if the current task is finished. If so, pops a task from the queue and makes the current goal
	const float& getWaitCountDown()const;
	void setWaitCountDown(const float& t_seconds);
	const std::string& getCurrentTask()const;
	bool hasTask(const std::string& t_taskName)const;
	bool getIsDone()const;

	
	virtual void update(Actor_Base* t_owner, const float& t_elapsed); // Called from Actor_Base::update; this is bridge of the Ai with the system

protected:
	virtual void Task_Idle(Actor_Base* t_owner, const float& t_elapsed); // Special task: default of no actions in queue and no wait timer
	virtual void Task_Waiting(Actor_Base* t_owner, const float& t_elapsed); // Special task: the actor is freezed until the countdown reaches 0; decreased on update
};


#endif // !AI_BASE_H
