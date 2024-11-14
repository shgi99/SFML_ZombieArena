#pragma once

enum class AnimationLoopTypes
{
	Single,
	Loop,
	PingPong, // 정방향으로 진행되다가 역방향으로 진행되는 느낌
};

struct AnimationFrame
{
	std::string texId;
	sf::IntRect texCoord; // 스프라이트 짜를 좌표?
	bool flipX = false;

	AnimationFrame() {}
	AnimationFrame(const std::string& texId, const sf::IntRect& texCoord)
		: texId(texId), texCoord(texCoord) {}
};

struct AnimationClip
{
	std::string id;
	AnimationLoopTypes loopType;
	int fps = 30;
	std::vector<AnimationFrame> frames;

	bool loadFromFile(const std::string& filePath);
};

