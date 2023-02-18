//
// Created by Admin on 13/02/2023.
//

#ifndef POPOSIBENGINE_PLAYER_HPP
#define POPOSIBENGINE_PLAYER_HPP

#include "Creature.hpp"
#include <string>
#include <array>
#include <vector>

class Player {
public:
    explicit Player(const std::string& textureName);

    constexpr static int MAX_CREATURES = 8;

    [[nodiscard]] const std::array<Creature, MAX_CREATURES>& GetCreatures() const { return m_creatures; }
    [[nodiscard]] std::array<Creature, MAX_CREATURES>& GetCreatures() { return m_creatures; }

    [[nodiscard]] bool IsDead() const {
        for (const auto& creature : m_creatures) {
            if (!creature.IsDead()) {
                return false;
            }
        }
        return true;
    }

    Creature* GetPlayableCreature() {
        std::vector<int> playableCreatures;

        for(int i = 0; i < m_creatures.size(); i++) {
            if(!m_creatures[i].IsDead()) {
                playableCreatures.push_back(i);
            }
        }

        for(int aJouer : m_lastCreaturesPlay) {
            //for(int peutJouer : playableCreatures) {
            for(int peutJouer = 0; peutJouer < playableCreatures.size(); peutJouer++) {
                if(aJouer == playableCreatures[peutJouer]) {
                    playableCreatures.erase(playableCreatures.begin() + peutJouer);
                }
            }
        }

        if(playableCreatures.empty()) {
            m_lastCreaturesPlay.clear();
            m_lastCreaturesPlay.push_back(0);
            return &m_creatures[0];
        }

        int idCreature = rand() % playableCreatures.size();
        m_lastCreaturesPlay.push_back(playableCreatures[idCreature]);
        return &m_creatures[playableCreatures[idCreature]];
    }

private:
    std::vector<int> m_lastCreaturesPlay;
    std::array<Creature, MAX_CREATURES> m_creatures;
};


#endif //POPOSIBENGINE_PLAYER_HPP
