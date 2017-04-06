#include "ScriptedCharacterController.h"

#include <fstream>
#include <sstream>
#include <iterator>
#include <iostream>

#include <algorithm>

#include "Character.h"
ScriptedCharacterController::ScriptedCharacterController(Character& ControlledCharacter)
	: m_controlledCharacter(&ControlledCharacter)
	, m_commandLocation(0)
	, m_isWaiting(false)
	, m_remainingWaitTime(0.0f)
{
}

void ScriptedCharacterController::loadScript(std::string script)
{
	m_isScriptLoaded = false;
	m_scriptPath = script;
	m_commandsToExectute.clear();

	std::ifstream scriptFile(script, std::ios::in);

	std::string text;
	while (scriptFile.good())
	{
		if (std::getline(scriptFile, text))
		{
			m_commandsToExectute.push_back(splitString(text));
		}
	}

	if (!m_commandsToExectute.empty())
	{
		m_isScriptLoaded = true;
		m_commandLocation = 0;
		m_isWaiting = false;
	}
}

void ScriptedCharacterController::reloadScript()
{
	loadScript(m_scriptPath);
}

void ScriptedCharacterController::onSetSprite(const CommandData & data)
{
	// ASSERTS Script Line matches required constraints, our TOKENS.
	assert(data.size() == 2);
	assert(
		data[1] == "adventurer" ||
		data[1] == "female" ||
		data[1] == "player" ||
		data[1] == "zombie" ||
		data[1] == "soldier"
	);
	m_controlledCharacter->setSprite(data[1]);
}

void ScriptedCharacterController::onSetState(const CommandData & data)
{
	std::unordered_map<std::string, Character::State> stringToState;
	stringToState["idle"] = Character::IDLE;
	stringToState["talking"] = Character::TALKING;
	stringToState["face_back"] = Character::FACE_BACK;
	stringToState["hurt"] = Character::HURT;

	assert(data.size() == 2);
	assert(stringToState.find(data[1]) != stringToState.end());

	m_controlledCharacter->setState(stringToState[data[1]]);
}

void ScriptedCharacterController::onWait(const CommandData & data)
{
	float waitTime = std::atof(data[1].c_str());
	assert(data.size() == 2);
	// std::atof returns 0 if it is told to convert a non-number string
	assert(waitTime > 0.0f);

	m_isWaiting = true;
	m_remainingWaitTime = waitTime;
}

void ScriptedCharacterController::onSpeak(const CommandData & data)
{
	assert(data.size() > 2);

	float time = std::atof(data[1].c_str());
	assert(time > 0.0f);

	std::string toSay;
	for (int i = 2; i < data.size(); ++i)
	{
		toSay.append(data[i]);
		toSay.append(" ");
	}
	m_isWaiting = true;
	m_remainingWaitTime = time;
	m_controlledCharacter->speak(toSay, time);
}

void ScriptedCharacterController::setupCommands()
{
	m_allowedCommands["SET_SPRITE"] =
		[this](const CommandData& data) {onSetSprite(data); };
	m_allowedCommands["SPEAK"] =
		[this](const CommandData& data) {onSpeak(data); };
	m_allowedCommands["WAIT"] =
		[this](const CommandData& data) {onWait(data); };
	m_allowedCommands["SET_STATE"] =
		[this](const CommandData& data) {onSetState(data); };
}

void ScriptedCharacterController::update(float deltaTime)
{
	if (m_isScriptLoaded && !m_isWaiting)
	{
		const CommandData& nextCommand =
			m_commandsToExectute[m_commandLocation];
		auto it = m_allowedCommands.find(nextCommand[0]);
		if (it != m_allowedCommands.end())
		{
			// EXECUTE LAMBDA FUNCTION: stored for this command
			// pass through vector of tokens as 1st argument
			it->second(nextCommand);
		}
		else
		{
			std::cout <<
				"Attempted to run invalid command '"
				<< nextCommand[0] << "'\n";
		}
		m_commandLocation++;
		if (m_commandLocation >= m_commandsToExectute.size())
		{
			m_commandLocation = 0;
		}
	}
	if (m_isWaiting)
	{
		m_remainingWaitTime -= deltaTime;
		if (m_remainingWaitTime <= 0) m_isWaiting = false;
	}
	/*
	m_timer += deltaTime;
	m_controller->update(deltaTime);
	m_character->update(deltaTime);*/

}


std::vector<std::string> ScriptedCharacterController::splitString(std::string str)
{
	std::istringstream iss(str);
	return std::vector<std::string>(std::istream_iterator<std::string>(iss),
				std::istream_iterator<std::string>());
}
