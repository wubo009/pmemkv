// SPDX-License-Identifier: BSD-3-Clause
/* Copyright 2020, Intel Corporation */

#include "unittest.hpp"

using namespace pmem::kv;

static void insert(const size_t iterations, pmem::kv::db &kv)
{

	for (size_t i = iterations; i >= 1; i--) {
		std::string istr = std::to_string(i);
		UT_ASSERT(kv.put(istr, ("ABC" + istr)) == status::OK);
		std::string value;
		UT_ASSERT(kv.get(istr, &value) == status::OK && value == ("ABC" + istr));
	}
}
static void check(const size_t iterations, pmem::kv::db &kv)
{
	for (size_t i = iterations; i >= 1; i--) {
		std::string istr = std::to_string(i);
		std::string value;
		UT_ASSERT(kv.get(istr, &value) == status::OK && value == ("ABC" + istr));
	}
	std::size_t cnt = std::numeric_limits<std::size_t>::max();
	UT_ASSERT(kv.count_all(cnt) == status::OK);
	UT_ASSERT(cnt == iterations);
}

static void test(int argc, char *argv[])
{
	if (argc < 5)
		UT_FATAL("usage: %s engine json_config insert/check iterations", argv[0]);

	std::string mode = argv[3];
	if (mode != "insert" && mode != "check")
		UT_FATAL("usage: %s engine json_config insert/check iterations", argv[0]);

	auto iterations = std::stoull(argv[4]);

	auto kv = INITIALIZE_KV(argv[1], CONFIG_FROM_JSON(argv[2]));

	if (mode == "insert") {
		insert(iterations, kv);
	} else {
		check(iterations, kv);
	}

	kv.close();
}

int main(int argc, char *argv[])
{
	return run_test([&] { test(argc, argv); });
}
