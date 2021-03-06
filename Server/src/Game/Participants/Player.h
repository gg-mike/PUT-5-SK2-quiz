#pragma once

#include "IGameParticipant.h"

#include "../../Enumerators/GameState.h"

class Player : public IGameParticipant {
private:
    std::atomic<int> fd;
    std::atomic<bool>& stateChange;
    std::atomic<bool> answered{false};
    std::atomic<bool> wasCorrectAnswer{false};
    std::atomic<int> score{0};
    std::atomic<int> pos{0};
    std::atomic<int> gameCode;
    std::string nick;

public:
    Player(int fd, std::atomic<bool>& stateChange, int gameCode, std::string nick);
    nlohmann::json Process(Enumerators::Request type, const nlohmann::json& request) override;
    [[nodiscard]] int GetType() const override { return 2; }

    [[nodiscard]] int GetFd() const { return fd; }

    void SetStateChange(bool _stateChange) { stateChange = _stateChange; }

    void SetAnswered(bool _answered) { answered = _answered; }

    void SetWasCorrectAnswer(bool _wasCorrectAnswer) { wasCorrectAnswer = _wasCorrectAnswer; }
    [[nodiscard]] bool GetWasCorrectAnswer() const { return wasCorrectAnswer; }

    void IncreaseScore(int value) { score+=value; }
    [[nodiscard]] int GetScore() const { return score; }

    void SetPos(int _pos) { pos = _pos; }
    [[nodiscard]] int GetPos() const { return pos; }

    [[nodiscard]] int GetGameCode() const { return gameCode; }

    [[nodiscard]] std::string GetNick() const { return nick; }

    void RequestHandler(Enumerators::GameState currGameState);
};
