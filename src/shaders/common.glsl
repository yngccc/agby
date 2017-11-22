const uint max_directional_light_count = 1;
const uint max_point_light_count = 4;
const uint max_spot_light_count = 4;

struct ambient_light_t {
	vec4 color;
};

struct directional_light_t {
  vec4 color;
  vec4 direction;
};

struct point_light_t {
  vec4 color;
  vec4 position;
};

struct spot_light_t {
  vec4 color;
  vec4 position;
  vec4 direction;
};


