#include <SFML/Graphics.hpp>
#include <optional>
using namespace sf;
using namespace std;

int main() {
    RenderWindow window(VideoMode({800,600}), "Lone",Style::Default,State::Windowed);
    window.setFramerateLimit(100);
    RectangleShape rect({100,100});
    rect.setFillColor(Color::Green);
    rect.setOutlineColor(Color::Yellow);
    while (window.isOpen()){
        while (const optional event = window.pollEvent()) {
            if (event->is <Event::Closed>()) {
                window.close();
            }
            else if(const auto* KeyPressed = event ->getIf<Event::KeyPressed>()){
                if(KeyPressed->scancode == Keyboard::Scancode::Escape){
                    window.close();
                }
            }
        }
        //update

        //draw
         window.clear(sf::Color::Black);
         window.draw(rect);


         window.display();
    }

    return 0;
}