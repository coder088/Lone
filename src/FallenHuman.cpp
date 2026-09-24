
#include <iostream>
#include <SFML/Graphics.hpp>
#include <memory>
#include "Enemy.hpp"
#include "Player.hpp"
#include "FallenHuman.hpp"
#include <algorithm>
#include <cmath>


   
    FallenHuman::FallenHuman(sf::Vector2f spawnPos) : Enemy(){
      if(!enemyTexture.loadFromFile("assets/FallenHumanSS.png")){
         std::cerr << "an error occurrend while loading the enemy sprite sheet \n";
      }
       speed = 4.5f;
       triggetDistance = 150.f;
        attackDamage = 10;
        enemyHp = 100;
        enemySpeed = 4;
        enemyX = spawnPos.x;  
        enemyY = spawnPos.y;
        enemyWidth = 100;
        enemyHeight = 100;
        hitboxH = 120;
        hitboxW = 50;
        attackHitboxW = 50;
        attackHitboxH = 50;

        frameDuration = 0.15f;
        attackClock.restart();
        enemySprite.setTexture(enemyTexture);
        enemySprite.setTextureRect(sf::IntRect({0,0},{ssFrameWidth,ssFrameHeight}));
      enemySprite.setScale({static_cast<float>(enemyWidth) / ssFrameWidth,
                  static_cast<float>(enemyHeight) / ssFrameHeight});
        enemySprite.setPosition({enemyX, enemyY});
     }

float FallenHuman::getSpeed(){return speed;}
void FallenHuman::updateAnimation(){
    if(isDead){
      enemyState = FallenHumanState::Death;        
   
        if(animationClock.getElapsedTime().asSeconds() >= frameDuration){
          if(currentSsColumn < totalSsColums - 1){
            currentSsColumn++;
          }
          int currentRow = static_cast<int>(enemyState);
          int posX = currentSsColumn * ssFrameWidth;
          int posY = currentRow * ssFrameHeight;

          enemySprite.setTextureRect(sf::IntRect({posX,posY},{ssFrameWidth,ssFrameHeight}));
          animationClock.restart();
        }
        return;
    }

    if(animationClock.getElapsedTime().asSeconds() >= frameDuration){
      currentSsColumn = (currentSsColumn + 1) % totalSsColums;
        int currentRow = static_cast<int>(enemyState);
        int posX = currentSsColumn * ssFrameWidth;
        int posY = currentRow * ssFrameHeight;

        enemySprite.setTextureRect(sf::IntRect({posX,posY},{ssFrameWidth,ssFrameHeight}));
        animationClock.restart();

      
        if((enemyState == FallenHumanState::AttackRight || enemyState == FallenHumanState::AttackLeft)
            && currentSsColumn == totalSsColums - 1){
            isAttacking = false;
         attackHitboxActive = false;
            attackClock.restart();
            setState(FallenHumanState::Idle);
            }
      else if(enemyState == FallenHumanState::AttackRight
         || enemyState == FallenHumanState::AttackLeft){
         attackHitboxActive = currentSsColumn == 2;
      }
    }
}

void FallenHuman::checkEnemyAndPlayerPosition(Player *playerP,float, float, float deltaTime){
   if (playerP == nullptr || isDead) {
      return;
   }

   const sf::FloatRect playerHitbox = playerP->getPlayerHitbox();
   const sf::FloatRect enemyHitbox = getHitbox();
   const bool canSeePlayerOnYAxis =
      playerHitbox.position.y < enemyHitbox.position.y + enemyHitbox.size.y
      && playerHitbox.position.y + playerHitbox.size.y > enemyHitbox.position.y;

   if (!canSeePlayerOnYAxis) {
      isAttacking = false;
      attackHitboxActive = false;
      setState(FallenHumanState::Idle);
      return;
   }

   if(playerHitbox.findIntersection(enemyHitbox)){
      if(!isAttacking && attackClock.getElapsedTime().asSeconds() >= attackCooldown){
         if(playerP -> getPlayerX() > enemyX){ //checks if the player is further from the screen border than the enemy and if it is it means its on his right
            setState(FallenHumanState::AttackRight);
            isAttacking = true;
            attackHitboxActive = false;
         }
         else{
            setState(FallenHumanState::AttackLeft);
            isAttacking = true;
            attackHitboxActive = false;
         }
         attackClock.restart();
      }
   }
   else{
      if(enemyState == FallenHumanState::AttackRight || enemyState == FallenHumanState::AttackLeft){
          setState(FallenHumanState::Idle);
          isAttacking = false;
           attackHitboxActive = false;
      }
      const float distanceToPlayer = playerP->getPlayerX() - enemyX;
      const float frameDelta = std::min(deltaTime, 0.05f);
      const float movement = std::clamp(distanceToPlayer, -speed * frameDelta * 60.0f,
         speed * frameDelta * 60.0f);

      if(std::abs(distanceToPlayer) <= 0.5f){
         setState(FallenHumanState::Idle);
      }
      else if(distanceToPlayer > 0.0f){
         if((playerP -> getPlayerX() - enemyX) <= triggetDistance){
            setState(FallenHumanState::WalkRight);
            setPosition(enemyX + movement, enemyY);
           
         }
         else{
            setState(FallenHumanState::Idle);
         }
      
      }
      else {
         if((enemyX - playerP ->getPlayerX()) <= triggetDistance){
            setState(FallenHumanState::WalkLeft);
            setPosition(enemyX + movement, enemyY);
         }
         else{
            setState(FallenHumanState::Idle);
         }
      }
  }

}
void FallenHuman::setState(FallenHumanState newState){
   if(enemyState != newState){
      enemyState = newState;
       currentSsColumn = 0;
       animationClock.restart();
   }
}
void FallenHuman::handleDeath(){
   if(enemyHp <= 0 && !isDead){
      isDead = true;
      setState(FallenHumanState::Death);
      deathClock.restart();
   }
}
bool FallenHuman::getIsDead() const{
   return isDead && deathClock.getElapsedTime().asSeconds() >= deathAnimationDuration;
}
bool FallenHuman::getIsAttacking() const{
   return attackHitboxActive
      && (enemyState == FallenHumanState::AttackRight
         || enemyState == FallenHumanState::AttackLeft);
}
void FallenHuman::setSsPosition(float x,float y){
   setPosition(x, y);
}

void FallenHuman::setPosition(float x, float y){
   enemyX = x;
   enemyY = y;
   enemySprite.setPosition({enemyX, enemyY});
}
sf::FloatRect FallenHuman::getHitbox() const{
   sf::FloatRect rect({enemyX,enemyY + hitboxH/2},{hitboxW,hitboxH});
   return rect;
}
sf::FloatRect FallenHuman::getAttackHitbox() const{
   const bool attacksRight = enemyState == FallenHumanState::AttackRight;
   const float attackX = attacksRight ? enemyX + hitboxW : enemyX - attackHitboxW;
   sf::FloatRect rect({attackX,enemyY + hitboxH/2},{attackHitboxW,attackHitboxH});
   return rect;
}

float FallenHuman::getFhHitboxHeght(){
   return hitboxH;
}
float FallenHuman::getFhHitboxWidth(){
   return hitboxW;
}
float FallenHuman::getAttackDamage() const{
   return attackDamage;
}

