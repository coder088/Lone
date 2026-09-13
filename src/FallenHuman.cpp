#include <iostream>
#include <SFML/Graphics.hpp>
#include <memory>
#include "Enemy.hpp"
#include "Player.hpp"

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
    int ssFrameHeight = 171;
    int ssFrameWidth = 300;
    public:
     FallenHuman(sf::Vector2f spawnPos) : Enemy(){
      if(!enemyTexture.loadFromFile("assets/FallenHumanSS.png")){
         std::cerr << "an error occurrend while loading the enemy sprite sheet \n";
      }
        enemyHp = 100;
        enemySpeed = 4;
        enemyX = spawnPos.x;
        enemyY = spawnPos.y;
        enemyWidth = 100;
        enemyHeight = 100;
        hitboxH = 120;
        hitboxW = 100;
        frameDuration = 0.15f;
        enemySprite.setTexture(enemyTexture);
        enemySprite.setTextureRect(sf::IntRect({0,0},{ssFrameWidth,ssFrameHeight}));
        enemySprite.setScale({static_cast<float>(enemyWidth) / ssFrameWidth,
                              static_cast<float>(enemyHeight) / ssFrameHeight});
        enemySprite.setPosition({enemyX, enemyY});
     }


void updateAnimation(){
    // Gestisci lo stato di morte
    if(isDead){
        if(deathClock.getElapsedTime().asSeconds() >= deathAnimationDuration){
            // L'animazione di morte è finita, il nemico è completamente scomparso
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
            setState(FallenHumanState::Idle);
            }
    }
}

void checkEnemyAndPlayerPosition(Player *playerP,float x,float y){
   sf::FloatRect playerHiboxRect = playerP -> getPlayerHitbox();
   if(playerHiboxRect.findIntersection(getHitbox())){
      if(playerP -> getPlayerX() > enemyX){ //checks if the player is further from the screen border than the enemy and if it is it means its on his right
         setState(FallenHumanState::AttackRight);
         isAttacking = true;
      }
      else{
         setState(FallenHumanState::AttackLeft);
         isAttacking = true;
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
void setState(FallenHumanState newState){
   if(enemyState != newState){
      enemyState = newState;
       currentSsColumn = 0;
       animationClock.restart();
   }
}
void handleDeath(){
   if(enemyHp <= 0 && !isDead){
      isDead = true;
      setState(FallenHumanState::Death);
      deathClock.restart();
   }
}
bool getIsDead(){
   return isDead && deathClock.getElapsedTime().asSeconds() >= deathAnimationDuration;
}
void setSsPosition(float x,float y){
   enemySprite.setPosition({x,y});

}
sf::FloatRect getHitbox(){
   sf::FloatRect rect({enemyX,enemyY + hitboxH/2},{hitboxW,hitboxH});
   return rect;
}

float getFhHitboxHeght(){
   return hitboxH;
}
float getFhHitboxWidth(){
   return hitboxW;
}
};