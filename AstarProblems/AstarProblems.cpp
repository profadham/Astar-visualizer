#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <vector>
#include "Astar.h"
#include "Problem.h"

using namespace std;
using namespace sf;



void visualizePoint(Point point, RenderWindow* Window) {
	RectangleShape rect;
	Font font;
	Text text;
	int size = 25;
	rect.setPosition(Vector2f(point.getX() - (size / 2), point.getY() - (size / 2)));
	rect.setSize(Vector2f(size, size));
	rect.setFillColor(Color::White);

	font.loadFromFile("DayDisplay-3zRPX.ttf");
	text.setFont(font);
	text.setPosition(Vector2f(point.getX() - (size / 3), point.getY() - (size / 2)));
	string indexStr = to_string(point.getIndex());
	text.setString(indexStr);
	text.setFillColor(Color::Blue);
	text.setCharacterSize(size / 1.5);


	Window->draw(rect);
	Window->draw(text);
}

void showPointIndex(Point point, RenderWindow* Window) {
	Font font2;
	Text text2;
	int size = 25;
	font2.loadFromFile("DayDisplay-3zRPX.ttf");
	text2.setFont(font2);
	text2.setPosition(Vector2f(point.getX() - (size / 3), point.getY() - (size / 2)));
	string indexStr = to_string(point.getIndex());
	text2.setString(indexStr);
	text2.setFillColor(Color::Blue);
	text2.setCharacterSize(size / 1.5);



	Window->draw(text2);
}

void changeFillPoint(Point point, RenderWindow* Window) {
	int size = 25;
	RectangleShape rect2;
	rect2.setPosition(Vector2f(point.getX() - (size / 2), point.getY() - (size / 2)));
	rect2.setSize(Vector2f(size, size));
	rect2.setFillColor(Color::Green);
	Window->draw(rect2);
}

void drawBarrier(Barrier barr, RenderWindow* window) {
	sf::Vertex line[2];
	line[0].position = Vector2f(barr.getPoint1().getX(), barr.getPoint1().getY());
	line[0].color = Color::White;
	line[1].position = Vector2f(barr.getPoint2().getX(), barr.getPoint2().getY());
	line[1].color = Color::White;

	sf::Vertex line2[2];
	line2[0].position = Vector2f(barr.getPoint1().getX() - 1, barr.getPoint1().getY() - 1);
	line2[0].color = Color::White;
	line2[1].position = Vector2f(barr.getPoint2().getX() - 1, barr.getPoint2().getY() - 1);
	line2[1].color = Color::White;

	sf::Vertex line3[2];
	line3[0].position = Vector2f(barr.getPoint1().getX() + 1, barr.getPoint1().getY() + 1);
	line3[0].color = Color::White;
	line3[1].position = Vector2f(barr.getPoint2().getX() + 1, barr.getPoint2().getY() + 1);
	line3[1].color = Color::White;

	window->draw(line, 2, Lines);
	window->draw(line2, 2, Lines);
	window->draw(line3, 2, Lines);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	RenderWindow window(VideoMode(WIDTH, HEIGHT), "title", Style::Titlebar | Style::Close);
	Event ev;


	Robot robot = Robot(30, 5);
	robot.testMode = 1;
	Astar<Node> astar;
	astar.testMode = 1;
	astar.Solve(&robot, robot.getEndNode());

	bool showSolution = false;

	while (window.isOpen()) {
		

		while (window.pollEvent(ev)) {
			switch (ev.type) {
			case Event::Closed:
				window.close();
				break;
			case Event::KeyPressed:
				if (ev.key.code == Keyboard::Escape) {
					window.close();
				}
				if (ev.key.code == Keyboard::Enter) {
					if (showSolution) {
						showSolution = false;
					}
					else {
						showSolution = true;
					}
					
				}
				if (ev.key.code == Keyboard::R) {
					
				}
			}
		}

		
		//Render

		window.clear(Color(0, 0, 0, 255));


		

		//visualize before
		for (int i = 0; i < robot.getBarriers().size(); i++) {
			drawBarrier(robot.getBarriers()[i], &window);
		}
		for (int i = 0; i < robot.getNodes().size(); i++) {
			visualizePoint(robot.getNodes()[i].point, &window);
		}


		//visualise solution
		if (showSolution) {
			Point prevPoint = astar.getClose()[0].point;

			for (int i = 1; i < astar.getClose().size(); i++) {

				sf::Vertex line[2];
				line[0].position = Vector2f(prevPoint.getX(), prevPoint.getY());
				line[0].color = Color::Green;
				line[1].position = Vector2f(astar.getClose()[i].point.getX(), astar.getClose()[i].point.getY());
				line[1].color = Color::Green;

				window.draw(line, 2, Lines);

				changeFillPoint(prevPoint, &window);
				showPointIndex(prevPoint, &window);
				prevPoint = astar.getClose()[i].point;
			}

			changeFillPoint(astar.getClose().back().point, &window);
			showPointIndex(astar.getClose().back().point, &window);
		}


		
		

		window.display();
	}
}

