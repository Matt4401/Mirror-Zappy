/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Image
*/

#pragma once
#include <raylib.h>

#include <span>
#include <string>
#include <string_view>
#include <utility>

#include "rtextures/Texture2D.hpp"

namespace zappy::gui::raylib::rtextures {
class Image {
  public:
    explicit Image(const std::string& path) : _image{LoadImage(path.c_str())} {}
    explicit Image(::Image image) : _image{image} {}

    ~Image() { reset(); }

    Image(const Image& other) = delete;
    Image& operator=(const Image& other) = delete;

    Image(Image&& other) noexcept : _image{std::exchange(other._image, {})} {}

    Image& operator=(Image&& other) noexcept {
        if (this != &other) {
            reset();
            _image = std::exchange(other._image, {});
        }
        return *this;
    }

    [[nodiscard]] static Image fromMemory(std::string_view fileType, std::span<const unsigned char> data) {
        const std::string extension{fileType};
        return Image{LoadImageFromMemory(extension.c_str(), data.data(), static_cast<int>(data.size()))};
    }

    [[nodiscard]] static Image fromScreen() { return Image{LoadImageFromScreen()}; }
    [[nodiscard]] static Image color(int width, int height, Color color) {
        return Image{GenImageColor(width, height, color)};
    }
    [[nodiscard]] static Image checked(int width, int height, int checksX, int checksY, Color first, Color second) {
        return Image{GenImageChecked(width, height, checksX, checksY, first, second)};
    }
    [[nodiscard]] static Image perlinNoise(int width, int height, int offsetX, int offsetY, float scale) {
        return Image{GenImagePerlinNoise(width, height, offsetX, offsetY, scale)};
    }

    [[nodiscard]] bool valid() const { return IsImageValid(_image); }
    [[nodiscard]] int width() const { return _image.width; }
    [[nodiscard]] int height() const { return _image.height; }
    [[nodiscard]] int mipmaps() const { return _image.mipmaps; }
    [[nodiscard]] int format() const { return _image.format; }
    [[nodiscard]] ::Image image() const { return _image; }
    [[nodiscard]] ::Image& image() { return _image; }

    [[nodiscard]] Image copy() const { return Image{ImageCopy(_image)}; }
    [[nodiscard]] Texture2D toTexture() const { return Texture2D{LoadTextureFromImage(_image)}; }
    [[nodiscard]] bool exportTo(const std::string& path) const { return ExportImage(_image, path.c_str()); }

    void resize(int width, int height) { ImageResize(&_image, width, height); }
    void resizeNearestNeighbor(int width, int height) { ImageResizeNN(&_image, width, height); }
    void crop(Rectangle rectangle) { ImageCrop(&_image, rectangle); }
    void flipVertical() { ImageFlipVertical(&_image); }
    void flipHorizontal() { ImageFlipHorizontal(&_image); }
    void rotate(int degrees) { ImageRotate(&_image, degrees); }
    void tint(Color color) { ImageColorTint(&_image, color); }
    void invert() { ImageColorInvert(&_image); }
    void grayscale() { ImageColorGrayscale(&_image); }
    void contrast(float value) { ImageColorContrast(&_image, value); }
    void brightness(int value) { ImageColorBrightness(&_image, value); }
    void replaceColor(Color color, Color replacement) { ImageColorReplace(&_image, color, replacement); }

  protected:
  private:
    void reset() {
        if (valid()) {
            UnloadImage(_image);
        }
        _image = {};
    }

    ::Image _image{};
};
}  // namespace zappy::gui::raylib::rtextures
