#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>
using namespace sf;
using namespace std;
class Player{
    protected:
    const float GRAVITY = 0.5f;
     float playerX;
     float playerY;
     float playerSpeed;
     float playerWidth;
     float playerHeight;
     float playerJumpForce;
     float velocityY;
     bool isGrounded;

    public:

    Player(){
        playerX = 400.f;
        playerY = 300.f;
        playerSpeed = 2.f;
        playerWidth = 100.f;
        playerHeight = 100.f;
        playerJumpForce = 3.f;
        velocityY = 0.f;
        isGrounded = true;
    }

    //getters 
    float getVelocityY(){return velocityY;}
    float getPlayerX(){return playerX;}
    float getPlayerY(){return playerY;}
    float getPlayerWidth(){return playerWidth;}
    float getPlayerheight(){return playerHeight;}
    float getPlayerSpeed(){return playerSpeed;}
    float getPlayerJumpForce(){return playerJumpForce;}
    bool getIsGrounded(){return isGrounded;}

    //setters
    void setVelocityY(float v){velocityY =v;}
    void setPlayerX(float x){playerX = x;}
    void setPlayerY(float y){playerY = y;}
    void setPlayerSpeed(float s){playerSpeed = s;}
    void setPlayerWidth(float w){playerWidth = w;}
    void setPlayerHeight(float h){playerHeight = h;}
    void setJumpForce(float j){playerJumpForce = j;}
    void setIsGrounded(bool i){isGrounded = i;}

    void drawPlayer(RenderWindow& window){
        RectangleShape rect({playerWidth,playerHeight});
        rect.setPosition({playerX,playerY});
        rect.setFillColor(Color::Blue);
        rect.setOutlineColor(Color::Yellow);
         window.draw(rect);
    }

    void handlePlayerMovement(){
        if(Keyboard::isKeyPressed(Keyboard::Key::D) && playerX != 800 - playerWidth){
            playerX += playerSpeed;
        }
        if(Keyboard::isKeyPressed(Keyboard::Key::A) && playerX != 0){
            playerX -= playerSpeed;
        }
        if(Keyboard::isKeyPressed(Keyboard::Key::Space)&& isGrounded){
            velocityY = -6.0f;
            isGrounded = false;
      }
      if(!isGrounded){
        velocityY += GRAVITY;
    }
    else{
        velocityY = 0.0f;
    }
    playerY += velocityY;
    float groundLevel =  400.0f - playerHeight;
    if(playerY >= groundLevel){
        playerY = groundLevel;
        isGrounded = true;
    }
}


};
