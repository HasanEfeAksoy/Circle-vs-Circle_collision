#include <iostream>
#include <vector>
#include <math.h>
#include <time.h>
#include <SFML/Graphics.hpp>


class Circle {
private:
    sf::CircleShape* shape = nullptr;
    sf::Vector2f* position = nullptr;
    double radius = 10.0;

public:
    Circle(const sf::Vector2f& position, double radius) {
        this->shape = new sf::CircleShape();

        this->position = new sf::Vector2f(position);
        this->radius = radius;

        this->shape->setRadius(this->radius);
        this->shape->setOrigin(this->radius, this->radius);
        this->shape->setPosition(*this->position);
    }
    ~Circle() {
        delete shape;
        delete position;
    }

    virtual void tick (sf::RenderWindow* window) {
        window->draw(*this->shape);

        if (this->position->x - this->radius <= 0) {
            this->position->x = 0 + this->radius;
        }
        if (this->position->x + this->radius >= window->getSize().x) {
            this->position->x = window->getSize().x - this->radius;
        }
        if (this->position->y - this->radius <= 0) {
            this->position->y = 0 + this->radius;
        }
        if (this->position->y + this->radius >= window->getSize().y) {
            this->position->y = window->getSize().y - this->radius;
        }

        this->shape->setRadius(this->radius);
        this->shape->setOrigin(this->radius, this->radius);
        this->shape->setPosition(*this->position);
    }


    sf::Vector2f* get_position() const {
        return this->position;
    }
    void set_position(const sf::Vector2f& position) {
        this->position = new sf::Vector2f(position);
    }

    double get_radius() const {
        return this->radius;
    }
    void set_radius(double radius) {
        this->radius = radius;
    }

};


int main()
{
    srand(time(NULL));

    sf::RenderWindow window(sf::VideoMode(500, 500), "Window");
    window.setFramerateLimit(60);

    std::vector<Circle*> circles;

    for (int i = 0; i < 15; i++) {
        circles.push_back(new Circle(sf::Vector2f(rand() % window.getSize().x, rand() % window.getSize().y), 30.0f));
    }


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        circles[0]->set_position((sf::Vector2f)sf::Mouse::getPosition(window));

        for (Circle* circle : circles) {
            for (Circle* otherCircle : circles) {
                if (circle != otherCircle) {
                    
                    double distance = sqrt((circle->get_position()->x - otherCircle->get_position()->x) * (circle->get_position()->x - otherCircle->get_position()->x) + (circle->get_position()->y - otherCircle->get_position()->y) * (circle->get_position()->y - otherCircle->get_position()->y));

                    if (distance < circle->get_radius() + otherCircle->get_radius()) {
                        double collideIn = (distance - circle->get_radius() - otherCircle->get_radius()) / 2;

                        double circleSubX = circle->get_position()->x - (collideIn * (circle->get_position()->x - otherCircle->get_position()->x) / distance);
                        double circleSubY = circle->get_position()->y - (collideIn * (circle->get_position()->y - otherCircle->get_position()->y) / distance);
                        circle->set_position(sf::Vector2f(circleSubX, circleSubY));
		    }
                }
            }
        }



        window.clear();
        
        for (auto& circle : circles) {
            circle->tick(&window);            
        }

        window.display();
    }

    return 0;
}