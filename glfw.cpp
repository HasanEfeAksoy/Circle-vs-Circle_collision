#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <GLFW/glfw3.h>


const double PI = 3.14159;

double mouseX = 0.0;
double mouseY = 0.0;

int windowSizeX = 0;
int windowSizeY = 0;

void cursor_position_callback(GLFWwindow *window, double x, double y)
{
    mouseX = x;
    mouseY = y;
}


double random (double min, double max)
{
    return ( (double)rand() * ( max - min ) ) / (double)RAND_MAX + min;
}
double map_range(double value, double a, double b, double c, double d) {
    value = (value - a) / (b - a);
    return c + value * (d - c);
}



class Circle {
public:
    double x = 0.0;
    double y = 0.0;
    double radius = 0.1f;


    Circle(double x, double y, double radius) {
        this->x = x;
        this->y = y;
        this->radius = radius;
    }
    ~Circle() {}


    virtual void tick() {
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < 360; i++) {
            glVertex2f(this->x + cos(i * PI / 180) * this->radius, this->y + sin(i * PI / 180) * this->radius);
        }
        glEnd();


        if (this->x - this->radius <= -1.0) {
            this->x = -1.0 + this->radius;
        }
        else if (this->x + this->radius >= 1.0) {
            this->x = 1.0 - this->radius;
        }
        if (this->y - this->radius <= -1.0) {
            this->y = -1.0 + this->radius;
        }
        else if (this->y + this->radius >= 1.0) {
            this->y = 1.0 - this->radius;
        }

    }
};


int main()
{
    srand(time(0));

    GLFWwindow* window = nullptr;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(500, 500, "Window", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwWindowHint(GLFW_RESIZABLE, 0);

    glfwMakeContextCurrent(window);

    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwGetWindowSize(window, &windowSizeX, &windowSizeY);


    Circle* circles[20];
    for (int i = 0; i < sizeof(circles) / sizeof(Circle*); i++) {
        circles[i] = new Circle(random(-1.0, 1.0), random(-1.0, 1.0), 0.1);
    }

    while (!glfwWindowShouldClose(window))
    {
        circles[0]->x = map_range(mouseX, 0.0, windowSizeX, -1.0, 1.0);
        circles[0]->y = map_range(mouseY, 0.0, windowSizeY, 1.0, -1.0);

        for (int i = 0; i < sizeof(circles) / sizeof(Circle*); i++) {
            for (int j = 0; j < sizeof(circles) / sizeof(Circle*); j++) {
                if (i != j) {
                    
                    double distance = sqrt((circles[i]->x - circles[j]->x) * (circles[i]->x - circles[j]->x) + (circles[i]->y - circles[j]->y) * (circles[i]->y - circles[j]->y));

                    if (distance < circles[i]->radius + circles[j]->radius) {
                        double collideIn = (distance - circles[i]->radius - circles[j]->radius) / 2;

                        double circleSubX = circles[i]->x - (collideIn * (circles[i]->x - circles[j]->x) / distance);
                        double circleSubY = circles[i]->y - (collideIn * (circles[i]->y - circles[j]->y) / distance);
                        circles[i]->x = circleSubX;
                        circles[i]->y = circleSubY;
                    }
                }
            }
        }

        
        glClear(GL_COLOR_BUFFER_BIT);
        
        for (int i = 0; i < sizeof(circles) / sizeof(Circle*); i++) {
            circles[i]->tick();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
