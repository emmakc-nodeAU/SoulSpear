#include "Character.h"

#include <assert.h>

#include "Renderer2D.h"
#include "Texture.h"
#include "Font.h"

Character::Character(std::string baseSprite /*= "adventurer"*/)
	: m_ePlayerState(IDLE)
	, m_renderPosition(0, 0)
	, m_isSpeaking(false)
	, m_speakingText()
	, m_speechColour(0.0f, 0.0f, 0.0f, 1.0f)
{
	loadTextures();
	setSprite(baseSprite);
}

Character::~Character()
{
	unloadTextures();
}

void Character::setSprite(std::string sprite)
{
	m_baseSprite = sprite;
}

std::string Character::getSprite() const
{
	return m_baseSprite;
}

void Character::setState(State eState)
{
	m_ePlayerState = eState;
}

Character::State Character::getState() const
{
	return m_ePlayerState;
}

void Character::setPosition(const glm::vec2& pos)
{
	m_renderPosition = pos;
}

glm::vec2 Character::getPosition() const
{
	return m_renderPosition;
}



void Character::setSpeechColour(const glm::vec4& colour)
{
	m_speechColour = colour;
}

void Character::speak(std::string speechText, float speechTime)
{
	m_isSpeaking = true;
	m_speakingText = speechText;
	m_remainingSpeechTime = speechTime;
}

void Character::update(float deltaTime)
{
	if (m_isSpeaking && m_remainingSpeechTime >= 0.0f)
	{
		m_remainingSpeechTime -= deltaTime;
		if (m_remainingSpeechTime < 0) m_isSpeaking = false;
	}
}

void Character::render(aie::Renderer2D* renderer)
{
	renderer->drawSprite(m_textures[m_baseSprite][m_ePlayerState],
		m_renderPosition.x, m_renderPosition.y);

	if (m_isSpeaking)
	{
		float textWidth = renderer->measureTextWidth(m_speechFont, m_speakingText.c_str());
		glm::vec4 oldColor = glm::vec4(
			renderer->getRenderColorR(),
			renderer->getRenderColorG(),
			renderer->getRenderColorB(),
			renderer->getRenderColorA());
		renderer->setRenderColour(m_speechColour.r, m_speechColour.g, m_speechColour.b, m_speechColour.a);
		
		renderer->drawText(m_speechFont, m_speakingText.c_str(),
			m_renderPosition.x - (textWidth * 0.5f), m_renderPosition.y + 50.0f);

		renderer->setRenderColour(oldColor.r, oldColor.g, oldColor.b, oldColor.a);
	}
						
}

void Character::loadTextures()
{
	if (!m_textures.empty()) return; //Already loaded!

	loadTexturesForSpite("adventurer");
	loadTexturesForSpite("female");
	loadTexturesForSpite("player");
	loadTexturesForSpite("zombie");
	loadTexturesForSpite("soldier");

	m_speechFont = new aie::Font("./font/consolas.ttf", 32);
}

void Character::loadTexturesForSpite(const std::string & sprite)
{
	std::string path = "./Textures/Characters/" + sprite + "/" + sprite + "_";
	m_textures[sprite][IDLE] = new aie::Texture((path + "idle.png").c_str());
	m_textures[sprite][TALKING] = new aie::Texture((path + "talk.png").c_str());
	m_textures[sprite][FACE_BACK] = new aie::Texture((path + "back.png").c_str());
	m_textures[sprite][HURT] = new aie::Texture((path + "hurt.png").c_str());
}

void Character::unloadTextures()
{
	for (auto& spriteTextures : m_textures)
	{
		for (auto& texture : spriteTextures.second)
		{
			delete texture.second;
		}
	}
	m_textures.clear();

	delete m_speechFont;
	m_speechFont = nullptr;
}

