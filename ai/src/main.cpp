/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** main
*/

#include <sys/wait.h>
#include <unistd.h>

#include <cstdlib>
#include <filesystem>
#include <string>
#include <system_error>
#include <vector>

int main(const int argc, char* argv[]) {
    std::error_code ec;
    std::filesystem::path const exePath = std::filesystem::read_symlink("/proc/self/exe", ec);
    if (ec) {
        return 1;
    }
    std::filesystem::path const scriptPath = exePath.parent_path() / "ai" / "src" / "zappy_ai.py";
    std::vector<std::string> args = {"python3", scriptPath.string()};
    for (int i = 1; i < argc; ++i) {
        args.emplace_back(argv[i]);  // NOLINT
    }
    std::vector<char*> c_args;
    c_args.reserve(args.size());
    for (auto& arg : args) {
        c_args.push_back(arg.data());
    }
    c_args.push_back(nullptr);
    if (pid_t const pid = fork(); pid == 0) {
        execvp(c_args.at(0), c_args.data());
        exit(1);
    } else if (pid > 0) {
        int status = 0;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
    } else {
        return 1;
    }
}
