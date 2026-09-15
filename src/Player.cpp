#include "Player.hpp"
#include "FallenHuman.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <algorithm>
#include <iostream>


using namespace sf;


 Player::Player() : sprite(texture){
    if(!texture.loadFromFile("assets/LonePlayerSpritesheet.png")){
        std::cerr << "error while loading the texture file \n ";
    }
    sprite.setTextureRect(IntRect({0,0},{ssframeWidth,ssframeHeight}));
    sprite.setScale({playerWidth / ssframeWidth, playerHeight / ssframeHeight});
    currentssColumn = 0;
    currentState = PlayerState::IdleRight;
    frameDuration = 0.15f;
    playerSpeed_ = 5.0f;
    playerX_ = 400.0f;
    playerY_ = groundLevel;
    playerVerticalSpeed_ = 0.0f;
    isGrounded_ = true;
    isAttacking_ = false;
    immunityClock.start();
    setSsPosition(playerX_, playerY_);

 }
// getters
std::vector<Projectile> &Player::getProjectile(){
    return projectiles;
}
float Player::getPlayerX(){
    return playerX_;
}

float Player::getPlayerY(){
    return playerY_;
}

float Player::getPlayerJumpForce(){
    return playerJumpForce_;
}
float Player::getPlayerSpeed(){
    return playerSpeed_;
}
float Player::getGravity(){
    return gravity;
}
float Player::getPlayerVerticalSpeed(){
    return playerVerticalSpeed_;
}
float Player::getPlayerHp(){
    return playerHp;
}
bool Player::getIsAttacking(){
    return isAttacking_;
}
bool Player::getIsGorounded(){
    return isGrounded_;
}
bool Player::getIsDead(){
    return isDead;
}

void Player::respawn(){
    isDead = false;
    playerHp = maxPlayerHp;
    playerX_ = 400.0f;
    playerY_ = groundLevel;
    playerVerticalSpeed_ = 0.0f;
    isGrounded_ = true;
    isAttacking_ = false;
    currentState = PlayerState::IdleRight;
    currentssColumn = 0;
    sprite.setTextureRect(IntRect({0, 0}, {ssframeWidth, ssframeHeight}));
    setSsPosition(playerX_, playerY_);
    immunityClock.restart();
}



//setters
void Player::setPlayerSpeed(float s){
    playerSpeed_ = s;
}
void Player::setPlayerX(float x){
    playerX_ = x;
}
void Player::setPlayerY(float y){
    playerY_ = y;
}
void Player::setPlayerVerticalSpeed(float v){
    playerVerticalSpeed_ = v;
}
void Player::setIsAttacking(bool i){
    isAttacking_ = i;
}
void Player::setIsGrounded(bool i){
    isGrounded_ = i;
}


int Player::drawPlayer(RenderWindow &window, float deltaTime){
    window.draw(sprite);
    // draw and update projectiles
    updateProjectiles(window, deltaTime);
    return 0;
}
void Player::setState(PlayerState newState){
    if(currentState != newState){
        currentState = newState;
        currentssColumn = 0;
        ssanimationmClock.restart();
    }
}
void Player::updateAnimation(){
    if(isDead){
        if(ssanimationmClock.getElapsedTime().asSeconds() >= frameDuration){
            if(currentssColumn < sstotalColumns - 1){
                currentssColumn++;
                int currentRow = static_cast<int>(currentState);
                int posX = currentssColumn * ssframeWidth;
                int posY = currentRow * ssframeHeight;
                sprite.setTextureRect(sf::IntRect({posX,posY},{ssframeWidth,ssframeHeight}));
            }
            ssanimationmClock.restart();
        }
        return;
    }

    if(ssanimationmClock.getElapsedTime().asSeconds() >= frameDuration){
        currentssColumn = (currentssColumn + 1) % sstotalColumns;
        int currentRow = static_cast<int>(currentState);
        int posX = currentssColumn * ssframeWidth;
        int posY = currentRow * ssframeHeight;

        sprite.setTextureRect(IntRect({posX,posY},{ssframeWidth,ssframeHeight}));
        ssanimationmClock.restart();

        if((currentState == PlayerState::AttackRight || currentState == PlayerState::AttackLeft)
            && currentssColumn == sstotalColumns - 1){
            isAttacking_ = false;
        }
    }
}
void Player::setSsPosition(float x,float y){
    sprite.setPosition({x,y});

}


