/***************************************************************************************************/
/*          Copyright (C) 2017-2018 By Yang Chen (yngccc@gmail.com). All Rights Reserved.          */
/***************************************************************************************************/

#include "platform_windows.cpp"
#include "math.cpp"
#include "simd.cpp"

#include "../build/simple.ispc.h"

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

#define m_test(test_name)                                                                                         \
  bool _test_##test_name = false;                                                                                 \
  bool _skip_test_##test_name = false;                                                                            \
  bool _test_print_##test_name = false;                                                                           \
  for (int i = 1; i < argc; i += 1) {                                                                             \
    if (!strcmp(argv[i], "-list")) {                                                                              \
      _test_print_##test_name = true;                                                                             \
      break;                                                                                                      \
    }                                                                                                             \
  }                                                                                                               \
  if (_test_print_##test_name) {                                                                                  \
    printf("%s\n", #test_name);                                                                                   \
  }                                                                                                               \
  for (int i = 1; i < argc; i += 1) {                                                                             \
    if (!strcmp(argv[i], "skip_" #test_name)) {                                                                   \
      _skip_test_##test_name = true;                                                                              \
      break;                                                                                                      \
    }                                                                                                             \
  }                                                                                                               \
  if (argc == 1) {                                                                                                \
    _test_##test_name = true;                                                                                     \
  }                                                                                                               \
  else {                                                                                                          \
    for (int i = 1; i < argc; i += 1) {                                                                           \
      if (!strcmp(argv[i], "all") || !strcmp(argv[i], #test_name)) {                                              \
        _test_##test_name = true;                                                                                 \
        break;                                                                                                    \
      }                                                                                                           \
    }                                                                                                             \
  }                                                                                                               \
  bool _trigger_test_##test_name = !_test_print_##test_name && !_skip_test_##test_name && _test_##test_name;      \
  if (_trigger_test_##test_name) {                                                                                \
    printf("[Testing ");                                                                                          \
    printf(#test_name);                                                                                           \
    printf("]\n");                                                                                                \
    num_test_performed += 1;                                                                                      \
  }                                                                                                               \
  for (test_guard _test = {0, _trigger_test_##test_name}; _test.trigger && _test.counter < 1; _test.counter += 1)

#define m_case(case)                                                                             \
  printf(#case);                                                                                 \
  printf(" ... ");                                                                               \
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

  uint32 num_test_performed = 0;
  m_scope_exit(printf("Performed %d tests\n", num_test_performed));

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

  m_test(string) {
    m_case(cat) {
      {
        char path0[32] = "apple";
        char path1[32] = " orange";
        string path0_str = string_from_array(path0);
        string_cat(&path0_str, path1, (uint32)strlen(path1));
        m_assert(!(strcmp(path0, "apple orange")));
        m_assert(path0_str.len == strlen(path0));
      }
      {
        char path0[6] = "apple";
        char path1[32] = " orange";
        string path0_str = string_from_array(path0);
        string_cat(&path0_str, path1, (uint32)strlen(path1));
        m_assert(!(strcmp(path0, "apple")));
        m_assert(path0_str.len == strlen("apple"));
      }
      {
        char path0[10] = "apple";
        char path1[32] = " orange";
        string path0_str = string_from_array(path0);
        string_cat(&path0_str, path1, (uint32)strlen(path1));
        m_assert(!(strcmp(path0, "apple ora")));
        m_assert(path0_str.len == strlen("apple ora"));
      }
    }
    m_case(pop_back) {
      {
        char path[256] = "c:/sdk/include/foo.h";
        string path_str = string_from_array(path);
        string_pop_back(&path_str, '/');
        m_assert(!strcmp(path, "c:/sdk/include/"));
        m_assert(path_str.len == (uint32)strlen(path));
        string_pop_back(&path_str, '/');
        m_assert(!strcmp(path, "c:/sdk/include/"));
        m_assert(path_str.len == (uint32)strlen(path));
        string_pop_back(&path_str, ':');
        m_assert(!strcmp(path, "c:"));
        m_assert(path_str.len == (uint32)strlen(path));
      }
      {
        char path[256] = "";
        string path_str = string_from_array(path);
        string_pop_back(&path_str, 'x');
        m_assert(!strcmp(path, ""));
        m_assert(path_str.len == 0);
      }
      {
        char path[256] = "1234567890";
        string path_str = string_from_array(path);
        string_pop_back(&path_str, 'x');
        m_assert(!strcmp(path, "1234567890"));
        m_assert(path_str.len == (uint32)strlen(path));
      }
    }
  }

  m_test(math) {
    m_case(ray_intersect_triangle) {
      vec3 a = {-1, -1, 0};
      vec3 b = {1, -1, 0};
      vec3 c = {0, 1, 0};
      ray ray = {};
      ray.origin = {0, 0, 1};
      ray.direction = {0, 0, -1};
      ray.len = 10;
      vec3 barycentric_coord = {};
      float intersect_distance = 0;
      bool intersect = ray_intersect_triangle(ray, a, b, c, &intersect_distance);
      m_assert(intersect);
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
      timer timer = {};
      initialize_timer(&timer);
      uint32 count = 0;
      {
        start_timer(&timer);
        for (uint32 i = 0; i < array_size; i += 1) {
          if (in[i] <= (float)test_num) {
            out[count++] = in[i];
          }
        }
        stop_timer(&timer);
        uint64 duration = get_timer_duration_microsecs(timer);
        printf("\n%llu\n", duration);
      }
      {
        start_timer(&timer);
        uint32 n = simd_filter_floats(in, out_simd, array_size, (float)test_num, compare_op_le);
        // uint32 n = ispc::filter_floats(in, out_simd, array_size, (float)test_num);
        stop_timer(&timer);
        uint64 duration = get_timer_duration_microsecs(timer);
        printf("%llu\n", duration);
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
}
