#include "ink.hpp"
#include "raylib.h"
#include "raymath.h"
#include "rlImGui.h"
#include "rlgl.h"
#include <array>
#include <imgui.h>
#include <memory>
#include <vector>

namespace TineValues {
float z{60.0F};
constexpr float c{16.0F};
} // namespace TineValues

namespace DropSizes {
constexpr int lowest{10};
constexpr int highest{75};
} // namespace DropSizes

struct TinePoints {
  Vector2 start{Vector2Zero()};
  Vector2 end{Vector2Zero()};
};

struct TimeData {
  float since_last_toggle_drop{0.0F};
  float since_last_manual_drop{0.0F};
  float since_mouse_down{0.0F};
  static constexpr float spawn_interval{0.06F};
  static constexpr float hold_minimum{0.2F};
};

auto add_new_drop(std::vector<std::unique_ptr<Drop>> &drops, const std::vector<Color> &user_colors)
    -> void;
auto add_new_drop(std::vector<std::unique_ptr<Drop>> &drops, Vector2 mouse_pos,
                  const std::vector<Color> &user_colors) -> void;
auto tine_line(Vector2 vec, Vector2 pos, float z, float c,
               std::vector<std::unique_ptr<Drop>> &drops) -> void;
auto handle_input(std::vector<std::unique_ptr<Drop>> &drops, TinePoints &tine_points,
                  TimeData &time_data, bool &toggle_drop_spawn,
                  const std::vector<Color> &user_colors) -> void;
auto get_drop_color(const std::vector<Color> &user_colors) -> Color;
auto render_imgui_controls(std::array<float, 4> &clear, std::array<float, 4> &new_color,
                           std::vector<Color> &user_defined_colors,
                           const std::vector<std::unique_ptr<Drop>> &drops) -> void;

auto main() -> int {
  SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
  InitWindow(1280, 720, "Paper Marbling");
  SetTargetFPS(60);

  rlImGuiSetup(true);

  std::array<float, 4> clear{0.10F, 0.12F, 0.15F, 1.0F};

  std::vector<std::unique_ptr<Drop>> drops;
  drops.reserve(250);

  std::vector<Color> user_defined_colors;
  std::array<float, 4> new_color{1.0F, 0.5F, 0.2F, 1.0F};

  bool toggle_drop_spawn{false};
  TinePoints tine_points;
  TimeData time_data;

  while (!WindowShouldClose()) {
    const int screen_width = GetScreenWidth();
    const int screen_height = GetScreenHeight();

    float delta_time = GetFrameTime();
    time_data.since_last_toggle_drop += delta_time;
    time_data.since_last_manual_drop += delta_time;
    time_data.since_mouse_down += delta_time;

    handle_input(drops, tine_points, time_data, toggle_drop_spawn, user_defined_colors);

    std::erase_if(drops, [&](const std::unique_ptr<Drop> &d) -> bool {
      return !d->is_in_window(
          {static_cast<float>(screen_width), static_cast<float>(screen_height)});
    });

    BeginDrawing();
    ClearBackground(Color{(unsigned char)(clear[0] * 255.0F), (unsigned char)(clear[1] * 255.0F),
                          (unsigned char)(clear[2] * 255.0F), (unsigned char)(clear[3] * 255.0F)});

    for (auto &drop : drops) {
      drop->show();
    }

    rlImGuiBegin();
    render_imgui_controls(clear, new_color, user_defined_colors, drops);
    rlImGuiEnd();

    EndDrawing();
  }

  rlImGuiShutdown();
  CloseWindow();
  return 0;
}

auto render_imgui_controls(std::array<float, 4> &clear, std::array<float, 4> &new_color,
                           std::vector<Color> &user_defined_colors,
                           const std::vector<std::unique_ptr<Drop>> &drops) -> void {
  if (ImGui::Begin("Controls")) {
    ImGui::ColorEdit3("Clear Color", clear.data());

    ImGui::Text("FPS: %d", GetFPS());
    ImGui::Text("Drops: %zu", drops.size());

    ImGui::Separator();
    ImGui::Text("Custom Colors");

    ImGui::ColorEdit4("New Color", new_color.data(),
                      ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_PickerHueWheel);

    if (ImGui::Button("Add Color")) {
      Color c{static_cast<unsigned char>(new_color[0] * 255.0F),
              static_cast<unsigned char>(new_color[1] * 255.0F),
              static_cast<unsigned char>(new_color[2] * 255.0F),
              static_cast<unsigned char>(new_color[3] * 255.0F)};
      user_defined_colors.push_back(c);
    }

    ImGui::SameLine();
    if (ImGui::Button("Clear All Colors")) {
      user_defined_colors.clear();
    }

    if (!user_defined_colors.empty()) {
      ImGui::Text("User Colors (%zu):", user_defined_colors.size());
      int to_remove = -1;
      for (size_t i = 0; i < user_defined_colors.size(); ++i) {
        ImGui::PushID(static_cast<int>(i));
        ImVec4 col{static_cast<float>(user_defined_colors[i].r) / 255.0F,
                   static_cast<float>(user_defined_colors[i].g) / 255.0F,
                   static_cast<float>(user_defined_colors[i].b) / 255.0F,
                   static_cast<float>(user_defined_colors[i].a) / 255.0F};
        ImGui::ColorButton("##color", col, 0, ImVec2(20, 20));
        ImGui::SameLine();
        if (ImGui::SmallButton("X")) {
          to_remove = static_cast<int>(i);
        }
        ImGui::PopID();
      }
      if (to_remove >= 0) {
        user_defined_colors.erase(user_defined_colors.begin() + to_remove);
      }
    } else {
      ImGui::TextDisabled("No custom colors (using default palette)");
    }

    ImGui::Separator();
    ImGui::Text("Tine Z: %.1f (scroll to adjust)", TineValues::z);
  }
  ImGui::End();
}

