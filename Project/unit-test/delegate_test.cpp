#include <boost/test/unit_test.hpp>

#include <stx/delegate.hpp>

namespace {

	void hoge()
	{
		std::cout << "hoge" << std::endl;
	}

	BOOST_AUTO_TEST_SUITE(delegate)

	BOOST_AUTO_TEST_CASE(delegate_basic_test)
	{
		stx::delegate test;
		auto lhash = test += []() {std::cout << "lambda" << std::endl; };
		auto fhash = test += hoge;

		test();
	}

	BOOST_AUTO_TEST_SUITE_END()

}