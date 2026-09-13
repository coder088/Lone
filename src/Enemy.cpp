#include "Enemy.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <iostream>

Enemy::Enemy() : enemySprite(enemyTexture){
    immunityClock.start();
}

int Enemy::getEnemyWidth(){
    return enemyWidth;
}
int Enemy::getEnemyHeight(){
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
    window.draw(enemySprite);
}

void Enemy::checkCollisionsWithProjectiles(std::vector<Projectile> &projvect,float hitboxH,float hitboxW){
   
    sf::FloatRect enemyRect({enemyX,enemyY-hitboxH/2},{hitboxW,hitboxH});
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


