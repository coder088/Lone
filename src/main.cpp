#include "Player.hpp"
#include "Enemy.hpp"
#include <SFML/Graphics.hpp>
#include <optional>


int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Lone", sf::Style::Default, sf::State::Windowed);
    window.setFramerateLimit(60);
    Player player;
    Enemy enemy;
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
        enemy.checkCollisions(player.getProjectile());
        if(enemy.getEnemyHp() > 0){
            enemy.drawEnemy(window);
        }

        window.display();
      }
        return 0;
    }

  