void Player::handlePlayerMovement(float deltaTime){
    const float distanceScale = deltaTime * 60.0f;
    bool isMoving = false;

    if(Keyboard::isKeyPressed(Keyboard::Key::D) && playerX_ < 800 - playerWidth){
        setPlayerX(std::min(playerX_ + playerSpeed_ * distanceScale, 800 - playerWidth));
        lastKeyPressed = 'D';
        isMoving = true;
    }
    else if(Keyboard::isKeyPressed(Keyboard::Key::A) && playerX_ > 0){
        setPlayerX(std::max(playerX_ - playerSpeed_ * distanceScale, 0.0f));
        lastKeyPressed = 'A';
        isMoving = true;
    }

    const bool jumpIsPressed = Keyboard::isKeyPressed(Keyboard::Key::Space);
    if(jumpIsPressed && !jumpWasPressed_ && isGrounded_){
        playerVerticalSpeed_ = jumpStrenght;
        isGrounded_ = false;
    }
    jumpWasPressed_ = jumpIsPressed;

    if(!isGrounded_){
        playerVerticalSpeed_ += gravity * distanceScale;
        playerY_ += playerVerticalSpeed_ * distanceScale;

        if(playerY_ >= groundLevel){
            playerY_ = groundLevel;
            playerVerticalSpeed_ = 0;
            isGrounded_ = true;
        }
    }

    if(isAttacking_){
        setState(currentState);
    }
    else if(!isGrounded_){
        setState(lastKeyPressed == 'A' ? PlayerState::JumpLeft : PlayerState::JumpRight);
    }
    else if(isMoving){
        setState(lastKeyPressed == 'D' ? PlayerState::WalkRight : PlayerState::WalkLeft);
    }
    else{
        setState(lastKeyPressed =='D' ? PlayerState::IdleLeft : PlayerState::IdleRight); //the names are wrong im too lazy to change them
    }
    setSsPosition(playerX_, playerY_);
}

void Player::handlePlayerAttack(){ 
    const bool mouseLeft = Mouse::isButtonPressed(Mouse::Button::Left);
    if(mouseLeft){
        isAttacking_ = true;
        if(lastKeyPressed == 'D'){
            setState(PlayerState::AttackRight);
        }
        else if(lastKeyPressed == 'A'|| lastKeyPressed == ' '){
            setState(PlayerState::AttackLeft);
        }
        // spawn projectile if cooldown elapsed
        if(projectileClock.getElapsedTime().asSeconds() >= projectileCooldown){
            Projectile p;
            float spawnX = (lastKeyPressed == 'D') ? (playerX_ + playerWidth) : (playerX_ - 100.0f);
            float spawnY = playerY_ + playerHeight/2.f - 20.f;
            p.setSsPosition(spawnX, spawnY);
            p.setState((lastKeyPressed == 'D') ? ProjectileState::ani1R : ProjectileState::ani1L);
            projectiles.push_back(p);
            projectileClock.restart();
        }
    }
}

void Player::updateProjectiles(sf::RenderWindow &window, float deltaTime){
    const float projMoveSpeed = 10.f;
    const float distanceScale = deltaTime * 60.0f;
    for(size_t i = 0; i < projectiles.size(); ++i){
        Projectile &pr = projectiles[i];
        pr.updateAnimation();
        int dir = (static_cast<int>(pr.getState()) < 4) ? 1 : -1;
        pr.updatePosition(projMoveSpeed * dir * distanceScale);
        pr.drawProjectile(window);
        // remove if offscreen
        if(pr.isOffscreen(800, 600)){
            projectiles.erase(projectiles.begin() + i);
            --i;
        }
    }
}
sf::FloatRect Player::getPlayerHitbox(){
    sf::FloatRect rect({playerX_,playerY_ + playerHitboxHeight/2},{playerHitboxWidht,playerHitboxHeight});
    return rect;
}

void Player::checkCollisionWithEnemy(FallenHuman *fallenHumanP){
    if(isDead || fallenHumanP == nullptr || fallenHumanP->getIsDead()){
        return;
    }

    sf::FloatRect tempEnemyRect = fallenHumanP ->getHitbox();
   // sf::FloatRect tempEnemyAttackRect = fallenHumanP ->getAttackHitbox();
    if(getPlayerHitbox().findIntersection(tempEnemyRect) /*|| getPlayerHitbox().findIntersection(tempEnemyAttackRect)*/){
        if(immunityClock.getElapsedTime().asSeconds() >= immunityCooldown){
        playerHp = std::max(0.0f, playerHp - fallenHumanP->getAttackDamage());
        if(playerHp <= 0.0f){
            isDead = true;
            isAttacking_ = false;
            currentState = lastKeyPressed == 'D' ? PlayerState::DeathR : PlayerState::DeathL;
            currentssColumn = 0;
            ssanimationmClock.restart();
        }
        std::cout << playerHp << std::endl;
        immunityClock.restart();
        }
    
    }
}


