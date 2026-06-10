/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Getopt
*/

#pragma once

#include <string>
#include <vector>

namespace zappy::parser::encapsulation {
class GetOptWrapper {
  public:
    GetOptWrapper(int argc, char** argv, std::string optString);
    ~GetOptWrapper() = default;

    GetOptWrapper(const GetOptWrapper&) = delete;
    GetOptWrapper(GetOptWrapper&&) = delete;
    GetOptWrapper& operator=(const GetOptWrapper&) = delete;
    GetOptWrapper& operator=(GetOptWrapper&&) = delete;

    /**
     * @brief Get the next option from the command line arguments
     * @return The next option character, or -1 if there are no more options to process
     */
    [[nodiscard]] int getNextOption() const;

    /**
     * @brief Get the argument associated with the current option
     * @return The argument string, or an empty string if there is no argument
     */
    static std::string getOptionArg();

    /**
     * @brief Get all arguments associated with the current option, including the option argument and any subsequent
     * non-option arguments
     * @return A vector of strings containing the option argument and any subsequent non-option arguments, or an empty
     * vector if there are no arguments
     */
    [[nodiscard]] std::vector<std::string> getMultiArgs() const;

    /**
     * @brief Get the character of the unknown option that caused an error
     * @return The character of the unknown option, or 0 if there is no unknown option
     */
    static char getUnknownOption();

  private:
    int _argc;
    char** _argv;
    std::vector<std::string> _stringArgv;
    std::string _optString;
};
}  // namespace zappy::shared::encapsulation
