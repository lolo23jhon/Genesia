#ifndef ACTION_H
#define ACTION_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

enum class ActionId;
using ActionNames = std::unordered_map<std::string, ActionId>;

enum class ActionId {
	INVALID_ACTION = -1,
	Pause,
	MoveViewUp,
	MoveViewDown,
	MoveViewLeft,
	MoveViewRight,
	ResetView,
	ZoomIn,
	ZoomOut,
	ResetZoom,
	Save,
	Quit
};

static const std::string S_EMPTY_STR{ "" };

class Action {


	static const ActionNames s_actionNames;
public:

	////////////////////////////////////////////////////////////
	static const std::string& getActionStr(const ActionId& t_id) {
		auto it{ std::find_if(s_actionNames.cbegin(),s_actionNames.cend(),
		[&t_id](const std::pair<std::string, ActionId>& t_p) {return t_p.second == t_id; }) };
		return (it == s_actionNames.cend() ? S_EMPTY_STR : it->first);
	}

	////////////////////////////////////////////////////////////
	static ActionId getActionId(const std::string& t_name) {
		auto it{ s_actionNames.find(t_name) };
		return (it == s_actionNames.cend() ? ActionId::INVALID_ACTION : it->second);
	}
};

const ActionNames Action::s_actionNames{
	{"Pause",			ActionId::Pause},
	{"MoveViewUp",		ActionId::MoveViewUp},
	{"MoveViewDown",	ActionId::MoveViewDown},
	{"MoveViewLeft",	ActionId::MoveViewLeft},
	{"MoveViewRight",	ActionId::MoveViewRight},
	{"ResetView",		ActionId::ResetView},
	{"ZoomIn",			ActionId::ZoomIn},
	{"ZoomOut",			ActionId::ZoomOut},
	{"ResetZoom",		ActionId::ResetZoom},
	{"Save",			ActionId::Save},
	{"Quit",			ActionId::Quit}
};



#endif // !ACTION_H
struct Binding {
	friend class Action;
	const std::string& m_actionName;
	BoundKeys m_keys; // Contains all the possible keys that can trigger that action

	Binding(const std::string& t_name);
	void addKey(const sf::Keyboard::Key& t_key); // Adds another possible key that can execute this action
	void removeKey(const sf::Keyboard::Key& t_key); // Removes the key
	bool isActive(); // Checks the state of all its keys (doesn't use events) and tells if the binding is positively activated (if at least one of its keys is pressed)
};