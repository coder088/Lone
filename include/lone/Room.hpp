#ifndef ROOM_HPP
#define ROOM_HPP

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>
#include <tileson.hpp>
#include "lone/Enemy.hpp"

struct EnemySpawnData {
    std::string type;     
    sf::Vector2f position; 
};

class Room : public sf::Drawable {
private:
    struct LayerData {
        std::string name;
        sf::VertexArray vertices;
    };

    std::vector<LayerData> m_layers;       // All the visible layers of the tileset map
    sf::Texture m_tilesetTexture;          // the entire texture

    std::vector<sf::FloatRect> m_collisionBoxes; //Solid walls
    std::vector<EnemySpawnData> m_enemySpawns;   
    std::vector<std::unique_ptr<Enemy>> m_enemies;

    // Mandatory func in order to inherit sf::Drawable
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    int getCleanTileId(uint32_t rawGid) const;
    bool createEnemy(const EnemySpawnData& spawn);

public:
    Room() = default;
    bool loadFromFile(const std::string& jsonPath, const std::string& tilesetTexturePath = {});
    bool addEnemy(const std::string& type, sf::Vector2f position);
    const std::vector<sf::FloatRect>& getCollisionBoxes() const { return m_collisionBoxes; }
    const std::vector<EnemySpawnData>& getEnemySpawnData() const { return m_enemySpawns; }
    std::vector<std::unique_ptr<Enemy>>& getEnemies() { return m_enemies; }
    const std::vector<std::unique_ptr<Enemy>>& getEnemies() const { return m_enemies; }
};

#endif
