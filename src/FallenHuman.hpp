#ifndef FALLENHUMAN_HPP
#define FALLENHUMAN_HPP
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Enemy.hpp"

class Player;

enum class FallenHumanState{
    Idle = 0,
    WalkRight = 1,
    WalkLeft = 2,
    AttackRight = 3,
    AttackLeft =4,
    Death = 5
 
 };


class FallenHuman : public Enemy{
   private:
    const  int totalSsColums = 4;
    const  int totalSsRows = 6;
    int currentSsColumn = 0;
    int currentSsRow = 0; 
    FallenHumanState enemyState = FallenHumanState::Idle;
    FallenHumanState previousState = FallenHumanState::Idle;
    bool isDead = false;
    sf::Clock deathClock;
    const float deathAnimationDuration = 1.0f;
    sf::Clock attackClock;
    const float attackCooldown = 0.5f;
    int ssFrameHeight = 171;
    int ssFrameWidth = 300;
    float attackHitboxW;
    float attackHitboxH;
    float attackDamage;
    
   public:
    FallenHuman(sf::Vector2f spawnPos);
    void updateAnimation();
    void checkEnemyAndPlayerPosition(Player *playerP,float x,float y);
    void setState(FallenHumanState newState);
    void handleDeath();
    bool getIsDead();
    void setSsPosition(float x,float y);
    sf::FloatRect getHitbox();
    sf::FloatRect getAttackHitbox();
    float getFhHitboxHeght();
    float getFhHitboxWidth();
    float getAttackDamage();


};



#endif