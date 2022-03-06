class Vec {
 public:
  float x, y;

  enum Dim { X, Y };

  Vec(){};
  Vec(float x, float y) { setAt(x, y); };
  ~Vec();

  void setAt(float _x, float _y) {
    x = _x;
    y = _y;
  }

  float getDim(Dim dim) {
    if (dim == Dim::X) {
      return x;
    }
    if (dim == Dim::Y) {
      return y;
    }
  };

  Vec& operator+=(const Vec& rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
  };

  void print(bool newline) {
    Serial.printf("Vec(%f, %f)", x, y);
    if (newline) Serial.print("\n");
  };
};

float distance(Vec a) {
  return sqrt(pow(a.x, 2) + pow(a.y, 2));
}

float distance(Vec a, Vec b) {
  return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}
