#include "Player.hpp"
#include <SFML/Graphics.hpp>
#include <optional>


int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Lone", sf::Style::Default, sf::State::Windowed);
    window.setFramerateLimit(60);
    Player player;
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    window.close();
                }
            }
        }
        //drawing
        window.clear(sf::Color::Black);
        player.handlePlayerMovement();
        player.handlePlayerAttack();  
        player.updateAnimation();
        player.drawPlayer(window);
        if(player.getIsAttacking()){
            player.drawPlayerAttackHitbox(window);
        }
        window.display();
      }
        return 0;
    }

  



