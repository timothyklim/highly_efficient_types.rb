//
//  main.cpp
//  ulib_gem
//
//  Created by Timothy Klim on 5/26/13.
//  Copyright (c) 2013 Timothy Klim. All rights reserved.
//

//#include <time.h>
//#include <stdio.h>
//#include <assert.h>
//#include <string.h>

#include "ulib/hash_func.h"
#include "ulib/hash_align.h"
#include "ulib/math_rand_prot.h"

//#include "murmur/MurmurHash3.h"

using namespace ulib;

extern "C" {

    #include <ruby.h>
    
    #define GetCallbackStruct(obj)	(Check_Type(obj, T_DATA), (RCallback*)DATA_PTR(obj))
    
    static VALUE hola_bonjour(VALUE self) {
        return rb_str_new2("bonjour!");
    }

    typedef struct {
        align_hash_map<VALUE, VALUE> *hash_map;
    } RCallback;
    
    static void mark_hash_map_values(RCallback *incoming) {
            for(align_hash_map<VALUE, VALUE>::iterator it = incoming->hash_map->begin(); it != incoming->hash_map->end(); ++it) {
                rb_gc_mark(it.key());
                rb_gc_mark(it.value());
            }
    }

    static void free_hash_callback(RCallback* cb) {
        delete cb->hash_map;
    }
    
    static VALUE
    callback_alloc( VALUE klass )
    {
        RCallback* cbs;
        
        VALUE current_instance = Data_Make_Struct(klass, RCallback, mark_hash_map_values, free_hash_callback, cbs);
        
        cbs->hash_map = new align_hash_map<VALUE, VALUE>();
        cbs->hash_map->insert(0, 0);
        
        return current_instance;
    }
    
    static VALUE rb_ah_hash_set(VALUE cb, VALUE set_this, VALUE to_this) {
        RCallback* cbs = GetCallbackStruct(cb);

//        insert(const _Key &key, const _Val &val, bool replace = false)
        printf("size: %llu", (unsigned long long)cbs->hash_map->size());
        
        
        return set_this;
    }
    
    static VALUE rb_ah_hash_get_value(VALUE cb, VALUE get_this) {
        return rb_str_new2("bonjour!");
    }
    
    void
    Init_align_hash_with_murmur(void) {
        VALUE klass = rb_define_class("AlignHashWithMurmur", rb_cObject);
    
        rb_define_alloc_func(klass, callback_alloc);
        rb_define_method(klass, "[]=", RUBY_METHOD_FUNC(rb_ah_hash_set), 2);
        rb_define_method(klass, "[]", RUBY_METHOD_FUNC(rb_ah_hash_get_value), 1);

        rb_define_singleton_method(klass, "hello", (VALUE (*) (ANYARGS)) hola_bonjour, 0);
    }
}
//struct str {
//	const char *c_str;
//    
//	str(const char *s = 0)
//    : c_str(s) { }
//    
//	operator size_t() const
//	{ return hash_fast64((const unsigned char *)c_str, strlen(c_str), 0); }
//    
//	bool operator==(const str &other) const
//	{ return strcmp(c_str, other.c_str) == 0; }
//};
//
//int main(int argc, const char * argv[])
//{
//	align_hash_map<str, int> months;
//    
//	months["january"] = 31;
//	months["february"] = 28;
//	months["march"] = 31;
//	months["april"] = 30;
//	months["may"] = 31;
//	months["june"] = 30;
//	months["july"] = 31;
//	months["august"] = 31;
//	months["september"] = 30;
//	months["october"] = 31;
//	months["november"] = 30;
//	months["december"] = 31;
//    
//	assert(months["september"] == 30);
//	assert(months["april"] == 30);
//	assert(months["february"] == 28);
//	assert(months["december"] == 31);
//    
//	align_hash_map<uint64_t, int> map;
//    
//	map[1] = 2;
//	map[2] = 1;
//	map[3] = 3;
//
//    align_hash_map<uint32_t*, str> map32;
//    
//    uint32_t result[4];
//    uint64_t _int;
//    
//    str argV = "wowow::awdawdawdawdawd::ad awJBJHDBAWJHBD::BJKAWBDKJAWBDJKBAWJKDB::ajwndkjawbkjdb ajkwdbkj abwdkj bajkwdb kjawbd kjawbdjk bawkjdb awjkdb kjawbdkj abwkdj bajkwdb kjawbdkj abwdkj bawkjdb jkawbd jkawbdkj abwkjd bawjkdb akjwdbjk awbdkj bawjkdb awjkdb jkawbd kjawbdkj abwjkd bawjkdb jkawbdjk awbdjk bawjkd bajwkdb jkawb jkabwjk dbakjwbd jkawbdkj abwkjdb akjwdb kjawbdjk abwdjkb ajkwdbjk abwdkjabw jawbdjk abwd не верю";
//    MurmurHash3_x64_128(&_int, 8, (uint32_t)argV, result);
//    
//    map32[result] = "wowowowo!!!!!!!!!!!!!!!!!!!!";
//    
//    printf("result: %u\n", *result);
//    printf("map32[result]: %s\n", map32[result].c_str);
//    
//	align_hash_map<uint64_t, int> copy1(map);
//	assert(copy1[1] == 2);
//	assert(copy1[2] == 1);
//	assert(copy1[3] == 3);
//    
//	align_hash_map<uint64_t, int> copy2 = map;
//	assert(copy2[1] == 2);
//	assert(copy2[2] == 1);
//	assert(copy2[3] == 3);
//    
//	assert(copy1.size() == 3);
//	assert(copy1.size() == copy2.size());
//	copy2 = copy1;
//	assert(copy1.size() == copy2.size());
//	copy2 = copy2;
//	assert(copy2[1] == 2);
//	assert(copy2[2] == 1);
//	assert(copy2[3] == 3);
//	assert(copy1.size() == copy2.size());
//    
//	uint64_t seed = time(NULL);
//	uint64_t num = seed;
//    
//	// insert 100000 random numbers
//	for (int i = 0; i < 100000; ++i) {
//		RAND_XORSHIFT(num, 7, 5, 47);
//		map[num] = -1;
//	}
//	// check if all random numbers can be found
//	num = seed;
//	for (int i = 0; i < 100000; ++i) {
//		RAND_XORSHIFT(num, 7, 5, 47);
//		assert(map[num] == -1);
//	}
//	// then remove all random numbers
//	num = seed;
//	for (int i = 0; i < 100000; ++i) {
//		RAND_XORSHIFT(num, 7, 5, 47);
//		map.erase(num);
//	}
//	assert(map.size() == 3);
//	// ensure all random numbers were erased
//	num = seed;
//	for (int i = 0; i < 100000; ++i) {
//		RAND_XORSHIFT(num, 7, 5, 47);
//		assert(map[num] == 0); // default value for new elemnets is zero
//	}
//    
//	printf("passed\n");
//    
//	return 0;
//}

