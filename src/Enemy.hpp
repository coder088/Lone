#ifndef ENEMY_HPP
#define ENEMY_HPP
#include "Player.hpp"
#include "Projectile.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <iostream>
//base class for any enemy
class Enemy{
    protected:
     float enemyHp;
     float enemySpeed;
     float enemyWidth;
     float enemyHeight;
     float enemyX;
     float enemyY;
     float immunityTime = 1;
     sf::Texture enemyTexture;
     sf::Sprite enemySprite;
     sf::Clock animationClock;
     sf::Clock immunityClock;

    


    public:
    Enemy();
     float getEnemyHp();
     float getEnemySpeed();
     float getEnemyWidth();
     float getEnemyHeight();
     float getEnemyX();
     float getEnemyY();

     void attack();
     void drawEnemy(sf::RenderWindow &window);
     void checkCollisions(std::vector<Projectile> &projvect);

};



#endif