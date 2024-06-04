#include "Polygon.h"

Polygon::Polygon()
{
	position = { (float)(rand() % (SCREEN_SIZE / 2)), (float)(rand() % (SCREEN_SIZE / 2)) };
	body.setPosition(position);
}

void Polygon::makePolygon(int t_points, int t_radius)
{
	if (t_points <= 2)
	{
		std::cout << "FAILED TO MAKE POLYGON \n";
	}
	else
	{
		body.setPointCount(t_points);

		const float D_THETA = 2 * PI / t_points;

		for (int i = 0; i < t_points; i++)
		{
			const float THETA = i * D_THETA;

			body.setPoint(i, sf::Vector2f(t_radius * cos(THETA), t_radius * sin(THETA)));
		}
	}
}

void Polygon::move()
{
	sf::Vector2f movement = { 0.0f, 0.0f };

	switch (direction)
	{
	case Direction::None:
		break;

	case Direction::Up:
		movement.y = -speed;
		break;

	case Direction::Down:
		movement.y = speed;
		break;

	case Direction::Left:
		movement.x = -speed;
		break;

	case Direction::Right:
		movement.x = speed;
		break;
	}
	position += movement;
	body.setPosition(position); // change hitbox position
}

void Polygon::checkDirection()
{
	direction = Direction::None;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		direction = Direction::Up;
		move();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		direction = Direction::Down;
		move();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		direction = Direction::Left;
		move();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		direction = Direction::Right;
		move();
	}
}

void Polygon::rotate()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		rotation++;
		body.setRotation(rotation);

		if (rotation > 360)
		{
			rotation = 0;
		}
	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		rotation--;
		body.setRotation(rotation);

		if (rotation < 0)
		{
			rotation = 360;
		}
	}
}

bool Polygon::SATPolygonCollision(const sf::ConvexShape& polygonA, const sf::ConvexShape& polygonB)
{
	//if (polygonA.getGlobalBounds().intersects(polygonB.getGlobalBounds()))
	//{
	//	
	//}

	const size_t nPointsA = polygonA.getPointCount(), nPointsB = polygonB.getPointCount();
	std::vector<sf::Vector2f> verticesA(nPointsA), verticesB(nPointsB);

	for (size_t i = 0; i < nPointsA; i++)
	{
		verticesA[i] = TransformPoint(polygonA.getPoint(i), polygonA.getRotation(), polygonA.getPosition());
	}

	for (size_t i = 0; i < nPointsB; i++)
	{
		verticesB[i] = TransformPoint(polygonB.getPoint(i), polygonB.getRotation(), polygonB.getPosition());
	}

	// calculate normals for each side of polygons
	auto polygonANormals = CalculateAllNormals(verticesA);
	auto polygonBNormals = CalculateAllNormals(verticesB);

	// combine normals of the two polygon in one array to loop over
	std::vector<sf::Vector2f> normals;
	normals.insert(normals.end(), polygonANormals.begin(), polygonANormals.end());
	normals.insert(normals.end(), polygonBNormals.begin(), polygonBNormals.end());

	bool isColliding = true;
	for (sf::Vector2f normal : normals) {

		// min and max projection of polygon A
		float paMin = FLT_MAX, paMax = FLT_MIN;

		for (sf::Vector2f vertex : verticesA)
		{


			float projection = Vector2fDot(vertex, normal);


			paMin = std::min(paMin, projection);
			paMax = std::max(paMax, projection);
		}

		float pbMin = FLT_MAX, pbMax = FLT_MIN;

		for (sf::Vector2f vertex : verticesB)
		{
			float projection = Vector2fDot(vertex, normal);
			pbMin = std::min(pbMin, projection);
			pbMax = std::max(pbMax, projection);
		}

		if (paMax < pbMin || pbMax < paMin)
		{
			isColliding = false;
		}
		else
		{
			// continue checking along other normals
		}
	}

	return isColliding;
}

// Change the point to be in the global quardinates
sf::Vector2f Polygon::TransformPoint(sf::Vector2f t_point, float t_rotation, sf::Vector2f t_translation)
{
	sf::Vector2f transformedPoint(
		t_point.x * cos((float)(t_rotation) * (PI / 180)) - t_point.y * sin((float)(t_rotation) * (PI / 180)),
		t_point.y * cos((float)(t_rotation) * (PI / 180)) - t_point.x * sin((float)(t_rotation) * (PI / 180))
	);

	transformedPoint += t_translation;

	return transformedPoint;
}

sf::Vector2f Polygon::CalculateNormal(sf::Vector2f t_pointA, sf::Vector2f t_pointB)
{
	// Get the vector pointgoing from one point to another
	sf::Vector2f directionVector = t_pointB - t_pointA;

	// Swap the co-efficients and negate one to calculate the normal (not normalized)
	return sf::Vector2f(directionVector.y, -directionVector.x);
}

std::vector<sf::Vector2f> Polygon::CalculateAllNormals(const std::vector<sf::Vector2f>& t_vertices)
{
	std::vector<sf::Vector2f> normals(t_vertices.size());

	for (size_t i = 0; i < t_vertices.size() - 1; i++)
	{
		normals[i] = CalculateNormal(t_vertices[i + 1], t_vertices[i]);
	}

	normals[t_vertices.size() - 1] = CalculateNormal(t_vertices[0], t_vertices[t_vertices.size() - 1]);

	return normals;
}


