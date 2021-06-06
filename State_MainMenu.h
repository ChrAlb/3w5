#pragma once
#include "BaseState.h"
#include "EventManager.h"
#include "SharedContext.h"

class State_MainMenu : public BaseState{
public:
	State_MainMenu(StateManager* l_stateManager);
	~State_MainMenu();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& l_time);
	void Draw();
	void Play(EventDetails* l_details);
	void Edit(EventDetails* l_details);
	void Quit(EventDetails* l_details);
	void Lab(EventDetails* l_details);

private:
	sf::Font m_font;
	sf::Text m_text;
	sf::Sprite m_bgSprite;

	sf::Vector2f m_buttonSize;
	sf::Vector2f m_buttonPos;
	unsigned int m_buttonPadding;

	
};