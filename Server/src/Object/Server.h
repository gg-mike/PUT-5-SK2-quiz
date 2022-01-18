#pragma once

#include "Client.h"

class Server {
public:
    static constexpr const char * DEFAULT_CONFIG{"res/defaultConfig.json"};

    using RequestCode = unsigned short;

    enum Request: RequestCode {
        SHUTDOWN = 1 << 1
    };


    struct Config {
        int port;
        ssize_t bufSize;
        int listenConnNum;
        int maxTimeBetweenHb_sec;
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
    std::thread handleThread;

public:
    static std::shared_ptr<Server> GetInstance();

    void Run(const std::string& configFile=DEFAULT_CONFIG);
    void Shutdown();

    void ClientRequest(int clientFd, RequestCode request);

    [[nodiscard]] Config GetConfig() const { return config; }

private:
    void Init(const std::string &configFile=DEFAULT_CONFIG);
    void AcceptClients();
    void HandleClients();
    void ReadConfig(const std::string& configFile);

public:
    friend void SignalCallbackHandler(int signal);
};