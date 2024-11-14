#pragma once

enum class AnimationLoopTypes
{
	Single,
	Loop,
	PingPong, // ���������� ����Ǵٰ� ���������� ����Ǵ� ����
};

struct AnimationFrame
{
	std::string texId;
	sf::IntRect texCoord; // ��������Ʈ ¥�� ��ǥ?
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

