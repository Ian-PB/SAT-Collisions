#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#include "Globals.h"

enum class Direction
{
	None,
	Up,
	Down,
	Left,
	Right
};

class Polygon
{
public:
	Polygon();
	sf::ConvexShape getBody() { return body; }

	void makePolygon(int t_points, int t_radius);

	// Movement
	void move();
	void checkDirection();
	void rotate();

	// Collision Detection
	bool SATPolygonCollision(const sf::ConvexShape& polygonA, const sf::ConvexShape& polygonB);

private:
	sf::ConvexShape body;
	sf::Vector2f position;
	int speed = 5;
	int rotation = 0;

	Direction direction;

	// Collision calculations
	sf::Vector2f TransformPoint(sf::Vector2f t_point, float t_rotation, sf::Vector2f t_translation);
	sf::Vector2f CalculateNormal(sf::Vector2f t_pointA, sf::Vector2f t_pointB);
	std::vector<sf::Vector2f> CalculateAllNormals(const std::vector<sf::Vector2f>& t_vertices);
	float Vector2fDot(sf::Vector2f a, sf::Vector2f b) { return (a.x * b.x + a.y * b.y); }

};

