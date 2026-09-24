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
   static constexpr   int totalSsColums = 4;
   static constexpr   int totalSsRows = 6;
   static constexpr float attackCooldown = 0.5f;
   static constexpr float verticalVisionTolerance = 24.0f;
   static constexpr  float deathAnimationDuration = 1.0f;
    int currentSsColumn = 0;
    int currentSsRow = 0; 
    FallenHumanState enemyState = FallenHumanState::Idle;
    FallenHumanState previousState = FallenHumanState::Idle;
    bool isDead = false;
    bool attackHitboxActive = false;
    sf::Clock deathClock;
    sf::Clock attackClock;
    int ssFrameHeight = 171;
    int ssFrameWidth = 300;
    float attackHitboxW;
    float attackHitboxH;
    float attackDamage;
   public:
    FallenHuman(sf::Vector2f spawnPos);
    void updateAnimation() override;
    void checkEnemyAndPlayerPosition(Player *playerP,float x,float y,float deltaTime) override;
    void checkEnemyAndPlayerPosition(Player *playerP,float x,float y,float deltaTime,
        const std::vector<sf::FloatRect>& collisionBoxes) override;
    void setState(FallenHumanState newState);
    void handleDeath() override;
    bool getIsDead() const override;
    bool getIsAttacking() const override;
    void setSsPosition(float x,float y);
    sf::FloatRect getHitbox() const override;
    sf::FloatRect getAttackHitbox() const override;
    float getFhHitboxHeght();
    float getFhHitboxWidth();
    float getAttackDamage() const override;
    float getSpeed() override;

   private:
    void setPosition(float x, float y);
};



#endif