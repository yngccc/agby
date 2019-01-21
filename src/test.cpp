/***************************************************************************************************/
/*          Copyright (C) 2017-2018 By Yang Chen (yngccc@gmail.com). All Rights Reserved.          */
/***************************************************************************************************/

#include "common.cpp"
#include "math.cpp"
#include "simd.cpp"

#include "ispc/simple.ispc.h"

struct test_guard {
	uint32 counter;
	bool trigger;
	~test_guard() {
		if (trigger) {
			printf("\n");
		}
	}
};

struct test_case_guard {
	uint32 counter;
	bool passed;
	char error[256];
	~test_case_guard() {
	  if (passed) {
      printf("passed\n");
    }
    else {
      printf("failed [%s]\n", error);
    }
  }
};

#define m_test(test_name)                                                                                    \
  bool _test_##test_name = false;                                                                            \
  bool _skip_test_##test_name = false;                                                                       \
  bool _test_print_##test_name = false;                                                                      \
  for (int i = 1; i < argc; i += 1) {                                                                        \
    if (!strcmp(argv[i], "-list")) {                                                                         \
      _test_print_##test_name = true;                                                                        \
      break;                                                                                                 \
    }                                                                                                        \
  }                                                                                                          \
  if (_test_print_##test_name) {                                                                             \
    printf("%s\n", #test_name);                                                                              \
  }                                                                                                          \
  for (int i = 1; i < argc; i += 1) {                                                                        \
    if (!strcmp(argv[i], "skip_" #test_name)) {                                                              \
      _skip_test_##test_name = true;                                                                         \
      break;                                                                                                 \
    }                                                                                                        \
  }                                                                                                          \
  if (argc == 1) {                                                                                           \
    _test_##test_name = true;                                                                                \
  }                                                                                                          \
  else {                                                                                                     \
    for (int i = 1; i < argc; i += 1) {                                                                      \
      if (!strcmp(argv[i], "all") || !strcmp(argv[i], #test_name)) {                                         \
        _test_##test_name = true;                                                                            \
        break;                                                                                               \
      }                                                                                                      \
    }                                                                                                        \
  }                                                                                                          \
  bool _trigger_test_##test_name = !_test_print_##test_name && !_skip_test_##test_name && _test_##test_name; \
  if (_trigger_test_##test_name) {                                                                           \
    printf("[Testing ");                                                                                     \
    printf(#test_name);                                                                                      \
    printf("]\n");                                                                                           \
    test_performed_count += 1;                                                                                 \
  }                                                                                                          \
  for (test_guard _test = {0, _trigger_test_##test_name}; _test.trigger && _test.counter < 1; _test.counter += 1)

#define m_case(case) \
  printf(#case);     \
  printf(" ... ");   \
  for (test_case_guard _test_case = {0, true, {}}; _test_case.counter < 1; _test_case.counter += 1)

#undef m_assert
#define m_assert(expr) if (!(expr)) { _test_case.passed = false; snprintf(_test_case.error, sizeof(_test_case.error), "expr: \"%s\", line: %d", #expr, __LINE__); }

int main(int argc, char **argv) {
  for (int32 i = 1; i < argc; i += 1) {
    if (!strcmp(argv[i], "-help")) {
      printf("option: -help -list\n");
      printf("selection: all || test_name || skip_test_name\n");
      return 0;
    }
  }

  uint32 test_performed_count = 0;

  m_test(array) {
    m_case(remove) {
      {
        uint32 integer_array[4] = {0, 1, 2, 3};
        uint32 integer_array_size = m_countof(integer_array);
        array_remove(integer_array, &integer_array_size, 0);
        m_assert(integer_array_size == 3);
        m_assert(integer_array[0] == 1);
        m_assert(integer_array[1] == 2);
        m_assert(integer_array[2] == 3);
      }
      {
        uint32 integer_array[4] = {0, 1, 2, 3};
        uint32 integer_array_size = m_countof(integer_array);
        array_remove(integer_array, &integer_array_size, 2);
        m_assert(integer_array_size == 3);
        m_assert(integer_array[0] == 0);
        m_assert(integer_array[1] == 1);
        m_assert(integer_array[2] == 3);
      }
      {
        uint32 integer_array[4] = {0, 1, 2, 3};
        uint32 integer_array_size = m_countof(integer_array);
        array_remove(integer_array, &integer_array_size, 3);
        m_assert(integer_array_size == 3);
        m_assert(integer_array[0] == 0);
        m_assert(integer_array[1] == 1);
        m_assert(integer_array[2] == 2);
      }
      {
        uint32 integer_array[1] = {0xdeadbeef};
        uint32 integer_array_size = m_countof(integer_array);
        array_remove(integer_array, &integer_array_size, 0);
        m_assert(integer_array_size == 0);
      }
    }
  }
	m_test(ring_buffer) {
		m_case(double_elems) {
			uint32 buffer[2] = {};
			uint32 value1 = 10;
			uint32 value2 = 100;
			uint32 read_index = 0;
			uint32 write_index = 0;
			ring_buffer_write(buffer, m_countof(buffer), &read_index, &write_index, value1);
			m_assert(ring_buffer_size(m_countof(buffer), read_index, write_index) == 1);
			m_assert(buffer[read_index] == value1);
			ring_buffer_write(buffer, m_countof(buffer), &read_index, &write_index, value2);
			m_assert(ring_buffer_size(m_countof(buffer), read_index, write_index) == 1);
		  m_assert(buffer[read_index] == value2);
		}
		m_case(many_elems) {
			uint32 buffer[256] = {};
			uint32 read_index = 0;
			uint32 write_index = 0;
			for (uint32 i = 0; i < m_countof(buffer) - 1; i += 1) {
				ring_buffer_write(buffer, m_countof(buffer), &read_index, &write_index, i);
			}
			uint32 size = ring_buffer_size(m_countof(buffer), read_index, write_index);
			m_assert(size == (m_countof(buffer) - 1));
			for (uint32 i = 0; i < size; i += 1) {
				m_assert(buffer[(read_index + i) % m_countof(buffer)] == i);
			}
			uint32 value = 9999;
			for (uint32 i = 0; i < 10; i += 1) {
				ring_buffer_write(buffer, m_countof(buffer), &read_index, &write_index, value);
			}
			m_assert(ring_buffer_size(m_countof(buffer), read_index, write_index) == size);
			m_assert(read_index == 10);
		}
	}
	m_test(hash_map) {
		m_case(insert) {
			hash_map<string, int> map;
			hash_map_initialize(&map, 512);
			string key = string_from_cstr("hello");
			int value = 0;
			m_assert(!hash_map_get(&map, key, &value));
			m_assert(value == 0);
			m_assert(hash_map_insert(&map, key, 0x1234));
			m_assert(hash_map_get(&map, key, &value));
			m_assert(value == 0x1234);
			for (int i = 0; i < 256; i += 1) {
				char str[8];
				snprintf(str, sizeof(str), "%d", i);
				string key = string_from_cstr(str);
				m_assert(hash_map_insert(&map, key, i));
			}
			for (int i = 0; i < 256; i += 1) {
				char str[8];
				snprintf(str, sizeof(str), "%d", i);
				string key = string_from_cstr(str);
				int value = 0;
				m_assert(hash_map_get(&map, key, &value));
				m_assert(value == i);
			}
			for (int i = 256; i < 1024; i += 1) {
				char str[8];
				snprintf(str, sizeof(str), "%d", i);
				string key = string_from_cstr(str);
				m_assert(hash_map_insert(&map, key, i));
			}
			for (int i = 256; i < 1024; i += 1) {
				char str[8];
				snprintf(str, sizeof(str), "%d", i);
				string key = string_from_cstr(str);
				int value = 0;
				m_assert(hash_map_get(&map, key, &value));
				m_assert(value == i);
			}
			m_assert(hash_map_remove(&map, key));
			hash_map_delete(&map);
		}
	}
	m_test(memory_pool) {
		memory_pool memory_pool = {};
		uint32 block_count = 1024;
		uint32 block_size = sizeof(mat4);
		uint32 block_alignment = alignof(mat4);
		initialize_memory_pool(&memory_pool, block_count, block_size, block_alignment);
		m_scope_exit(destroy_memory_pool(&memory_pool));
		m_case(allocate_free_allocate_same_block) {
			mat4 *block = allocate_block<mat4>(&memory_pool);
			uintptr_t block_addr = (uintptr_t)block;
			free_block(&memory_pool, block);
			block = allocate_block<mat4>(&memory_pool);
			m_assert(block_addr == (uintptr_t)block);
			clear_memory_pool(&memory_pool);
		}
		m_case(allocate_until_empty) {
			for (uint32 i = 0; i < block_count; i += 1) {
				mat4 *block = allocate_block<mat4>(&memory_pool);
				m_assert(block);
			}
			mat4 *block = allocate_block<mat4>(&memory_pool);
			m_assert(!block);
			clear_memory_pool(&memory_pool);
		}
	}
  m_test(collision) {
		m_case(ray_hit_sphere) {
			float h;
			sphere s = {{0, 0, 0}, 1};
			ray r1 = {{0, 0, 0}, {0, 1, 0}, 0.9f};
			ray r2 = {{0, 0, 0}, {0, 1, 0}, 1.1f};
			ray r3 = {{0, 2, 0}, {0, -1, 0}, 1};
			ray r4 = {{0, 2, 0}, {0, 1, 0}, 1};
			ray r5 = {{0, 2, 0}, {1, 0, 0}, 1};

			m_assert(!ray_hit_sphere(r1, s, &h));
			m_assert(ray_hit_sphere(r2, s, &h));
			m_assert(ray_hit_sphere(r3, s, &h));
			m_assert(!ray_hit_sphere(r4, s, &h));
			m_assert(!ray_hit_sphere(r5, s, &h));
		}
    m_case(ray_hit_triangle) {
      vec3 a = {-1, -1, 0};
      vec3 b = {1, -1, 0};
      vec3 c = {0, 1, 0};
      ray ray = {};
      ray.origin = {0, 0, 1};
      ray.dir = {0, 0, -1};
      ray.len = 10;
			float h;
      vec3 hp = {};
      m_assert(ray_hit_triangle(ray, a, b, c, &h, &hp));
    }
  }
  m_test(simd) {
    m_case(filter_floats) {
      const uint32 array_size = 100000;
      const uint32 test_num = 5;
      float *in = new float[array_size]();
      float *out = new float[array_size]();
      float *out_simd = new float[array_size]();
      for (uint32 i = 0; i < array_size; i += 1) {
        in[i] = (float)(rand() % 10);
      }
      uint32 count = 0;
      {
        for (uint32 i = 0; i < array_size; i += 1) {
          if (in[i] <= (float)test_num) {
            out[count++] = in[i];
          }
        }
      }
      {
        uint32 n = simd_filter_floats(in, out_simd, array_size, (float)test_num, compare_op_le);
        // uint32 n = ispc::filter_floats(in, out_simd, array_size, (float)test_num);
        m_assert(n == count);
        for (uint32 i = 0; i < count; i += 1) {
          m_assert(out_simd[i] == out[i]);
        }
      }
      delete []in;
      delete []out;
      delete []out_simd;
    }
  }
	
  printf("Performed %d tests\n", test_performed_count);
}
