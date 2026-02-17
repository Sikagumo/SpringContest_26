#pragma once
class SceneManager;
class ResourceManager;

class StageBase
{
public:

	StageBase(void);

	virtual ~StageBase(void) = default;

	virtual void Init(void) {};
	virtual void Update(void) {};
	virtual void Draw(void) {};
	virtual void DrawDebug(void) {};
	virtual void Release(void) {};

protected:

	SceneManager& sceneMng_;
	ResourceManager& resMng_;
};