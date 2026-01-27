#ifndef H_INK
#define H_INK

#include "raylib.h"
#include <array>

constexpr int VERT_COUNT = 360;
constexpr std::array<Color, 9> COLORS{
    Color{18, 8, 38, 255},    Color{43, 15, 84, 255},   Color{171, 31, 38, 255},
    Color{255, 79, 105, 255}, Color{255, 129, 66, 255}, Color{255, 218, 69, 255},
    Color{51, 104, 220, 255}, Color{73, 231, 236, 255}, Color{255, 247, 248, 255},
};

class Drop {

public:
  Drop(Vector2 position, float radius);
  Drop(Vector2 position, float radius, Color color);

  auto show() -> void;
  auto marble(const Drop &other) -> void;
  auto general_tine(const Vector2 &m, const Vector2 &b, float z, float c) -> void;

  [[nodiscard]] auto is_in_window(const Vector2 &) const -> bool;
  [[nodiscard]] auto pos() const -> Vector2;
  [[nodiscard]] auto radius() const -> float;

private:
  Vector2 _pos;
  float _radius;
  Color _color;
  std::array<Vector2, VERT_COUNT> _vertices;
};

namespace RNG {
auto generate_random_num(int upper, int lower) -> int;
auto generate_random_size() -> int;
} // namespace RNG

#endif