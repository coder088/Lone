#include "lone/FallenHuman.hpp"
#include "lone/Room.hpp"
#include <filesystem>
#include <iostream>
#include <tuple> 

bool Room::createEnemy(const EnemySpawnData& spawn) {
    if (spawn.type == "FallenHuman" || spawn.type == "fallenhuman" || spawn.type == "Fallen Human"
        || spawn.type == "enemy1" || spawn.type == "enemy2") {
        m_enemies.push_back(std::make_unique<FallenHuman>(spawn.position));
        return true;
    }

    std::cerr << "unknown enemy type: " << spawn.type << "\n";
    return false;
}

bool Room::addEnemy(const std::string& type, sf::Vector2f position) {
    return createEnemy({type, position});
}

void Room::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.texture = &m_tilesetTexture;
    for (const auto& layer : m_layers) {
        target.draw(layer.vertices, states);
    }
}

int Room::getCleanTileId(uint32_t rawGid) const {
    return rawGid & ~(0x80000000 | 0x40000000 | 0x20000000);
}

bool Room::loadFromFile(const std::string& jsonPath, const std::string& tilesetTexturePath) {
    m_layers.clear();
    m_collisionBoxes.clear();
    m_enemySpawns.clear();
    m_enemies.clear();

    tson::Tileson t;
    std::unique_ptr<tson::Map> map = t.parse(fs::path(jsonPath));

    if (!map || map->getStatus() != tson::ParseStatus::OK) {
        std::cerr << "Errore Tileson: Parsing fallito per il file " << jsonPath << "\n";
        return false;
    }

    std::filesystem::path tilesetPath = tilesetTexturePath;
    if (tilesetPath.empty() && !map->getTilesets().empty()) {
        const std::filesystem::path mapDirectory = std::filesystem::path(jsonPath).parent_path();
        tilesetPath = mapDirectory / map->getTilesets().front().getImage();
    }

    if (tilesetPath.empty() || !m_tilesetTexture.loadFromFile(tilesetPath.string())) {
        std::cerr << "Errore: Impossibile caricare il tileset della stanza\n";
        return false;
    }

    tson::Vector2i mapSize = map->getSize();     
    tson::Vector2i tileSize = map->getTileSize();   
    int tilesetColumns = m_tilesetTexture.getSize().x / tileSize.x;

    for (auto& layer : map->getLayers()) {
        
        // Graphic elements
        if (layer.getType() == tson::LayerType::TileLayer) {
            LayerData currentLayer;
            currentLayer.name = layer.getName();
            
            currentLayer.vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
            currentLayer.vertices.resize(mapSize.x * mapSize.y * 6); 

            auto tileData = layer.getTileData();

            for (int y = 0; y < mapSize.y; ++y) {
                for (int x = 0; x < mapSize.x; ++x) {
                
                    std::tuple<int, int> tupleKey = std::make_tuple(x, y);
                    
                    if (tileData.count(tupleKey) == 0) continue; 

                    tson::Tile* tile = tileData[tupleKey];
                    if (!tile) continue;

                    int cleanId = getCleanTileId(tile->getGid()) - 1; 
                    if (cleanId < 0) continue;

                    int tu = cleanId % tilesetColumns;
                    int tv = cleanId / tilesetColumns;

                    float pLeft   = static_cast<float>(x * tileSize.x);
                    float pRight  = static_cast<float>((x + 1) * tileSize.x);
                    float pTop    = static_cast<float>(y * tileSize.y);
                    float pBottom = static_cast<float>((y + 1) * tileSize.y);

                    float tLeft   = static_cast<float>(tu * tileSize.x);
                    float tRight  = static_cast<float>((tu + 1) * tileSize.x);
                    float tTop    = static_cast<float>(tv * tileSize.y);
                    float tBottom = static_cast<float>((tv + 1) * tileSize.y);

                    sf::Vector2f texTopLeft(tLeft, tTop);
                    sf::Vector2f texTopRight(tRight, tTop);
                    sf::Vector2f texBottomRight(tRight, tBottom);
                    sf::Vector2f texBottomLeft(tLeft, tBottom);

                    if (tile->hasFlipFlags(tson::TileFlipFlags::Diagonally)) {
                        std::swap(texTopRight, texBottomLeft);
                    }
                    if (tile->hasFlipFlags(tson::TileFlipFlags::Horizontally)) {
                        std::swap(texTopLeft, texTopRight);
                        std::swap(texBottomLeft, texBottomRight);
                    }
                    if (tile->hasFlipFlags(tson::TileFlipFlags::Vertically)) {
                        std::swap(texTopLeft, texBottomLeft);
                        std::swap(texTopRight, texBottomRight);
                    }

                    int vertexIndex = (x + y * mapSize.x) * 6;
                    sf::Vertex* triangleVertices = &currentLayer.vertices[vertexIndex];

                    //First triangle
                    triangleVertices[0].position = sf::Vector2f(pLeft, pTop);
                    triangleVertices[0].texCoords = texTopLeft;

                    triangleVertices[1].position = sf::Vector2f(pRight, pTop);
                    triangleVertices[1].texCoords = texTopRight;

                    triangleVertices[2].position = sf::Vector2f(pRight, pBottom);
                    triangleVertices[2].texCoords = texBottomRight;

                    //second triangle
                    triangleVertices[3].position = sf::Vector2f(pLeft, pTop);
                    triangleVertices[3].texCoords = texTopLeft;

                    triangleVertices[4].position = sf::Vector2f(pRight, pBottom);
                    triangleVertices[4].texCoords = texBottomRight;

                    triangleVertices[5].position = sf::Vector2f(pLeft, pBottom);
                    triangleVertices[5].texCoords = texBottomLeft;
                }
            }
            m_layers.push_back(currentLayer);
        }
        
        //obj leayer
        else if (layer.getType() == tson::LayerType::ObjectGroup) {
            if (layer.getName() == "Collisions") {
                for (auto& obj : layer.getObjects()) {
                    sf::FloatRect rect(
                        sf::Vector2f(static_cast<float>(obj.getPosition().x), static_cast<float>(obj.getPosition().y)),
                        sf::Vector2f(static_cast<float>(obj.getSize().x), static_cast<float>(obj.getSize().y))
                    );
                    m_collisionBoxes.push_back(rect);
                }
            } 
            else if (layer.getName() == "entities") {
                for (auto& obj : layer.getObjects()) {
                    EnemySpawnData spawn;
                    spawn.type = obj.getName(); 
                    spawn.position = sf::Vector2f(static_cast<float>(obj.getPosition().x), static_cast<float>(obj.getPosition().y));
                    m_enemySpawns.push_back(spawn);
                    createEnemy(spawn);
                }
            }
        }
    }
    return true;
}
