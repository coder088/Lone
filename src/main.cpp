#include <SFML/Graphics.hpp>
#include <optional>
#include "Player.hpp"
using namespace sf;
using namespace std;

int main() {
    RenderWindow window(VideoMode({800,600}), "Lone",Style::Default,State::Windowed);
    window.setFramerateLimit(100);
    Player player;
    RectangleShape groundLine({800,10});
    groundLine.setPosition({0,400});
    groundLine.setFillColor(Color::Green);
    player.setPlayerWidth(100);
    player.setPlayerHeight(100);
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
        player.handlePlayerInput(window);
        //draw
         window.clear(sf::Color::Black);
         window.draw(groundLine);
         player.drawPlayer(window);
         if(player.getIsAttacking()){
            RectangleShape attackhitbox;
            attackhitbox.setSize({50,50});
            attackhitbox.setPosition({player.getPlayerX() + player.getPlayerWidth(), player.getPlayerY() + player.getPlayerheight()/2 -25});
            attackhitbox.setFillColor(Color::Yellow);
            window.draw(attackhitbox);
        }

         window.display();
    }

    return 0;
}