auto get_drop_color(const std::vector<Color> &user_colors) -> Color {
  if (!user_colors.empty()) {
    return user_colors[static_cast<size_t>(
        RNG::generate_random_num(static_cast<int>(user_colors.size()) - 1, 0))];
  }
  // Fall back to default palette
  return COLORS[static_cast<size_t>(
      RNG::generate_random_num(static_cast<int>(COLORS.size()) - 1, 0))];
}

auto add_new_drop(std::vector<std::unique_ptr<Drop>> &drops, const std::vector<Color> &user_colors)
    -> void {

  auto rand_y = static_cast<float>(RNG::generate_random_num(GetScreenHeight(), 0));
  auto rand_x = static_cast<float>(RNG::generate_random_num(GetScreenWidth(), 0));

  Color color = get_drop_color(user_colors);
  auto drop = std::make_unique<Drop>(
      Vector2{rand_x, rand_y},
      static_cast<float>(RNG::generate_random_num(DropSizes::highest, DropSizes::lowest)), color);

  for (auto &other : drops) {
    other->marble(*drop);
  }

  drops.push_back(std::move(drop));
}

auto add_new_drop(std::vector<std::unique_ptr<Drop>> &drops, Vector2 mouse_pos,
                  const std::vector<Color> &user_colors) -> void {

  Color color = get_drop_color(user_colors);
  auto drop =
      std::make_unique<Drop>(mouse_pos, static_cast<float>(RNG::generate_random_size()), color);

  for (auto &other : drops) {
    other->marble(*drop);
  }

  drops.push_back(std::move(drop));
}

auto tine_line(Vector2 vec, Vector2 pos, float z, float c,
               std::vector<std::unique_ptr<Drop>> &drops) -> void {

  for (auto &drop : drops) {
    drop->general_tine(vec, pos, z, c);
  }
}

auto handle_input(std::vector<std::unique_ptr<Drop>> &drops, TinePoints &tine_points,
                  TimeData &time_data, bool &toggle_drop_spawn,
                  const std::vector<Color> &user_colors) -> void {

  bool imgui_wants_mouse = ImGui::GetIO().WantCaptureMouse;
  float mouse_wheel_movement = GetMouseWheelMove();

  if (toggle_drop_spawn && time_data.since_last_toggle_drop >= TimeData::spawn_interval * 2.0F) {
    add_new_drop(drops, user_colors);
    time_data.since_last_toggle_drop = 0.0F;
  }

  if (!imgui_wants_mouse) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
        time_data.since_last_manual_drop >= TimeData::spawn_interval) {
      add_new_drop(drops, GetMousePosition(), user_colors);
      time_data.since_last_manual_drop = 0.0F;
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) &&
        time_data.since_last_manual_drop >= TimeData::spawn_interval &&
        time_data.since_mouse_down >= TimeData::hold_minimum) {
      add_new_drop(drops, GetMousePosition(), user_colors);
      time_data.since_last_manual_drop = 0.0F;
    }

    if (IsMouseButtonUp(MOUSE_BUTTON_LEFT)) {
      time_data.since_mouse_down = 0.0F;
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
      tine_points.start = GetMousePosition();
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
      tine_points.end = GetMousePosition();
      Vector2 pos_vec = Vector2Subtract(tine_points.end, tine_points.start);
      pos_vec = Vector2Normalize(pos_vec);
      tine_line(pos_vec, tine_points.end, TineValues::z, TineValues::c, drops);
      tine_points.start = Vector2Zero();
      tine_points.end = Vector2Zero();
    }

    if (mouse_wheel_movement != 0) {
      TineValues::z += mouse_wheel_movement;
      TineValues::z = Clamp(TineValues::z, 10.0F, 100.0F);
    }
  }

  if (IsKeyPressed(KEY_R)) {
    drops.clear();
  }

  if (IsKeyPressed(KEY_SPACE)) {
    toggle_drop_spawn = !toggle_drop_spawn;
  }
}
