#include "Marco_define.h"
#ifdef SHOW_SAMPLE

#define BOOST_ALL_NO_LIB
#include <boost/filesystem.hpp>
using namespace boost::filesystem;

int main()
{
	namespace fs = boost::filesystem;		//���ֿռ����

	path ptest = "/home/chrono/test";
	if (exists(ptest))		//���·���Ƿ����
	{
		if (fs::is_empty(ptest))		//ע�����ֿռ��޶�
		{
			remove(ptest);		//removeֻ��ɾ����Ŀ¼���ļ�
		}
		else
		{
			remove_all(ptest);		//remove_all���Եݹ�ɾ��
		}
		
		assert(!exists(ptest));		//���ļ��Ѿ���ɾ��
		create_directory(ptest);	//����һ��Ŀ¼

		copy_file("/home/chrono/boost/INSTALL", ptest/"a.txt");		//�����ļ�
		assert(exists(ptest/"a.txt"));

		rename(ptest/"a.txt", ptest/"b.txt");		//����
		assert(exists(ptest/"b.txt"));

		//ʹ��create_directories����һ�δ����༶Ŀ¼
		create_directories(ptest/"sub_dir1"/"sub_dir1");
	}
}


#include <boost/optional.hpp>
#include <boost/filesystem.hpp>

using namespace boost::filesystem;

//ʵ�ֲ����ļ�����
typedef recursive_directory_iterator rd_iterator;
optional<path> 
find_file(const path& dir, const string& filename)
{
	typedef optional<path> result_type;		//����ֵ���Ͷ���
	if (!exists(dir) || !is_directory(dir))		//���Ŀ¼����Ч��
	{
		return result_type();
	}
	rd_iterator end;		//�ݹ������
	for (rd_iterator pos(dir);pos != end; ++pos)
	{
		if (!is_directory(*pos) && pos->path.filename() == filename)		//����Ŀ¼���ļ������
		{
			return result_type(pos->path());
		}
	}
	return result_type();
}


#include <boost/xpressive/xpressive_dynamic.hpp>
#include <boost/algorithm/string.hpp>
using namespace boost::xpressive;

//ʵ��ģ�������ļ�����
void find_files(const path& dir, const string& filename, vector<path>& v)
{
	static xpressive::sregex_compiler rc;		//������ʽ����
	if (!rc[filename].regex_id())
	{
		string str = replace_all_copy(replace_all_copy(filename, ".", "\\."), "*", ".*");		//�����ļ���
		rc[filename] = rc.compile(str);		//����������ʽ
	}
	typedef vector<path> result_type;		//����ֵ���Ͷ���

	if (!exists(dir) || !is_directory(dir))		//Ŀ¼���
	{
		return;
	}
	rd_iterator end;		//�ݹ��������βλ��
	for (rd_iterator pos(dir); pos != end; ++pos)
	{
		if(!is_directory(*pos) && regex_match(pos->path().filename(),rc[filename]))
		{
			v.push_back(pos->path());		//�ҵ�������vector
		}
	}
}


#include <boost/progress.hpp>
#include <iostream>

using namespace std;
//ʵ�ֿ���Ŀ¼����
size_t copy_files(const path& from_dir, const path& to_dir, const string& filename = "*")
{
	if(!is_directory(from_dir))		//Դ�����Ǹ�Ŀ¼
	{
		cout<<"args is not a dir."<<endl;
		return 0;
	}
	cout<<"prepare for copy, please wait..."<<endl;
	vector<path> v;

	find_files(from_dir, filename, v);		//����Դ�������ļ�
	if(v.empty())		//��Ŀ¼�򲻿���
	{
		cout<<"0 file copied."<<endl;
		return 0;
	}
	cout<<"now begin copy files ..."<<endl;
	path tmp;
	progress_display pd(v.size());		//������ʾ

	BOOST_FOREACH(path &p, v)		//foreach�㷨
	{
		tmp = to_dir/p.string().substr(from_dir.string().length());		//��ֻ���·����Ŀ��·��
		if(!exists(tmp.parent_path()))		//������Ŀ¼
		{
			create_directories(tmp.parent_path());
		}
		copy_file(p, tmp);		//�����ļ�
		++pd;		//���½���
	}
	
	cout<<v.size()<<"file copied."<<endl;
	return v.size();		//��ɿ���
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


