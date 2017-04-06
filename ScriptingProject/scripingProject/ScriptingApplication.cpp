#include "ScriptingApplication.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"

#include "Character.h"
#include "ScriptedCharacterController.h"

ScriptingApplication::ScriptingApplication()
{

}

ScriptingApplication::~ScriptingApplication()
{

}

bool ScriptingApplication::startup()
{
	m_2dRenderer = new aie::Renderer2D();
	m_background = new aie::Texture("./textures/background.png");
	m_character = new Character();

	m_character->setSpeechColour(glm::vec4(0, 0, 0, 1));
	m_character->setPosition(glm::vec2(getWindowWidth() * 0.5f, getWindowHeight() * 0.35f));
	m_cameraX = 0;
	m_cameraY = 0;
	m_timer = 0;

	m_controller = new ScriptedCharacterController(*m_character);
	m_controller->loadScript("./scripts/story.script");


	return true;
}

void ScriptingApplication::shutdown()
{
	delete m_background;

	delete m_controller;
	delete m_character;

	delete m_2dRenderer;
}

void ScriptingApplication::update(float deltaTime)
{

	m_timer += deltaTime;

	m_character->update(deltaTime);
	m_controller->update(deltaTime);

	// input example
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_R)) m_controller->reloadScript();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void ScriptingApplication::draw()
{

	// wipe the screen to the background colour
	clearScreen();

	// set the camera position before we begin rendering
	m_2dRenderer->setCameraPos(m_cameraX, m_cameraY);

	// begin drawing sprites
	m_2dRenderer->begin();

	m_2dRenderer->drawSprite(m_background, 0, 0, (float)getWindowWidth(), (float)getWindowHeight(), 0, 0, 0, 0);
	m_character->render(m_2dRenderer);

	// done drawing sprites
	m_2dRenderer->end();
}
