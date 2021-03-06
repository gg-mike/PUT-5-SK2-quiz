#pragma once

#include "Client.h"
#include "../Enumerators/ServerAction.h"

class Server {
public:
    static constexpr const char * DEFAULT_CONFIG{"res/defaultConfig.json"};

    struct Config {
        int port;
        ssize_t bufSize;
        int listenConnNum;
        std::map<std::string, int> maxTimeBetweenHb_sec;
        int timeBetweenAnswersNumberSend_sec;
        std::string messageBegin;
        std::string messageEnd;
    };

private:
    std::atomic<bool> running{false};
    int fd{-1};
    Config config{};

    std::condition_variable cv;
    std::recursive_mutex clientsMtx;
    std::mutex handleMtx;
    std::map<int, std::shared_ptr<Client>> clients{};
    std::set<int> clientsToShutdown{};

    std::thread acceptThread;
    std::thread shutdownClientsThread;

public:
    static std::shared_ptr<Server> GetInstance();

    void Run(const std::string& configFile=DEFAULT_CONFIG);
    void Shutdown();

    void Action(int clientFd, Enumerators::ServerActionCode actionCode);

    void Send(int receiverFd, const nlohmann::json& message, int offset = 2) const;

    [[nodiscard]] Config GetConfig() const { return config; }

private:
    void Init(const std::string &configFile=DEFAULT_CONFIG);
    void AcceptHandler();
    void ShutdownClientsHandler();
    void ReadConfig(const std::string& configFile);

public:
    friend void SignalCallbackHandler(int signal);
};
