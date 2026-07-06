#pragma once
class Scene
{
public:
	virtual ~Scene() = default;

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	// ƒV[ƒ“Ø‚è‘Ö‚¦
	virtual bool IsEnd() const = 0;
	virtual Scene* GetNextScene() = 0;
};



