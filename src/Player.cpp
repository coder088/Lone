#include "Player.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <iostream>


using namespace sf;
const auto WINDOWHEIGHT = 600;
const auto WINDOWWIDTH = 800;


 Player::Player() : sprite(texture){
    if(!texture.loadFromFile("assets/LonePlayerSpritesheet.png")){
        std::cerr << "error while loading the texture file \n ";
    }
    sprite.setTextureRect(IntRect({0,0},{ssframeWidth,ssframeHeight}));
    sprite.setScale({playerWidth / ssframeWidth, playerHeight / ssframeHeight});
    currentssColumn = 0;
    currentState = PlayerState::Idle;
    frameDuration = 0.15f;
    playerSpeed_ = 5.0f;
    playerX_ = 400.0f;
    playerY_ = 400.0f;
    playerVerticalSpeed_ = 0.0f;
    isGrounded_ = true;
    isAttacking_ = false;
    setSsPosition(playerX_, playerY_);

 }
// getters
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
bool Player::getIsAttacking(){
    return isAttacking_;
}
bool Player::getIsGorounded(){
    return isGrounded_;
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

int Player::drawPlayer(RenderWindow &window){
    window.draw(sprite);
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
    if(ssanimationmClock.getElapsedTime().asSeconds() >= frameDuration){
        currentssColumn = (currentssColumn + 1) % sstotalColumns;
        int currentRow = static_cast<int>(currentState);
        int posX = currentssColumn * ssframeWidth;
        int posY = currentRow * ssframeHeight;

        sprite.setTextureRect(IntRect({posX,posY},{ssframeWidth,ssframeHeight}));
        ssanimationmClock.restart();
    }
}
void Player::setSsPosition(float x,float y){
    sprite.setPosition({x,y});

}


void Player::handlePlayerMovement(){
    if(Keyboard::isKeyPressed(Keyboard::Key::D) && playerX_ != WINDOWWIDTH - playerWidth){
        setState(PlayerState::WalkRight);
        setPlayerX(playerX_ + playerSpeed_);
        lastKeyPressed = 'D';
    }
    else if(Keyboard::isKeyPressed(Keyboard::Key::A) && playerX_ != 0){
        setState(PlayerState::WalkLeft);
        setPlayerX(playerX_ - playerSpeed_);
        lastKeyPressed = 'A';
    }
    if(lastKeyPressed == 'D'){
        setState(PlayerState::WalkRight);
    }
    else if(lastKeyPressed == 'A'){
        setState(PlayerState::WalkLeft);
    }

    else{
        setState(PlayerState::Idle);
    }
    setSsPosition(playerX_, playerY_);
}

void Player::handlePlayerAttack(){ 
  if(Mouse::isButtonPressed(Mouse::Button::Left)){
    isAttacking_ = true;
    std::cout << "isAttacking:" << isAttacking_  <<"\n "; // todo fix the spawn position of the hitbox, make it disappear
  }
}

void Player::drawPlayerAttackHitbox(RenderWindow &window){
  RectangleShape attackHitbox({attackHitboxWidth,attackHitboxHeight});
  attackHitbox.setFillColor(Color::Yellow);
  attackHitbox.setPosition({playerX_ + playerWidth,playerY_ /2 + attackHitboxHeight/2});
  window.draw(attackHitbox);

}
