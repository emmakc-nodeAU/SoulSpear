#pragma once
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

namespace aie
{
	class Font;
	class Texture;
	class Renderer2D;
}

class Character
{
public:
	enum State
	{
		IDLE,
		TALKING,
		FACE_BACK,
		HURT
	};
private:
	using TextureMap = std::unordered_map<State, aie::Texture*>;
	using SpriteMap = std::unordered_map<std::string, TextureMap>;
public:
	Character(std::string baseSprite = "adventurer");
	~Character();

	void setSprite(std::string sprite);
	std::string getSprite() const;

	void setState(State eState);
	State getState() const;

	void setPosition(const glm::vec2& pos);
	glm::vec2 getPosition() const;
	
	void setSpeechColour(const glm::vec4& colour);
	void speak(std::string speechText, float speechTime);

	void update(float deltaTime);
	void render(aie::Renderer2D* renderer );
private:
	void loadTextures();

	void loadTexturesForSpite(const std::string& sprite);
	void unloadTextures();

	std::string m_baseSprite;
	State		m_ePlayerState;
	glm::vec2	m_renderPosition;

	bool		m_isSpeaking;
	std::string m_speakingText;
	float		m_remainingSpeechTime;
	glm::vec4	m_speechColour;

	SpriteMap	m_textures;
	aie::Font*	m_speechFont;
};
