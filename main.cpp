//
// Created by teslarct on 12/12/2025
//

#include <raylib.h>
#include <iostream>
#include <numbers>
#include <cmath>
#include <vector>
#include <string>
#include <format>
#include <stdexcept>



#if defined(__GNUC__) || defined(__clang__)
#define FORCE_INLINE [[gnu::always_inline]] inline
#elif defined(_MSC_VER)
#define FORCE_INLINE __forceinline
#else
#define FORCE_INLINE inline
#endif




// Vector - 2D ---------------------------
class vector {
  float x{0.f};
  float y{0.f};
public:
  vector() noexcept = default;
  explicit vector(float const x_pos, float const y_pos) noexcept: x(x_pos), y(y_pos) {}
  [[nodiscard]] float magnitude() const noexcept;
  [[nodiscard]] vector direction() const;
  [[nodiscard]] float distance(const vector &) const noexcept;

  vector operator+(const vector &other) const {
    return vector{this->x + other.x, this->y + other.y};
  }
  vector operator-(vector const &other) const {
    return vector{this->x - other.x, this->y - other.y};
  }
  float operator^(vector const &other) const {
    return this->x * other.x + this->y * other.y;
  }

  vector operator*(float const scalar) const {
    return vector{this->x * scalar, this->y * scalar};
  }
  vector &operator-=(vector const &other) {
    this->x -= other.x;
    this->y -= other.y;
    return *this;
  }
  vector &operator+=(vector const &other) {
    this->x += other.x;
    this->y += other.y;
    return *this;
  }


  bool operator==(const vector &other) const {
    return this->x == other.x && this->y == other.y;
  }


  // Vector2 converter --------
  [[nodiscard]] Vector2 converter() const {
    Vector2 const pos{.x = this->x, .y = this->y};
    return pos;
  }
};

[[nodiscard]] FORCE_INLINE float vector::magnitude() const noexcept {
  return std::sqrt(x * x + y * y);
}

[[nodiscard]] FORCE_INLINE vector vector::direction() const {
  float const mag = magnitude();
  if (mag == 0.f) throw std::invalid_argument("Null vector hasn't direction");
  float const inverse = 1.f / mag;
  return vector(x * inverse, y * inverse);
}

[[nodiscard]] float vector::distance(vector const &other) const noexcept {
  vector const disp = *this - other;
  return std::sqrt(disp ^ disp);
}



namespace obj {
  class ObjIntrinsicProperty;
  class ObjDynamicState;
  class Object;
}


namespace spc {
  void space(std::initializer_list<obj::Object> const &) ;
  vector GField(vector);
}

void engine();
void loader();


namespace obj{

class ObjIntrinsicProperty {

protected:
  // Extensive Properties(Shape is sphere (circle in 2D))
  float mass{0.f};
  float radius{0.f};

  // Intensive Properties
  CLITERAL(Color) color{0, 0, 0, 0};

  ObjIntrinsicProperty() noexcept = default;
  explicit ObjIntrinsicProperty(float const m, float const r): color({0, 0, 0, 255}) {
    if (m <= float{} || r <= float{}) throw std::invalid_argument("Object should exist");
    mass = m;
    radius = r;
  }
  explicit ObjIntrinsicProperty(float const m, float const r, CLITERAL(Color) const c): color(c) {
    if (m <= float{} || r <= float{}) throw std::invalid_argument("Object should exist");
    mass = m;
    radius = r;
  }

  // Extensive Methods
  [[nodiscard]] constexpr auto volume() const noexcept;

  // Intensive Methods
  [[nodiscard]] auto density() const noexcept;

};

FORCE_INLINE constexpr auto ObjIntrinsicProperty::volume() const noexcept{
  return 1.333333f * std::numbers::pi * radius * radius * radius;
}




FORCE_INLINE auto ObjIntrinsicProperty::density() const noexcept{
  return mass / volume();
}




class ObjDynamicState {

protected:
  vector velocity{};
  vector position{};


  ObjDynamicState() noexcept = default;
  explicit ObjDynamicState(vector const vel, vector const pos): velocity(vel), position(pos) {}
};



class Object: ObjIntrinsicProperty, ObjDynamicState {

public:
  Object() noexcept = default;
  explicit Object(float const m, float const r): ObjIntrinsicProperty(m, r) {}
  explicit Object(float const m, float const r, CLITERAL(Color) const c): ObjIntrinsicProperty(m, r, c), ObjDynamicState() {}
  explicit Object(float const m, float const r, CLITERAL(Color) const c, vector const v, vector const p): ObjIntrinsicProperty(m, r, c), ObjDynamicState(v, p) {}

  friend vector spc::GField(vector);
  friend void ::engine();
  friend void ::loader();

};



}


namespace spc {

  std::vector<obj::Object> objects{};

  constexpr float G{50.f};


  void space(std::initializer_list<obj::Object> const &matters) {
    for (auto const &matter: matters) objects.push_back(matter);
  }

  // Mass assumed as a point
  vector GField(vector const point) {
    vector g{0.f, 0.f};
    for (auto const &o: objects) {
      vector R = point - o.position;
      if (R == vector{}) continue;
      float const modR = R.magnitude();
      g -= R * (G * o.mass / (modR * modR * modR));
    }
    return g;
  }
}




// This function generate each frame ---------------------
void engine() {

  for (auto &o: spc::objects) {
    vector acceleration = spc::GField(o.position);
    float const t{GetFrameTime()};
    o.position += o.velocity * t + acceleration * (0.5f * t * t);
    o.velocity += acceleration * t;
  }
}

void loader() {
  ClearBackground(RAYWHITE);

  std::string str = std::format("{}", spc::objects[2].velocity.magnitude());
  char const* text1 = str.c_str();
  DrawText(text1,2,2,20,GREEN);

  str = std::format("{}", spc::objects[0].velocity.magnitude());
  char const* text2 = str.c_str();
  DrawText(text2,2,26,20,RED);

  str = std::format("{}", spc::objects[1].velocity.magnitude());
  char const* text3 = str.c_str();
  DrawText(text3,2,50,20,BLUE);


  for (auto &obj: spc::objects) {
    DrawCircleV(obj.position.converter(), obj.radius, obj.color);
  }
  engine();
}


constexpr int screenWidth{1500};
constexpr int screenHeight{1000};



using namespace obj;
int main() {
  InitWindow(screenWidth, screenHeight, "FrameRate");
  SetTargetFPS(60);
  Object o1(50,20,RED,vector{0.f,0.f},vector{750,45});
  Object o2(5000,50,BLUE,vector{0.f,0.f},vector{100,500});
  Object o3(5000,50,GREEN,vector{0.f,0.f},vector{1400,500});
  // Object o4(10,10,BLUE,vector{0.f,0.f},vector{300,20});
  spc::space({o1, o2, o3});
  while (!WindowShouldClose()) {
    BeginDrawing();
      loader();
    EndDrawing();
  }
  CloseWindow();
  return 0;
}


//N body gravitation simulation