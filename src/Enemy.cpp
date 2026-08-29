#include "Enemy.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <iostream>

Enemy::Enemy() : enemySprite(enemyTexture){
    immunityClock.start();
    enemyHp = 30.f;
    enemySpeed = 3.0f;
    enemyX = 100;
    enemyY = 450 - enemyHeight*2;
    enemyWidth = 40;
    enemyHeight = 40;

}

float Enemy::getEnemyWidth(){
    return enemyWidth;
}
float Enemy::getEnemyHeight(){
    return enemyHeight;
}
float Enemy::getEnemyHp(){
    return enemyHp;
}
float Enemy::getEnemySpeed(){
    return enemySpeed;
}
float Enemy::getEnemyX(){
    return enemyX;
}
float Enemy::getEnemyY(){
    return enemyY;
}

void Enemy::drawEnemy(sf::RenderWindow &window){
    //temporary logic to draw a squared foe TODO ADD A SPRITE AND TEXTURE ALONG WITH THE OTHER NECESSARY METHODS
    sf::RectangleShape enemy;
    enemy.setFillColor(sf::Color::Green);
    enemy.setSize({enemyWidth,enemyHeight});
    enemy.setPosition({enemyX,enemyY});
    window.draw(enemy);
}

void Enemy::checkCollisions(std::vector<Projectile> &projvect){
   
    sf::FloatRect enemyRect({enemyX,enemyY-enemyHeight},{enemyWidth,enemyHeight});
    for(int i = 0; i < projvect.size(); i++){
        sf::FloatRect tempRect = projvect[i].getProjectileHitbox();
        if(enemyRect.findIntersection(tempRect)){
            if(immunityClock.getElapsedTime().asSeconds() >= immunityTime){
            enemyHp -= projvect[i].getProjectileDamage();
            std::cout << enemyHp << std::endl;
            immunityClock.restart();
        }
    }

        }
    }


