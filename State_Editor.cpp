#include "State_Editor.h"
#include "StateManager.h"



State_Editor::State_Editor(StateManager* l_stateManager)
	: BaseState(l_stateManager) {}

State_Editor::~State_Editor()
{

}

void State_Editor::OnCreate()
{
	m_Map_Size = { 64,21 };

	m_debuging = true;

	m_TileView.reset(sf::FloatRect(0, 0, 400, sf::VideoMode::getDesktopMode().height));
	m_DesignView.reset(sf::FloatRect(400, 0, sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height));
	m_DesignView.setCenter(sf::VideoMode::getDesktopMode().width/2, sf::VideoMode::getDesktopMode().height/2);
	m_LevelView.reset(sf::FloatRect(0, 0, (m_Map_Size.x * Sheet::Tile_Size), (m_Map_Size.y * Sheet::Tile_Size)));
	m_LayerView.reset(sf::FloatRect(0, 0, sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height));

	m_TileLevelSize.x = 6;
	m_TileLevelSize.y = 12;

	m_LevelSize.x = m_Map_Size.x;
	m_LevelSize.y = m_Map_Size.y;

	m_pos_TileArray.x = 50;
	m_pos_TileArray.y = 50;

	m_pos_DesingArray.x = 400;
	m_pos_DesingArray.y = 0;

	m_mouse_pressed = false;

	GUI_Manager* gui = m_stateMgr->GetContext()->m_guiManager;
	gui->LoadInterface(StateType::Edit, "LevelEditor.interface", "LevelEditor");
	gui->GetInterface(StateType::Edit, "LevelEditor")->SetPosition(sf::Vector2f(25.f, 900.f));

	TileFlache = { m_pos_TileArray.x,m_pos_TileArray.y, m_TileLevelSize.x * Sheet::Tile_Size,   m_TileLevelSize.y * Sheet::Tile_Size };
	DesignFlache = { m_pos_DesingArray.x, m_pos_DesingArray.y,m_LevelSize.x * Sheet::Tile_Size, m_LevelSize.y * Sheet::Tile_Size };

	EventManager* evMgr = m_stateMgr->GetContext()->m_eventManager;
	evMgr->AddCallback(StateType::Edit, "Mouse_Left", &State_Editor::MouseClick, this);
	evMgr->AddCallback(StateType::Edit, "Mouse_Unpressed", &State_Editor::MouseUnClick, this);
	evMgr->AddCallback(StateType::Edit, "LevelEditor_OK", &State_Editor::OK, this);
	evMgr->AddCallback(StateType::Edit, "LevelEditor_Abbrechen", &State_Editor::Abbrechen, this);
	evMgr->AddCallback(StateType::Edit, "LevelEditor_Laden", &State_Editor::Load, this);

	m_gameMap = new Map(m_stateMgr->GetContext(),this);
	m_gameMap->LoadMap("media/Maps/editordefault.map");
	
	
}

void State_Editor::OnDestroy()
{
	EventManager* evMgr = m_stateMgr->GetContext()->m_eventManager;
	evMgr->RemoveCallback(StateType::Edit, "Mouse_Left");
	evMgr->RemoveCallback(StateType::Edit, "Mouse_Unpressed");
	evMgr->RemoveCallback(StateType::Edit, "LevelEditor_OK");
	evMgr->RemoveCallback(StateType::Edit, "LevelEditor_Abbrechen");
	evMgr->RemoveCallback(StateType::Edit, "Laden");
}

void State_Editor::Activate()
{
}

void State_Editor::Deactivate()
{
}

void State_Editor::Update(const sf::Time& l_time)
{
	m_mouseposition = sf::Mouse::getPosition();
	
	if (CheckMouspos())
		if (inDesignFlache)
		{
			
			if (m_mouseposition.x > 1800)
				m_DesignView.move(1, 0);
			else if (m_mouseposition.x < 500)
				m_DesignView.move(-1, 0);
			

			
			m_stateMgr->GetContext()->m_wind->GetRenderWindow()->setView(m_DesignView);
            sf::FloatRect viewSpace = m_stateMgr->GetContext()->m_wind->GetViewSpace();
			
			if (viewSpace.left <= 0) 
			{
				m_DesignView.setCenter(viewSpace.width / 2, m_DesignView.getCenter().y);
				m_stateMgr->GetContext()->m_wind->GetRenderWindow()->setView(m_DesignView);

				
			}

			else
			{
				if (viewSpace.left + viewSpace.width > (m_gameMap->GetMapSize().x )* Sheet::Tile_Size)
				{
					m_DesignView.setCenter(((m_gameMap->GetMapSize().x) * Sheet::Tile_Size) - (viewSpace.width / 2), m_DesignView.getCenter().y);
					m_stateMgr->GetContext()->m_wind->GetRenderWindow()->setView(m_DesignView);
				}
			}
			

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		     m_DesignView.move(1.5, 0);

	    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		     m_DesignView.move(-1.5, 0);

		}
	

	if (inDesignFlache && m_Tile_picked && m_mouse_pressed)
	{

		m_stateMgr->GetContext()->m_wind->GetRenderWindow()->setView(m_DesignView);
		sf::Vector2f pixpos = m_stateMgr->GetContext()->m_wind->GetRenderWindow()->mapPixelToCoords(sf::Mouse::getPosition());
		pixpos.x = floor(pixpos.x / 50);
		pixpos.y = floor(pixpos.y / 50);
		sf::Vector2i coord;
		coord.x = pixpos.x;
		coord.y = pixpos.y;

		m_gameMap->InsertTile(coord, m_picked_TileNumber);


	}

}

