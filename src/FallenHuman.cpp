
#include <iostream>
#include <SFML/Graphics.hpp>
#include <memory>
#include "lone/Enemy.hpp"
#include "lone/FallenHuman.hpp"
#include "lone/Player.hpp"


   
    FallenHuman::FallenHuman(sf::Vector2f spawnPos) : Enemy(){
      if(!enemyTexture.loadFromFile("assets/FallenHumanSS.png")){
         std::cerr << "an error occurrend while loading the enemy sprite sheet \n";
      }
        attackDamage = 10;
        enemyHp = 100;
        enemySpeed = 4;
        enemyX = spawnPos.x;  
        enemyY = spawnPos.y;
        enemyWidth = 100;
        enemyHeight = 100;
        hitboxH = 120;
        hitboxW = 85;
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


void FallenHuman::updateAnimation(){
    // Gestisci lo stato di morte
    if(isDead){
      enemyState = FallenHumanState::Death;        
        // Aggiorna i frame dell'animazione di morte
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

        // Se l'animazione di attacco è finita, torna a Idle
        if((enemyState == FallenHumanState::AttackRight || enemyState == FallenHumanState::AttackLeft)
            && currentSsColumn == totalSsColums - 1){
            isAttacking = false;
            attackClock.restart();
            setState(FallenHumanState::Idle);
            }
    }
}

void FallenHuman::checkEnemyAndPlayerPosition(Player *playerP,float x,float y){
   sf::FloatRect playerHiboxRect = playerP -> getPlayerHitbox();
   if(playerHiboxRect.findIntersection(getHitbox())){
      // Controlla se il cooldown di attacco è trascorso
      if(attackClock.getElapsedTime().asSeconds() >= attackCooldown){
         if(playerP -> getPlayerX() > enemyX){ //checks if the player is further from the screen border than the enemy and if it is it means its on his right
            setState(FallenHumanState::AttackRight);
            isAttacking = true;
         }
         else{
            setState(FallenHumanState::AttackLeft);
            isAttacking = true;
         }
         attackClock.restart();
      }
  }
  else{
      // Il player si è allontanato, torna a Idle
      if(enemyState == FallenHumanState::AttackRight || enemyState == FallenHumanState::AttackLeft){
          setState(FallenHumanState::Idle);
          isAttacking = false;
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
void FallenHuman::setSsPosition(float x,float y){
   enemySprite.setPosition({x,y});

}
sf::FloatRect FallenHuman::getHitbox() const{
   sf::FloatRect rect({enemyX,enemyY + hitboxH/2},{hitboxW,hitboxH});
   return rect;
}
sf::FloatRect FallenHuman::getAttackHitbox() const{
   sf::FloatRect rect({enemyX + hitboxW,enemyY + hitboxH/2},{attackHitboxW,attackHitboxH});
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

