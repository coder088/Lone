#include "Enemy.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <iostream>

Enemy::Enemy() : isAttacking(false), enemySprite(enemyTexture){
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
float Enemy::getSpeed(){return speed;}

void Enemy::drawEnemy(sf::RenderWindow &window){
    window.draw(enemySprite);
}

void Enemy::checkCollisionsWithProjectiles(std::vector<Projectile> &projvect){
    checkCollisionsWithProjectiles(projvect, hitboxH, hitboxW);
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

void Enemy::updateAnimation() {}

void Enemy::checkEnemyAndPlayerPosition(Player *, float, float, float) {}

void Enemy::handleDeath() {}

bool Enemy::getIsDead() const {
    return false;
}

bool Enemy::getIsAttacking() const {
    return isAttacking;
}

sf::FloatRect Enemy::getHitbox() const {
    return sf::FloatRect({enemyX, enemyY + hitboxH / 2.0f}, {hitboxW, hitboxH});
}

sf::FloatRect Enemy::getAttackHitbox() const {
    return sf::FloatRect({enemyX, enemyY + hitboxH / 2.0f}, {hitboxW, hitboxH});
}

float Enemy::getAttackDamage() const {
    return 0.0f;
}
void Enemy::setSsPosition(float x,float y){}


