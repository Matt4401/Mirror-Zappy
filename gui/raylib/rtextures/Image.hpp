/*
** EPITECH PROJECT, 2026
** Mirror-Zappy
** File description:
** Image
*/

#pragma once
#include <raylib.h>

#include <string>
#include <utility>

#include "rmath/Vector3.hpp"
#include "rmodels/Mesh.hpp"
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

    rmodels::Mesh genMeshHeightmap(rmath::Vector3 size) {
        return rmodels::Mesh{GenMeshHeightmap(_image, size.vector())};
    }
    rmodels::Mesh genMeshCubicmap(rmath::Vector3 cubeSize) {
        return rmodels::Mesh{GenMeshCubicmap(_image, cubeSize.vector())};
    }

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
