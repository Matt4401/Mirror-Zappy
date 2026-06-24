/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** core tests
*/

#include "Core.hpp"

#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <gtest/gtest.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <chrono>
#include <span>
#include <string>
#include <thread>
#include <vector>

#include "Parser.hpp"

namespace {

std::vector<char*> buildArgv(std::vector<std::string>& args) {
    std::vector<char*> argv;

    argv.reserve(args.size());
    for (std::string& arg : args) {
        argv.push_back(arg.data());
    }
    return argv;
}

}  // namespace

namespace zappy::server::tests {

TEST(CoreTest, ConstructionSucceeds) {
    std::vector<std::string> args{"./zappy_server", "-p", "4243", "-x", "10", "-y", "10", "-n", "TeamA", "-c", "1"};
    auto argv = buildArgv(args);
    const std::span<char*> argvSpan{argv.data(), argv.size()};

    EXPECT_NO_THROW({ const Core core{argvSpan}; });
}

TEST(CoreTest, RunWithValidArgumentsReturnsSuccess) {
    std::vector<std::string> args{"./zappy_server", "-p", "4245", "-x", "10", "-y", "10", "-n", "TeamA", "-c", "1"};
    auto argv = buildArgv(args);
    const std::span<char*> argvSpan{argv.data(), argv.size()};
    Core core{argvSpan};

    std::thread serverThread([&core]() {
        const int result = core.run();
        EXPECT_EQ(result, zappy::parser::kExitSuccess);
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    core.stop();

    const int sock = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        FAIL();
    }
    if (sock >= 0) {
        struct sockaddr_in serv_addr{};
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = ::htons(4245);
        ::inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

        // NOLINTNEXTLINE
        ::connect(sock, reinterpret_cast<struct sockaddr*>(&serv_addr), sizeof(serv_addr));
        ::close(sock);
    }

    if (serverThread.joinable()) {
        serverThread.join();
    }
}

}  // namespace zappy::server::tests
