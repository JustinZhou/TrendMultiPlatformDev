#define BOOST_TEST_MAIN
#include <boost/integer_traits.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <string.h>
#include "Marco_define.h"

using namespace std;
using namespace boost::filesystem;

BOOST_AUTO_TEST_CASE(path_construct_test)
{
	path p1;
	path p2("/usr/local/lib");
	path p3(p2);
	char str[] = "The path is (/root)";
	path p4(str+13, str+14);

	BOOST_ASSERT(p1.empty());
	BOOST_ASSERT(p2.string()=="/usr/local/lib");
	BOOST_ASSERT(p3.string()=="/usr/local/lib");
	BOOST_ASSERT(p4.string()=="/");
}

BOOST_AUTO_TEST_CASE(path_splice_test)
{
	path p1("c:\\tmp\\test.txt");
	//path p2(R"(C:\tmp\test.txt)");
	path p3("c:/tmp/test.txt");
	path p4 = "c:/tmp/test.txt";
	path p5("./test.txt");
	path p6 = system_complete(p5);
	path p7 = current_path();
	p7/="test.txt";

	BOOST_ASSERT(p1==p3 && p1==p4);
	BOOST_ASSERT(p6==p7);
	BOOST_ASSERT(p5!=p6);
}

BOOST_AUTO_TEST_CASE(path_checkfunction_test)
{
	BOOST_ASSERT(!portable_name("w+()abc.txt") && !portable_name("./abc"));
	BOOST_ASSERT(!portable_directory_name("a.txt") && portable_directory_name("abc"));
	BOOST_ASSERT(portable_file_name("a.bc") && !portable_file_name("y.conf"));
}


BOOST_AUTO_TEST_SUITE(path_handle);

BOOST_AUTO_TEST_CASE(path_get_test)
{
	path p1("/usr/local/include/xxx.hpp");
	path p2("C:/usr/local/yyy.hpp");

	BOOST_ASSERT(p1.parent_path() == "/usr/local/include");
	BOOST_ASSERT(p1.stem() == "xxx");
	BOOST_ASSERT(p1.filename() == "xxx.hpp");
	BOOST_ASSERT(p1.extension() == ".hpp");
	BOOST_ASSERT(p1.root_name() == "");
	BOOST_ASSERT(p1.root_directory() == "/");
	BOOST_ASSERT(p1.root_path() == "/");

	BOOST_ASSERT(p2.parent_path() == "C:/usr/local");
	BOOST_ASSERT(p2.stem() == "yyy");
	BOOST_ASSERT(p2.filename() == "yyy.hpp");
	BOOST_ASSERT(p2.extension() == ".hpp");
	BOOST_ASSERT(p2.root_name() == "C:");
	BOOST_ASSERT(p2.root_directory() == "/");
	BOOST_ASSERT(p2.root_path() == "C:/");
}

BOOST_AUTO_TEST_CASE(path_connect_test)
{
	string filename = "xinetd.conf";
	//add separator
	path p1("/root");
	p1/="doc";
	p1.append(filename.begin(),filename.end());

	//not add separator
	path p2("/root");
	p2+="etc";
	p2.concat(filename.begin(),filename.end());

	BOOST_ASSERT(p1.string()=="/root\\doc\\xinetd.conf");
	BOOST_ASSERT(p2.string()=="/rootetcxinetd.conf");
}

BOOST_AUTO_TEST_CASE(path_check_test)
{
	path p1("/usr/local/include/xxx.hpp");

	BOOST_ASSERT(!p1.has_root_name());
	BOOST_ASSERT(p1.has_root_path());
	BOOST_ASSERT(p1.has_parent_path());
}

BOOST_AUTO_TEST_CASE(path_modify_test)
{
	path p1("C:/windows/test.txt");
	path p2 = p1.replace_extension();
	BOOST_ASSERT(p2.string() == "C:/windows/test");
	p1.replace_extension("txt");

	path p3 = p1.remove_filename();
	BOOST_ASSERT(p3.string() == "C:/windows");
}

BOOST_AUTO_TEST_CASE(path_compare_test)
{
	path p1("/test/1.cpp");
	path p2("/TEST/2/cpp");
	path p3("/abc/1.cpp");

	BOOST_ASSERT(p1 != p2);
	BOOST_ASSERT(p2 < p3);
	BOOST_ASSERT(p1.compare(p2) == 1);
}

BOOST_AUTO_TEST_CASE(path_interation_test)
{
	path p("/boost/tools/libs");
	int i = 0;
	BOOST_FOREACH(const path& x, p)
	{
		switch (i)
		{
		case 0: BOOST_ASSERT(x == "/");
			break;
		case 1: BOOST_ASSERT(x == "boost");
			break;
		case 2: BOOST_ASSERT(x == "tools");
			break;
		case 3: BOOST_ASSERT(x == "libs");
			break;
		default:
			break;
		}
		i++;
	}
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(file_relate_test)

BOOST_AUTO_TEST_CASE(file_status_test)
{
#ifdef LOCAL_TEST
	int test = status("d:/boost").type();
	BOOST_ASSERT(status("d:/boost").type() == directory_file);
	BOOST_ASSERT(status("d:/boost/nofile").type() == file_not_found);
	BOOST_ASSERT(status("d:/boost/INSTALL").type() != symlink_file);
	BOOST_ASSERT(status("d:/boost/INSTALL").type() == regular_file);

	BOOST_ASSERT(is_directory("d:/boost"));
	BOOST_ASSERT(!exists("d:/boost/nofile"));
	BOOST_ASSERT(!is_symlink("d:/boost/INSTALL"));
	BOOST_ASSERT(!is_other("d:/boost/INSTALL"));
	BOOST_ASSERT(is_regular_file("d:/boost/INSTALL"));
	BOOST_ASSERT(!boost::filesystem::is_empty("d:/boost/INSTALL"));
#endif // LOCAL_TEST
}

BOOST_AUTO_TEST_SUITE_END()