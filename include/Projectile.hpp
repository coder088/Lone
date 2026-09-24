#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>

enum class ProjectileState{
    ani1R = 0,
    ani2R = 1,
    ani3R = 2,
    ani4R = 3,
    ani1L = 4,
    ani2L = 5,
    ani3L = 6,
    ani4L = 7
};
class Projectile{
   protected:
    static constexpr float projectileTextWidth = 160.f;
    static constexpr float projectileTextHeight = 64.f;
    static constexpr float projectileHitboxWidth = 140.f;
    static constexpr float projectileHitboxHeight = 10.f;
    static constexpr int ssframeWidth = 128;
    static constexpr int ssframeHeight = 128;
    static constexpr int sstotalColumns = 4;
    static constexpr int ssTotalRows = 2;
    ProjectileState currentState;
    static sf::Texture sharedTexture;
    static bool sharedTextureLoaded;
    sf::Sprite sprite;
    sf::Clock ssanimationmClock;
    int currentssColumn;
    float frameDuration;
    float projectileSpeed;
    float projectileDamage = 10.f;
    bool frameChecked = false;
  public:
  Projectile();
  float getProjectileWidth();
  float getProjectileHeight();
  float getProjectileDamage();
  void setProjectileDamage(float d);
  void drawProjectile(sf::RenderWindow &window);
  void setState(ProjectileState newState);
  void updateAnimation();
  void setSsPosition(float x ,float y);
  // move the projectile horizontally by dx
  void updatePosition(float dx);
  // get X position
  float getX() const;
  // get global bounds for collision
  sf::FloatRect getGlobalBounds() const;
  // test if projectile is outside the given window bounds
  bool isOffscreen(float windowWidth, float windowHeight) const;
  ProjectileState getState() const;
  sf::FloatRect getProjectileHitbox();



};

#endif