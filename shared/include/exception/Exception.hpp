/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** exceptions
*/

#pragma once

#include <exception>
#include <string>

namespace zappy::shared::exception {

class Exception : public std::exception {
  public:
    explicit Exception(std::string message);

    [[nodiscard]] const char* what() const noexcept override;

  private:
    std::string _message;
};

}  // namespace zappy::shared::exception