int State_Editor::Calc_TileNumber(sf::Vector2i mousepos)
{

	sf::Vector2f pos;
	int Number;

	mousepos.x = mousepos.x - m_pos_TileArray.x;
	mousepos.y = mousepos.y - m_pos_TileArray.y;

	pos.x = (int)mousepos.x / Sheet::Tile_Size;
	Number = pos.x;

	pos.y = (int)mousepos.y / Sheet::Tile_Size;

	Number = Number + pos.y * m_TileLevelSize.x;

	if (Number == 64 || Number == 65)
		Number == 63;

	return Number;
}



void State_Editor::Draw()
{
	
	
	m_stateMgr->GetContext()->m_wind->GetRenderWindow()->clear(sf::Color(255, 160, 0));
    m_stateMgr->GetContext()->m_wind->GetRenderWindow()->setView(m_DesignView);
	m_DesignView.setViewport(sf::FloatRect(0.2, 0, 0.8, 1));
	
	m_gameMap->Draw(0);

	
	m_stateMgr->GetContext()->m_wind->GetRenderWindow()->setView(m_TileView);
	m_TileView.setViewport(sf::FloatRect(0, 0, 0.21, 1));
	m_gameMap->drawTileSheet();

	m_stateMgr->GetContext()->m_wind->GetRenderWindow()->setView(m_LevelView);
	m_LevelView.setViewport(sf::FloatRect(0, 0.58, 0.19, 0.19));
	m_gameMap->Draw(0);

	if (m_Tile_picked)
	{

		m_stateMgr->GetContext()->m_wind->GetRenderWindow()->setView(m_LayerView);
		sf::RectangleShape rectangle;
		rectangle.setPosition(m_ClickedTile.left, m_ClickedTile.top);
		rectangle.setSize(sf::Vector2f(m_ClickedTile.width, m_ClickedTile.height));
		rectangle.setOutlineThickness(6);
		rectangle.setOutlineColor(sf::Color::Blue);
		rectangle.setFillColor(sf::Color::Transparent);

		m_stateMgr->GetContext()->m_wind->GetRenderWindow()->draw(rectangle);

	}

	if (m_debuging)
	{
		
			
	}

	
}

bool State_Editor::CheckMouspos()
{   
	inTileFlache = false;
	inDesignFlache = false;
	
	if (
		(m_mouseposition.x > DesignFlache.left) && (m_mouseposition.x < DesignFlache.left + DesignFlache.width) &&
		(m_mouseposition.y > DesignFlache.top) && (m_mouseposition.y < DesignFlache.top + DesignFlache.height)
		)
		inDesignFlache  = true;
	
	if (
		(m_mouseposition.x > TileFlache.left) && (m_mouseposition.x < TileFlache.left + TileFlache.width) &&
		(m_mouseposition.y > TileFlache.top) && (m_mouseposition.y < TileFlache.top + TileFlache.height)
		)
		inTileFlache = true;

	if (inTileFlache || inDesignFlache)

		return true;
	else
		return false;
}

void State_Editor::MouseClick(EventDetails* l_details)
{
	sf::Vector2i mousepos = l_details->m_mouse;
	
	if (inTileFlache)
	{
		m_Tile_picked = true;
		m_ClickedTile.left = (mousepos.x / Sheet::Tile_Size) * Sheet::Tile_Size;
		m_ClickedTile.top = (mousepos.y / Sheet::Tile_Size) * Sheet::Tile_Size;
		m_ClickedTile.width = Sheet::Tile_Size;
		m_ClickedTile.height = Sheet::Tile_Size;

		m_picked_TileNumber = Calc_TileNumber(mousepos);
	}

	if (inDesignFlache)
	   m_mouse_pressed = true;
}

void State_Editor::MouseUnClick(EventDetails* l_details)
{
	m_mouse_pressed = false;
}

void State_Editor::OK(EventDetails* l_details)
{
	GUI_Interface* menu = m_stateMgr->GetContext()->m_guiManager->GetInterface(StateType::Edit, "LevelEditor");
	std::string name = menu->GetElement("DatName")->GetText();
	name = name + ".map";

	std::string path;
	path = "media\\Maps\\";
	m_gameMap->SaveMap(path + name);
	
	m_stateMgr->Remove(StateType::Edit);
	m_stateMgr->SwitchTo(StateType::MainMenu);
}

void State_Editor::Abbrechen(EventDetails* l_details)
{
	m_stateMgr->Remove(StateType::Edit);
	m_stateMgr->SwitchTo(StateType::MainMenu);
}

void State_Editor::Load(EventDetails* l_details)
{
	GUI_Interface* menu = m_stateMgr->GetContext()->m_guiManager->GetInterface(StateType::Edit, "LevelEditor");
	std::string name = menu->GetElement("DatName")->GetText();
	name = name + ".map";

	std::string path;
	path = "media\\Maps\\";
	//m_gameMap->PurgeMap();
	m_gameMap->LoadMap(path + name);
	

}

void State_Editor::MainMenu(EventDetails* l_details)
{
	m_stateMgr->SwitchTo(StateType::MainMenu);
}
