#include "ink.hpp"
#include "raymath.h"
#include <cstddef>
#include <random>

namespace RNG {

static std::random_device rd;
static std::mt19937 gen(rd());

auto generate_random_num(int upper, int lower) -> int {
  std::uniform_int_distribution<> dist(lower, upper);
  return dist(gen);
}

auto generate_random_size() -> int {
  static std::uniform_int_distribution<> dist(10, 75);
  return dist(gen);
}

} // namespace RNG

static const auto UNIT_CIRCLE = []() -> std::array<Vector2, VERT_COUNT> {
  std::array<Vector2, VERT_COUNT> table{};
  for (int i = 0; i < VERT_COUNT; ++i) {
    const float t = static_cast<float>(i) / static_cast<float>(VERT_COUNT - 1);
    const float theta = 2.0F * PI * (1.0F - t);
    table[static_cast<size_t>(i)] = Vector2{cosf(theta), sinf(theta)};
  }
  return table;
}();

Drop::Drop(Vector2 position, float radius) : _pos(position), _radius(radius) {

  for (int i{0}; i < VERT_COUNT; ++i) {
    _vertices[static_cast<size_t>(i)] =
        Vector2Add(Vector2Scale(UNIT_CIRCLE[static_cast<size_t>(i)], radius), position);
  }

  _color = COLORS[static_cast<size_t>(RNG::generate_random_num(static_cast<int>(COLORS.size()) - 1, 1))];
}

Drop::Drop(Vector2 position, float radius, Color color)
    : _pos(position), _radius(radius), _color(color) {

  for (int i{0}; i < VERT_COUNT; ++i) {
    _vertices[static_cast<size_t>(i)] =
        Vector2Add(Vector2Scale(UNIT_CIRCLE[static_cast<size_t>(i)], radius), position);
  }
}

auto Drop::marble(const Drop &other) -> void {
  // /***********************************************************************************/
  //    This is where Jaffers Formula comes in hand. Before adding the next drop to the
  //      array we need to first recaluclate all other dots based on the newest dots
  //                                    position.
  //
  //                                Jaffers Formula
  //                     C + (P-C) * sqrt(1 + (r^2 / ||P-C||^2)
  //
  //                              P = Vertext on the blob
  //                              C = New blobs center point
  //                              r = Radius of new Blob
  //                        (P - C) = Vector from C to P
  //                            r^2 = Scalar of the Vector
  //                      ||P - C|| = Magnitude of Vector
  // /***********************************************************************************/

  const Vector2 center = other.pos();
  const float r_squared = other.radius() * other.radius();

  for (Vector2 &vert : _vertices) {
    const Vector2 vert_sub_center = Vector2Subtract(vert, center);
    const float mag_squared = Vector2LengthSqr(vert_sub_center);
    const float root = sqrtf(1.0F + (r_squared / mag_squared));

    vert = Vector2Add(Vector2Scale(vert_sub_center, root), center);
  }

  _pos = _vertices[0];
}

auto Drop::general_tine(const Vector2 &m, const Vector2 &b, float z, float c) -> void {
  // /***********************************************************************************/
  //  The regular Tine Line alogrithem only works in straight lines up and down. Although
  //  you could swap the opperation from the y coordinate to the x coordinate to flip it
  //   horozontally, using a generalization for any direction would be the best option.
  //
  //                                  d = |(P-B) . N|
  //                              F_L(P)=P + z * u^d * M
  //
  //         P = Vertex
  //         M = Vector eminating from B
  //         d = the magnitude of the vector perpindicular to M, spaning from M to P
  //         n = Vertex rotated 90 degrees
  //
  //
  //
  //
  // /***********************************************************************************/

  const float u = exp2f(-1.0F / c);
  const Vector2 n = Vector2Rotate(m, PI / 2.0F);

  for (Vector2 &vert : _vertices) {
    const Vector2 pb = Vector2Subtract(vert, b);
    const float d = fabsf(Vector2DotProduct(pb, n));
    const float mag = z * powf(u, d);

    vert = Vector2Add(vert, Vector2Scale(m, mag));
  }
}

auto Drop::show() -> void {
  DrawTriangleFan(_vertices.data(), VERT_COUNT, _color);
}

auto Drop::pos() const -> Vector2 {
  return _pos;
}
auto Drop::radius() const -> float {
  return _radius;
}

auto Drop::is_in_window(const Vector2 &window) const -> bool {

  return _pos.x >= 0 - (_radius * 2) && _pos.x <= window.x + (_radius * 2) &&
         _pos.y >= 0 - (_radius * 2) && _pos.y <= window.y + (_radius * 2);
}
