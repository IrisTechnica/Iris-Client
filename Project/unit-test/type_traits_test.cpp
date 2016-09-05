
#include <boost/test/unit_test.hpp>

#include <stx/typetraits/function_traits.hpp>

namespace {

	struct member_func_test
	{
		void test1(int)
		{};

		static void test2(int)
		{};
	};

	struct Fixture {
		std::shared_ptr<member_func_test> test;
		Fixture()
		{
			test = std::make_shared<member_func_test>();
		}
	};

	BOOST_FIXTURE_TEST_SUITE(type_traits, Fixture)

	BOOST_AUTO_TEST_CASE(lambda_to_function)
	{
		auto lambda = []() {};
		using func_type = stx::to_function<decltype(lambda)>::type;

		BOOST_REQUIRE_EQUAL(typeid(func_type).name(), typeid(std::function<void()>).name());
	}

	BOOST_AUTO_TEST_CASE(lambda_to_function2)
	{
		auto lambda = [](int) {};
		using func_type = stx::to_function<decltype(lambda)>::type;

		BOOST_REQUIRE_EQUAL(typeid(func_type).name(), typeid(std::function<void(int)>).name());
	}
	

	BOOST_AUTO_TEST_CASE(member_to_function)
	{
		using func_type = stx::to_function<decltype(&member_func_test::test1)>::type;
		using class_type = stx::to_function<decltype(&member_func_test::test1)>::class_type;

		BOOST_REQUIRE_EQUAL(typeid(func_type).name(), typeid(std::function<void(int)>).name());
		BOOST_REQUIRE_EQUAL(typeid(class_type).name(), typeid(member_func_test).name());

	}

	BOOST_AUTO_TEST_CASE(member_to_function2)
	{
		using func_type = stx::to_function<decltype(&member_func_test::test2)>::type;
		using class_type = stx::to_function<decltype(&member_func_test::test2)>::class_type;

		BOOST_REQUIRE_EQUAL(typeid(func_type).name(), typeid(std::function<void(int)>).name());
		BOOST_REQUIRE_EQUAL(typeid(class_type).name(), typeid(void).name());

	}



	BOOST_AUTO_TEST_SUITE_END()
}