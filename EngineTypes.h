#ifndef ENGINE_TYPES_H
#define ENGINE_TYPES_H

enum class EngineState {
	PAUSED,
	RUNNING
};

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
	Quit,
	ACTION_COUNT
};


#endif // !ENGINE_TYPES_H