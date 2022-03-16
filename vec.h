class Vec {
 public:
  float x, y, z;

  enum Dim { X, Y, Z};

  Vec(){};
  Vec(float x, float y, float z) { setAt(x, y, z); };
  ~Vec(){};

  void setAt(float _x, float _y, float _z) {
    x = _x;
    y = _y;
    z = _z;
  }

  float getDim(Dim dim) {
    if (dim == Dim::X) {
      return x;
    }
    if (dim == Dim::Y) {
      return y;
    }
    
    if (dim == Dim::Z) {
      return Z;
    }
  };

  Vec& operator+=(const Vec& rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
  };

  Vec operator+(const Vec& other) {
    return Vec(x + other.x, y + other.y, z + other.z);
  };

  Vec operator-(const Vec& other) {
    return Vec(x - other.x, y - other.y, z - other.z);
  };

  void print(bool newline = false, uint8_t decimals = 2) {
    Serial.printf("Vec(%.*f, %.*f, %.*f)", decimals, x, decimals, y, decimals, z);
    if (newline) Serial.print("\n");
  };
};

float distance(Vec a) { return sqrt(pow(a.x, 2) + pow(a.y, 2) + pow(a.x, 2)); }

float distance(Vec a, Vec b) {
  return distance(b - a);
}
