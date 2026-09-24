#ifndef ENEMY_HPP
#define ENEMY_HPP
#include "Projectile.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <iostream>
class Player;

//base class for any enemy
class Enemy{
    protected:
     float speed;
     float triggetDistance;
     float enemyHp;
     float enemySpeed;
     int enemyWidth;
     int enemyHeight;
     float enemyX;
     float enemyY;
     float immunityTime = 0.75f;
     float hitboxH;
     float hitboxW;
     bool isAttacking;
     sf::Texture enemyTexture;
     sf::Sprite enemySprite;
     sf::Clock animationClock;
     sf::Clock immunityClock;
     float frameDuration;
     

    


    public:
      Enemy();
     virtual ~Enemy() = default;
     virtual float getEnemyHp();
     virtual float getEnemySpeed();
     virtual int getEnemyWidth();
     virtual int getEnemyHeight();
     virtual float getEnemyX();
     virtual float getEnemyY();
     virtual float getSpeed();
     virtual void drawEnemy(sf::RenderWindow &window);
    virtual void checkCollisionsWithProjectiles(std::vector<Projectile> &projvect);
     virtual void checkCollisionsWithProjectiles(std::vector<Projectile> &projvect,float hitboxH,float hitboxW);
    virtual void updateAnimation();
    virtual void checkEnemyAndPlayerPosition(Player *playerP, float x, float y, float deltaTime);
    virtual void handleDeath();
    virtual bool getIsDead() const;
    virtual bool getIsAttacking() const;
    virtual sf::FloatRect getHitbox() const;
    virtual sf::FloatRect getAttackHitbox() const;
    virtual float getAttackDamage() const;
    virtual void setSsPosition(float x,float y);
  

};



#endif