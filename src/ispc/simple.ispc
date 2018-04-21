/***************************************************************************************************/
/*          Copyright (C) 2017-2018 By Yang Chen (yngccc@gmail.com). All Rights Reserved.          */
/***************************************************************************************************/

typedef unsigned int uint32;

export uniform uint32 filter_floats(uniform float input[], uniform float output[], uniform uint32 count, uniform float cmp) {
	uniform uint32 n = 0;
  for (uniform uint32 i = 0; i < count; i += 1) {
	  uniform float v = input[i];
	  if (v <= cmp) {
	    output[n] = v;
	    n += 1;
	  }
  }
  return n;
}
