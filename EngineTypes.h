#ifndef ENGINE_TYPES_H
#define ENGINE_TYPES_H

class Engine;
struct EventInfo;
enum class ActionId;
enum class EngineState;
enum class ActionTrigger;
using ActionFunctor = void (Engine::*)(const EventInfo&);
using ActionCallback = std::function<void(const EventInfo&)>;
using ActionTuple = std::tuple < std::string, EngineState, ActionTrigger, ActionFunctor>;
using ActionFactory = std::unordered_map <ActionId, ActionTuple>;

enum ActionTupleTypes {
	ACTION_NAME,
	ACTION_ENGINE_STATE,
	ACTION_TRIGGER,
	ACTION_FUNCTOR
};

enum class EngineState {
	INVALID_STATE = -1,
	Loading,
	Paused,
	Running,
	STATE_COUNT
};

enum class ActionId {
	INVALID_ACTION = -1,
	Pause,
	Unpause,
	MoveViewUp,
	MoveViewDown,
	MoveViewLeft,
	MoveViewRight,
	ResetView,
	ZoomIn,
	ZoomOut,
	ResetZoom,
	Save,
	Quit,
	ACTION_COUNT
};

enum class ActionTrigger {
	ContinousKeyPress,
	SingleKeyRelease
};

class Action {

	ActionId m_id;
	EngineState m_state;
	ActionTrigger m_trigger;
	std::string m_name;
	ActionCallback m_callback;

public:
	////////////////////////////////////////////////////////////
	Action(const ActionId& t_id, const EngineState& t_state, const std::string& t_name, const ActionTrigger& t_trigger, const ActionCallback& t_callback) :
		m_id{ t_id }, m_state{ t_state }, m_name{ t_name }, m_trigger{ t_trigger }, m_callback{ t_callback }{}

	////////////////////////////////////////////////////////////
	Action(Action&& t_action) : m_name{ std::move(t_action.m_name) }, m_trigger{ std::move(t_action.m_trigger) }, m_callback{ std::move(t_action.m_callback) }{}

	////////////////////////////////////////////////////////////
	void execute(const EventInfo& t_info) { m_callback(t_info); }

	////////////////////////////////////////////////////////////
	const std::string& getName()const { return m_name; }

	////////////////////////////////////////////////////////////
	const ActionTrigger& getTrigger()const { return m_trigger; };

	////////////////////////////////////////////////////////////
	const EngineState& getEngineState()const { return m_state; };

	////////////////////////////////////////////////////////////
	const ActionId& getActionId()const { return m_id; }
};
#endif // !ENGINE_TYPES_H