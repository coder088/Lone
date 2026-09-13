#include "Player.hpp"
#include "FallenHuman.cpp"
#include <SFML/Graphics.hpp>
#include <optional>

enum class GameState{
    Menu,
    Game
};

int main() {
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktopMode, "Lone", sf::Style::Default, sf::State::Windowed);
    sf::View view(sf::FloatRect({0.f,0.f},{800.f,600.f}));
    window.setView(view);
    window.setFramerateLimit(60);
    Player player;
    Player *playerP = &player;
   
    FallenHuman fH({100,400});
    sf::Font font;
    if(!font.openFromFile("CinzelDecorative-Regular.ttf")){
        std::cerr << "an error occurred while loading the text font \n";
        return -1;
    }
    //Menu / font handling
    sf::Text titleScreenText(font, "LONE",70);
    titleScreenText.setFillColor(sf::Color::White);
    titleScreenText.setPosition({400.f - titleScreenText.getGlobalBounds().size.x / 2.f,200.f});

    sf::Text startGameText(font,"Press SPACE to start the game",20);
    startGameText.setFillColor(sf::Color::White);
    startGameText.setPosition({400.f - startGameText.getGlobalBounds().size.x /2.f,350.f});

    sf::Text exitGametext(font, "Press ESC to exit the game",20);
    exitGametext.setFillColor(sf::Color::White);
    exitGametext.setPosition({400.f- exitGametext.getGlobalBounds().size.x /2.f,450.f});

    //enum variable to handle the state the game is in
    GameState currentState = GameState::Menu;

    //main window
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } 
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if(currentState == GameState::Menu){
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    window.close();
                }
            
             
                if(keyPressed->scancode == sf::Keyboard::Scancode::Space){
                    currentState = GameState::Game;
                }
              }
            
            else if(currentState == GameState::Game){
                if(keyPressed->scancode == sf::Keyboard::Scancode::M){
                    currentState = GameState::Menu;
                }
            }

            
        
        }
    }
    if(currentState == GameState::Game){
        //update here all the game logic
        player.handlePlayerMovement(window);
        player.handlePlayerAttack();  
        player.updateAnimation();
        fH.updateAnimation();
        fH.checkCollisionsWithProjectiles(player.getProjectile(),fH.getFhHitboxHeght(),fH.getFhHitboxWidth());
        fH.handleDeath();
        fH.checkEnemyAndPlayerPosition(playerP,player.getPlayerX(),player.getPlayerY());

    }

        //drawing
        window.clear(sf::Color::Black);
        if(currentState == GameState::Menu){
            //menu graphics handling
            window.draw(titleScreenText);
            window.draw(startGameText);
            window.draw(exitGametext);

        }
        else if(currentState == GameState::Game){
            //draw here all the game elements
            player.drawPlayer(window);
            if(!fH.getIsDead()){
                fH.drawEnemy(window);
            }
        }

   
       

        window.display();
      }
        return 0;
    }

  



