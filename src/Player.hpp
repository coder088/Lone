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
    JumpLeft = 6,
    DeathR = 7,
    DeathL = 8,
    IdleLeft = 9
  };
  class Player{
   protected:
    static constexpr float playerJumpForce_ = 10;
    static constexpr  float gravity = 0.5f;
    static constexpr  float playerWidth = 100;
    static constexpr  float playerHeight = 100; 
    static constexpr  float playerHitboxHeight = 110;
    static constexpr  float playerHitboxWidht = 110;
    static constexpr float maxPlayerHp = 100.0f;
    static constexpr  float attackHitboxHeight = 10;
    static constexpr  float groundLevel = 500.0f - playerHeight;
    static constexpr  int ssframeWidth = 314;
    static constexpr  int ssframeHeight = 251;
    static constexpr  int sstotalColumns = 4;
    static constexpr  int ssTotalRows = 9;
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
    float playerHp = maxPlayerHp;
    float projectileCooldown = 0.4f; // seconds between shots
    sf::Clock immunityClock;
    float immunityCooldown = 1.0f;
    bool isDead = false;
   public:
    Player();

    //getters
    float getPlayerX();
    float getPlayerY();
    float getPlayerSpeed();
    float getPlayerVerticalSpeed();
    float getGravity();
    float getPlayerJumpForce();
    float getPlayerHp();
    bool getIsGorounded();
    bool getIsAttacking();
    bool getIsDead();
    void respawn();
    std::vector<Projectile> &getProjectile();

   //setters
    void setPlayerX(float x);
    void setPlayerY(float y);
    void setPlayerSpeed(float s);
    void setPlayerVerticalSpeed(float s);
    void setIsGrounded(bool g);
    void setIsAttacking(bool a);
    sf::FloatRect getPlayerHitbox();
    int drawPlayer(sf::RenderWindow &window, float deltaTime);
    void handlePlayerMovement(float deltaTime);
    void handlePlayerAttack();
    void updateProjectiles(sf::RenderWindow &window, float deltaTime);
    void setState(PlayerState newState);
    void updateAnimation();
    void setSsPosition(float x,float y);
    void checkCollisionWithEnemy(FallenHuman *fallenHumanp);






  };


#endif


