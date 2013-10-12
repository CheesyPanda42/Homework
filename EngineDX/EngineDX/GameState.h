#pragma once
class GameState
{
public:
	GameState(void);
	~GameState(void);

	void Load();
	void Init();
	void Update();
	void Draw();
	void Free();
	void Unload();

};

