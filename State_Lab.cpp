#include "State_Lab.h"
#include "StateManager.h"
#include <filesystem>



State_Lab::State_Lab(StateManager* l_stateManager)
	: BaseState(l_stateManager) {}

State_Lab::~State_Lab() {}

void State_Lab::OnCreate() {

	m_LayerView.reset(sf::FloatRect(0, 0, sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height));
	m_DirectoryView.reset(sf::FloatRect(700, 50, 500, 500));
	m_Directory.setSize(sf::Vector2f(500, 500));
	m_Directory.setPosition(sf::Vector2f(700, 50));
	m_Directory.setFillColor(sf::Color::Blue);

	winposx = 0;
	winposy = 0;
	posx = 50;
	posy = 30;


	sf::Vector2u windowSize = m_stateMgr->GetContext()->m_wind->GetRenderWindow()->getSize();

	TextureManager* textureMgr = m_stateMgr->GetContext()->m_textureManager;
	textureMgr->RequireResource("BgMenu");
	m_bgSprite.setTexture(*textureMgr->GetResource("BgMenu"));

	
	m_EntryCounter = 0;

	std::string path = "c:\\Users\\albch\\Documents\\GitHub\\3w4\\3w4\\media\\maps";


	std::ofstream file;
	file.open(Utils::GetWorkingDirectory() + "media/GUI_Interfaces/" + "temp.interface");


	std::string line = "Interface SomeWindow TestInterface.style " + Utils::inttostring(winposx) + " " + Utils::inttostring(winposy) + " Immovable Title \"Wäu di Datei\"";
	file << line;

	
	for (const auto& entry : std::filesystem::directory_iterator(path))
	
	{
		m_EntryCounter++;
		std::string tt = entry.path().filename().string();
		//std::string  tt1 = "Label" + Utils::inttostring(m_EntryCounter);
		file << "\n";
		line = "Element Label " +  tt  + " " + Utils::inttostring(posx) + " " + Utils::inttostring(posy * m_EntryCounter) + " Test.style " + "\""+ tt +"\"";
		//line = "Element Label " + tt1 + " " + Utils::inttostring(posx) + " " + Utils::inttostring(posy * m_EntryCounter) + " Test.style " + "\"" + tt + "\"";
		file << line;
		
	}
	file << "\n";
	line = "Element Scrollbar Slider1 0 0 TestScrollbar.style Vertical";
	file << line;

	file.close();
	

	GUI_Manager* gui = m_stateMgr->GetContext()->m_guiManager;
	gui->LoadInterface(StateType::Lab, "temp.interface", "Test");
	gui->GetInterface(StateType::Lab, "Test")->SetPosition(sf::Vector2f(50.f, 50.f));



	gui->LoadInterface(StateType::Lab, "TestOK.interface", "Tes");
	gui->GetInterface(StateType::Lab, "Tes")->SetPosition(sf::Vector2f(50.f, 250.f));

	
    EventManager* evMgr = m_stateMgr->GetContext()->m_eventManager;
	evMgr->AddCallback(StateType::Lab, "Lab_Ok", &State_Lab::OK, this);
	evMgr->AddCallback(StateType::Lab, "Lab_Abbrechen", &State_Lab::Abbrechen, this);
}

void State_Lab::OnDestroy() {
	
	
	EventManager* evMgr = m_stateMgr->GetContext()->m_eventManager;
	evMgr->RemoveCallback(StateType::Lab, "Lab_Ok");
	evMgr->RemoveCallback(StateType::Lab, "Lab_Abbrechen");
	

	m_stateMgr->GetContext()->m_guiManager->RemoveInterface(StateType::Lab, "Test");
	m_stateMgr->GetContext()->m_guiManager->RemoveInterface(StateType::Lab, "Tes");
	
}

void State_Lab::Activate()
{
	

	
}



void State_Lab::Update(const sf::Time& l_time)
{
	GUI_Manager* gui = m_stateMgr->GetContext()->m_guiManager;
	GUI_Interface* i = gui->GetInterface(StateType::Lab, "Test");
	
	Elements elements = i->get_mElements();

	for (auto& itr : elements)
	{
		if ( (itr.second->GetState() == GUI_ElementState::Clicked)  && (itr.second->GetType() != GUI_ElementType::Scrollbar))
					

        		{
					ClickedLabel = itr.second->GetName();
					itr.second->SetState(GUI_ElementState::Neutral);
					return;
				}
			
	
	}

}

void State_Lab::Draw()

{
	
	sf::RenderWindow* window = m_stateMgr->
		GetContext()->m_wind->GetRenderWindow();
	window->setView(m_LayerView);
	window->draw(m_bgSprite);

	window->setView(m_DirectoryView);
	m_DirectoryView.setViewport(sf::FloatRect(0.4, 0.025, 0.3,0.5));
	window->draw(m_Directory);

}

void State_Lab::OK(EventDetails* l_details)
{
	if (ClickedLabel != "")
	{
		
		std::cout << ClickedLabel;
		std::cout <<  "\n";
		
		m_stateMgr->SwitchTo(StateType::MainMenu);
	}
}

void State_Lab::Abbrechen(EventDetails* l_details)
{
	m_stateMgr->SwitchTo(StateType::MainMenu);
}


void State_Lab::Deactivate() {}