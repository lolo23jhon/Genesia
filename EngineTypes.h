#ifndef ENGINE_TYPES_H
#define ENGINE_TYPES_H

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


#endif // !ENGINE_TYPES_H