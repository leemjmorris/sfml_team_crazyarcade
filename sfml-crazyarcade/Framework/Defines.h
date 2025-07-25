#pragma once

#define DEF_DEV

enum class SortingLayers
{
	Background,
	Foreground,
	Default,
	UI,
};

enum class Origins
{
	TL, TC, TR,	// 0 1 2
	ML, MC, MR,	// 3 4 5
	BL, BC, BR, // 6 7 8
	Custom
};

// LMJ: Map Editor Added to the SCENE
enum class SceneIds
{
	None = -1,
	Dev1,
	Dev2,
	MapEditor,
	Ready,
	Demo,
	Count,
};

enum class Sides
{
	Left,
	Right,
	None,
};
