
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

void FallenHuman::checkEnemyAndPlayerPosition(Player *playerP, float x, float y, float deltaTime){
   static const std::vector<sf::FloatRect> noCollisionBoxes;
   checkEnemyAndPlayerPosition(playerP, x, y, deltaTime, noCollisionBoxes);
}

void FallenHuman::checkEnemyAndPlayerPosition(Player *playerP, float, float, float deltaTime,
   const std::vector<sf::FloatRect>& collisionBoxes){
   if (playerP == nullptr || isDead) {
      return;
   }

   const float distanceScale = std::min(deltaTime, 0.05f) * 60.0f;
   const float hitboxOffsetX = (enemyWidth - hitboxW) / 2.0f;
   const sf::FloatRect playerHitbox = playerP->getPlayerHitbox();
   const sf::FloatRect enemyHitbox = getHitbox();
   const bool canSeePlayerOnYAxis =
      playerHitbox.position.y < enemyHitbox.position.y + enemyHitbox.size.y
      && playerHitbox.position.y + playerHitbox.size.y > enemyHitbox.position.y;
   const float distanceToPlayer = playerP->getPlayerX() - enemyX;
   const bool playerIsClose = std::abs(distanceToPlayer) <= triggetDistance;
   const bool playerIsInAttackRange = playerHitbox.findIntersection(enemyHitbox).has_value();

   if (playerIsInAttackRange && canSeePlayerOnYAxis) {
      if (!isAttacking && attackClock.getElapsedTime().asSeconds() >= attackCooldown) {
         setState(distanceToPlayer >= 0.0f ? FallenHumanState::AttackRight
                                          : FallenHumanState::AttackLeft);
         isAttacking = true;
         attackHitboxActive = false;
         attackClock.restart();
      }
   } else if (isAttacking) {
      isAttacking = false;
      attackHitboxActive = false;
      setState(FallenHumanState::Idle);
   } else if (playerIsClose && canSeePlayerOnYAxis) {
      const float movement = std::clamp(distanceToPlayer,
         -speed * distanceScale, speed * distanceScale);
      setState(distanceToPlayer >= 0.0f ? FallenHumanState::WalkRight
                                       : FallenHumanState::WalkLeft);
      setPosition(enemyX + movement, enemyY);
   } else {
      setState(FallenHumanState::Idle);
   }

   // Enemies gravity
   if (isGrounded) {
      const float enemyBottom = enemyY + enemyHeight;
      bool supported = false;
      for (const sf::FloatRect& collisionBox : collisionBoxes) {
         const bool overlapsX = enemyX + hitboxOffsetX + hitboxW > collisionBox.position.x
            && enemyX + hitboxOffsetX < collisionBox.position.x + collisionBox.size.x;
         if (overlapsX && std::abs(enemyBottom - collisionBox.position.y) < 1.0f) {
            supported = true;
            break;
         }
      }
      if (!supported) {
         isGrounded = false;
      }
   }

   if (!isGrounded) {
      const float previousBottom = enemyY + enemyHeight;
      verticalSpeed += gravity * distanceScale;
      const float nextY = enemyY + verticalSpeed * distanceScale;
      const float nextBottom = nextY + enemyHeight;
      float landingY = nextY;
      bool landed = false;

      if (verticalSpeed >= 0.0f) {
         for (const sf::FloatRect& collisionBox : collisionBoxes) {
            const bool overlapsX = enemyX + hitboxOffsetX + hitboxW > collisionBox.position.x
               && enemyX + hitboxOffsetX < collisionBox.position.x + collisionBox.size.x;
            const bool crossesTop = previousBottom <= collisionBox.position.y
               && nextBottom >= collisionBox.position.y;
            if (overlapsX && crossesTop
                && (!landed || collisionBox.position.y < landingY + enemyHeight)) {
               landingY = collisionBox.position.y - enemyHeight;
               landed = true;
            }
         }
      }

      if (landed) {
         setPosition(enemyX, landingY);
         verticalSpeed = 0.0f;
         isGrounded = true;
      } else {
         setPosition(enemyX, nextY);
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
