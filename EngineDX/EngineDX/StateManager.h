#pragma once
#include <vector>

class GameState;

class StateManager
{
public:
	StateManager(void);
	~StateManager(void);
	void Load();
	void Unload();
	void Init();
	void Free();
	void Update();
	void Draw();

	bool Running() {return m_running;}
	void Quit()    {m_running = false;}

private:
	std::vector<GameState *> states;
	bool m_running;


};

