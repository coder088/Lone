#include "Player.hpp"
#include "FallenHuman.hpp"
#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>

enum class GameState{
    Menu,
    Game
};

int main() {
    static constexpr int windowWidth = 1920;
    static constexpr int windowHeight = 1080;
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktopMode, "Lone", sf::Style::Default, sf::State::Fullscreen);
    sf::View view(sf::FloatRect({0.f,0.f},{800.f,600.f}));
    window.setView(view);
    window.setFramerateLimit(60);
    Player player;
    Player *playerP = &player;
    FallenHuman fH({100,400});
    FallenHuman *fallenHumanP = &fH;
    sf::Font font;
    // Fade overlays use the game view's 800x600 coordinates.
    sf::RectangleShape fadeOutOverlay(sf::Vector2f(windowWidth, windowHeight));
    fadeOutOverlay.setFillColor(sf::Color(0, 0, 0, 0));
    float fadeAlpha = 0.0f;
    float fadeSpeed = 150.0f;
    bool isFadeComplete = false;
    sf::RectangleShape fadeInOverlay(sf::Vector2f(windowWidth, windowHeight));
    fadeInOverlay.setFillColor(sf::Color(0, 0, 0, 255));
    float fadeInAlpha = 255.0f;
    float fadeInSpeed = 150.0f;
    bool isFadeInActive = false;
    const bool fontLoaded = font.openFromFile("CinzelDecorative-Regular.ttf")
        || font.openFromFile("assets/CinzelDecorative-Regular.ttf")
        || font.openFromFile("build/CinzelDecorative-Regular.ttf")
        || font.openFromFile("assets/tuffy.ttf")
        || font.openFromFile("extern/SFML/examples/island/resources/tuffy.ttf");
    if(!fontLoaded){
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

    //death screen text handling
    sf::Text youDiedText(font,"YOU DIED", 70);
    youDiedText.setFillColor(sf::Color::Red);
    youDiedText.setPosition({400.f - youDiedText.getGlobalBounds().size.x /2.f,200.f});

    sf::Text deathScreenText(font,"Press R to respawn",20);
    deathScreenText.setFillColor(sf::Color::White);
    deathScreenText.setPosition({400.f - deathScreenText.getGlobalBounds().size.x /2.f,350.f});

    //enum variable to handle the state the game is in
    GameState currentState = GameState::Menu;
    sf::Clock frameClock;

    //main window
    while (window.isOpen()) {
        const float deltaTime = frameClock.restart().asSeconds();
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
                    fadeInAlpha = 255.0f;
                    isFadeInActive = true;
                }
              }
            else if(currentState == GameState::Game && !player.getIsDead()){
                if(keyPressed->scancode == sf::Keyboard::Scancode::M){
                    currentState = GameState::Menu;
                }
            }
            else if(currentState == GameState::Game && player.getIsDead()){
                if(keyPressed->scancode == sf::Keyboard::Scancode::R){
                    player.respawn();
                    fadeAlpha = 0.0f;
                    isFadeComplete = false;
                    fadeOutOverlay.setFillColor(sf::Color::Transparent);
                    fadeInAlpha = 255.0f;
                    isFadeInActive = true;
                }
            }

            
        
        }
    }
    if(currentState == GameState::Game){
        //update here all the game logic
        if(!player.getIsDead()){
            player.handlePlayerMovement(deltaTime);
            player.handlePlayerAttack();
            player.checkCollisionWithEnemy(fallenHumanP);
            fH.checkCollisionsWithProjectiles(player.getProjectile(),fH.getFhHitboxHeght(),fH.getFhHitboxWidth());
            fH.handleDeath();
            fH.checkEnemyAndPlayerPosition(playerP,player.getPlayerX(),player.getPlayerY());
        }
        player.updateAnimation();
        fH.updateAnimation();
        if (isFadeInActive) {
            fadeInAlpha -= fadeInSpeed * deltaTime;
            if (fadeInAlpha <= 0.0f) {
                fadeInAlpha = 0.0f;
                isFadeInActive = false;
            }
            fadeInOverlay.setFillColor(sf::Color(0, 0, 0, static_cast<std::uint8_t>(fadeInAlpha)));
        }

        if(player.getIsDead() && !isFadeComplete){
            fadeAlpha += fadeSpeed * deltaTime;
            if (fadeAlpha >= 255.0f) {
                fadeAlpha = 255.0f;
                isFadeComplete = true;
            }
            fadeOutOverlay.setFillColor(sf::Color(0, 0, 0, static_cast<std::uint8_t>(fadeAlpha)));

         }


    }

    window.clear(sf::Color::Black);
    if(currentState == GameState::Menu){
        window.draw(titleScreenText);
        window.draw(startGameText);
        window.draw(exitGametext);
    }
    else if(currentState == GameState::Game){
        player.drawPlayer(window, deltaTime);
        if(!fH.getIsDead()){
            fH.drawEnemy(window);
        }
        if(player.getIsDead()){
            window.draw(fadeOutOverlay);
        }
        if (player.getIsDead() && isFadeComplete) {
            window.draw(youDiedText);
            window.draw(deathScreenText);
        }
        if (isFadeInActive) {
            window.draw(fadeInOverlay);
        }
    }

    window.display();
}
        return 0;
    }


  



