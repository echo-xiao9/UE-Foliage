/*
	  quu..__
	   $$$b  `---.__
		"$$b        `--.                          ___.---uuudP
		 `$$b           `.__.------.__     __.---'      $$$$"              .
		   "$b          -'            `-.-'            $$$"              .'|
			 ".                                       d$"             _.'  |
			   `.   /                              ..."             .'     |
				 `./                           ..::-'            _.'       |
				  /                         .:::-'            .-'         .'
				 :                          ::''\          _.'            |
				.' .-.             .-.           `.      .'               |
				: /'$$|           .@"$\           `.   .'              _.-'
			   .'|$u$$|          |$$,$$|           |  <            _.-'
			   | `:$$:'          :$$$$$:           `.  `.       .-'
			   :                  `"--'             |    `-.     \
			  :##.       ==             .###.       `.      `.    `\
			  |##:                      :###:        |        >     >
			  |#'     `..'`..'          `###'        x:      /     /
			   \                                   xXX|     /    ./
				\                                xXXX'|    /   ./
				/`-.                                  `.  /   /
			   :    `-  ...........,                   | /  .'
			   |         ``:::::::'       .            |<    `.
			   |             ```          |           x| \ `.:``.
			   |                         .'    /'   xXX|  `:`M`M':.
			   |    |                    ;    /:' xXXX'|  -'MMMMM:'
			   `.  .'                   :    /:'       |-'MMMM.-'
				|  |                   .'   /'        .'MMM.-'
				`'`'                   :  ,'          |MMM<
				  |                     `'            |tbap\
				   \                                  :MM.-'
					\                 |              .''
					 \.               `.            /
					  /     .:::::::.. :           /
					 |     .:::::::::::`.         /
					 |   .:::------------\       /
					/   .''               >::'  /
					`',:                 :    .'
										 `:.:'

	  皮卡丘发现你写代码已经很久了，提醒你起来走一走，休息一下
	  快说，谢谢皮卡丘
	  */
#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "CoreMinimal.h"
#include <string>
#include <vector>
/**
 * The comments are all written in GB2312.
 * 这个类是一个接口类，目前是使用TCP的方式从Python服务器中获取资源，如果后续有优化可以在DBApi.cpp中进行相应修改。
 * 在Unreal Engine插件中所有对数据库的访问都是通过这些接口。
 * errno 0：结果正常
 * errno -1：json文件的方法名不对
 * errno -2：没有找到对应的项目数据库
 * errno -3：网络错误，没有成功和数据库连接
 */
class TETS_API DBApi
{
public:
	DBApi();
	~DBApi();
	//这个应该是Private，放在Public里属于是失误了^.^
	static std::string getDataFromDatabase(std::string request);
	struct stringKV {
		FString key;
		FString value;
	};

	struct numberKV {
		FString key;
		float value;
	};

	struct numberKVRequest {
		FString key;
		float lowerBound;
		float upperBound;
	};

	enum tagType {
		NUMBER,
		STRING
	};
	struct plant {
		FString name;
		int hierarchy;
	};

	//给定一个植物名称，寻找其全部tag
	USTRUCT()
		struct getAllTagsByPlantNameRequest {
		FString projName;
		FString plantName;

	};
	USTRUCT()
		struct getAllTagsByPlantNameResult {
		int errCode;
		TArray<stringKV> stringTags;
		TArray<numberKV> numberTags;
	};
	static getAllTagsByPlantNameResult getAllTagsByPlantName(getAllTagsByPlantNameRequest request);

	//给定一个植物名称和tag的种类，寻找在给定种类下的所有tag
	USTRUCT()
		struct getOneTagByPlantNameRequest {
		FString projName;
		FString plantName;
		tagType type;
		FString tag;
	};
	USTRUCT()
		struct getOneTagByPlantNameResult {
		int errCode;
		TArray<stringKV> stringTags;
		TArray<numberKV> numberTags;
	};
	static getOneTagByPlantNameResult getOneTagByPlantName(getOneTagByPlantNameRequest request);

	//寻找拥有一些标签的植物，拥有这些标签中的任意一个便可以满足要求。
	//对于字符型标签是寻找key和value都要匹配，数值型标签是key匹配并且value需要处于一定范围
	USTRUCT()
		struct findAllPlantsWithSomeTagsRequest {
		FString projName;
		TArray<stringKV> stringTags;
		TArray<numberKVRequest> numberTags;
	};
	USTRUCT()
		struct findAllPlantsWithSomeTagsResult {
		int errCode;
		TArray<plant> plants;
	};
	static findAllPlantsWithSomeTagsResult findAllPlantsWithSomeTags(findAllPlantsWithSomeTagsRequest request);

	//获取database中所有的植物
	USTRUCT()
		struct getAllPlantsInDatabaseRequest {
		FString projName;
	};
	USTRUCT()
		struct getAllPlantsInDatabaseResult {
		int errCode;
		TArray<plant> plants;
	};
	static getAllPlantsInDatabaseResult getAllPlantsInDatabase(getAllPlantsInDatabaseRequest request);

	//获取某个层级下所有的植物
	USTRUCT()
		struct getAllPlantsInOneHierarchyRequest {
		FString projName;
		int hierarchy;
	};
	USTRUCT()
		struct getAllPlantsInOneHierarchyResult {
		int errCode;
		TArray<plant> plants;
	};
	static getAllPlantsInOneHierarchyResult getAllPlantsInOneHierarchy(getAllPlantsInOneHierarchyRequest request);
};
