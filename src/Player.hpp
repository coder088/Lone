#ifndef PLAYER_HPP
#define PLAYER_HPP 
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <vector>
#include "Projectile.hpp"

class FallenHuman;
  enum class PlayerState{
    IdleRight = 0,
    WalkRight = 1,
    WalkLeft = 2,
    JumpRight = 3,
    AttackRight = 4,
    AttackLeft = 5,
    JumpLeft = 6
  };
  class Player{
   protected:
    const float playerJumpForce_ = 10;
    const float gravity = 0.5f;
    const float playerWidth = 100;
    const float playerHeight = 100; 
    const float playerHitboxHeight = 110;
    const float playerHitboxWidht = 110;
    const float attackHitboxWidth = 10;
    const float attackHitboxHeight = 10;
    const float groundLevel = 500.0f - playerHeight;
    const int ssframeWidth = 314;
    const int ssframeHeight = 251;
    const int sstotalColumns = 4;
    const int ssTotalRows = 7;
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
    float jumpStrenght = -12.f;
    bool isGrounded_;
    bool isAttacking_;
    bool jumpWasPressed_ = false;
    char lastKeyPressed = ' ';
    std::vector<Projectile> projectiles;
    sf::Clock projectileClock;
    float playerHp = 100;
    float projectileCooldown = 0.4f; // seconds between shots
    sf::Clock immunityClock;
    float immunityCooldown = 1.0f;
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
    std::vector<Projectile> &getProjectile();

   //setters
    void setPlayerX(float x);
    void setPlayerY(float y);
    void setPlayerSpeed(float s);
    void setPlayerVerticalSpeed(float s);
    void setIsGrounded(bool g);
    void setIsAttacking(bool a);
    sf::FloatRect getPlayerHitbox();
    int drawPlayer(sf::RenderWindow &window);
    void handlePlayerMovement(sf::RenderWindow &window);
    void handlePlayerAttack();
    void updateProjectiles(sf::RenderWindow &window);
    void setState(PlayerState newState);
    void updateAnimation();
    void setSsPosition(float x,float y);
    void checkCollisionWithEnemy(FallenHuman *fallenHumanp);






  };


#endif


