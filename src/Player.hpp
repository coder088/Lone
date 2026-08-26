#ifndef PLAYER_HPP
#define PLAYER_HPP 
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
  enum class PlayerState{
    Idle = 0,
    WalkRight = 1,
    WalkLeft = 2,
    Jump = 3,
    Attack = 4
  };
  class Player{
   protected:
    const float playerJumpForce_ = 10;
    const float gravity = 0.2f;
    const float playerWidth = 100;
    const float playerHeight = 100;
    const float attackHitboxWidth = 10;
    const float attackHitboxHeight = 10;
    const int ssframeWidth = 314;
    const int ssframeHeight = 251;
    const int sstotalColumns = 4;
    const int ssTotalRows = 5;\
    sf::Texture texture;
    sf::Sprite sprite;
    int currentssColumn;
    PlayerState currentState;
    sf::Clock ssanimationmClock;
    float frameDuration;
    float playerX_;
    float playerY_;
    float playerSpeed_;                           
    float playerVerticalSpeed_;
    bool isGrounded_;
    bool isAttacking_;
    char lastKeyPressed = ' ';
   public:
    Player();

    //getters
    float getPlayerX();
    float getPlayerY();
    float getPlayerSpeed();
    float getPlayerVerticalSpeed();
    float getGravity();
    float getPlayerJumpForce();
    bool getIsGorounded();
    bool getIsAttacking();

   //setters
    void setPlayerX(float x);
    void setPlayerY(float y);
    void setPlayerSpeed(float s);
    void setPlayerVerticalSpeed(float s);
    void setIsGrounded(bool g);
    void setIsAttacking(bool a);

    int drawPlayer(sf::RenderWindow &window);
    void handlePlayerMovement();
    void handlePlayerAttack();
    void drawPlayerAttackHitbox(sf::RenderWindow &window);
    void setState(PlayerState newState);
    void updateAnimation();
    void setSsPosition(float x,float y);
  






  };


#endif


