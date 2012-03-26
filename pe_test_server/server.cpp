#include "iso8583_field_def.h"
#include "iso8583_wrappers.h"
#include "SimpleIni.h"
#include <iostream>
#include <map>
#include <algorithm>
#include <string>
#include <vector>
using namespace std;


/*
 *����iniFile��ȡ��������Ҫ�Ŀ�����Ϣ����map<string,string>��ʽװ����
 *�������е��ֶ���Ϣ���� map< int , string> �洢
 *�κδ��󷵻� -1
 */
int loadConfig(map<string,string>& outControl ,map<int , string>& outData , const string& iniFile)
{
	const char* tmpStr = NULL;
	const char* pause  = NULL;
	const char* circle = NULL;
	const char* log    = NULL;
	const char* ip     = NULL;
	const char* port   = NULL;
	const char* timeout= NULL;
	const char* demo   = NULL;
	const char* type   = NULL;
	const char* header = NULL;
	char  tmpsprint[99]= { 0 };
	string  bitKey    ;
	string str ;
	CSimpleIniA ini;
	CSimpleIniA::TNamesDepend pe_keys;

	assert(iniFile != "" );
	outControl.clear();
	ini.SetUnicode();
	ini.LoadFile(iniFile.c_str());
	
	if (ini.IsEmpty())
	{
		return -1;
	}
	pause  = ini.GetValue("control", "pause");
	circle = ini.GetValue("control", "circle");
	log    = ini.GetValue("control", "log");

	ip     = ini.GetValue("serv" , "ip");
	port   = ini.GetValue("serv",  "port");
	timeout= ini.GetValue("serv",  "timeout");

	demo   = ini.GetValue("pe" , "demo");
	type   = ini.GetValue("pe" , "type");
	header = ini.GetValue("pe" , "header");

	if (  !(pause || circle || log || ip || port || timeout) )
	{
		return -1;
	}

	outControl["pause"]	= pause;
	outControl["circle"]	= circle;
	outControl["log"]		= log;
	outControl["ip"]		= ip;
	outControl["port"]		= port;
	outControl["timeout"]  = timeout;
	
	demo ? outControl["demo"] = demo : outControl["demo"] = "" ;
	type ? outControl["type"] = type : outControl["type"] = "" ;
	header?outControl["header"]=header:outControl["header"]="" ;

	//ѭ����ȡ�ֶ�ֵ
	//���ĵ�SimpleIni�⣬���˲ŷ��֣��ܰѼ򵥵��������ô���ӣ�����������simple�ˣ�
	//��������Է��ؽ������򣬵����������hack��
	//���������ֵ�װ������������~
	vector<int> vi;
	ini.GetAllKeys("pe" , pe_keys);
	for (CSimpleIniA::TNamesDepend::const_iterator it = pe_keys.begin(); it != pe_keys.end() ; ++it)
	{
		str = it->pItem ;
		if (  string(str.begin() , str.begin()+3) == "bit"  )
		{
			vi.push_back( atoi(  string(str.begin() + 3,str.end()).c_str()  )   );
		}
	}
	sort(vi.begin(),vi.end());

	for(int i=0; i!= vi.size() ; ++i)
	{
		tmpStr = NULL;
		bitKey = "bit";
		sprintf_s(tmpsprint , sizeof(tmpsprint) , "%d" , vi[i]);
		bitKey += tmpsprint ;
		tmpStr = ini.GetValue("pe" , bitKey.c_str() );
		tmpStr ? outData[vi[i]] = tmpStr : 0;
	}

	return 0;
}

int main()
{
	return 0;
}