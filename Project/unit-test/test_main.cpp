#define BOOST_TEST_NO_LIB	// static�Ƀ����N����ꍇ
//#define BOOST_TEST_REPORT_LEVEL detailed
//#define BOOST_LOG_LEVEL all

#ifdef _DEBUG
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_log.hpp>

#include <iostream>

#include "basic_dostream.h"
dostream trace;

boost::unit_test_framework::test_suite* init_unit_test_suite(int argc, char* argv[])
{
	// ���O�̏o�͐��ύX(�w�肵�Ȃ��ƃf�t�H���g�ł�std::cout)
	boost::unit_test::unit_test_log_t::instance().set_stream(trace);
	std::cerr.rdbuf(trace.rdbuf());

	return BOOST_TEST_SUITE("replace output stream");
}
#endif


#define BOOST_TEST_MODULE boost_unit_test
#include <boost/test/included/unit_test.hpp>