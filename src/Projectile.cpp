#include "Projectile.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <iostream>

// static shared texture definitions
sf::Texture Projectile::sharedTexture;
bool Projectile::sharedTextureLoaded = false;

Projectile::Projectile() : sprite(sharedTexture){
    if(!sharedTextureLoaded){
        if(!sharedTexture.loadFromFile("assets/stdProjectileSpritesheet.png")){
            std::cerr << "An error occurred while loading the projectile Sprite \n";
        }
    }
    // Ensure sprite references the loaded shared texture explicitly
    sprite.setTexture(sharedTexture);
    sprite.setTextureRect(sf::IntRect(sf::Vector2i(0,0), sf::Vector2i(ssframeWidth, ssframeHeight)));
    sprite.setScale(sf::Vector2f(projectileHitboxWidth / static_cast<float>(ssframeWidth), projectileHitboxHeight / static_cast<float>(ssframeHeight)));
    currentssColumn = 0;
    currentState = ProjectileState::ani1R;
    frameDuration = 0.10f;
    projectileSpeed = 10.f;
}
void Projectile::drawProjectile(sf::RenderWindow &window){
    window.draw(sprite);
}
void Projectile::setState(ProjectileState newState){
    if(currentState != newState){
        currentState = newState;
        currentssColumn = 0;
        ssanimationmClock.restart();
    }
}
void Projectile::updateAnimation(){
    if(ssanimationmClock.getElapsedTime().asSeconds() >= frameDuration){
        currentssColumn = (currentssColumn + 1) % sstotalColumns;
        int stateIndex = static_cast<int>(currentState);
        int currentRow = (stateIndex < sstotalColumns) ? 0 : 1;
        int posX = currentssColumn * ssframeWidth;
        int posY = currentRow * ssframeHeight;

        sprite.setTextureRect(sf::IntRect(sf::Vector2i(posX,posY), sf::Vector2i(ssframeWidth,ssframeHeight)));
        ssanimationmClock.restart();
    }
}

void Projectile::setSsPosition(float x,float y){
    sprite.setPosition({x,y});
}

void Projectile::updatePosition(float dx){
    sprite.move(sf::Vector2f(dx, 0.f));
}

float Projectile::getX() const{
    return sprite.getPosition().x;
}

sf::FloatRect Projectile::getGlobalBounds() const{
    return sprite.getGlobalBounds();
}

bool Projectile::isOffscreen(float windowWidth, float windowHeight) const{
    auto bounds = getGlobalBounds();

    float left = bounds.position.x;
    float top = bounds.position.y;
    float width = bounds.size.x;
    float height = bounds.size.y;
    return (left + width < 0.f) || (left > windowWidth) || (top > windowHeight) || (top + height < 0.f);
}

ProjectileState Projectile::getState() const{
    return currentState;
}
void Projectile::setProjectileDamage(float d){
    projectileDamage = d;
}

float Projectile::getProjectileDamage(){return projectileDamage;}
float Projectile::getProjectileWidth(){return projectileHitboxWidth;}
float Projectile::getProjectileHeight(){return projectileHitboxHeight;}
