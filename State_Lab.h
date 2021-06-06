#pragma once
#include "BaseState.h"
#include "EventManager.h"
#include "GUI_Manager.h"
#include "SharedContext.h"




class State_Lab : public BaseState {
public:
	State_Lab(StateManager* l_stateManager);
	~State_Lab();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& l_time);
	void Draw();

	void OK(EventDetails* l_details);
	void Abbrechen(EventDetails* l_details);	
	
	
private:
	sf::Font m_font;
	sf::Text m_text;
	sf::Sprite m_bgSprite;

	sf::View m_DirectoryView;
	sf::View m_LayerView;
	sf::RectangleShape m_Directory;

	sf::Vector2f m_buttonSize;
	sf::Vector2f m_buttonPos;
	unsigned int m_buttonPadding;

	int m_EntryCounter;

	int winposx; 
	int winposy;
	int posx;
	int posy;

	std::string ClickedLabel;

	

};