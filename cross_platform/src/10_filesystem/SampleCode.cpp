#include "Marco_define.h"
#ifdef SHOW_SAMPLE

#define BOOST_ALL_NO_LIB
#include <boost/filesystem.hpp>
using namespace boost::filesystem;

int main()
{
	namespace fs = boost::filesystem;		//名字空间别名

	path ptest = "/home/chrono/test";
	if (exists(ptest))		//检查路径是否存在
	{
		if (fs::is_empty(ptest))		//注意名字空间限定
		{
			remove(ptest);		//remove只能删除空目录或文件
		}
		else
		{
			remove_all(ptest);		//remove_all可以递归删除
		}
		
		assert(!exists(ptest));		//该文件已经被删除
		create_directory(ptest);	//创建一个目录

		copy_file("/home/chrono/boost/INSTALL", ptest/"a.txt");		//拷贝文件
		assert(exists(ptest/"a.txt"));

		rename(ptest/"a.txt", ptest/"b.txt");		//改名
		assert(exists(ptest/"b.txt"));

		//使用create_directories可以一次创建多级目录
		create_directories(ptest/"sub_dir1"/"sub_dir1");
	}
}


#include <boost/optional.hpp>
#include <boost/filesystem.hpp>

using namespace boost::filesystem;

//实现查找文件功能
typedef recursive_directory_iterator rd_iterator;
optional<path> 
find_file(const path& dir, const string& filename)
{
	typedef optional<path> result_type;		//返回值类型定义
	if (!exists(dir) || !is_directory(dir))		//检查目录的有效性
	{
		return result_type();
	}
	rd_iterator end;		//递归迭代器
	for (rd_iterator pos(dir);pos != end; ++pos)
	{
		if (!is_directory(*pos) && pos->path.filename() == filename)		//不是目录且文件名相等
		{
			return result_type(pos->path());
		}
	}
	return result_type();
}


#include <boost/xpressive/xpressive_dynamic.hpp>
#include <boost/algorithm/string.hpp>
using namespace boost::xpressive;

//实现模糊查找文件功能
void find_files(const path& dir, const string& filename, vector<path>& v)
{
	static xpressive::sregex_compiler rc;		//正则表达式工厂
	if (!rc[filename].regex_id())
	{
		string str = replace_all_copy(replace_all_copy(filename, ".", "\\."), "*", ".*");		//处理文件名
		rc[filename] = rc.compile(str);		//创建正则表达式
	}
	typedef vector<path> result_type;		//返回值类型定义

	if (!exists(dir) || !is_directory(dir))		//目录检查
	{
		return;
	}
	rd_iterator end;		//递归迭代器逾尾位置
	for (rd_iterator pos(dir); pos != end; ++pos)
	{
		if(!is_directory(*pos) && regex_match(pos->path().filename(),rc[filename]))
		{
			v.push_back(pos->path());		//找到，加入vector
		}
	}
}


#include <boost/progress.hpp>
#include <iostream>

using namespace std;
//实现拷贝目录功能
size_t copy_files(const path& from_dir, const path& to_dir, const string& filename = "*")
{
	if(!is_directory(from_dir))		//源必须是个目录
	{
		cout<<"args is not a dir."<<endl;
		return 0;
	}
	cout<<"prepare for copy, please wait..."<<endl;
	vector<path> v;

	find_files(from_dir, filename, v);		//查找源的所有文件
	if(v.empty())		//空目录则不拷贝
	{
		cout<<"0 file copied."<<endl;
		return 0;
	}
	cout<<"now begin copy files ..."<<endl;
	path tmp;
	progress_display pd(v.size());		//进度显示

	BOOST_FOREACH(path &p, v)		//foreach算法
	{
		tmp = to_dir/p.string().substr(from_dir.string().length());		//拆分基本路径与目标路径
		if(!exists(tmp.parent_path()))		//创建子目录
		{
			create_directories(tmp.parent_path());
		}
		copy_file(p, tmp);		//拷贝文件
		++pd;		//更新进度
	}
	
	cout<<v.size()<<"file copied."<<endl;
	return v.size();		//完成拷贝
}



#include <boost/filesystem/fstream.hpp>
namespace newfs = boost::filesystem;

int main()
{
	path p("./INSTALL");
	newfs::ifstream ifs(p);
	assert(ifs.is_open());
	cout<<ifs.rdbuf();
}
#endif


