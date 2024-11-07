#pragma once

class DebugBox
{
private:
	sf::RectangleShape shape;
	bool isVisible;

public:
	DebugBox();

	void SetBounds(const sf::FloatRect& bounds);
	sf::RectangleShape GetShape() const { return shape; }
	void SetVisible(bool v) { isVisible = v; }
	bool IsVisible() const { return isVisible; }

	void Draw(sf::RenderWindow& window);
	void SetOutlineColor(sf::Color color) { shape.setOutlineColor(color); }
};