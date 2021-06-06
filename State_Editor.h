#pragma once
#include "BaseState.h"
#include "Map.h"
#include "EventManager.h"
#include <string>

class State_Editor : public BaseState {
public:
	State_Editor(StateManager* l_stateManager);
	~State_Editor();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& l_time);
	void Draw();

	bool CheckMouspos();
	int  Calc_TileNumber(sf::Vector2i mousepos);
	

	void MouseClick(EventDetails* l_details);
	void MouseUnClick(EventDetails* l_details);
	void OK(EventDetails* l_details);
	void Abbrechen(EventDetails* l_details);
	void Load(EventDetails* l_details);

	void MainMenu(EventDetails* l_details);

private:
	Map* m_gameMap;

	sf::View       m_TileView;
	sf::View       m_DesignView;
	sf::View       m_LevelView;
	sf::View       m_LayerView;

	sf::Vector2i   m_Map_Size;
	sf::Vector2f   m_pos_TileArray;
	sf::Vector2f   m_pos_DesingArray;
	sf::Vector2f   m_TileLevelSize;
	sf::Vector2f   m_LevelSize;

	sf::Vector2i   m_mouseposition;
	sf::Vector2i   m_oldmouseposition;


	sf::FloatRect TileFlache;
	sf::FloatRect DesignFlache;
	sf::FloatRect m_ActualTile;
	sf::FloatRect m_ClickedTile;

	bool inTileFlache;
	bool inDesignFlache;
	bool m_Tile_picked;
	bool m_mouse_pressed;

	bool m_debuging;

	int m_picked_TileNumber;
		
};